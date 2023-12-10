/*
功能：棋子存储器信息
作者：Hu Junwei
*/
#pragma once

#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <vector>
#include "ChessPiece.h"

using std::vector;

class Storage :public Ref
{
private:
	vector<tank>tankVector;
	vector<shooter>shooterVector;
	vector<mage>mageVector;

	// 清空三个卡牌存储器
	virtual bool init();

	// xxxMemorize就是进入卡库 xxxDelete就是从卡库中删除
	void tankMemorize(tank obj);
	void tankDelete(tank* ptr);
	void shooterMemorize(shooter obj);
	void shooterDelete(shooter* ptr);
	void mageMemorize(mage obj);
	void mageDelete(mage* ptr);
};


#endif

