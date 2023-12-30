/*
* 功能：实现备战席可视化、绑定鼠标事件
* 作者：马立欣
*/

#pragma once
#ifndef _PREPARATIONSEATLAYER_H_
#define  _PREPARATIONSEATLAYER_H_

#include <utility>

#include "cocos2d.h"
USING_NS_CC;
#include "ui/CocosGUI.h" 
using namespace ui;
#include "PlayerInfo.h"
#include "Chess.h"
#include "ConfigController.h"
#include "ChessBoard.h"
#include "StartAndLoginScene.h"

//场上只允许前两排摆放
#define RSIZE 2
#define CSIZE 8

class PreparationSeat :public Ref
{
private:
	// 屏幕相关
	Size pageSize;
	Vec2 pageCoord;

	// 备战席button集合
	std::vector<Menu*> preAreaChessMenu = vector<Menu*>(9, nullptr);

	// 对象指针
	PlayerInfo* player;
	ChessBoard* cBoard;
	EventListenerMouse* listenerMouse;
	Label* playerCoin;

	//记录当前场上棋子数
	int numOfBoard = 0;

	//场上棋子位置记录
	shared_ptr<Chess> onBoardChess[RSIZE][CSIZE];
	Sprite* onBoardSprite[RSIZE][CSIZE];

	//是否在战斗中
	bool* canBuyChess;

	//当前选中备战席
	pair < Menu*, int >curHero = { nullptr, -1 };

	//羁绊图标
	Sprite* PreviousBuffmage;
	Sprite* PreviousBuffshooter;
	Sprite* PreviousBufftank;

public:

	// 玩家信息初始化函数
	virtual bool init(PlayerInfo* pr, ChessBoard* board, EventListenerMouse* listener, Layer* preArea, bool* buyChess,Label* player_coin);

	// 创建对象
	static PreparationSeat* create(PlayerInfo* pr, ChessBoard* board, EventListenerMouse* listener, Layer* preArea, bool* buyChess, Label* player_coin) {
		PreparationSeat* pRet = new(std::nothrow) PreparationSeat(); if (pRet && pRet->init(pr, board, listener, preArea, buyChess,player_coin)) {
			pRet->autorelease(); return pRet;
		}
		else {
			delete pRet; pRet = nullptr; return nullptr;
		}
	};

	void CallBack(Ref* sender, int index);

	//备战席层
	Layer* PreAreaLayer;

	//羁绊图片改变
	void PreparationSeat::ChangeBuffImage();

	//生成备战席精灵
	void CreatePreAreaButton(shared_ptr<Chess> curHero, int index);

	//按照下标得到备战席坐标
	inline void GetCoordAndScale(int index, Vec2& coord, float& scale);

	Sprite* ChessToBoard(const Vec2& onBoardCoord, int index);

	void PromoteChessLevel(int index);

	//卖出备战席棋子的回调函数
	void SellOut(Ref* sender);
};
#endif
