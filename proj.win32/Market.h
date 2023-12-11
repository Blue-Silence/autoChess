/*
���ܣ��̵���Ϣ
���ߣ�Hu Junwei
*/
#pragma once

#ifndef _MARKET_H_
#define _MARKET_H_

#include <stdlib.h>
#include <time.h>
#include <vector>
#include "Storage.h"

const int MAXEQUIPPMENT = 6;
const int HEROCATAGORY = 6;

class Market :public Ref
{
private:
	// ���ӿ��������
	vector<Chess*>chessList;

	// װ�������װ��
	int equippment;

public:
	CREATE_FUNC(Market);

	// ��ʼ������
	virtual bool init();

	// ˢ���̵꺯��
	virtual bool RefreshMarket();

	// �ܷ�������Ȩ���ж�
	template<class ClassName,typename price>
	bool BoughtPermission(int money, int maxChessNum, int nowChessNum, ClassName* A, typename price);

	// ������
	template<class ClassName>
	ClassName BuyChess(ClassName* A);

	// ��װ��
	int BuyEquippment() const;

	// ������/װ��
	template<class ClassName>
	int SellOut(ClassName chess);
};

#endif