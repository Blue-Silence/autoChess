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

    int chessBoard[BOARDMAXR][BOARDMAXC] = { 0 };

    static int turn;
public:
    //-------------------//
    //     �ӿں���      //
    //-------------------//
    vector<PlayerInfo> getPlayersVectorInfo();


    //-------------------//
    //     ���ܺ���      //
    //-------------------//
   
    // ���캯��������AI������ʼ��
    AIMode(int aiCount);              

    // �����������캯��
    AIMode(const AIMode& other);

    // ������ֵ�����
    AIMode& operator=(const AIMode& other);

    // ���غ��õ�������� ��Ҫ���ܱ��ڻغ���+1
    AIMode operator++(int);

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
    // ���ڸú���ǰһ��Ҫ��AIModeʵ���ĺ���++����
    void eachTurnOver();                     
};

#endif