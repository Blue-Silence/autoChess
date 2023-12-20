/********************************************
 * 功能：进入游戏界面
 * 作者：Gorgeous, VenusHui
 * 版本：1.1.0.beta
 * 游戏引擎：Cocos2d-x 4.0
 * ******************************************
 * 说明：数据仍需调整
 ********************************************/
#pragma once
#ifndef _PLAYSCENE_H_
#define _PLAYSCENE_H_

#include <cocos2d.h>
USING_NS_CC;

#include "ui/CocosGUI.h" 
using namespace ui;

#include <vector>
using std::vector;

#include "StartAndLoginScene.h"
#include "ConfigController.h"
#include "ChessBoard.h"
#include "Chess.h"
#include "Market.h"
#include "PlayerInfo.h"
#include "PreparationSeatLayer.h"

#define ROW_BOARD			5
#define COL_BOARD			8

#define NOT_IN_BOARD		0
#define IN_WAR_ZONE			1
#define IN_READY_ZONE		2

class PlayScene : public Scene
{
public:
	static Scene* createScene();
	Market* shopModel;
	virtual bool init();
	EventListenerMouse* mouseListener;

	// 创建可视化棋盘， position为(0, 0)位置欺骗图片左下角的锚点坐标
	void createBoard(Vec2 position);

	// 创建可视化商店，position为第一张卡片左下角的锚点坐标
	void createShop(Vec2 position);

	// 创建可视化棋子卡片
	MenuItemSprite* createPieceCard(string pieceName, string piecePicPath, Vec2 position, const ccMenuCallback& callback);

	// 坐标转换函数
	static ChessCoordinate* coordingrevert(Vec2 realPosition);

	CREATE_FUNC(PlayScene);

private:
	// 场景层
	Layer* playLayer;

	// 棋盘
	ChessBoard* chessBoardModel;
	vector<Sprite*> chessBoard[ROW_BOARD];


	//五个商店里的棋子
	Menu* previousMenu;
	// 商店
	
	vector<MenuItemSprite*> shop;

	// 玩家
	PlayerInfo* playerA;



	//备战席层
	PreparationSeat* preArea;

	// 退出按钮的点击事件
	void menuExitCallBack(Ref* sender);

	// 购买卡片的点击事件
	void menuPieceCardCallBack(Ref* sender);

	// 刷新商店的点击事件
	void menuFreshShopCallBack(Ref* sender);

	// 购买经验的点击事件
	void menuBuyExpCallBack(Ref* sender);
	// 购买棋子的点击事件
	void BuyChess(Ref* sender,int index);
	//按下回调
	virtual int onTouchBegan(Touch* touch, Event* event);

	//释放回调
	virtual void onTouchEnded(Touch* touch, Event* event);

	// 移动回调
	void onMouseMove(Event* event);
	
};

#endif








// 点击商店触发事件，刷新
// 购买棋子,要出现在备战席
// 放到战场上
// 右键移动小小英雄
// 战斗逻辑
// 收集素材



