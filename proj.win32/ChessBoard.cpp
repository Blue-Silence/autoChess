/********************************************
 * ���ܣ�����ģ��
 * ���ߣ�֣ΰة ������
 * �����޸����ڣ�12.12
 ********************************************/

#include "ChessBoard.h"

bool ChessBoard::init()
{
	int i = 0;
	int j = 0;
	for (i = 0; i < BOARDMAXR; i++)
		for (j = 0; j < BOARDMAXC; j++)
		{
			board[i][j] = false;
		}

	if (i != BOARDMAXR || j != BOARDMAXC)
		return false;

	return true;
}



//-----------------------------------------------------//
//������������һ������targetType����ʾĿ����������     //
//          oldPos��newPos�ֱ��ʾװ��ǰ�������       //
//          ���ĸ�����sprite���������½ǵ�һ�������   //
//          ָ��                                       //
//�������ܣ����������������Ļ����Ļ�ת               //
//��������ֵ��Ŀ����������(x,y),���������൱�ڵ�x�У���y��//
//ע���������������������                           //
//-----------------------------------------------------//
void CoordinateConvert(CoordinateType targetType, Vec2 oldPos, ChessCoordinate* newPos)
{
	auto config = ConfigController::getInstance();

	//��ȡ��ʼ����߼��ߴ�
	auto texture = Director::getInstance()->getTextureCache();
	auto sprite = Sprite::createWithTexture(texture->getTextureForKey("/res/Background/bgwhite.png"));
	Size oneP = sprite->getContentSize();

	int halfBoardWidth = oneP.width / 2;
	int halfBoardHeight = oneP.height / 2;

	//���С�����½�λ��(Ĭ���޸�С��ê��λ�����½�)
	//С��ê��Ķ���������޸ļ���
	Vec2 XY = Vec2(config->getPx()->x * 38, config->getPx()->y * 30);
	XY.x = XY.x - 2 * halfBoardWidth;

	if (targetType == CoordinateType::chessBoardCoordinates)
	{
		newPos->setX(static_cast<int>(oldPos.x - XY.x) % (halfBoardWidth * 2) + 1);
		newPos->setY(static_cast<int>(oldPos.y - XY.y) % (halfBoardHeight * 2) + 1);

	}
	else
	{
		//������߼��ߴ������Ƕ�Ӧ����λ��С���������
		newPos->setX(oldPos.x * 2 * halfBoardWidth - halfBoardWidth);
		newPos->setY(oldPos.y * 2 * halfBoardHeight - halfBoardHeight);
	}
}

// ����Ƿ�����������,��������Ϊ
bool ChessBoard::isInBoard(Vec2 posi)
{
	if (posi.x > 500 && posi.y > 500)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int turn = 1;//�ڼ���

// �жϸ�λ���Ƿ���Է�������,�����������ת��Ϊ��������
inline bool ChessBoard::isAvailable(int row, int col)
{
	if (row < 0 || row >= BOARDMAXR || col < 0 || col >= BOARDMAXC)
		return false;
	return board[row][col] == 0;
}

//-------------------------------------------�ֽ���-------------------------------------------//
// �������йر�սϯ��������ƶ��������ϵĺ���ʵ��
//-------------------//
// ���ߣ�������      //
// �޸����ڣ�12.12   //
// ------------------//


void ChessBoard::onMouseDown(Event* event)
{
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
	if (!mouseEvent)
		return;

	Vec2 clickedPosition = mouseEvent->getLocation();

	// �����λ���Ƿ��ڱ�ս����
	if (isInPreparationZone(clickedPosition)) 
	{
		// ѡ�б�ս��������
		selectedChess = getChessAtPreparationZone(clickedPosition);
	}
	else if (selectedChess && isInBoard(clickedPosition)) 
	{
		// �˴�����Ӧ������ʾ����ʱ�򲹳�
		if ((*player.getBattleAreaChesses()).size() > player.getMaxBattleChessNum())
			return;
		// ��ѡ�е����ӷ��õ�ս����
		ChessCoordinate boardPosition;
		CoordinateConvert(CoordinateType::chessBoardCoordinates, clickedPosition, &boardPosition);
		moveChessFromPreZoneToWarZone(selectedChess,boardPosition.getY(), boardPosition.getX());
		selectedChess = nullptr;  // ���ѡ�е�����
	}
}

bool ChessBoard::createMouseListener(Node* targetNode)
{
	selectedChess = nullptr;  // ��ʼʱû��ѡ�е�����

	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(ChessBoard::onMouseDown, this);
	targetNode->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, targetNode);

	return true;
}


bool ChessBoard::isInPreparationZone(const Vec2& position) 
{
	// ��������귶Χ�Ǽ���ģ���Ҫ������Ϸ���沼��������
	// ���豸ս������Ļ���귶ΧΪ (minX, minY) �� (maxX, maxY)
	const float X_min = 100.0f; // ��ս����߽��X����
	const float X_max = 300.0f; // ��ս���ұ߽��X����
	const float Y_min = 100.0f; // ��ս���±߽��Y����
	const float Y_max = 200.0f; // ��ս���ϱ߽��Y����

	return position.x >= X_min && position.x <= X_max && position.y >= Y_min && position.y <= Y_max;
}

Chess* ChessBoard::getChessAtPreparationZone(const Vec2& position)
{
	// ����ÿ�������ڱ�ս������һ���̶�����Ļ���귶Χ
	// ���ݱ�ս����ʵ�ʲ�������������߼�
	for (auto chess : *player.getPreAreaChesses())
	{
		// ��ȡ���ӵ���Ļ����λ��
		ChessCoordinate* chessPosition = chess->getChessCoordinateByType(CoordinateType::screenCoordinates);

		// ����ÿ������ռ�ݵ���Ļ�ռ��С
		const float chessSizeWidth = 50.0f;  // ���ӿ��
		const float chessSizeHeight = 50.0f; // ���Ӹ߶�

		// �����λ���Ƿ������ӵķ�Χ��
		if (position.x >= chessPosition->getX() && position.x <= chessPosition->getX() + chessSizeWidth &&
			position.y >= chessPosition->getY() && position.y <= chessPosition->getY() + chessSizeHeight)
		{
			return chess; // ���λ�������������
		}
	}

	return nullptr; // û���ҵ��κ�����
}

void ChessBoard::moveChessFromPreZoneToWarZone(Chess* selectedChess, int targetRow, int targetCol)
{
	// ���ȼ���Ƿ�ѡ���˱�ս���е�����
	auto it = find((*player.getPreAreaChesses()).begin(), (*player.getPreAreaChesses()).end(), selectedChess);
	if (it == (*player.getPreAreaChesses()).end())
	{
		// ������Ӳ��ڱ�ս�����򲻽����κβ���
		return;
	}

	// ���Ŀ��λ���Ƿ����
	if (!isAvailable(targetRow, targetCol))
	{
		// ���Ŀ��λ���Ѿ���ռ�ã��򲻽����κβ���
		return;
	}

	// �ӱ�ս���Ƴ�����
	(*player.getPreAreaChesses()).erase(it);

	// �����ӷ��õ�ս������ָ��λ��
	player.putChessInBattleArea(selectedChess);
	selectedChess->setChessCoordinateByType(Vec2(targetCol, targetRow), CoordinateType::chessBoardCoordinates);

	// ��������״̬����Ǹ�λ���ѱ�ռ��
	board[targetRow][targetCol] = true;
}
