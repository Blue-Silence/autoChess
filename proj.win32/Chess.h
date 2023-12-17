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
#include "ConfigController.h"
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
int const hpData[6] = { 3229,3019,3182,3242,3500,3450 };
// 攻击值
int const attackData[6] = { 170,168,180,169,157,153 };
// 防御值
int const defenceData[6] = { 86,89,86,95,103,125 };
// 攻击距离，指与对手在棋盘上的曼哈顿距离
int const attackDistanceData[6] = { 3,3,2,2,1,1 };
// 触发技能的攻击次数
int const skillCooldowns[6] = { 3 ,3,3,3,4,4 };
// 英雄贴图地址  待添加
string const chessImagePaths[6] = { };
// 英雄技能贴图地址
//.....

using std::string;
using std::vector;

USING_NS_CC;

 // 棋子属性数据类
class ChessInfo 
{
public:
	double HP; // 实时生命
	double maxHP;//最大生命值

	double basicAttack; // 基础攻击力
	double improvedAttack;//加成后的攻击

	double basicDefence; // 实时防御力
	double improvedDefence;//加成后的防御力

	double basicAttackDistance; // 基础攻击距离
	double improvedAttackDistance; // 加成后的攻击距离

	// 技能冷却时间
	int skillCooldown;



	//Condition myCondition;//当前状态
};


// 坐标类型  屏幕上的坐标(可视化用)和棋盘上的坐标(游戏逻辑用)
enum class CoordinateType { screenCoordinates, chessBoardCoordinates};


// 棋子坐标
class ChessCoordinate 
{
public:
	// 获取棋子横坐标
	int getX() const;

	// 获取棋子纵坐标
	int getY() const;

	// 设置棋子横坐标
	void setX(const int X);

	// 设置棋子纵坐标
	void setY(const int Y);

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
	// 棋子名称
	int chessName; // 名称

	// 英雄职业
	string career; 

	// 模型图片相对路径
	string chessImagePath; 

	// 星级，最高为三
	int chessLevel; 

	// 棋子的花费
	int chessPrice; 

	// 棋子的各项属性
	ChessInfo chessCondition; 

	// 技能伤害效果
	float skillScale=2;

	// 棋子在棋盘中的位置
	ChessCoordinate chessBoardCoordinate; 

	// 棋子在屏幕中的位置，可视化用
	ChessCoordinate screenCoordinate; 

	//棋子可视化指针
	Sprite* chessImage;

	//普通攻击可视化
	Sprite* attackImage;

	// 技能攻击可视化指针
	Sprite* skillImage;

	// 生命条
	ProgressTimer* hpBarProgress;

	// 技能条
	ProgressTimer* mpBarProgress;


public:

	// 棋子当前状态
	enum class State
	{
		Idle,
		Finding,
		Moving,
		Attacking
	};
	State state = State::Idle;
	// 攻击目标的像素位置
	Vec2 targetPos;

	// 注意这里的状态和上面state的区分，state指一整个时间区间的状态，
	// 比如在Moving状态中也分 正在走这一步（isMoving）和走完这一步该走下一步(!isMoving)的区别
	bool isMoving=false;
	bool isAttacking=false;

	// 普攻计数器
	int attackNum = 0;

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

	// 更新生命值
	void updateHpBar();

	// 更新技能值
	void updateMpBar(int attackNum);

	Chess& operator=(const Chess& other);

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
	//~shooter();

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
	//~tank();

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
