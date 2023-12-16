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
#include"ConfigController.h"
#include"PlayerInfo.h"
#include<vector>
#include"cocos2d.h"

USING_NS_CC;


// ������������Ļ�����ת��,ת���Ϊ��ս�����½ǵ�һ�����̶�Ӧ�ľ���
void CoordinateConvert(CoordinateType type, Vec2 oldPos, ChessCoordinate* newPos);

class ChessBoard :public Ref
{

private:

	static int turn;

	//��¼����������Ϊ������������
	//������ȷ���Ƿ�����ƶ����޷�ȷ������λ�õľ���������ʲô
	//vector<Sprite*>board[BOARD_SIZE];
	bool board[BOARDMAXR][BOARDMAXC];

public:

	// ����������ҵ�ʵ��
	PlayerInfo player;

	// ��������´洢�Է�ս��������Ϣ
	vector<Chess*>oppBattleAreaInfo;

	CREATE_FUNC(ChessBoard);

	virtual bool init();

	//CREATE_FUNC(ChessBoard);



	//�������Ƿ������̷�Χ��
	static bool isInBoard(Vec2 posi);

	// �жϸ�λ���Ƿ���Է�������,�����������ת��Ϊ��������
	inline bool isAvailable(int row, int col);

	//-------------------------------------------�ֽ���-------------------------------------------//
	// �������йر�սϯ��������ƶ��������ϵı����뺯��
	//-------------------//
	// ���ߣ�������      //
	// �޸����ڣ�12.12   //
	// ------------------//


	// ��ǰѡ�е�����
	Chess* selectedChess;

	// ����������¼�
	void onMouseDown(Event* event);

	// ��������������¼�������
	bool createMouseListener(Node* targetNode);

	// �ж������λ���Ƿ��ڱ�ս��
	bool isInPreparationZone(const Vec2& position);

	// ��ȡ��ս�����ӵ���Ϣ
	Chess* getChessAtPreparationZone(const Vec2& position);

	// ����ս���������ƶ���ս����
	void moveChessFromPreZoneToWarZone(Chess* selectedChess, int targetRow, int targetCol);


	// TODO
	// ��������⣬���̴�СӦ��û�İɣ�ֱ�Ӱ�����ԭ������
	// ��Ļ����x,y,(0,0)Ӧ�������½ǰ�
	// 
	// ��Щλ�ã������ͺ�����������ͼ����ͨ������ͼ���ԣ���ͨ������ô��ͼ
	// 
	// ��ս���У��ƶ�������ͼ��ûд��
	// 
	// ������������������ת��������ֵҪע�⣡��ʱ����Ҫ��������x�൱���У�y�൱����
	// Ѱ·
	// 
	// ȷ������Ŀ��֮���

};

#endif // !_CHESSBOARD_H_