/*
* ���ܣ�ʵ�ֱ�սϯ���ӻ���������¼�
* ���ߣ�������
*/

#pragma once
#ifndef _PREPARATIONSEATLAYER_H_
#define  _PREPARATIONSEATLAYER_H_

#include <utility>

#include "cocos2d.h"
USING_NS_CC;
#include "ui/CocosGUI.h" 
using namespace ui;
#include "PlayerInfo.h"
#include "Chess.h"
#include "ConfigController.h"
#include "ChessBoard.h"
#include "StartAndLoginScene.h"

//����ֻ����ǰ���Űڷ�
#define RSIZE 2
#define CSIZE 8

class PreparationSeat :public Ref
{
private:
	// ��Ļ���
	Size pageSize;
	Vec2 pageCoord;

	// ��սϯbutton����
	std::vector<Menu*> preAreaChessMenu = vector<Menu*>(9, nullptr);

	// ����ָ��
	PlayerInfo* player;
	ChessBoard* cBoard;
	EventListenerMouse* listenerMouse;
	Label* playerCoin;

	//��¼��ǰ����������
	int numOfBoard = 0;

	//��������λ�ü�¼
	shared_ptr<Chess> onBoardChess[RSIZE][CSIZE];
	Sprite* onBoardSprite[RSIZE][CSIZE];

	//�Ƿ���ս����
	bool* canBuyChess;

	//��ǰѡ�б�սϯ
	pair < Menu*, int >curHero = { nullptr, -1 };

	//�ͼ��
	Sprite* PreviousBuffmage;
	Sprite* PreviousBuffshooter;
	Sprite* PreviousBufftank;

public:

	// �����Ϣ��ʼ������
	virtual bool init(PlayerInfo* pr, ChessBoard* board, EventListenerMouse* listener, Layer* preArea, bool* buyChess,Label* player_coin);

	// ��������
	static PreparationSeat* create(PlayerInfo* pr, ChessBoard* board, EventListenerMouse* listener, Layer* preArea, bool* buyChess, Label* player_coin) {
		PreparationSeat* pRet = new(std::nothrow) PreparationSeat(); if (pRet && pRet->init(pr, board, listener, preArea, buyChess,player_coin)) {
			pRet->autorelease(); return pRet;
		}
		else {
			delete pRet; pRet = nullptr; return nullptr;
		}
	};

	void CallBack(Ref* sender, int index);

	//��սϯ��
	Layer* PreAreaLayer;

	//�ͼƬ�ı�
	void PreparationSeat::ChangeBuffImage();

	//���ɱ�սϯ����
	void CreatePreAreaButton(shared_ptr<Chess> curHero, int index);

	//�����±�õ���սϯ����
	inline void GetCoordAndScale(int index, Vec2& coord, float& scale);

	Sprite* ChessToBoard(const Vec2& onBoardCoord, int index);

	void PromoteChessLevel(int index);

	//������սϯ���ӵĻص�����
	void SellOut(Ref* sender);
};
#endif
