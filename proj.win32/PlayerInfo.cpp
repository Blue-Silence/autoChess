/*
���ܣ����ģ����Ϣ
���ߣ�Hu Junwei
*/
#include "PlayerInfo.h"

//-----------------------------------------------------//
//������������                                         //
//�������ܣ���ʼ����ҵ���Ϣ                           //
//��������ֵ��bool��                                   //
//����ע������(ѡ)����                                 //
//-----------------------------------------------------//
bool PlayerInfo::init()
{
	coinNum = 3;
	lifeValue = 50;
	level = PLAYERLEVELINIT;
	maxBattleChessNum = BATTLEINIT;
	mageNumInBattleArea = shooterNumInBattleArea = tankNumInBattleArea = 0;
	//mageNumTotal = shooterNumTotal = tankNumTotal = 0;
	heroOneStarNumArr[HERONUM] = { 0 };
	heroTwoStarNumArr[HERONUM] = { 0 };

	return true;
}

//-----------------------------------------------------//
//����������int coinNum ,int level                     //
//�������ܣ�������ҵȼ�                               //
//��������ֵ��bool��                                   //
//����ע������(ѡ)����                                 //
//-----------------------------------------------------//
bool PlayerInfo::RaiseLevel(int coinNum, int level)
{
	if (coinNum < 4 || level == PLAYERLEVELMAX)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//-----------------------------------------------------//
//������������					                       //
//�������ܣ�������ҵȼ�                               //
//��������ֵ��void                                     //
//����ע�����1.����Ѿ��ڴ˺����п۳��������ظ����� //
//              2.���и��ȼ��йص����ݾ��ڴ˺������޸� //
//                �����ڱ��ظ�����                   //
//-----------------------------------------------------//
void PlayerInfo::ChangeLevel()
{
	// ���� �۳����
	++level;
	coinNum -= 4;

	// �������޸ģ�3��5��7��9������
	if (level == 3 || level == 5 || level == 7 || level == 9)
	{
		++maxBattleChessNum;
	}
}

//-----------------------------------------------------//
//������������                                         //
//�������ܣ�ս����۳�Ѫ��                             //
//��������ֵ��void                                     //
//����ע������(ѡ)����                                 //
//-----------------------------------------------------//
void PlayerInfo::DecreaseLifeValue(int hurt)
{
	lifeValue -= hurt;
}

//-----------------------------------------------------//
//������������                                         //
//�������ܣ���ȡ��ҵ�ǰѪ��                           //
//��������ֵ��int                                      //
//����ע������(ѡ)����                                 //
//-----------------------------------------------------//
int PlayerInfo::GetLifeValue() const
{
	return lifeValue;
}

int PlayerInfo::getMaxBattleChessNum() const
{
	return maxBattleChessNum;
}


// ����Ҷ�ս����������
void PlayerInfo::putChessInBattleArea(shared_ptr<Chess> chess)
{
	chessInBattleArea.push_back(chess);
}

// ��ȡ��Ҷ�ս�����Ӽ���
vector<shared_ptr<Chess>>* PlayerInfo::getBattleAreaChesses()
{
	return &chessInBattleArea;
}

// ����ұ�ս����������
void PlayerInfo::putChessInPreArea(shared_ptr<Chess> chess)
{
	chessInPreArea.push_back(chess);
}

// ��ȡ��ұ�ս�������Ӽ���
vector<shared_ptr<Chess>>* PlayerInfo::getPreAreaChesses()
{
	return &chessInPreArea;
}

// �����A��ս����ȥ����
void PlayerInfo::removeChessFromPreArea(shared_ptr<Chess> chess)
{
	// ��ȥ��һ��ƥ������Ӽ���
	for (auto it = chessInPreArea.begin(); it != chessInPreArea.end(); ++it)
	{
		if (*it == chess)
		{
			chessInPreArea.erase(it);
			break;
		}
	}
}

// �������й���жϺ������Ǽ����ж�
//-------------------//
// ���ߣ�������      //
// �޸����ڣ�12.15   //
// ------------------//


// ��жϺ���
void PlayerInfo::buffJudgment()
{
	for (int i = 0; i < chessInBattleArea.size(); ++i)
	{
		switch (chessInBattleArea[i]->getChessName())
		{
			case 0:
			case 1:
				mageNumInBattleArea++;
				break;
			case 2:
			case 3:
				shooterNumInBattleArea++;
				break;
			case 4:
			case 5:
				tankNumInBattleArea++;
				break;
		}
	}
	if (mageNumInBattleArea >= 2)
	{
		for (int i = 0; i < chessInBattleArea.size(); ++i)
		{
			if (chessInBattleArea[i]->GetCareer() == "mage")
			{
				chessInBattleArea[i]->careerBuff();
			}
		}
	}
	if (shooterNumInBattleArea >= 2)
	{
		for (int i = 0; i < chessInBattleArea.size(); ++i)
		{
			if (chessInBattleArea[i]->GetCareer() == "shooter")
			{
				chessInBattleArea[i]->careerBuff();
			}
		}
	}
	if (tankNumInBattleArea >= 2)
	{
		for (int i = 0; i < chessInBattleArea.size(); ++i)
		{
			if (chessInBattleArea[i]->GetCareer() == "tank")
			{
				chessInBattleArea[i]->careerBuff();
			}
		}
	}
	if (mageNumInBattleArea >= 1 && shooterNumInBattleArea >= 1 && tankNumInBattleArea >= 1)
	{
		for (int i = 0; i < chessInBattleArea.size(); ++i)
		{
			chessInBattleArea[i]->careerBuff();
		}
	}
}



void PlayerInfo::arrZero(int arrName[])
{
	for (int i = 0; i < HERONUM; ++i)
	{
		arrName[i] = 0;
	}
}

// ע�⣺ÿ�ι�����һ��Ӣ�ۺ�Ҫ��������ı������Ǻ���(����������ŵ���������)
// ���������жϺ���
void PlayerInfo::raiseLevel()
{
	arrZero(heroOneStarNumArr);
	arrZero(heroTwoStarNumArr);
	for (int i = 0; i < chessInBattleArea.size(); ++i)
	{
		switch (chessInBattleArea[i]->getChessLevel())
		{
			case 1:
				heroOneStarNumArr[chessInBattleArea[i]->getChessName()]++;
				break;
			case 2:
				heroTwoStarNumArr[chessInBattleArea[i]->getChessName()]++;
				break;
		}
	}
	for (int i = 0; i < chessInPreArea.size(); ++i)
	{
		if (chessInPreArea[i] != nullptr)
		{
			switch (chessInPreArea[i]->getChessLevel())
			{
				case 1:
					heroOneStarNumArr[chessInPreArea[i]->getChessName()]++;
					break;
				case 2:
					heroTwoStarNumArr[chessInPreArea[i]->getChessName()]++;
					break;
			}
		}
	}

	for (int i = 0; i < HERONUM; ++i)
	{
		if (heroOneStarNumArr[i] >= 3)
		{
			heroOneStarNumArr[i] -= 3;
			heroTwoStarNumArr[i]++;
			deleteLowLevelChess(i, 1);
			createHighLevelChess(i, 2);
		}
		if (heroTwoStarNumArr[i] >= 3)
		{
			heroTwoStarNumArr[i] -= 3;
			deleteLowLevelChess(i, 2);
			createHighLevelChess(i, 3);

		}
	}
}

// ���Ǻ����Ӣ��ɾ������
void PlayerInfo::deleteLowLevelChess(int heroFlag, int level)
{
	int count = 0;
	// ���ȴӱ�ս��ɾ��
	for (int i = 0; i < chessInPreArea.size(); ++i)
	{
		if (chessInPreArea[i] != nullptr && chessInPreArea[i]->getChessName() == heroFlag && chessInPreArea[i]->getChessLevel() == level)
		{
			chessInPreArea[i] = nullptr;
			count++;
			if (count == 3)
			{
				return; // �����ɾ��3����ֹͣ
			}
		}
	}

	// Ȼ���ս����ɾ��
	for (int i = 0; i < chessInBattleArea.size(); ++i)
	{
		if (chessInBattleArea[i]->getChessName() == heroFlag && chessInBattleArea[i]->getChessLevel() == level)
		{
			chessInBattleArea[i] = nullptr;
			count++;
			if (count == 3)
			{
				return; // �����ɾ��3����ֹͣ
			}
		}
	}
}

// �˴�new�Ķ������������ս��������Ҫ�����ͷ�
// ���Ǻ����Ӣ�۳�����ս��������
void PlayerInfo::createHighLevelChess(int heroflag, int level)
{
	shared_ptr<Chess> chess;
	switch (heroflag)
	{
		case 0:
		case 1:
			chess = make_shared<mage>(heroflag);
			break;
		case 2:
		case 3:
			chess = make_shared<shooter>(heroflag);
			break;
		case 4:
		case 5:
			chess = make_shared<tank>(heroflag);
			break;
		default:
			return;
	}
	chessInBattleArea.push_back(chess);
	chess->promoteRank(level);
}

//mlx
int PlayerInfo::GetMinIndex()
{
	for (int i = 0; i < 9; i++)
	{
		if (chessInPreArea[i] == nullptr)
		{
			return i;
		}
	}
	return -1;
}
