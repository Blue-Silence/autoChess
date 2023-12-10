/*
功能：玩家模型信息
作者：Hu Junwei
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
	// 当前玩家等级
	int level;

	// 当前玩家排名
	int rankNum;

	// 当前玩家金币数目（经验用于升级和购买棋子）
	int coinNum;

	// 当前玩家可以拥有的最多的棋子数目
	int maxOwnChessNum;

	// 当前玩家可上场战斗的棋子的最大值
	int maxBattleChessNum;

	// 当前玩家生命值
	int lifeValue;

public:
	// 玩家信息初始化函数
	virtual bool InitPlayerInfo();

	// 创建对象
	CREATE_FUNC(PlayerInfo);

	// 创建自己的商店对象
	Market market;

	// 提升等级函数
	bool RaiseLevel(int coinNum, int level);

	// 修改等级接口函数
	void ChangeLevel();

	// 战斗失败扣除血量函数
	void DecreaseLifeValue(int hurt);

	// 获取当前血量接口函数
	int GetLifeValue() const;
	
	// 返回指针数组的指针
	ChessPiece** GetBattleChessNum()const;

	// 玩家所拥有的棋子（初值4个棋子，上限8个棋子）
	ChessPiece* OwnChessNum[OWNMAX]{};

	// 玩家当前上场战斗的棋子（初值最大为3个棋子，上限7个棋子）
	ChessPiece* BattleChessNum[BATTLEMAX]{};

};
#endif