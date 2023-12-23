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

    int chessBoard[BOARDMAXR][BOARDMAXC] = { 0 };

    static int turn;
public:
    //-------------------//
    //     接口函数      //
    //-------------------//
    vector<PlayerInfo> getPlayersVectorInfo();


    //-------------------//
    //     功能函数      //
    //-------------------//
   
    // 构造函数，根据AI数量初始化
    AIMode(int aiCount);              

    // 声明拷贝构造函数
    AIMode(const AIMode& other);

    // 声明赋值运算符
    AIMode& operator=(const AIMode& other);

    // 重载后置递增运算符 主要功能便于回合数+1
    AIMode operator++(int);

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
    // 即在该函数前一定要有AIMode实例的后置++操作
    void eachTurnOver();                     
};

#endif