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

//#include "PlayScene.h"

class StartAndLoginScene :public Scene
{
public:
	static Scene* CreateScene();

	virtual bool init();

	CREATE_FUNC(StartAndLoginScene);

private:
	//Ԥ������Դ���
	const int totalFileNums = 46;
	int curFileNums;
	//ҳ�����
	Size pageSize;
	Vec2 pageCoord;
	//���ؽ�����ʾ
	Label* loadLabel;
	//��ʼ���˳�����
	MenuItemImage* start;
	MenuItemImage* exit;

	//������Դ
	void LoadResource();
	//����ͼƬ��Դ
	bool GetAllpng(const std::string& path);
	//����������Դ
	bool GetAllMusic(const std::string& path);
	//������Դ�ص�����
	void LoadingCallBack(Ref* pSender);
	//��ʼ��Ϸ�����ص�����
	void MenuToPlayScene(Ref* pSender);
	//�˳������ص�����
	void MenuToExit(Ref* pSender);
};