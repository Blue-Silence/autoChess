/******************************************************/
/*                  文件名：Chess.h                   */
/*                  功能：棋子模型声明                */
/*                  作者：郑伟丞                      */
/******************************************************/
#pragma once

#ifndef _CHESSPIECE_H_
#define _CHESSPIECE_H_
#include <string>
#include <vector>

#include "cocos2d.h"
#include "ConfigController.h"
#include "Condition.h"

// 星级提升强度
#define promoteScale 1.2

// 暂时用粗糙的方式存储英雄基本属性
#define DAJI 0
#define DIAOCHAN 1
#define HOUYI 2
#define DIRENJIE 3
#define XIANGYU 4
#define ZHANGFEI 5

// 生命值
double const hpData[6] = { 3229,3019,3182,3242,3500,3450 };
// 攻击值
double const attackData[6] = { 170,168,180,169,157,153 };
// 防御值
double const defenceData[6] = { 86,89,86,95,103,125 };
// 攻击距离，指与对手在棋盘上的曼哈顿距离
double const attackDistanceData[6] = { 3,3,2,2,1,1 };
// 触发技能的攻击次数
const int skillCooldowns[6] = {4,4,3,3,4,4 };
// 英雄贴图地址 
string const chessImagePaths[6] = { "/res/Hero/DaJi.png","/res/Hero/DiaoChan.png","/res/Hero/HouYi.png",
									"/res/Hero/DiRenJie.png","/res/Hero/XiangYu.png","/res/Hero/ZhangFei.png" };

using std::string;
using std::vector;

USING_NS_CC;

// 坐标类型  屏幕上的坐标(可视化用)和棋盘上的坐标(游戏逻辑用)
enum class CoordinateType { screenCoordinates, chessBoardCoordinates};

 // 棋子属性数据类
class ChessInfo 
{
public:
	// 实时生命和最大生命
	double HP; 
	double maxHP;

	// 基础攻击和加成后的攻击
	double basicAttack; 
	double improvedAttack;

	// 基础防御和加成后的防御力
	double basicDefence; 
	double improvedDefence;

	// 基础攻击距离和加成后的攻击距离
	double basicAttackDistance; 
	double improvedAttackDistance; 

	// 技能冷却时间
	int skillCooldown;
};

// 棋子坐标类
class ChessCoordinate 
{
public:

	ChessCoordinate() :x(0), y(0) {}

	// 获取棋子横坐标
	int getX() const ;

	// 获取棋子纵坐标
	int getY() const;

	// 设置棋子横坐标
	void setX(const int& X);

	// 设置棋子纵坐标
	void setY(const int& Y);


private:
	// 横坐标
	int x; 
	// 纵坐标
	int y; 
};



// 棋子类
class Chess : public Ref
{
protected:
	// 棋子名称
	int chessName; 

	// 英雄职业
	string career;

	// 模型图片相对路径
	string chessImagePath; 

	// 星级，最高为三
	int chessLevel=1; 

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
	Sprite* chessImage=nullptr;

	//普通攻击可视化
	Sprite* attackImage=nullptr;

	// 技能攻击可视化指针
	Sprite* skillImage=nullptr;

	// 生命条
	ProgressTimer* hpBarProgress=nullptr;

	// 技能条
	ProgressTimer* mpBarProgress=nullptr;


public:
	
	// 游戏中棋子的棋盘位置，会变化
	ChessCoordinate inGameChessBoardCoordinate;

	// 游戏中棋子的屏幕位置，会变化
	ChessCoordinate inGameScreenCoordinate;

	// 棋子的各自状态
	enum class State
	{
		Idle,
		Finding,
		Moving,
		Attacking,
		Dead
	};

	// 棋子的当前状态
	State state = State::Idle;

	// 攻击目标
	shared_ptr<Chess> targetChess;

	// 攻击目标的像素位置
	Vec2 targetPos;

	// 是否正在移动和正在攻击（处于移动状态下可能正在移动也可能移动停止）
	bool isMoving=false;
	bool isAttacking=true;

	// 普攻计数器
	double attackNum = 0;

	// 初始化棋子状态
	void initCondition();

	// 获取英雄职业
	const string getCareer() const;

	// 获取棋子贴图
	const string getChessImagePath() const;

	// 获取棋子名称
	const int getChessName() const;

	// 获取当前棋子数值
	ChessInfo* getChessCondition();

	// 获取当前棋子星级
	const int getChessLevel() const;

	// 根据需要类型获取当前棋子屏幕位置或棋盘
	ChessCoordinate* getChessCoordinateByType(CoordinateType type) ;

	//根据需要类型设定棋子的屏幕坐标或棋盘坐标
	void setChessCoordinateByType(Vec2 position,CoordinateType type);

	// 设置当前棋子星级
	void setChessLevel(const int newLevel);

	//提升棋子星级
	void promoteRank(int dstRank);

	//技能函数，继承
	virtual void skill(Chess& OPP) = 0;

	//羁绊效果, 继承
	virtual void careerBuff() = 0;

	// remove 羁绊效果
	virtual void removeCareerBuff() = 0;

	//攻击函数，返回自身的攻击值,作为攻击目标的被攻击函数的输入
	const int myAttack() const;

	//被攻击函数,自身的当前生命值扣除对手伤害量和自身的防御值的差值
	void beenAttack(int oppAttack);

	//将上面的攻击函数和被攻击函数进行封装，实现一次攻击动作.参数是被攻击的对象
	void attackOne(Chess& OPP);

	//判断棋子是否死亡
	const bool isDead() const ;


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
	void updateMpBar();

	// 重载赋值运算符
	Chess& operator=(const Chess& other);
};


// 以上是棋子的基类，以下是具体为不同职业的棋子类：法师、射手和坦克

// 法师职业类
class mage : public Chess
{
public:

	// 初始化棋子属性
	mage(int name);
	
	//技能函数
	virtual void skill(Chess& OPP) override;

	// 羁绊效果增益
	virtual void careerBuff() override;

	// 去除羁绊效果
	virtual void removeCareerBuff() override;

	// 普通攻击可视化指针
	virtual Sprite* createAttack() override;

	// 技能攻击可视化指针
	virtual Sprite* createSkill() override;

};

// 射手职业类
class shooter : public Chess
{
public:

	// 初始化棋子属性
	shooter(int name);

	// 销毁棋子,cocos2d-X中不需要自己提供虚析构函数的实现
	//virtual ~shooter();

	//技能函数
	virtual void skill(Chess& OPP) override;

	// 羁绊效果增益
	virtual void careerBuff() override;

	// 去除羁绊效果
	virtual void removeCareerBuff() override;

	// 普通攻击可视化指针
	virtual Sprite* createAttack() override;

	// 技能攻击可视化指针
	virtual Sprite* createSkill() override;
};

// 坦克职业类
class tank : public Chess
{
public:
	// 初始化棋子属性
	tank(int name);

	// 销毁棋子,cocos2d-X中不需要自己提供虚析构函数的实现
	//~tank();

	//技能函数
	virtual void skill(Chess& OPP) override;

	// 羁绊效果增益
	virtual void careerBuff() override;

	// 去除羁绊效果
	virtual void removeCareerBuff() override;

	// 普通攻击可视化指针
	virtual Sprite* createAttack() override;

	// 技能攻击可视化指针
	virtual Sprite* createSkill() override;
};
#endif
