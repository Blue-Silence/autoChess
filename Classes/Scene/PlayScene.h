#pragma once
#ifndef _PLAYSCENE_H_
#define _PLAYSCENE_H_

#include <cocos2d.h>
USING_NS_CC;

#include "ui/CocosGUI.h" 
using namespace ui;

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include <vector>
using std::vector;

#include "StartAndLoginScene.h"
#include "Config/ConfigController.h"
#include "BeforeBattle/ChessBoard.h"
#include "BeforeBattle/Chess.h"
#include "BeforeBattle/Market.h"
#include "BeforeBattle/PlayerInfo.h"
#include "BeforeBattle/PreparationSeatLayer.h"
#include "Battle/BattleLayer.h"
#include "AIMode/AIMode.h"
#include "ChatGLM/ChatGLM3.h"
#include "Battle/Clock.h"
#include "Network/network.h"
#include "Network/Connect.h"
#include "Config/ConfigScene.h"
#include "Network/NetworkConfigSetting.h"

#define ROW_BOARD			5
#define COL_BOARD			8

#define NOT_IN_BOARD		0
#define IN_WAR_ZONE			1
#define IN_READY_ZONE		2

class PlayScene : public Scene
{
public:
	static Scene* createScene(bool isServer,ConPort* cone, const string& gameMode = "人机对战");
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

	// 点击触发战斗
	void PlayScene::onBattleButtonClicked(Ref* sender);

	// 联网下：打包我方场上棋子信息
	void packageInfo();

	// 联网下：提取对方场上棋子信息
	void extractInfo();

	// 坐标转换函数
	static ChessCoordinate coordingrevert(Vec2 realPosition);

	// 结算动画
	void PlayScene::showEndGameAnimation(const string& gameResult);

	CREATE_FUNC(PlayScene);

	// 修改游戏模式
	void changeMode(string mode) { gameMode = mode; }

	// 点击进入设置页面
	void menuSettingsCallBack(cocos2d::Ref* pSender);

	// 点击进入网络设置页面
	void PlayScene::menuNetworkSettingsCallBack(cocos2d::Ref* pSender);

private:
	
	// IP地址
	string ip;
	
	// 端口
	int port;

	// 主/副
	bool server=true;
	
	//网络连接
	ConPort* connection;

	// 联网模式下对我方场上棋子信息的打包
	Packet myInfo;
	
	// 接收的敌方棋子信息包
	Packet oppInfo;

	string url = "https://u207393-a5b8-10e2f069.westb.seetacloud.com:8443/v1/chat/completions";

	CurlWrapper chatAI;

	Label* AILabel = Label::createWithTTF("欢迎进入游戏，我是你的AI小助手，让我来协助你战斗！", "fonts/Marker Felt.ttf", 24);;

	// AI 选手池
	AIMode* AI = nullptr;

	// 游戏模式
	string gameMode = "人机对战";

	// AI玩家数量
	int AINum = 0;

	// 当前对战的AI
	int TargetAI = -1;

	// 游戏状态
	bool isInBattle = false;

	// 能否买棋子
	bool canBuyChess = true;

	// 是否处于信息传输中
	bool isTransmittingInfo = false;

	// 联机模式是否开战
	bool beginFighting = false;

	// 场景层
	Layer* playLayer;

	// 棋盘
	ChessBoard* chessBoardModel;
	vector<Sprite*> chessBoard[ROW_BOARD];

	//五个商店里的棋子
	Menu* previousMenu;
	// 商店
	vector<MenuItemSprite*> shop;

	// 我方玩家
	PlayerInfo* playerME;

	// 对手玩家
	PlayerInfo* playerOPP;

	//金币显示和等级显示
	Label* player_coin;
	Label* player_level;
	Label* player_lifevalue;

	//备战席层
	PreparationSeat* preArea;
	SimpleAudioEngine* playSceneBGM;

	//背景音乐



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


