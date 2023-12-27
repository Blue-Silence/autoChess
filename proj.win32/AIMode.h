/******************************************************/
/*                  �ļ�����AIMode.h                  */
/*                  ���ܣ�AI��ҵ���Ϣ                */
/******************************************************/
#pragma once
#ifndef _AIMODE_H_
#define _AIMODE_H_

#include "ChessBoard.h"
#include <random>


class AIMode :public Layer
{
private:
    // �洢������Һ�AI����Ϣ
    vector<PlayerInfo> players;

    // �洢�ҷ�������Ϣ
    int chessBoard[BOARDMAXR][BOARDMAXC] = { 0 };

    // �ܵ�AI�����Ŀ
    int totalAINum = 0;

    // ����AI�����Ŀ
    int liveAINum = 0;

    // �غ���
    int turn = 0;

public:
    //-------------------//
    //     �ӿں���      //
    //-------------------//
    vector<PlayerInfo>* getPlayersVectorInfo();

    //-------------------//
    //     ���ܺ���      //
    //-------------------//
   
    // ���캯��������AI������ʼ��
    AIMode(int aiCount);              

    // �����������캯��
    AIMode(const AIMode& other);

    // ������ֵ�����
    AIMode& operator=(const AIMode& other);

    // AI�������һ��
    void deleteOneAI() { liveAINum--; }

    // �Ƿ��д���AI��ҿ���ս��
    bool existLiveAI() { return liveAINum > 0; }

    // �������ں���Ҷ�ս��AI������AI�����Ѫ
    void randomBloodLose(int AIInBattle);

    // �������ȡ����
    int getRandomNumber(int minNum, int upNum);    

    // ��ʼ����Ϸ
    void initializeGame(int index);             

    // ��AI�����ӳ���Ӣ��
    void addHero();                

    // �������
    void promoteLevel();                       

    // ���������Ǽ�
    void promoteHeroStar();            

    // ����ÿ���غϽ�������Ҫ���еĹ��� ע���ڵ��øú���ǰ�غ�����Ҫ+1
    void eachTurnOver();                     
};

#endif