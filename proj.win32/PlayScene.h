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
#include "BattleLayer.h"
#include "AIMode.h"

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

	void update(float delta);

	EventListenerMouse* mouseListener;

	// 创建可视化棋盘， position为(0, 0)位置欺骗图片左下角的锚点坐标
	void createBoard(Vec2 position);

	// 创建可视化商店，position为第一张卡片左下角的锚点坐标
	void createShop(Vec2 position);

	// 创建可视化棋子卡片
	MenuItemSprite* createPieceCard(string pieceName, string piecePicPath, Vec2 position, const ccMenuCallback& callback);

	void PlayScene::onBattleButtonClicked(Ref* sender);

	// 坐标转换函数
	static ChessCoordinate* coordingrevert(Vec2 realPosition);

	CREATE_FUNC(PlayScene);

private:
	// AI 选手池
	AIMode* AI = nullptr;

	// 游戏模式
	string gameMode = "人机对战";

	// AI玩家数量
	int AINum = 4;

	// 当前对战的AI
	int TargetAI = 0;

	// 游戏状态
	bool isInBattle = false;

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

	// 对手玩家
	PlayerInfo* playerOPP;

	//金币显示和等级显示
	Label* player_coin;
	Label* player_level;
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


	
};

#endif








// 点击商店触发事件，刷新
// 购买棋子,要出现在备战席
// 放到战场上
// 右键移动小小英雄
// 战斗逻辑
// 收集素材



