/*
* ���ܣ���ʼ��ҳ����ʾ��������Դ
* ���ߣ�������
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
	// ������ʼ����
	static Scene* CreateScene();

	// ��ʼ������
	virtual bool init();

	// ��������
	CREATE_FUNC(StartAndLoginScene);

	// ������ť����
	static MenuItemSprite* createGameButton(std::string normalPicPath, std::string pressedPicPath, const ccMenuCallback& callback);

private:

	// Ԥ������Դ���
	const int totalFileNums = 91;
	int curFileNums;

	// ҳ�����
	Size pageSize;
	Vec2 pageCoord;

	// ���ؽ�����ʾ
	Label* loadLabel;

	// ģʽѡ�񰴼��ͽ�������
	MenuItemSprite* AIMode;
	MenuItemSprite* onlineMode;
	MenuItemSprite* exit;

	// ��������
	SimpleAudioEngine* startSceneBGM;


	// ������Դ
	void LoadResource();

	// ����ͼƬ��Դ
	bool GetAllpng(const std::string& path);

	// ����������Դ
	bool GetAllMusic(const std::string& path);

	// ������Դ�ص�����
	void LoadingCallBack(Ref* pSender);

	// ��ʼ��Ϸ�����ص�����
	void AIModeToPlayScene(Ref* pSender);
	void onlineModeToPlayScene(Ref* pSender);

	// �˳������ص�����
	void MenuToExit(Ref* pSender);
};