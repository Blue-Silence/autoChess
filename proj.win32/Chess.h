/********************************************
 * 功能：棋子模型头文件
 * 作者：郑伟丞
 * 最新修改日期：12.09
 ********************************************/
#pragma once

#ifndef _CHESSPIECE_H_
#define _CHESSPIECE_H_
#include <string>
#include <vector>

#include "cocos2d.h"
#include "Csv.h"
#include "ConfigController.h"
#include "Equipment.h"
#include "Condition.h"

#define promoteScale 1.2
#define maxX 7
#define maxY 4

// 暂时用粗糙的方式存储英雄基本属性
#define DAJI 0
#define DIAOCHAN 1
#define HOUYI 2
#define DIRENJIE 3
#define XIANGYU 4
#define ZHANGFEI 5

// 生命值
int const hpData[6] = {3229,3019,3182,3242,3500,3450};
// 法力值
int const mpData[6] = { 490,490,440,440,400,400 };
// 攻击值
int const attackData[6] = { 170,168,180,169,157,153 };
// 防御值
int const defenceData[6] = { 86,89,86,95,103,125 };
// 攻击距离，指与对手在棋盘上的曼哈顿距离
int const attackDistanceData[6] = { 3,3,2,2,1,1 };
// 蓝耗
int const mpCostData[6] = { 50,50,40,40,40,40 };

using std::string;
using std::vector;

USING_NS_CC;

 // 棋子属性数据类
class ChessInfo 
{
public:
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

	Condition myCondition;//当前状态
};


// 坐标类型  屏幕上的坐标(可视化用)和棋盘上的坐标(游戏逻辑用)
enum class CoordinateType { screenCoordinates, chessBoardCoordinates};


// 棋子坐标
class ChessCoordinate 
{
public:
	// 获取棋子横坐标
	int GetX() const;

	// 获取棋子纵坐标
	int GetY() const;

	// 设置棋子横坐标
	void SetX(const int X) noexcept;

	// 设置棋子纵坐标
	void SetY(const int Y);

private:
	// 横坐标
	int x; 
	// 纵坐标
	int y; 

	//CoordinateType type; // 坐标类型
};



// 棋子类
class Chess : public Ref
{
protected:
	int chessName; // 名称

	string career; // 英雄职业

	string chessImagePath; // 模型图片相对路径

	string chessInfoPath; // 基础属性在数据表中的路径

	int chessLevel; // 星级

	int chessPrice; // 棋子的花费

	ChessInfo chessCondition; // 当前棋子各项属性

	//ChessInfo buff; // 当前装备效果加成(大概不需要)

	ChessCoordinate chessBoardCoordinate; // 棋子在棋盘中的位置

	ChessCoordinate screenCoordinate; // 棋子在屏幕中的位置

	Sprite* chessImage;//指向棋子可视化
	Sprite* attackImage;//指向普通攻击可视化
	Sprite* skillImage;//指向技能可视化


public:


	// 初始化棋子状态
	void initCondition();
	//void initPieceIfo();

	// 获取英雄职业
	string GetCareer();

	// 更新棋子状态
	//bool updatePieceInfo(const double damage, ChessCoordinate* newScreenCoordinate);

	// 获取棋子贴图
	const string getChessImagePath();

	// 获取棋子名称
	const int getChessName();

	// 获取当前棋子数值
	const ChessInfo* getChessCondition();

	// 获取当前棋子星级
	const int getChessLevel();

	// 根据需要类型获取当前棋子屏幕位置或棋盘
	ChessCoordinate* getChessCoordinateByType(CoordinateType type);

	//根据需要类型设定棋子的屏幕坐标或棋盘坐标
	void setChessCoordinateByType(Vec2 position,CoordinateType type);

	// 设置当前棋子星级
	void setChessLevel(const int newLevel);

	//获取棋子类型
	virtual string getTag() = 0;

	//CREATE_FUNC(ChessPiece);   这里ChessPiece是抽象类不能create

	//技能函数，继承
	virtual void skill(Chess& OPP) = 0;

	//羁绊效果, 继承
	virtual void careerBuff() = 0;

	//提升棋子星级
	void promoteRank();

	//佩戴装备
	void wearEquip(int equipNum, int equipType);
	

	//计算buff并修改自身属性
	//virtual void readCondition();
	//virtual void setCondition(double s, int Condition);

	//攻击函数，返回攻击值,作为攻击目标的被攻击函数的输入
	int myAttack();

	//被攻击函数,自身的当前生命值扣除对手伤害量和自身的防御值的差值
	void beenAttack(int oppAttack);

	//将上面的攻击函数和被攻击函数进行封装，实现一次攻击动作.参数是被攻击的对象
	void attackOne(Chess& OPP);

	//判断棋子是否死亡
	bool isDead();

	// 这是啥
	int storageNum = 0;//上方棋子为负数，下方棋子为正数

	//棋子的可视化
	Sprite* createChess(Vec2 chessPosition);
	Sprite* getChessSprite();

	// 普通攻击的可视化
	virtual Sprite* createAttack()=0;
	Sprite* getAttackSprite();

	// 技能攻击的可视化
	virtual Sprite* createSkill() = 0;
	Sprite* getSkillSprite();

	
};





// 以上是棋子的基类，以下是具体为不同职业的棋子类：法师、射手和坦克
// 法师职业类
class mage : public Chess
{
private:
	string career = "mage";

public:

	// 初始化棋子属性
	mage(int name);
	
	//技能函数
	virtual void skill(Chess& OPP) override;

	// 羁绊效果增益
	virtual void careerBuff() override;

	// 普通攻击可视化指针
	virtual Sprite* createAttack() override;

	// 技能攻击可视化指针
	virtual Sprite* createSkill() override;

};

// 射手职业类
class shooter : public Chess
{
private:
	string career = "shooter";

public:

	// 初始化棋子属性
	shooter(int name);

	// 销毁棋子
	~shooter();

	//技能函数
	virtual void skill(Chess& OPP) override;

	// 羁绊效果增益
	virtual void careerBuff() override;

	// 普通攻击可视化指针
	virtual Sprite* createAttack() override;

	// 技能攻击可视化指针
	virtual Sprite* createSkill() override;
};

// 坦克职业类
class tank : public Chess
{
private:
	string career = "tank";

public:
	// 初始化棋子属性
	tank(int name);

	// 销毁棋子
	~tank();

	//技能函数
	virtual void skill(Chess& OPP) override;

	// 羁绊效果增益
	virtual void careerBuff() override;

	// 普通攻击可视化指针
	virtual Sprite* createAttack() override;

	// 技能攻击可视化指针
	virtual Sprite* createSkill() override;
};
#endif
