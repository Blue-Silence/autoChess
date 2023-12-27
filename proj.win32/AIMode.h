/******************************************************/
/*                  文件名：AIMode.h                  */
/*                  功能：AI玩家的信息                */
/******************************************************/
#pragma once
#ifndef _AIMODE_H_
#define _AIMODE_H_

#include "ChessBoard.h"
#include <random>


class AIMode :public Layer
{
private:
    // 存储所有玩家和AI的信息
    vector<PlayerInfo> players;

    // 存储我方棋盘信息
    int chessBoard[BOARDMAXR][BOARDMAXC] = { 0 };

    // 总的AI玩家数目
    int totalAINum = 0;

    // 存活的AI玩家数目
    int liveAINum = 0;

    // 回合数
    int turn = 0;

public:
    //-------------------//
    //     接口函数      //
    //-------------------//
    vector<PlayerInfo>* getPlayersVectorInfo();

    //-------------------//
    //     功能函数      //
    //-------------------//
   
    // 构造函数，根据AI数量初始化
    AIMode(int aiCount);              

    // 声明拷贝构造函数
    AIMode(const AIMode& other);

    // 声明赋值运算符
    AIMode& operator=(const AIMode& other);

    // AI玩家阵亡一个
    void deleteOneAI() { liveAINum--; }

    // 是否还有存活的AI玩家可以战斗
    bool existLiveAI() { return liveAINum > 0; }

    // 除了正在和玩家对战的AI，其余AI随机扣血
    void randomBloodLose(int AIInBattle);

    // 随机数获取函数
    int getRandomNumber(int minNum, int upNum);    

    // 初始化游戏
    void initializeGame(int index);             

    // 给AI玩家添加场上英雄
    void addHero();                

    // 玩家升级
    void promoteLevel();                       

    // 提升棋子星级
    void promoteHeroStar();            

    // 集成每个回合结束后需要进行的工作 注意在调用该函数前回合数需要+1
    void eachTurnOver();                     
};

#endif