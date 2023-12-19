/*
功能：玩家模型信息
作者：Hu Junwei
*/
#include "PlayerInfo.h"

//-----------------------------------------------------//
//函数参数：空                                         //
//函数功能：初始化玩家的信息                           //
//函数返回值：bool型                                   //
//函数注意事项(选)：无                                 //
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
//函数参数：int coinNum ,int level                     //
//函数功能：提升玩家等级                               //
//函数返回值：bool型                                   //
//函数注意事项(选)：无                                 //
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
//函数参数：无					                       //
//函数功能：提升玩家等级                               //
//函数返回值：void                                     //
//函数注意事项：1.金币已经在此函数中扣除，请勿重复操作 //
//              2.所有跟等级有关的数据均在此函数内修改 //
//                请勿在别处重复操作                   //
//-----------------------------------------------------//
void PlayerInfo::ChangeLevel()
{
	// 升级 扣除金币
	++level;
	coinNum -= 4;

	// 棋子数修改：3、5、7、9级更改
	if (level == 3 || level == 5 || level == 7 || level == 9)
	{
		++maxBattleChessNum;
	}
}

//-----------------------------------------------------//
//函数参数：空                                         //
//函数功能：战斗后扣除血量                             //
//函数返回值：void                                     //
//函数注意事项(选)：无                                 //
//-----------------------------------------------------//
void PlayerInfo::DecreaseLifeValue(int hurt)
{
	lifeValue -= hurt;
}

//-----------------------------------------------------//
//函数参数：空                                         //
//函数功能：获取玩家当前血量                           //
//函数返回值：int                                      //
//函数注意事项(选)：无                                 //
//-----------------------------------------------------//
int PlayerInfo::GetLifeValue() const
{
	return lifeValue;
}

int PlayerInfo::getMaxBattleChessNum() const
{
	return maxBattleChessNum;
}


// 在玩家对战区放置棋子
void PlayerInfo::putChessInBattleArea(shared_ptr<Chess> chess)
{
	chessInBattleArea.push_back(chess);
}

// 获取玩家对战区棋子集合
vector<shared_ptr<Chess>>* PlayerInfo::getBattleAreaChesses()
{
	return &chessInBattleArea;
}

// 在玩家备战区放置棋子
void PlayerInfo::putChessInPreArea(shared_ptr<Chess> chess)
{
	chessInPreArea.push_back(chess);
}

// 获取玩家备战区的棋子集合
vector<shared_ptr<Chess>>* PlayerInfo::getPreAreaChesses()
{
	return &chessInPreArea;
}

// 从玩家A备战区移去棋子
void PlayerInfo::removeChessFromPreArea(shared_ptr<Chess> chess)
{
	// 移去第一个匹配的棋子即可
	for (auto it = chessInPreArea.begin(); it != chessInPreArea.end(); ++it)
	{
		if (*it == chess)
		{
			chessInPreArea.erase(it);
			break;
		}
	}
}

// 以下是有关羁绊判断和升级星级的判断
//-------------------//
// 作者：胡峻玮      //
// 修改日期：12.15   //
// ------------------//


// 羁绊判断函数
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

// 注意：每次购买完一个英雄后都要调用下面的遍历升星函数(把这个函数放到购买函数里)
// 遍历升星判断函数
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

// 升星后低星英雄删除函数
void PlayerInfo::deleteLowLevelChess(int heroFlag, int level)
{
	int count = 0;
	// 优先从备战区删除
	for (int i = 0; i < chessInPreArea.size(); ++i)
	{
		if (chessInPreArea[i] != nullptr && chessInPreArea[i]->getChessName() == heroFlag && chessInPreArea[i]->getChessLevel() == level)
		{
			chessInPreArea[i] = nullptr;
			count++;
			if (count == 3)
			{
				return; // 如果已删除3个则停止
			}
		}
	}

	// 然后从战斗区删除
	for (int i = 0; i < chessInBattleArea.size(); ++i)
	{
		if (chessInBattleArea[i]->getChessName() == heroFlag && chessInBattleArea[i]->getChessLevel() == level)
		{
			chessInBattleArea[i] = nullptr;
			count++;
			if (count == 3)
			{
				return; // 如果已删除3个则停止
			}
		}
	}
}

// 此处new的对象在最后整个战斗结束后要进行释放
// 升星后高星英雄出现在战斗区函数
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
