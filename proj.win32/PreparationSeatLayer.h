/*
* ���ܣ�ʵ�ֱ�սϯ���ӻ���������¼�
* ���ߣ�������
*/

#pragma once
#ifndef _PREPARATIONSEATLAYER_H_
#define  _PREPARATIONSEATLAYER_H_

#include "cocos2d.h"
USING_NS_CC;
#include "PlayerInfo.h"
#include "Chess.h"
#include "StartAndLoginScene.h"

class PreparationSeat:public Ref
{
private:
	Size pageSize;
	Vec2 pageCoord;
	//��սϯbutton����
	std::vector<Button*> preAreaChessButton;
public:
	// �����Ϣ��ʼ������
	virtual bool init();

	// ��������
	CREATE_FUNC(PreparationSeat);

	//��սϯ��
	Layer* PreAreaLayer;

	//���ɱ�սϯ����
	void CreatePreAreaButton(Chess* curHero,int index);

	//�ϳ��ص�����
	void menuToBattleCallBack(Ref* sender);

	//�����±�õ���սϯ����
	inline void GetCoordAndScale(int index, Vec2& coord, float& scale);

	//������¼�
	void ChessBindMouseEvent(int index);
};
#endif
