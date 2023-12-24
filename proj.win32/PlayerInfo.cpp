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
	coinNum = 50;
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
int PlayerInfo::getcoin() const
{
	return coinNum;
}

void PlayerInfo::PayForRefresh()
{
	coinNum -= 2;
}
void PlayerInfo::setCoinNum(int num)
{
	coinNum = num;
}

int PlayerInfo::getLevel()const
{
	return level;
}

int PlayerInfo::getMaxBattleChessNum() const
{
	return maxBattleChessNum;
}

// ����Ҷ�ս����������
void PlayerInfo::putChessInBattleArea(shared_ptr<Chess> chess)
{
	//������ֱ�Ӽ�
	if (chessInBattleArea.size() == 0)
	{
		chessInBattleArea.push_back(chess);
		return;
	}
	//�ǿ����������λ��
	for (int i = 0; i < chessInBattleArea.size(); i++)
	{
		if (chessInBattleArea[i] == nullptr)
		{
			chessInBattleArea[i] = chess;
			return;
		}
	}
	//�޿�λֱ�Ӽ�
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
int* PlayerInfo::buffJudgment()
{
	int* buff = new int[3];//�ֱ�Ϊ��ʦ�����֣�̹��
	buff[0] = 0;
	buff[1] = 0;
	buff[2] = 0;
	mageNumInBattleArea = 0;
	shooterNumInBattleArea = 0;
	tankNumInBattleArea = 0;
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
		buff[0] = 1;
		int s = buff[0];
		int a = 0;
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
		buff[1] = 1;
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
		buff[2] = 1;
	}
	return buff;
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
//������12.22 mlx:ͨ������ֵ�ж��Ƿ�������ǲ���
bool PlayerInfo::starRaiseLevel(int location, int& delLoc_1, int& delLoc_2)
{
	bool isRaising = 0;
	arrZero(heroOneStarNumArr);
	arrZero(heroTwoStarNumArr);
	for (int i = 0; i < chessInBattleArea.size(); ++i)
	{
		if (chessInBattleArea[i] != nullptr) {
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
	//ÿ������ֻ�漰һ�θı�
	for (int i = 0; i < HERONUM; ++i)
	{
		if (heroOneStarNumArr[i] >= 3)
		{
			isRaising = 1;
			heroOneStarNumArr[i] -= 3;
			heroTwoStarNumArr[i]++;
			deleteLowLevelChess(i, 1, location, delLoc_1, delLoc_2);
			createHighLevelChess(i, 2, location);
			break;
		}
		else if (heroTwoStarNumArr[i] >= 3)
		{
			isRaising = 1;
			heroTwoStarNumArr[i] -= 3;
			deleteLowLevelChess(i, 2, location, delLoc_1, delLoc_2);
			createHighLevelChess(i, 3, location);
			break;
		}
	}
	return isRaising;
}

// ���Ǻ����Ӣ��ɾ������
void PlayerInfo::deleteLowLevelChess(int heroFlag, int level, int location, int& delLoc_1, int& delLoc_2)
{
	int count = 0;
	// ���ȴӱ�ս��ɾ��
	for (int i = 0; i < chessInPreArea.size(); ++i)
	{
		if (chessInPreArea[i] != nullptr && chessInPreArea[i]->getChessName() == heroFlag && chessInPreArea[i]->getChessLevel() == level)
		{
			if (delLoc_1 == -1 && i != location)
			{
				delLoc_1 = i;
			}
			else if (delLoc_2 == -1 && i != location)
			{
				delLoc_2 = i;
			}

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
		if (chessInBattleArea[i] != nullptr
			&& chessInBattleArea[i]->getChessName() == heroFlag && chessInBattleArea[i]->getChessLevel() == level)
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
void PlayerInfo::createHighLevelChess(int heroflag, int level, int location)
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
	chessInPreArea[location] = chess;
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

//mlx
//���³�������
void PlayerInfo::ReBattleChess(shared_ptr<Chess> curChess, shared_ptr<Chess>newChess)
{
	for (int i = 0; i < chessInBattleArea.size(); i++)
	{
		if (chessInBattleArea[i] != nullptr && chessInBattleArea[i] == curChess)
		{
			chessInBattleArea[i] = newChess;
			break;
		}
	}
}
