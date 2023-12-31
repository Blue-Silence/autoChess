/*
* 功能：初始化页面显示、加载资源
* 作者：马立欣
*/
#pragma once

#ifndef _STARTANDLOFINSCENE_
#define _STARTANDLOFINSCENE_
#endif

#include <string>
#include <filesystem>

using namespace std::filesystem;

#include "cocos2d.h"
USING_NS_CC;

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include "ui/CocosGUI.h"
using namespace ui;

#include "PlayScene.h"
#include "Network/NetworkConfigSetting.h"
#include "Network/network.h"

class StartAndLoginScene :public Scene
{
public:
	// 创建开始界面
	static Scene* CreateScene();

	// 初始化函数
	virtual bool init();

	// 创建对象
	CREATE_FUNC(StartAndLoginScene);

	// 创建按钮函数
	static MenuItemSprite* createGameButton(std::string normalPicPath, std::string pressedPicPath, const ccMenuCallback& callback);

private:

	// 预加载资源相关
	const int totalFileNums = 91;
	int curFileNums;

	// 页面相关
	Size pageSize;
	Vec2 pageCoord;

	// 加载进度显示
	Label* loadLabel;

	// 模式选择按键和结束按键
	MenuItemSprite* AIMode;
	MenuItemSprite* onlineMode;
	MenuItemSprite* exit;

	// 背景音乐
	SimpleAudioEngine* startSceneBGM;


	// 加载资源
	void LoadResource();

	// 加载图片资源
	bool GetAllpng(const std::string& path);

	// 加载音乐资源
	bool GetAllMusic(const std::string& path);

	// 加载资源回调函数
	void LoadingCallBack(Ref* pSender);

	// 开始游戏按键回调函数
	void AIModeToPlayScene(Ref* pSender);
	void onlineModeToPlayScene(Ref* pSender);

	// 退出按键回调函数
	void MenuToExit(Ref* pSender);
};