/********************************************
 * 功能：棋盘模型
 * 作者：郑伟丞
 * 最新修改日期：12.10
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
ChessCoordinate* ChessBoard::coordinateConvert(CoordinateType targetType, ChessCoordinate* oldPos, ChessCoordinate* newPos, Sprite* sprite)
{
	//获取起始块的逻辑尺寸
	Size oneP = sprite->getContentSize();
	int halfBoardWidth = oneP.width / 2;
	int halfBoardHeight = oneP.height / 2;

	//获得小格左下角位置(默认修改小格锚点位于右下角)
	//小格锚点改动，这里简单修改即可
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
		//给予的逻辑尺寸坐标是对应坐标位置小方块的中心
		newPos->setX(oldPos->getX() * 2 * halfBoardWidth - halfBoardWidth);
		newPos->setY(oldPos->getY() * 2 * halfBoardHeight - halfBoardHeight);
		return newPos;
	}
}

// 在玩家A对战区放置棋子,输入坐标为棋盘坐标
void ChessBoard::putChessInPlayerAWarZone(int row,int col,Chess* chess)
{
	

	if (isAvailable(row,col))
	{
		board[row][col] = 1;
		playerAWarZoneChesses.push_back(chess);
	}
}

// 获取玩家A对战区棋子集合
vector<Chess*>* ChessBoard::getPlayerAWarZoneChesses()
{
	return &playerAWarZoneChesses;
}

// 在玩家A备战区放置棋子
void ChessBoard::putChessInPlayerAPreZone(Chess* chess)
{
	playerAPreZoneChesses.push_back(chess);
}

// 获取玩家A备战区的棋子集合
vector<Chess*>* ChessBoard::getPlayerAPreZoneChesses()
{
	return &playerAPreZoneChesses;
}

// 从玩家A备战区移去棋子
void ChessBoard::removeChessFromPlayerAPreZone(Chess* chess)
{
	// 移去第一个匹配的棋子即可
	for (auto it = playerAPreZoneChesses.begin(); it != playerAPreZoneChesses.end(); ++it) {
		if (*it == chess) 
		{
			playerAPreZoneChesses.erase(it);
			break; 
		}
	}
}

// 在玩家B对战区放置棋子,输入坐标为棋盘坐标
void ChessBoard::putChessInPlayerBWarZone(int row,int col, Chess* chess)
{
	if (isAvailable(row,col))
	{
		board[row][col] = 1;
		playerBWarZoneChesses.push_back(chess);
	}
}
// 获取玩家B对战区棋子集合
vector<Chess*>* ChessBoard::getPlayerBWarZoneChesses()
{
	return &playerBWarZoneChesses;
}

// 在玩家B备战区放置棋子
void ChessBoard::putChessInPlayerBPreZone(Chess* chess)
{
	playerBPreZoneChesses.push_back(chess);
}

// 获取玩家B备战区棋子集合
vector<Chess*>* ChessBoard::getPlayerBPreZoneChesses()
{
	return &playerBPreZoneChesses;
}

// 从玩家B备战区移去棋子
void ChessBoard::removeChessFromPlayerBPreZone(Chess* chess)
{
	// 移去第一个匹配的棋子即可
	for (auto it = playerBPreZoneChesses.begin(); it != playerBPreZoneChesses.end(); ++it) {
		if (*it == chess)
		{
			playerBPreZoneChesses.erase(it);
			break;
		}
	}
}


// 鼠标是否在棋盘区域,输入坐标为
bool ChessBoard::isInBoard(Vec2 posi)
{
	if (posi.x > 500 && posi.y > 500) {
		return true;
	}
	else {
		return false;
	}
}

int turn = 1;//第几局

// 判断该位置是否可以放置棋子,输入参数请先转化为棋盘坐标
inline bool ChessBoard::isAvailable(int row ,int col)
{
	if (row < 0 || row >= BOARDMAXR || col < 0 || col >= BOARDMAXC)
		return false;
	return board[row][col] == 0;
}