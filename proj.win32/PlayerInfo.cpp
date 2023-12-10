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
bool PlayerInfo::InitPlayerInfo()
{
	coinNum = 3;
	lifeValue = 50;
	level = LEVELINIT;
	maxOwnChessNum = OWNINIT;
	maxBattleChessNum = BATTLEINIT;

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
	if (coinNum < 4 || level == LEVELMAX)
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
		++maxOwnChessNum;
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

//-----------------------------------------------------//
//函数参数：空                                         //
//函数功能：返回保存战斗棋子的数组的地址               //
//函数返回值：ChessPiece**                             //
//函数注意事项(选)：无                                 //
//-----------------------------------------------------//
ChessPiece** PlayerInfo::GetBattleChessNum() const
{
	return BattleChessNum;
}