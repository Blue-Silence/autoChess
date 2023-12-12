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
#include"ConfigController.h"
#include<vector>
#include"cocos2d.h"

USING_NS_CC;


// 棋盘坐标与屏幕坐标的转化,转入的为对战区左下角第一格棋盘对应的精灵
void CoordinateConvert(CoordinateType type, Vec2 oldPos, ChessCoordinate* newPos);

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


	// 在玩家A对战区放置棋子,输入坐标为棋盘坐标
	void putChessInPlayerAWarZone(int row, int col, Chess* chess);

	// 获取玩家A对战区棋子集合
	vector<Chess*>* getPlayerAWarZoneChesses();

	// 在玩家A备战区放置棋子
	void putChessInPlayerAPreZone(Chess* chess);

	// 获取玩家A备战区棋子集合
	vector<Chess*>* getPlayerAPreZoneChesses();

	// 从玩家A备战区移去棋子
	void removeChessFromPlayerAPreZone(Chess* chess);

	// 在玩家B对战区放置棋子,输入坐标为棋盘坐标
	void putChessInPlayerBWarZone(int row, int col, Chess* chess);

	// 获取玩家B对战区棋子集合
	vector<Chess*>* getPlayerBWarZoneChesses();

	// 在玩家B备战区放置棋子
	void putChessInPlayerBPreZone(Chess* chess);

	// 获取玩家B备战区棋子集合
	vector<Chess*>* getPlayerBPreZoneChesses();

	// 从玩家B备战区移去棋子
	void removeChessFromPlayerBPreZone(Chess* chess);

	//检测鼠标是否在棋盘范围内
	static bool isInBoard(Vec2 posi);

	// 判断该位置是否可以放置棋子,输入参数请先转化为棋盘坐标
	inline bool isAvailable(int row, int col);

	//-------------------------------------------分界线-------------------------------------------//
	// 以下是有关备战席棋子如何移动到棋盘上的变量与函数
	//-------------------//
	// 作者：胡峻玮      //
	// 修改日期：12.12   //
	// ------------------//


	// 当前选中的棋子
	Chess* selectedChess;

	// 处理鼠标点击事件
	void onMouseDown(Event* event);

	// 创建并设置鼠标事件监听器
	bool CreateMouseListener(Node* targetNode);

	// 判断鼠标点击位置是否在备战区
	bool isInPreparationZone(const Vec2& position);

	// 获取备战区棋子的信息
	Chess* getChessAtPreparationZone(const Vec2& position);

	// 将备战区的棋子移动至战斗区
	void moveChessFromPreZoneToWarZone(Chess* selectedChess, int targetRow, int targetCol);












	// TODO
	// 坐标的问题，棋盘大小应该没改吧，直接按他们原来的来
	// 屏幕坐标x,y,(0,0)应该是左下角吧
	// 
	// 有些位置，蓝条和红条，技能贴图和普通攻击贴图，对，普通攻击怎么贴图
	// 
	// 在战斗中，移动各自贴图还没写吧
	// 
	// 坐标坐标和棋盘坐标的转化，返回值要注意！有时候需要反过来，x相当于列，y相当于行
	// 寻路
	// 
	// 确定攻击目标之类的

};

#endif // !_CHESSBOARD_H_