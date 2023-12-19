/*
* 功能：实现备战席可视化、绑定鼠标事件
* 作者：马立欣
*/

#pragma once
#ifndef _PREPARATIONSEATLAYER_H_
#define  _PREPARATIONSEATLAYER_H_

#include "cocos2d.h"
USING_NS_CC;
#include "ui/CocosGUI.h" 
using namespace ui;
#include "PlayerInfo.h"
#include "Chess.h"
#include "ChessBoard.h"
#include "StartAndLoginScene.h"

class PreparationSeat :public Ref
{
private:
	Size pageSize;
	Vec2 pageCoord;
	//备战席button集合
	std::vector<Menu*> preAreaChessMenu;
	PlayerInfo* player;
	ChessBoard* cBoard;
	EventListenerMouse* listenerMouse;
public:
	// 玩家信息初始化函数
	virtual bool init(PlayerInfo* pr, ChessBoard* board, EventListenerMouse* listener, Layer* preArea);

	// 创建对象
	static PreparationSeat* create(PlayerInfo* pr, ChessBoard* board, EventListenerMouse* listener, Layer* preArea) {
		PreparationSeat* pRet = new(std::nothrow) PreparationSeat(); if (pRet && pRet->init(pr, board, listener, preArea)) {
			pRet->autorelease(); return pRet;
		}
		else {
			delete pRet; pRet = nullptr; return nullptr;
		}
	};
	void CallBack(Ref* sender, int index);
	//备战席层
	Layer* PreAreaLayer;

	//生成备战席精灵
	void CreatePreAreaButton(shared_ptr<Chess> curHero, int index);

	//上场回调函数
	void menuToBattleCallBackCallBack(Ref* sender, Chess* curHero);

	//按照下标得到备战席坐标
	inline void GetCoordAndScale(int index, Vec2& coord, float& scale);

	//绑定鼠标事件
	void ChessBindMouseEvent(int index);
};
#endif
