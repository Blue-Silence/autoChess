/********************************************
 * ���ܣ�����ģ��
 * ���ߣ�֣ΰة ������
 * �����޸����ڣ�12.12
 ********************************************/

#pragma once
#ifndef _CHESSBOARD_H_
#define _CHESSBOARD_H_

 //�������̴�С
#define BOARDMAXR 5
#define BOARDMAXC 8

#include"Chess.h"
#include"Config/ConfigController.h"
#include"PlayerInfo.h"
#include<vector>
#include"cocos2d.h"

USING_NS_CC;


// ������������Ļ�����ת��,ת���Ϊ��ս�����½ǵ�һ�����̶�Ӧ�ľ���
void CoordinateConvert(CoordinateType targetType, Vec2 oldPos, ChessCoordinate* newPos);

class ChessBoard :public Ref
{

private:

	static int turn;

	//��¼����������Ϊ������������
	bool board[BOARDMAXR][BOARDMAXC];

public:

	// ����������ҵ�ʵ��
	PlayerInfo player;

	// ��������´洢�Է�ս��������Ϣ
	vector<Chess*>oppBattleAreaInfo;

	CREATE_FUNC(ChessBoard);

	// ��ʼ������
	virtual bool init();

	//�������Ƿ������̷�Χ��
	static bool isInBoard(Vec2 posi);

	// �жϸ�λ���Ƿ���Է�������,�����������ת��Ϊ��������
	bool isAvailable(int row, int col);

	//mlx:�������ӣ���������
	void OnBoard(int row, int col);

	//mlx:�������
	void WithdrawBoard(int row, int col);

	//-------------------------------------------�ֽ���-------------------------------------------//
	// �������йر�սϯ��������ƶ��������ϵı����뺯��
	//-------------------//
	// ���ߣ�������      //
	// �޸����ڣ�12.12   //
	// ------------------//


	// ��ǰѡ�е�����
	Chess* selectedChess;

	// �ж������λ���Ƿ��ڱ�ս��
	bool isInPreparationZone(const Vec2& position);

	// ��ȡ��ս�����ӵ���Ϣ
	shared_ptr<Chess> getChessAtPreparationZone(const Vec2& position);
};

#endif // !_CHESSBOARD_H_