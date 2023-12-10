/*
���ܣ����ģ����Ϣ
���ߣ�Hu Junwei
*/
#pragma once

#ifndef _PLAYERINFO_H
#define _PLAYERINFO_H_
#include "ChessPiece.h"
#include "Market.h"

USING_NS_CC;

const int OWNMAX = 8;
const int OWNINIT = 4;
const int BATTLEMAX = 7;
const int BATTLEINIT = 3;
const int LEVELMAX = 10;
const int LEVELINIT = 1;

class PlayerInfo :public Ref
{
private:
	// ��ǰ��ҵȼ�
	int level;

	// ��ǰ�������
	int rankNum;

	// ��ǰ��ҽ����Ŀ���������������͹������ӣ�
	int coinNum;

	// ��ǰ��ҿ���ӵ�е�����������Ŀ
	int maxOwnChessNum;

	// ��ǰ��ҿ��ϳ�ս�������ӵ����ֵ
	int maxBattleChessNum;

	// ��ǰ�������ֵ
	int lifeValue;

public:
	// �����Ϣ��ʼ������
	virtual bool InitPlayerInfo();

	// ��������
	CREATE_FUNC(PlayerInfo);

	// �����Լ����̵����
	Market market;

	// �����ȼ�����
	bool RaiseLevel(int coinNum, int level);

	// �޸ĵȼ��ӿں���
	void ChangeLevel();

	// ս��ʧ�ܿ۳�Ѫ������
	void DecreaseLifeValue(int hurt);

	// ��ȡ��ǰѪ���ӿں���
	int GetLifeValue() const;
	
	// ����ָ�������ָ��
	ChessPiece** GetBattleChessNum()const;

	// �����ӵ�е����ӣ���ֵ4�����ӣ�����8�����ӣ�
	ChessPiece* OwnChessNum[OWNMAX]{};

	// ��ҵ�ǰ�ϳ�ս�������ӣ���ֵ���Ϊ3�����ӣ�����7�����ӣ�
	ChessPiece* BattleChessNum[BATTLEMAX]{};

};
#endif