/*
功能：玩家模型信息
作者：Hu Junwei
*/
#pragma once

#ifndef _PLAYERINFO_H
#define _PLAYERINFO_H_
#include "Chess.h"
#include "Market.h"

USING_NS_CC;

const int NUMINPREAREA = 9;
const int OWNMAX = 8;
const int OWNINIT = 4;
const int BATTLEMAX = 7;
const int BATTLEINIT = 3;
const int PLAYERLEVELMAX = 10;
const int PLAYERLEVELINIT = 1;
//mlx:此处为不报错，注意更改
const int HERONUM = 9;
//

class PlayerInfo :public Ref
{
private:
	// 当前玩家等级
	int level;

	// 当前玩家排名
	int rankNum;

	// 当前玩家金币数目（经验用于升级和购买棋子）
	int coinNum;

	// 当前玩家可上场战斗的棋子的最大值
	int maxBattleChessNum;

	// 当前玩家生命值
	int lifeValue;

	// 当前玩家战斗区法师数量
	int mageNumInBattleArea;

	// 当前玩家战斗区射手数量
	int shooterNumInBattleArea;

	// 当前玩家战斗区坦克数量
	int tankNumInBattleArea;

	// 记录当前玩家全部棋子中各英雄的数量的数组
	int heroOneStarNumArr[HERONUM];
	int heroTwoStarNumArr[HERONUM];


	// 存储当前我方战斗区棋子
	vector<Chess*> chessInBattleArea;



	// 存储当前我方全部棋子
	vector<Chess*> chessTotal;


public:
	// 玩家信息初始化函数
	virtual bool init();

	// 创建对象
	CREATE_FUNC(PlayerInfo);

	// 存储当前我方备战区棋子
	vector<Chess*> chessInPreArea = vector<Chess*>(9, nullptr);

	//mlx:得到下标最小的空位
	int GetMinIndex();

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
//<<<<<<< Updated upstream
	
	// 返回指针数组的指针
	Chess** GetBattleChessNum()const;
//=======
//>>>>>>> Stashed changes

	// 获取当前能上场英雄最大值
	int getMaxBattleChessNum() const;
	// 对vector的修改函数接口
	//-------------------//
	// 作者：胡峻玮      //
	// 修改日期：12.14   //
	// ------------------//

	// 在玩家对战区放置棋子,输入坐标为棋盘坐标
	void putChessInBattleArea(Chess* chess);

	// 获取玩家对战区棋子集合
	vector<Chess*>* getBattleAreaChesses();

	// 在玩家备战区放置棋子
	void putChessInPreArea(Chess* chess);

	// 获取玩家备战区棋子集合
	vector<Chess*>* getPreAreaChesses();

	// 从玩家备战区移去棋子
	void removeChessFromPreArea(Chess* chess);

	// 以下是有关羁绊判断和升级星级的判断
	//-------------------//
	// 作者：胡峻玮      //
	// 修改日期：12.14   //
	// ------------------//


	// 羁绊判断函数
	void buffJudgment();

	// 数组置零函数
	void arrZero(int arrName[]);

	// 升星判断函数
	void raiseLevel();

	// 升星后低星英雄删除函数
	void deleteLowLevelChess(int heroFlag, int level);

	// 升星后高星英雄出现在战斗区函数
	void createHighLevelChess(int heroflag, int level);

	//友元声明
	friend class PreparationSeat;
};
#endif