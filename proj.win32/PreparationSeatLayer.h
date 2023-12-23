/*
* ���ܣ�ʵ�ֱ�սϯ���ӻ���������¼�
* ���ߣ�������
*/

#pragma once
#ifndef _PREPARATIONSEATLAYER_H_
#define  _PREPARATIONSEATLAYER_H_

#include "cocos2d.h"
USING_NS_CC;
#include "ui/CocosGUI.h" 
using namespace ui;
#include "PlayerInfo.h"
#include "Chess.h"
#include "ChessBoard.h"
#include "StartAndLoginScene.h"

//����ֻ����ǰ���Űڷ�
#define RSIZE 2
#define CSIZE 8

class PreparationSeat :public Ref
{
private:
	//��Ļ���
	Size pageSize;
	Vec2 pageCoord;
	//��սϯbutton����
	std::vector<Menu*> preAreaChessMenu = vector<Menu*>(9, nullptr);
	//����ָ��
	PlayerInfo* player;
	ChessBoard* cBoard;
	EventListenerMouse* listenerMouse;
	//��¼��ǰ����������
	int numOfBoard = 0;
	//��������λ�ü�¼
	shared_ptr<Chess> onBoardChess[RSIZE][CSIZE];
	Sprite* onBoardSprite[RSIZE][CSIZE];
public:
	// �����Ϣ��ʼ������
	virtual bool init(PlayerInfo* pr, ChessBoard* board, EventListenerMouse* listener, Layer* preArea);

	// ��������
	static PreparationSeat* create(PlayerInfo* pr, ChessBoard* board, EventListenerMouse* listener, Layer* preArea) {
		PreparationSeat* pRet = new(std::nothrow) PreparationSeat(); if (pRet && pRet->init(pr, board, listener, preArea)) {
			pRet->autorelease(); return pRet;
		}
		else {
			delete pRet; pRet = nullptr; return nullptr;
		}
	};
	void CallBack(Ref* sender, int index);
	//��սϯ��
	Layer* PreAreaLayer;

	//���ɱ�սϯ����
	void CreatePreAreaButton(shared_ptr<Chess> curHero, int index);

	//�����±�õ���սϯ����
	inline void GetCoordAndScale(int index, Vec2& coord, float& scale);

	Sprite* ChessToBoard(const Vec2& onBoardCoord, int index);

	void PromoteChessLevel(int index);
};
#endif
