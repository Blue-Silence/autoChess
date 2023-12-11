/********************************************
 * ���ܣ�����ģ��
 * ���ߣ�֣ΰة
 * �����޸����ڣ�12.10
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
ChessCoordinate* ChessBoard::coordinateConvert(CoordinateType targetType, ChessCoordinate* oldPos, ChessCoordinate* newPos, Sprite* sprite)
{
	//��ȡ��ʼ����߼��ߴ�
	Size oneP = sprite->getContentSize();
	int halfBoardWidth = oneP.width / 2;
	int halfBoardHeight = oneP.height / 2;

	//���С�����½�λ��(Ĭ���޸�С��ê��λ�����½�)
	//С��ê��Ķ���������޸ļ���
	Vec2 XY = sprite->getPosition();
	XY.x = XY.x - 2 * halfBoardWidth;

	if (targetType == CoordinateType::chessBoardCoordinates)
	{
		newPos->setX(static_cast<int>(oldPos->getX() - XY.x) % (halfBoardWidth * 2) + 1);
		newPos->setY(static_cast<int>(oldPos->getY() - XY.y) % (halfBoardHeight * 2) + 1);
		return newPos;
	}
	else
	{
		//������߼��ߴ������Ƕ�Ӧ����λ��С���������
		newPos->setX(oldPos->getX() * 2 * halfBoardWidth - halfBoardWidth);
		newPos->setY(oldPos->getY() * 2 * halfBoardHeight - halfBoardHeight);
		return newPos;
	}
}

// �����A��ս����������,��������Ϊ��������
void ChessBoard::putChessInPlayerAWarZone(int row,int col,Chess* chess)
{
	

	if (isAvailable(row,col))
	{
		board[row][col] = 1;
		playerAWarZoneChesses.push_back(chess);
	}
}

// ��ȡ���A��ս�����Ӽ���
vector<Chess*>* ChessBoard::getPlayerAWarZoneChesses()
{
	return &playerAWarZoneChesses;
}

// �����A��ս����������
void ChessBoard::putChessInPlayerAPreZone(Chess* chess)
{
	playerAPreZoneChesses.push_back(chess);
}

// ��ȡ���A��ս�������Ӽ���
vector<Chess*>* ChessBoard::getPlayerAPreZoneChesses()
{
	return &playerAPreZoneChesses;
}

// �����A��ս����ȥ����
void ChessBoard::removeChessFromPlayerAPreZone(Chess* chess)
{
	// ��ȥ��һ��ƥ������Ӽ���
	for (auto it = playerAPreZoneChesses.begin(); it != playerAPreZoneChesses.end(); ++it) {
		if (*it == chess) 
		{
			playerAPreZoneChesses.erase(it);
			break; 
		}
	}
}

// �����B��ս����������,��������Ϊ��������
void ChessBoard::putChessInPlayerBWarZone(int row,int col, Chess* chess)
{
	if (isAvailable(row,col))
	{
		board[row][col] = 1;
		playerBWarZoneChesses.push_back(chess);
	}
}
// ��ȡ���B��ս�����Ӽ���
vector<Chess*>* ChessBoard::getPlayerBWarZoneChesses()
{
	return &playerBWarZoneChesses;
}

// �����B��ս����������
void ChessBoard::putChessInPlayerBPreZone(Chess* chess)
{
	playerBPreZoneChesses.push_back(chess);
}

// ��ȡ���B��ս�����Ӽ���
vector<Chess*>* ChessBoard::getPlayerBPreZoneChesses()
{
	return &playerBPreZoneChesses;
}

// �����B��ս����ȥ����
void ChessBoard::removeChessFromPlayerBPreZone(Chess* chess)
{
	// ��ȥ��һ��ƥ������Ӽ���
	for (auto it = playerBPreZoneChesses.begin(); it != playerBPreZoneChesses.end(); ++it) {
		if (*it == chess)
		{
			playerBPreZoneChesses.erase(it);
			break;
		}
	}
}


// ����Ƿ�����������,��������Ϊ
bool ChessBoard::isInBoard(Vec2 posi)
{
	if (posi.x > 500 && posi.y > 500) {
		return true;
	}
	else {
		return false;
	}
}

int turn = 1;//�ڼ���

// �жϸ�λ���Ƿ���Է�������,�����������ת��Ϊ��������
inline bool ChessBoard::isAvailable(int row ,int col)
{
	if (row < 0 || row >= BOARDMAXR || col < 0 || col >= BOARDMAXC)
		return false;
	return board[row][col] == 0;
}