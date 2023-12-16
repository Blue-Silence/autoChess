/********************************************
 * 功能：棋盘模型
 * 作者：郑伟丞 胡峻玮
 * 最新修改日期：12.12
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
//函数参数：第一个参数targetType，表示目标坐标类型     //
//          oldPos和newPos分别表示装换前后的坐标       //
//          第四个参数sprite是棋盘左下角第一个祺格的   //
//          指针                                       //
//函数功能：用于棋盘坐标和屏幕坐标的互转               //
//函数返回值：目标类型坐标(x,y),到棋盘中相当于第x列，第y行//
//注意事项：仅适用于棋盘区域                           //
//-----------------------------------------------------//
void CoordinateConvert(CoordinateType targetType, Vec2 oldPos, ChessCoordinate* newPos)
{
	auto config = ConfigController::getInstance();

	//获取起始块的逻辑尺寸
	auto texture = Director::getInstance()->getTextureCache();
	auto sprite = Sprite::createWithTexture(texture->getTextureForKey("/res/Background/bgwhite.png"));
	Size oneP = sprite->getContentSize();

	int halfBoardWidth = oneP.width / 2;
	int halfBoardHeight = oneP.height / 2;

	//获得小格左下角位置(默认修改小格锚点位于右下角)
	//小格锚点改动，这里简单修改即可
	Vec2 XY = Vec2(config->getPx()->x * 38, config->getPx()->y * 30);
	XY.x = XY.x - 2 * halfBoardWidth;

	if (targetType == CoordinateType::chessBoardCoordinates)
	{
		newPos->setX(static_cast<int>(oldPos.x - XY.x) % (halfBoardWidth * 2) + 1);
		newPos->setY(static_cast<int>(oldPos.y - XY.y) % (halfBoardHeight * 2) + 1);

	}
	else
	{
		//给予的逻辑尺寸坐标是对应坐标位置小方块的中心
		newPos->setX(oldPos.x * 2 * halfBoardWidth - halfBoardWidth);
		newPos->setY(oldPos.y * 2 * halfBoardHeight - halfBoardHeight);
	}
}

// 鼠标是否在棋盘区域,输入坐标为
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

int turn = 1;//第几局

// 判断该位置是否可以放置棋子,输入参数请先转化为棋盘坐标
inline bool ChessBoard::isAvailable(int row, int col)
{
	if (row < 0 || row >= BOARDMAXR || col < 0 || col >= BOARDMAXC)
		return false;
	return board[row][col] == 0;
}

//-------------------------------------------分界线-------------------------------------------//
// 以下是有关备战席棋子如何移动到棋盘上的函数实现
//-------------------//
// 作者：胡峻玮      //
// 修改日期：12.12   //
// ------------------//


void ChessBoard::onMouseDown(Event* event)
{
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
	if (!mouseEvent)
		return;

	Vec2 clickedPosition = mouseEvent->getLocation();

	// 检查点击位置是否在备战区内
	if (isInPreparationZone(clickedPosition)) 
	{
		// 选中备战区的棋子
		selectedChess = getChessAtPreparationZone(clickedPosition);
	}
	else if (selectedChess && isInBoard(clickedPosition)) 
	{
		// 此处正常应该有提示，到时候补充
		if ((*player.getBattleAreaChesses()).size() > player.getMaxBattleChessNum())
			return;
		// 将选中的棋子放置到战斗区
		ChessCoordinate boardPosition;
		CoordinateConvert(CoordinateType::chessBoardCoordinates, clickedPosition, &boardPosition);
		moveChessFromPreZoneToWarZone(selectedChess,boardPosition.getY(), boardPosition.getX());
		selectedChess = nullptr;  // 清除选中的棋子
	}
}

bool ChessBoard::createMouseListener(Node* targetNode)
{
	selectedChess = nullptr;  // 初始时没有选中的棋子

	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(ChessBoard::onMouseDown, this);
	targetNode->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, targetNode);

	return true;
}


bool ChessBoard::isInPreparationZone(const Vec2& position) 
{
	// 这里的坐标范围是假设的，需要根据游戏界面布局来调整
	// 假设备战区的屏幕坐标范围为 (minX, minY) 到 (maxX, maxY)
	const float X_min = 100.0f; // 备战区左边界的X坐标
	const float X_max = 300.0f; // 备战区右边界的X坐标
	const float Y_min = 100.0f; // 备战区下边界的Y坐标
	const float Y_max = 200.0f; // 备战区上边界的Y坐标

	return position.x >= X_min && position.x <= X_max && position.y >= Y_min && position.y <= Y_max;
}

Chess* ChessBoard::getChessAtPreparationZone(const Vec2& position)
{
	// 假设每个棋子在备战区都有一个固定的屏幕坐标范围
	// 根据备战区的实际布局来调整这个逻辑
	for (auto chess : *player.getPreAreaChesses())
	{
		// 获取棋子的屏幕坐标位置
		ChessCoordinate* chessPosition = chess->getChessCoordinateByType(CoordinateType::screenCoordinates);

		// 假设每个棋子占据的屏幕空间大小
		const float chessSizeWidth = 50.0f;  // 棋子宽度
		const float chessSizeHeight = 50.0f; // 棋子高度

		// 检查点击位置是否在棋子的范围内
		if (position.x >= chessPosition->getX() && position.x <= chessPosition->getX() + chessSizeWidth &&
			position.y >= chessPosition->getY() && position.y <= chessPosition->getY() + chessSizeHeight)
		{
			return chess; // 点击位置在这个棋子上
		}
	}

	return nullptr; // 没有找到任何棋子
}

void ChessBoard::moveChessFromPreZoneToWarZone(Chess* selectedChess, int targetRow, int targetCol)
{
	// 首先检查是否选中了备战区中的棋子
	auto it = find((*player.getPreAreaChesses()).begin(), (*player.getPreAreaChesses()).end(), selectedChess);
	if (it == (*player.getPreAreaChesses()).end())
	{
		// 如果棋子不在备战区，则不进行任何操作
		return;
	}

	// 检查目标位置是否可用
	if (!isAvailable(targetRow, targetCol))
	{
		// 如果目标位置已经被占用，则不进行任何操作
		return;
	}

	// 从备战区移除棋子
	(*player.getPreAreaChesses()).erase(it);

	// 将棋子放置到战斗区的指定位置
	player.putChessInBattleArea(selectedChess);
	selectedChess->setChessCoordinateByType(Vec2(targetCol, targetRow), CoordinateType::chessBoardCoordinates);

	// 更新棋盘状态，标记该位置已被占用
	board[targetRow][targetCol] = true;
}
