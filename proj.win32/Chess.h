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
#include "Csv.h"
#include "ConfigController.h"
#include "Equipment.h"
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
int const hpData[6] = {3229,3019,3182,3242,3500,3450};
// ����ֵ
int const mpData[6] = { 490,490,440,440,400,400 };
// ����ֵ
int const attackData[6] = { 170,168,180,169,157,153 };
// ����ֵ
int const defenceData[6] = { 86,89,86,95,103,125 };
// �������룬ָ������������ϵ������پ���
int const attackDistanceData[6] = { 3,3,2,2,1,1 };
// ����
int const mpCostData[6] = { 50,50,40,40,40,40 };

using std::string;
using std::vector;

USING_NS_CC;

 // ��������������
class ChessInfo 
{
public:
	double HP; // ʵʱ����
	double maxHP;//�������ֵ

	double MP; // ʵʱ����
	double maxMP; // �����

	double basicAttack; // ����������
	double improvedAttack;//�ӳɺ�Ĺ���

	double basicDefence; // ʵʱ������
	double imporovedDefence;//�ӳɺ�ķ�����

	double basicAttackDistance; // ������������
	double improvedAttackDistance; // �ӳɺ�Ĺ�������

	Condition myCondition;//��ǰ״̬
};


// ��������  ��Ļ�ϵ�����(���ӻ���)�������ϵ�����(��Ϸ�߼���)
enum class CoordinateType { screenCoordinates, chessBoardCoordinates};


// ��������
class ChessCoordinate 
{
public:
	// ��ȡ���Ӻ�����
	int GetX() const;

	// ��ȡ����������
	int GetY() const;

	// �������Ӻ�����
	void SetX(const int X) noexcept;

	// ��������������
	void SetY(const int Y);

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
	int chessName; // ����

	string career; // Ӣ��ְҵ

	string chessImagePath; // ģ��ͼƬ���·��

	string chessInfoPath; // �������������ݱ��е�·��

	int chessLevel; // �Ǽ�

	int chessPrice; // ���ӵĻ���

	ChessInfo chessCondition; // ��ǰ���Ӹ�������

	//ChessInfo buff; // ��ǰװ��Ч���ӳ�(��Ų���Ҫ)

	ChessCoordinate chessBoardCoordinate; // �����������е�λ��

	ChessCoordinate screenCoordinate; // ��������Ļ�е�λ��

	Sprite* chessImage;//ָ�����ӿ��ӻ�
	Sprite* attackImage;//ָ����ͨ�������ӻ�
	Sprite* skillImage;//ָ���ܿ��ӻ�


public:


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

	//��ȡ��������
	virtual string getTag() = 0;

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
	~shooter();

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
	~tank();

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
