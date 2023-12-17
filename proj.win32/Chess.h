/********************************************
 * ���ܣ�����ģ��ͷ�ļ�
 * ���ߣ�֣ΰة
 * �����޸����ڣ�12.09
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

// ��ʱ�ôֲڵķ�ʽ�洢Ӣ�ۻ�������
#define DAJI 0
#define DIAOCHAN 1
#define HOUYI 2
#define DIRENJIE 3
#define XIANGYU 4
#define ZHANGFEI 5

// ����ֵ
int const hpData[6] = { 3229,3019,3182,3242,3500,3450 };
// ����ֵ
int const attackData[6] = { 170,168,180,169,157,153 };
// ����ֵ
int const defenceData[6] = { 86,89,86,95,103,125 };
// �������룬ָ������������ϵ������پ���
int const attackDistanceData[6] = { 3,3,2,2,1,1 };
// �������ܵĹ�������
int const skillCooldowns[6] = { 3 ,3,3,3,4,4 };
// Ӣ����ͼ��ַ  �����
string const chessImagePaths[6] = { };
// Ӣ�ۼ�����ͼ��ַ
//.....

using std::string;
using std::vector;

USING_NS_CC;

 // ��������������
class ChessInfo 
{
public:
	double HP; // ʵʱ����
	double maxHP;//�������ֵ

	double basicAttack; // ����������
	double improvedAttack;//�ӳɺ�Ĺ���

	double basicDefence; // ʵʱ������
	double improvedDefence;//�ӳɺ�ķ�����

	double basicAttackDistance; // ������������
	double improvedAttackDistance; // �ӳɺ�Ĺ�������

	// ������ȴʱ��
	int skillCooldown;



	//Condition myCondition;//��ǰ״̬
};


// ��������  ��Ļ�ϵ�����(���ӻ���)�������ϵ�����(��Ϸ�߼���)
enum class CoordinateType { screenCoordinates, chessBoardCoordinates};


// ��������
class ChessCoordinate 
{
public:
	// ��ȡ���Ӻ�����
	int getX() const;

	// ��ȡ����������
	int getY() const;

	// �������Ӻ�����
	void setX(const int X);

	// ��������������
	void setY(const int Y);

private:
	// ������
	int x; 
	// ������
	int y; 

	//CoordinateType type; // ��������
};



// ������
class Chess : public Ref
{
protected:
	// ��������
	int chessName; // ����

	// Ӣ��ְҵ
	string career; 

	// ģ��ͼƬ���·��
	string chessImagePath; 

	// �Ǽ������Ϊ��
	int chessLevel; 

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
	Sprite* chessImage;

	//��ͨ�������ӻ�
	Sprite* attackImage;

	// ���ܹ������ӻ�ָ��
	Sprite* skillImage;

	// ������
	ProgressTimer* hpBarProgress;

	// ������
	ProgressTimer* mpBarProgress;


public:

	// ���ӵ�ǰ״̬
	enum class State
	{
		Idle,
		Finding,
		Moving,
		Attacking
	};
	State state = State::Idle;
	// ����Ŀ�������λ��
	Vec2 targetPos;

	// ע�������״̬������state�����֣�stateָһ����ʱ�������״̬��
	// ������Moving״̬��Ҳ�� ��������һ����isMoving����������һ��������һ��(!isMoving)������
	bool isMoving=false;
	bool isAttacking=false;

	// �չ�������
	int attackNum = 0;

	// ��ʼ������״̬
	void initCondition();
	//void initPieceIfo();

	// ��ȡӢ��ְҵ
	string GetCareer();

	// ��������״̬
	//bool updatePieceInfo(const double damage, ChessCoordinate* newScreenCoordinate);

	// ��ȡ������ͼ
	const string getChessImagePath();

	// ��ȡ��������
	const int getChessName();

	// ��ȡ��ǰ������ֵ
	const ChessInfo* getChessCondition();

	// ��ȡ��ǰ�����Ǽ�
	const int getChessLevel();

	// ������Ҫ���ͻ�ȡ��ǰ������Ļλ�û�����
	ChessCoordinate* getChessCoordinateByType(CoordinateType type);

	//������Ҫ�����趨���ӵ���Ļ�������������
	void setChessCoordinateByType(Vec2 position,CoordinateType type);

	// ���õ�ǰ�����Ǽ�
	void setChessLevel(const int newLevel);

	//CREATE_FUNC(ChessPiece);   ����ChessPiece�ǳ����಻��create

	//���ܺ������̳�
	virtual void skill(Chess& OPP) = 0;

	//�Ч��, �̳�
	virtual void careerBuff() = 0;

	//���������Ǽ�
	void promoteRank();

	//���װ��
	void wearEquip(int equipNum, int equipType);
	

	//����buff���޸���������
	//virtual void readCondition();
	//virtual void setCondition(double s, int Condition);

	//�������������ع���ֵ,��Ϊ����Ŀ��ı���������������
	int myAttack();

	//����������,����ĵ�ǰ����ֵ�۳������˺���������ķ���ֵ�Ĳ�ֵ
	void beenAttack(int oppAttack);

	//������Ĺ��������ͱ������������з�װ��ʵ��һ�ι�������.�����Ǳ������Ķ���
	void attackOne(Chess& OPP);

	//�ж������Ƿ�����
	bool isDead();

	// ����ɶ
	int storageNum = 0;//�Ϸ�����Ϊ�������·�����Ϊ����

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
	void updateMpBar(int attackNum);

	Chess& operator=(const Chess& other);

};





// ���������ӵĻ��࣬�����Ǿ���Ϊ��ְͬҵ�������ࣺ��ʦ�����ֺ�̹��
// ��ʦְҵ��
class mage : public Chess
{
private:
	string career = "mage";

public:

	// ��ʼ����������
	mage(int name);
	
	//���ܺ���
	virtual void skill(Chess& OPP) override;

	// �Ч������
	virtual void careerBuff() override;

	// ��ͨ�������ӻ�ָ��
	virtual Sprite* createAttack() override;

	// ���ܹ������ӻ�ָ��
	virtual Sprite* createSkill() override;

};

// ����ְҵ��
class shooter : public Chess
{
private:
	string career = "shooter";

public:

	// ��ʼ����������
	shooter(int name);

	// ��������
	//~shooter();

	//���ܺ���
	virtual void skill(Chess& OPP) override;

	// �Ч������
	virtual void careerBuff() override;

	// ��ͨ�������ӻ�ָ��
	virtual Sprite* createAttack() override;

	// ���ܹ������ӻ�ָ��
	virtual Sprite* createSkill() override;
};

// ̹��ְҵ��
class tank : public Chess
{
private:
	string career = "tank";

public:
	// ��ʼ����������
	tank(int name);

	// ��������
	//~tank();

	//���ܺ���
	virtual void skill(Chess& OPP) override;

	// �Ч������
	virtual void careerBuff() override;

	// ��ͨ�������ӻ�ָ��
	virtual Sprite* createAttack() override;

	// ���ܹ������ӻ�ָ��
	virtual Sprite* createSkill() override;
};
#endif
