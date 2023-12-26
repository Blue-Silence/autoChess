/*
���ܣ����ģ����Ϣ
���ߣ�Hu Junwei
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
const int BATTLE_LAYER_TAG = 100;
//mlx:�˴�Ϊ������ע�����
//hjw:���޸�
const int HERONUM = 6;
//

class PlayerInfo :public Ref
{
private:
	// ��ǰ��ҵȼ�
	int level;

	// ��ǰ�������
	int rankNum;

	// ��ǰ��ҽ����Ŀ���������������͹������ӣ�
	int coinNum;

	// ��ǰ��ҿ��ϳ�ս�������ӵ����ֵ
	int maxBattleChessNum;

	// ��ǰ�������ֵ
	int lifeValue;

	// ��ǰ���ս������ʦ����
	int mageNumInBattleArea;

	// ��ǰ���ս������������
	int shooterNumInBattleArea;

	// ��ǰ���ս����̹������
	int tankNumInBattleArea;

	// ��ǰ��ҵ������
	int buff[3];

	// ��¼��ǰ���ȫ�������и�Ӣ�۵�����������
	int heroOneStarNumArr[HERONUM];
	int heroTwoStarNumArr[HERONUM];

	// �洢��ǰ�ҷ�ս��������
	vector<shared_ptr<Chess>> chessInBattleArea;

	// �洢��ǰ�ҷ�ȫ������
	vector<shared_ptr<Chess>> chessTotal;


public:
	// �����Ϣ��ʼ������
	virtual bool init();

	// ��������
	CREATE_FUNC(PlayerInfo);

	// �洢��ǰ�ҷ���ս������
	vector<shared_ptr<Chess>> chessInPreArea = vector<shared_ptr<Chess>>(9, nullptr);

	//mlx:�õ��±���С�Ŀ�λ
	int GetMinIndex();

	//mlx:���³�������
	void ReBattleChess(shared_ptr<Chess> curChess, shared_ptr<Chess>newChess);

	// �����Լ����̵����
	Market market;

	// �����ȼ�����
	bool RaiseLevel(int coinNum, int level);

	// �޸ĵȼ��ӿں���
	void ChangeLevel();

	// ս��ʧ�ܿ۳�Ѫ������
	void DecreaseLifeValue(int hurt);

	// ��ȡ��ǰѪ���ӿں���
	int GetLifeValue() const;

	// ˢ�¿۳����
	void PayForRefresh();

	// ����Ӣ�ۿ۳����
	void payForHero();

	// ����Ӣ���Ǽ�����Ӣ�����ӽ��
	int getSellCoin(int index);

	// �ж�����Ƿ񻹴��
	bool isAlive() const { return lifeValue > 0; }

	// AIģʽר��:���ý������
	void setCoinNum(int num);

	// ��ȡ��ǰ��ҵȼ�
	int getLevel()const;

	int getcoin()const;
	// ��ȡ��ǰ���ϳ�Ӣ�����ֵ
	int getMaxBattleChessNum() const;
	// ��vector���޸ĺ����ӿ�
	//-------------------//
	// ���ߣ�������      //
	// �޸����ڣ�12.14   //
	// ------------------//

	// ����Ҷ�ս����������,��������Ϊ��������
	void putChessInBattleArea(shared_ptr<Chess> chess);

	// ��ȡ��Ҷ�ս�����Ӽ���
	vector<shared_ptr<Chess>>* getBattleAreaChesses();

	// ����ұ�ս����������
	void putChessInPreArea(shared_ptr<Chess> chess);

	// ��ȡ��ұ�ս�����Ӽ���
	vector<shared_ptr<Chess>>* getPreAreaChesses();

	// ����ұ�ս����ȥ����
	void removeChessFromPreArea(shared_ptr<Chess> chess);

	// �������й���жϺ������Ǽ����ж�
	//-------------------//
	// ���ߣ�������      //
	// �޸����ڣ�12.14   //
	// ------------------//


	// ��жϺ���
	int* buffJudgment();

	// �������㺯��
	void arrZero(int arrName[]);

	// �����жϺ���
	bool starRaiseLevel(int location, int &delLoc_1, int &delLoc_2);

	// ���Ǻ����Ӣ��ɾ������
	void deleteLowLevelChess(int heroFlag, int level, int location, int& delLoc_1, int& delLoc_2);

	// ���Ǻ����Ӣ�۳�����ս��������
	void createHighLevelChess(int heroflag, int level, int location);

	//��Ԫ����
	friend class PreparationSeat;
};
#endif