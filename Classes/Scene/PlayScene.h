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

	// �������ӻ����̣� positionΪ(0, 0)λ����ƭͼƬ���½ǵ�ê������
	void createBoard(Vec2 position);

	// �������ӻ��̵꣬positionΪ��һ�ſ�Ƭ���½ǵ�ê������
	void createShop(Vec2 position);

	// �������ӻ����ӿ�Ƭ
	MenuItemSprite* createPieceCard(string pieceName, string piecePicPath, Vec2 position, const ccMenuCallback& callback);

	// �������ս��
	void PlayScene::onBattleButtonClicked(Ref* sender);

	// �����£�����ҷ�����������Ϣ
	void packageInfo();

	// �����£���ȡ�Է�����������Ϣ
	void extractInfo();

	// ����ת������
	static ChessCoordinate* coordingrevert(Vec2 realPosition);

	CREATE_FUNC(PlayScene);

private:
	//��������
	ConPort* connection;

	// ����ģʽ�¶��ҷ�����������Ϣ�Ĵ��
	Packet myInfo;
	
	// ���յĵз�������Ϣ��
	Packet oppInfo;

	string url = "https://u207393-a5b8-10e2f069.westb.seetacloud.com:8443/v1/chat/completions";

	CurlWrapper chatAI;

	Label* AILabel = Label::createWithTTF("��ӭ������Ϸ���������AIС���֣�������Э����ս����", "fonts/Marker Felt.ttf", 24);;

	// AI ѡ�ֳ�
	AIMode* AI = nullptr;

	// ��Ϸģʽ
	string gameMode = "�˻���ս";

	// AI�������
	int AINum = 4;

	// ��ǰ��ս��AI
	int TargetAI = 0;

	// ��Ϸ״̬
	bool isInBattle = false;

	// �ܷ�������
	bool canBuyChess = true;

	// �Ƿ�����Ϣ������
	bool isTransmittingInfo = false;

	// ����ģʽ�Ƿ�ս
	bool beginFighting = false;

	// ������
	Layer* playLayer;

	// ����
	ChessBoard* chessBoardModel;
	vector<Sprite*> chessBoard[ROW_BOARD];


	//����̵��������
	Menu* previousMenu;
	// �̵�
	
	vector<MenuItemSprite*> shop;

	// �ҷ����
	PlayerInfo* playerME;

	// �������
	PlayerInfo* playerOPP;

	//�����ʾ�͵ȼ���ʾ
	Label* player_coin;
	Label* player_level;
	Label* player_lifevalue;

	//��սϯ��
	PreparationSeat* preArea;
	SimpleAudioEngine* playSceneBGM;

	//��������


	// �˳���ť�ĵ���¼�
	void menuExitCallBack(Ref* sender);

	// ����Ƭ�ĵ���¼�
	void menuPieceCardCallBack(Ref* sender);

	// ˢ���̵�ĵ���¼�
	void menuFreshShopCallBack(Ref* sender);

	// ������ĵ���¼�
	void menuBuyExpCallBack(Ref* sender);

	// �������ӵĵ���¼�
	void BuyChess(Ref* sender,int index);


	
};

#endif








// ����̵괥���¼���ˢ��
// ��������,Ҫ�����ڱ�սϯ
// �ŵ�ս����
// �Ҽ��ƶ�ССӢ��
// ս���߼�
// �ռ��ز�


