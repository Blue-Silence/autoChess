/********************************************
 * 功能：棋子模型
 * 作者：郑伟丞
 * 最新修改日期：12.08
 ********************************************/
#pragma once

#ifndef _CHESSPIECE_H_
#define _CHESSPIECE_H_
#include <string>
#include <vector>

#include "Csv.h"
#include "ConfigController.h"
#include"Equipment.h"
#include"Condition.h"

using std::string;
using std::vector;


 // 棋子属性数据类
class ChessInfo 
{
	double HP; // 实时生命
	double maxHP;//最大生命值

	double MP; // 实时法力
	double maxMP; // 最大法力

	double basicAttack; // 基础攻击力
	double improvedAttack;//加成后的攻击

	double basicDefence; // 实时防御力
	double imporovedDefence;//加成后的防御力

	double basicAttackDistance; // 基础攻击距离
	double improvedAttackDistance; // 加成后的攻击距离

	double basicCriticalChance; // 基础暴击几率
	double improvedCriticalChance; // 加成后的暴击几率

	double basicCriticalDamage; // 暴击伤害
	double improvedCriticalDamage; // 加成后的暴击几率

	Condition myCondition;//当前状态
};


// 坐标类型  屏幕上的坐标(可视化用)和棋盘上的坐标(游戏逻辑用)
enum class CoordinateType { coordinatesInScreen, coordinatesInChessBoard }; 


// 棋子坐标
class ChessCoordinate 
{
public:
	// 获取棋子横坐标
	int GetX() const;

	// 获取棋子纵坐标
	int GetY() const;

	// 设置棋子横坐标
	void SetX(const int x);

	// 设置棋子纵坐标
	void SetY(const int y);

private:
	// 横坐标
	int x; 
	// 纵坐标
	int y; 

	CoordinateType type; // 坐标类型
};


// 棋子的三种星级
enum class Level { level1, level2, level3 };


// 棋子类
class Chess : public Ref
{
protected:
	string chessName; // 名称

	string chessImagePath; // 模型图片相对路径

	string chessInfoPath; // 基础属性在数据表中的路径

	Level chessLevel; // 星级

	int chessPrice; // 棋子的花费

	ChessInfo chessNowCondition; // 当前棋子各项属性

	//ChessInfo buff; // 当前装备效果加成(大概不需要)

	ChessCoordinate chessBoardCoordinate; // 棋子在棋盘中的位置

	ChessCoordinate screenCoordinate; // 棋子在屏幕中的位置

	Sprite* chessImage;//指向可视化

public:

	// 初始化棋子状态
	virtual bool init(int id);
	virtual bool init();

	// 初始化棋子状态
	void initPieceIfo(int id);

	// 更新棋子状态
	bool updatePieceInfo(const double damage, ChessCoordinate* newScreenCoordinate);

	// 获取棋子名称
	const string getChessName();

	// 获取当前棋子数值
	const ChessInfo* getChessCondition();

	// 获取当前棋子星级
	const Level getChessLevel();

	// 获取当前棋子位置
	ChessCoordinate* getChessCoordinateByType(CoordinateType type);

	// 设置当前棋子星级
	void setChessLevel(const Level newLevel);

	//获取棋子类型
	virtual string getTag() = 0;

	//CREATE_FUNC(ChessPiece);   这里ChessPiece是抽象类不能create

	//技能函数，继承
	virtual void skill() = 0;

	//羁绊效果, 继承
	virtual void careerBuff() = 0;

	//提升棋子星级
	void promoteRank();

	//佩戴装备
	virtual void wearEquip(int equipNum, int equipType);


	//计算buff并修改自身属性
	//virtual void readCondition();
	//virtual void setCondition(double s, int Condition);

	//攻击函数，返回攻击值,作为攻击目标的被攻击函数的输入
	int myAttack();

	//被攻击函数,自身的当前生命值扣除对手伤害量和自身的防御值的差值
	void beenAttack(int oppAttack);

	//将上面的攻击函数和被攻击函数进行封装，实现一次攻击动作.参数是被攻击的对象
	void attackOne(Chess& A);

	//判断棋子是否死亡
	bool isDead();

	// 这是啥
	int storageNum = 0;//上方棋子为负数，下方棋子为正数

	//棋子的可视化
	Sprite* createChess(string chessName, string chessPicPath, Vec2 chessPosition);

	//返回一个精灵指针，当精灵已经被当作战斗棋子可视化（即放上棋盘以后），这个精灵指针有一个指向对象，可以通过对指针操作完成动画效果
	Sprite* getChess();

	//设定棋子的坐标,指屏幕上的位置
	void setVec2(Vec2 position);
};





// 以上是棋子的基类，以下是具体为不同职业的棋子类：法师、射手和坦克
// 法师职业类
class mage : public Chess
{
private:

	string career = "mage";
public:

	mage();

	~mage();

	//获得英雄职业类别
	string getCareer();
	
	//技能函数
	virtual void skill() override;

	// 羁绊效果增益
	virtual void careerBuff() override;
};

// 射手职业类
class shooter : public Chess
{
private:
	string career = "shooter";

public:

	shooter();

	~shooter();

	//获得英雄职业类别
	string getCareer();

	//技能函数
	virtual void skill() override;

	// 羁绊效果增益
	virtual void careerBuff() override;
};

// 坦克职业类
class tank : public Chess
{
private:
	string career = "tank";

public:

	tank();

	~tank();

	//获得英雄职业类别
	string getCareer();

	//技能函数
	virtual void skill() override;

	// 羁绊效果增益
	virtual void careerBuff() override;
};
#endif
