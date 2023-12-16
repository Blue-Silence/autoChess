/*
* 功能：实现备战席可视化、绑定鼠标事件
* 作者：马立欣
*/

#pragma once
#ifndef _PREPARATIONSEATLAYER_H_
#define  _PREPARATIONSEATLAYER_H_

#include "cocos2d.h"
USING_NS_CC;
#include "PlayerInfo.h"
#include "Chess.h"
#include "StartAndLoginScene.h"

class PreparationSeat:public Ref
{
private:
	Size pageSize;
	Vec2 pageCoord;
	//备战席button集合
	std::vector<Button*> preAreaChessButton;
public:
	// 玩家信息初始化函数
	virtual bool init();

	// 创建对象
	CREATE_FUNC(PreparationSeat);

	//备战席层
	Layer* PreAreaLayer;

	//生成备战席精灵
	void CreatePreAreaButton(Chess* curHero,int index);

	//上场回调函数
	void menuToBattleCallBack(Ref* sender);

	//按照下标得到备战席坐标
	inline void GetCoordAndScale(int index, Vec2& coord, float& scale);

	//绑定鼠标事件
	void ChessBindMouseEvent(int index);
};
#endif
