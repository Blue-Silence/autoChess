/********************************************
 * 功能：棋盘模型
 * 作者：郑伟丞 胡峻玮
 * 最新修改日期：12.12
 ********************************************/

#pragma once
#ifndef _CHESSBOARD_H_
#define _CHESSBOARD_H_

 //定义棋盘大小
#define BOARDMAXR 5
#define BOARDMAXC 8

#include"Chess.h"
#include"Config/ConfigController.h"
#include"PlayerInfo.h"
#include<vector>
#include"cocos2d.h"

USING_NS_CC;


// 棋盘坐标与屏幕坐标的转化,转入的为对战区左下角第一格棋盘对应的精灵
void CoordinateConvert(CoordinateType targetType, Vec2 oldPos, ChessCoordinate* newPos);

class ChessBoard :public Ref
{

private:

	static int turn;

	//记录棋盘上哪里为空哪里有棋子
	bool board[BOARDMAXR][BOARDMAXC];

public:

	// 创建本方玩家的实例
	PlayerInfo player;

	// 联网情况下存储对方战斗区的信息
	vector<Chess*>oppBattleAreaInfo;

	CREATE_FUNC(ChessBoard);

	// 初始化函数
	virtual bool init();

	//检测鼠标是否在棋盘范围内
	static bool isInBoard(Vec2 posi);

	// 判断该位置是否可以放置棋子,输入参数请先转化为棋盘坐标
	bool isAvailable(int row, int col);

	//mlx:放置棋子，更改棋盘
	void OnBoard(int row, int col);

	//mlx:清除棋盘
	void WithdrawBoard(int row, int col);

	//-------------------------------------------分界线-------------------------------------------//
	// 以下是有关备战席棋子如何移动到棋盘上的变量与函数
	//-------------------//
	// 作者：胡峻玮      //
	// 修改日期：12.12   //
	// ------------------//


	// 当前选中的棋子
	Chess* selectedChess;

	// 判断鼠标点击位置是否在备战区
	bool isInPreparationZone(const Vec2& position);

	// 获取备战区棋子的信息
	shared_ptr<Chess> getChessAtPreparationZone(const Vec2& position);
};

#endif // !_CHESSBOARD_H_