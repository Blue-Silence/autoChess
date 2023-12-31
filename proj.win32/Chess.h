/******************************************************/
/*                  �ļ�����Chess.h                   */
/*                  ���ܣ�����ģ������                */
/*                  ���ߣ�֣ΰة                      */
/******************************************************/
#pragma once

#ifndef _CHESSPIECE_H_
#define _CHESSPIECE_H_
#include <string>
#include <vector>

#include "cocos2d.h"
#include "ConfigController.h"
#include "Condition.h"

// �Ǽ�����ǿ��
#define promoteScale 1.2

// ��ʱ�ôֲڵķ�ʽ�洢Ӣ�ۻ�������
#define DAJI 0
#define DIAOCHAN 1
#define HOUYI 2
#define DIRENJIE 3
#define XIANGYU 4
#define ZHANGFEI 5

// ����ֵ
double const hpData[6] = { 3229,3019,3182,3242,3500,3450 };
// ����ֵ
double const attackData[6] = { 170,168,180,169,157,153 };
// ����ֵ
double const defenceData[6] = { 86,89,86,95,103,125 };
// �������룬ָ������������ϵ������پ���
double const attackDistanceData[6] = { 3,3,2,2,1,1 };
// �������ܵĹ�������
const int skillCooldowns[6] = {4,4,3,3,4,4 };
// Ӣ����ͼ��ַ 
string const chessImagePaths[6] = { "/res/Hero/DaJi.png","/res/Hero/DiaoChan.png","/res/Hero/HouYi.png",
									"/res/Hero/DiRenJie.png","/res/Hero/XiangYu.png","/res/Hero/ZhangFei.png" };

using std::string;
using std::vector;

USING_NS_CC;

// ��������  ��Ļ�ϵ�����(���ӻ���)�������ϵ�����(��Ϸ�߼���)
enum class CoordinateType { screenCoordinates, chessBoardCoordinates};

 // ��������������
class ChessInfo 
{
public:
	// ʵʱ�������������
	double HP; 
	double maxHP;

	// ���������ͼӳɺ�Ĺ���
	double basicAttack; 
	double improvedAttack;

	// ���������ͼӳɺ�ķ�����
	double basicDefence; 
	double improvedDefence;

	// ������������ͼӳɺ�Ĺ�������
	double basicAttackDistance; 
	double improvedAttackDistance; 

	// ������ȴʱ��
	int skillCooldown;
};

// ����������
class ChessCoordinate 
{
public:

	ChessCoordinate() :x(0), y(0) {}

	// ��ȡ���Ӻ�����
	int getX() const ;

	// ��ȡ����������
	int getY() const;

	// �������Ӻ�����
	void setX(const int& X);

	// ��������������
	void setY(const int& Y);


private:
	// ������
	int x; 
	// ������
	int y; 
};



// ������
class Chess : public Ref
{
protected:
	// ��������
	int chessName; 

	// Ӣ��ְҵ
	string career;

	// ģ��ͼƬ���·��
	string chessImagePath; 

	// �Ǽ������Ϊ��
	int chessLevel=1; 

	// ���ӵĻ���
	int chessPrice; 

	// ���ӵĸ�������
	ChessInfo chessCondition; 

	// �����˺�Ч��
	float skillScale=2;

	// �����������е�λ��
	ChessCoordinate chessBoardCoordinate; 

	// ��������Ļ�е�λ�ã����ӻ���
	ChessCoordinate screenCoordinate; 

	//���ӿ��ӻ�ָ��
	Sprite* chessImage=nullptr;

	//��ͨ�������ӻ�
	Sprite* attackImage=nullptr;

	// ���ܹ������ӻ�ָ��
	Sprite* skillImage=nullptr;

	// ������
	ProgressTimer* hpBarProgress=nullptr;

	// ������
	ProgressTimer* mpBarProgress=nullptr;


public:
	
	// ��Ϸ�����ӵ�����λ�ã���仯
	ChessCoordinate inGameChessBoardCoordinate;

	// ��Ϸ�����ӵ���Ļλ�ã���仯
	ChessCoordinate inGameScreenCoordinate;

	// ���ӵĸ���״̬
	enum class State
	{
		Idle,
		Finding,
		Moving,
		Attacking,
		Dead
	};

	// ���ӵĵ�ǰ״̬
	State state = State::Idle;

	// ����Ŀ��
	shared_ptr<Chess> targetChess;

	// ����Ŀ�������λ��
	Vec2 targetPos;

	// �Ƿ������ƶ������ڹ����������ƶ�״̬�¿��������ƶ�Ҳ�����ƶ�ֹͣ��
	bool isMoving=false;
	bool isAttacking=true;

	// �չ�������
	double attackNum = 0;

	// ��ʼ������״̬
	void initCondition();

	// ��ȡӢ��ְҵ
	const string getCareer() const;

	// ��ȡ������ͼ
	const string getChessImagePath() const;

	// ��ȡ��������
	const int getChessName() const;

	// ��ȡ��ǰ������ֵ
	ChessInfo* getChessCondition();

	// ��ȡ��ǰ�����Ǽ�
	const int getChessLevel() const;

	// ������Ҫ���ͻ�ȡ��ǰ������Ļλ�û�����
	ChessCoordinate* getChessCoordinateByType(CoordinateType type) ;

	//������Ҫ�����趨���ӵ���Ļ�������������
	void setChessCoordinateByType(Vec2 position,CoordinateType type);

	// ���õ�ǰ�����Ǽ�
	void setChessLevel(const int newLevel);

	//���������Ǽ�
	void promoteRank(int dstRank);

	//���ܺ������̳�
	virtual void skill(Chess& OPP) = 0;

	//�Ч��, �̳�
	virtual void careerBuff() = 0;

	// remove �Ч��
	virtual void removeCareerBuff() = 0;

	//������������������Ĺ���ֵ,��Ϊ����Ŀ��ı���������������
	const int myAttack() const;

	//����������,����ĵ�ǰ����ֵ�۳������˺���������ķ���ֵ�Ĳ�ֵ
	void beenAttack(int oppAttack);

	//������Ĺ��������ͱ������������з�װ��ʵ��һ�ι�������.�����Ǳ������Ķ���
	void attackOne(Chess& OPP);

	//�ж������Ƿ�����
	const bool isDead() const ;


	//���ӵĿ��ӻ�
	Sprite* createChess(Vec2 chessPosition);
	Sprite* getChessSprite();

	// ��ͨ�����Ŀ��ӻ�
	virtual Sprite* createAttack()=0;
	Sprite* getAttackSprite();

	// ���ܹ����Ŀ��ӻ�
	virtual Sprite* createSkill() = 0;
	Sprite* getSkillSprite();

	// ��������ֵ
	void updateHpBar();

	// ���¼���ֵ
	void updateMpBar();

	// ���ظ�ֵ�����
	Chess& operator=(const Chess& other);
};


// ���������ӵĻ��࣬�����Ǿ���Ϊ��ְͬҵ�������ࣺ��ʦ�����ֺ�̹��

// ��ʦְҵ��
class mage : public Chess
{
public:

	// ��ʼ����������
	mage(int name);
	
	//���ܺ���
	virtual void skill(Chess& OPP) override;

	// �Ч������
	virtual void careerBuff() override;

	// ȥ���Ч��
	virtual void removeCareerBuff() override;

	// ��ͨ�������ӻ�ָ��
	virtual Sprite* createAttack() override;

	// ���ܹ������ӻ�ָ��
	virtual Sprite* createSkill() override;

};

// ����ְҵ��
class shooter : public Chess
{
public:

	// ��ʼ����������
	shooter(int name);

	// ��������,cocos2d-X�в���Ҫ�Լ��ṩ������������ʵ��
	//virtual ~shooter();

	//���ܺ���
	virtual void skill(Chess& OPP) override;

	// �Ч������
	virtual void careerBuff() override;

	// ȥ���Ч��
	virtual void removeCareerBuff() override;

	// ��ͨ�������ӻ�ָ��
	virtual Sprite* createAttack() override;

	// ���ܹ������ӻ�ָ��
	virtual Sprite* createSkill() override;
};

// ̹��ְҵ��
class tank : public Chess
{
public:
	// ��ʼ����������
	tank(int name);

	// ��������,cocos2d-X�в���Ҫ�Լ��ṩ������������ʵ��
	//~tank();

	//���ܺ���
	virtual void skill(Chess& OPP) override;

	// �Ч������
	virtual void careerBuff() override;

	// ȥ���Ч��
	virtual void removeCareerBuff() override;

	// ��ͨ�������ӻ�ָ��
	virtual Sprite* createAttack() override;

	// ���ܹ������ӻ�ָ��
	virtual Sprite* createSkill() override;
};
#endif
