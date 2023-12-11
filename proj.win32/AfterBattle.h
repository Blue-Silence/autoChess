/**********
功能：自走棋中场检测类
作者：刘兴源-2053291
位置：Class/Controller（筛选器）
版本：1.0.0
游戏引擎：cocos2d-x 4.0

************/
#pragma once

#include<cocos2d.h>
#include"PlayerInfo.h"
#include"ChessBoard.h"
#include<vector>
#include"Chess.h"
USING_NS_CC;
using namespace std;

#include <string>
using std::string;

class AfterBattle :public Ref
{
public:

	virtual bool init();

	CREATE_FUNC(AfterBattle);

    void damage(vector<Chess*> A, vector<Chess*> B,PlayerInfo a,PlayerInfo b);//用于检测哪一方在战斗中获胜,并减去相应的hp

	bool detect(PlayerInfo a, PlayerInfo b);//检测是否有玩家死亡

private:
	
};
