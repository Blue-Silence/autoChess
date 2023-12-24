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

	// �������ӻ����̣� positionΪ(0, 0)λ����ƭͼƬ���½ǵ�ê������
	void createBoard(Vec2 position);

	// �������ӻ��̵꣬positionΪ��һ�ſ�Ƭ���½ǵ�ê������
	void createShop(Vec2 position);

	// �������ӻ����ӿ�Ƭ
	MenuItemSprite* createPieceCard(string pieceName, string piecePicPath, Vec2 position, const ccMenuCallback& callback);

	void PlayScene::onBattleButtonClicked(Ref* sender);

	// ����ת������
	static ChessCoordinate* coordingrevert(Vec2 realPosition);

	CREATE_FUNC(PlayScene);

private:
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

	// ������
	Layer* playLayer;

	// ����
	ChessBoard* chessBoardModel;
	vector<Sprite*> chessBoard[ROW_BOARD];


	//����̵��������
	Menu* previousMenu;
	// �̵�
	
	vector<MenuItemSprite*> shop;

	// ���
	PlayerInfo* playerA;

	// �������
	PlayerInfo* playerOPP;

	//�����ʾ�͵ȼ���ʾ
	Label* player_coin;
	Label* player_level;
	//��սϯ��
	PreparationSeat* preArea;

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



