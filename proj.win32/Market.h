/*
功能：商店信息
作者：Hu Junwei
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
	

	// 装备库里的装备
	int equippment;

public:
	// 棋子库里的棋子
	vector<shared_ptr<Chess>>chessList;

	CREATE_FUNC(Market);

	// 初始化函数
	virtual bool init();

	// 刷新商店函数
	virtual bool RefreshMarket();

	// 能否购买棋子权限判断
	template<class ClassName,typename price>
	bool BoughtPermission(int money, int maxChessNum, int nowChessNum, ClassName* A, typename price);

	// 买棋子
	template<class ClassName>
	ClassName BuyChess(ClassName* A);

	// 买装备
	int BuyEquippment() const;

	// 卖棋子/装备
	template<class ClassName>
	int SellOut(ClassName chess);

	//买棋子
	Chess* GetChessByIndex(int index);
};

#endif