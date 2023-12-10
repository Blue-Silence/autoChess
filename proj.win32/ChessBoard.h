/********************************************
 * 功能：棋盘模型
 * 作者：郑伟丞
 * 最新修改日期：12.10
 ********************************************/

#pragma once
#ifndef _CHESSBOARD_H_
#define _CHESSBOARD_H_

 //定义棋盘大小
#define BOARDMAXR 5
#define BOARDMAXC 8

#include"Chess.h"
#include<vector>
#include"cocos2d.h"

USING_NS_CC;

class ChessBoard :public Ref
{

private:

	static int turn;

	vector<Chess*> playerAWarZoneChesses; // 记录玩家A对战区棋子集合

	vector<Chess*> playerAPreZoneChesses; // 记录玩家A备战区棋子集合

	vector<Chess*> playerBWarZoneChesses; // 记录玩家B对战区棋子集合

	vector<Chess*> playerBPreZoneChesses; // 记录玩家B备战区棋子集合

	//记录棋盘上哪里为空哪里有棋子
	//仅用来确定是否可以移动，无法确定具体位置的具体棋子是什么
	//vector<Sprite*>board[BOARD_SIZE];
	bool board[BOARDMAXR][BOARDMAXC];

public:

	CREATE_FUNC(ChessBoard);

	virtual bool init();

	//CREATE_FUNC(ChessBoard);

	// 棋盘坐标与屏幕坐标的转化,转入的为对战区左下角第一格棋盘对应的精灵
	ChessCoordinate* coordinateConvert(CoordinateType type, ChessCoordinate* oldPos, ChessCoordinate* newPos, Sprite* sprite);

	// 在玩家A对战区放置棋子,输入坐标为棋盘坐标
	void putChessInPlayerAWarZone(Vec2 posi,Chess* chess);

	// 获取玩家A对战区棋子集合
	vector<Chess*>* getPlayerAWarZoneChesses();

	// 在玩家A备战区放置棋子
	void putChessInPlayerAPreZone(Chess* chess);

	// 获取玩家A备战区棋子集合
	vector<Chess*>* getPlayerAPreZoneChesses();

	// 从玩家A备战区移去棋子
	void removeChessFromPlayerAPreZone(Chess* chess);

	// 在玩家B对战区放置棋子,输入坐标为棋盘坐标
	void putChessInPlayerBWarZone(Vec2 posi, Chess* chess);

	// 获取玩家B对战区棋子集合
	vector<Chess*>* getPlayerBWarZoneChesses();

	// 在玩家B备战区放置棋子
	void putChessInPlayerBPreZone(Chess* chess);

	// 获取玩家B备战区棋子集合
	vector<Chess*>* getPlayerBPreZoneChesses();

	// 从玩家B备战区移去棋子
	void removeChessFromPlayerBPreZone(Chess* chess);

	//检测是否在棋盘范围内
	static bool isInBoard(Vec2 posi);

	// 判断该位置是否可以放置棋子,输入参数请先转化为棋盘坐标
	inline bool isAvailable(int row,int col);

	// TODO
	// 寻路
	//确定攻击目标之类的

};

#endif // !_CHESSBOARD_H_