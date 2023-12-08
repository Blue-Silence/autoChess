/********************************************
 * ���ܣ�����ģ��
 * ���ߣ�֣ΰة
 * �����޸����ڣ�12.08
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


 // ��������������
class ChessInfo 
{
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

	double basicCriticalChance; // ������������
	double improvedCriticalChance; // �ӳɺ�ı�������

	double basicCriticalDamage; // �����˺�
	double improvedCriticalDamage; // �ӳɺ�ı�������

	Condition myCondition;//��ǰ״̬
};


// ��������  ��Ļ�ϵ�����(���ӻ���)�������ϵ�����(��Ϸ�߼���)
enum class CoordinateType { coordinatesInScreen, coordinatesInChessBoard }; 


// ��������
class ChessCoordinate 
{
public:
	// ��ȡ���Ӻ�����
	int GetX() const;

	// ��ȡ����������
	int GetY() const;

	// �������Ӻ�����
	void SetX(const int x);

	// ��������������
	void SetY(const int y);

private:
	// ������
	int x; 
	// ������
	int y; 

	CoordinateType type; // ��������
};


// ���ӵ������Ǽ�
enum class Level { level1, level2, level3 };


// ������
class Chess : public Ref
{
protected:
	string chessName; // ����

	string chessImagePath; // ģ��ͼƬ���·��

	string chessInfoPath; // �������������ݱ��е�·��

	Level chessLevel; // �Ǽ�

	int chessPrice; // ���ӵĻ���

	ChessInfo chessNowCondition; // ��ǰ���Ӹ�������

	//ChessInfo buff; // ��ǰװ��Ч���ӳ�(��Ų���Ҫ)

	ChessCoordinate chessBoardCoordinate; // �����������е�λ��

	ChessCoordinate screenCoordinate; // ��������Ļ�е�λ��

	Sprite* chessImage;//ָ����ӻ�

public:

	// ��ʼ������״̬
	virtual bool init(int id);
	virtual bool init();

	// ��ʼ������״̬
	void initPieceIfo(int id);

	// ��������״̬
	bool updatePieceInfo(const double damage, ChessCoordinate* newScreenCoordinate);

	// ��ȡ��������
	const string getChessName();

	// ��ȡ��ǰ������ֵ
	const ChessInfo* getChessCondition();

	// ��ȡ��ǰ�����Ǽ�
	const Level getChessLevel();

	// ��ȡ��ǰ����λ��
	ChessCoordinate* getChessCoordinateByType(CoordinateType type);

	// ���õ�ǰ�����Ǽ�
	void setChessLevel(const Level newLevel);

	//��ȡ��������
	virtual string getTag() = 0;

	//CREATE_FUNC(ChessPiece);   ����ChessPiece�ǳ����಻��create

	//���ܺ������̳�
	virtual void skill() = 0;

	//�Ч��, �̳�
	virtual void careerBuff() = 0;

	//���������Ǽ�
	void promoteRank();

	//���װ��
	virtual void wearEquip(int equipNum, int equipType);


	//����buff���޸���������
	//virtual void readCondition();
	//virtual void setCondition(double s, int Condition);

	//�������������ع���ֵ,��Ϊ����Ŀ��ı���������������
	int myAttack();

	//����������,����ĵ�ǰ����ֵ�۳������˺���������ķ���ֵ�Ĳ�ֵ
	void beenAttack(int oppAttack);

	//������Ĺ��������ͱ������������з�װ��ʵ��һ�ι�������.�����Ǳ������Ķ���
	void attackOne(Chess& A);

	//�ж������Ƿ�����
	bool isDead();

	// ����ɶ
	int storageNum = 0;//�Ϸ�����Ϊ�������·�����Ϊ����

	//���ӵĿ��ӻ�
	Sprite* createChess(string chessName, string chessPicPath, Vec2 chessPosition);

	//����һ������ָ�룬�������Ѿ�������ս�����ӿ��ӻ��������������Ժ󣩣��������ָ����һ��ָ����󣬿���ͨ����ָ�������ɶ���Ч��
	Sprite* getChess();

	//�趨���ӵ�����,ָ��Ļ�ϵ�λ��
	void setVec2(Vec2 position);
};





// ���������ӵĻ��࣬�����Ǿ���Ϊ��ְͬҵ�������ࣺ��ʦ�����ֺ�̹��
// ��ʦְҵ��
class mage : public Chess
{
private:

	string career = "mage";
public:

	mage();

	~mage();

	//���Ӣ��ְҵ���
	string getCareer();
	
	//���ܺ���
	virtual void skill() override;

	// �Ч������
	virtual void careerBuff() override;
};

// ����ְҵ��
class shooter : public Chess
{
private:
	string career = "shooter";

public:

	shooter();

	~shooter();

	//���Ӣ��ְҵ���
	string getCareer();

	//���ܺ���
	virtual void skill() override;

	// �Ч������
	virtual void careerBuff() override;
};

// ̹��ְҵ��
class tank : public Chess
{
private:
	string career = "tank";

public:

	tank();

	~tank();

	//���Ӣ��ְҵ���
	string getCareer();

	//���ܺ���
	virtual void skill() override;

	// �Ч������
	virtual void careerBuff() override;
};
#endif
