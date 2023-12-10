/********************************************
 * ���ܣ�����ģ��
 * ���ߣ�֣ΰة
 * �����޸����ڣ�12.10
 ********************************************/

#pragma once
#ifndef _CHESSBOARD_H_
#define _CHESSBOARD_H_

 //�������̴�С
#define BOARDMAXR 5
#define BOARDMAXC 8

#include"Chess.h"
#include<vector>
#include"cocos2d.h"

USING_NS_CC;

class ChessBoard :public Ref
{

private:

	static int turn;

	vector<Chess*> playerAWarZoneChesses; // ��¼���A��ս�����Ӽ���

	vector<Chess*> playerAPreZoneChesses; // ��¼���A��ս�����Ӽ���

	vector<Chess*> playerBWarZoneChesses; // ��¼���B��ս�����Ӽ���

	vector<Chess*> playerBPreZoneChesses; // ��¼���B��ս�����Ӽ���

	//��¼����������Ϊ������������
	//������ȷ���Ƿ�����ƶ����޷�ȷ������λ�õľ���������ʲô
	//vector<Sprite*>board[BOARD_SIZE];
	bool board[BOARDMAXR][BOARDMAXC];

public:

	CREATE_FUNC(ChessBoard);

	virtual bool init();

	//CREATE_FUNC(ChessBoard);

	// ������������Ļ�����ת��,ת���Ϊ��ս�����½ǵ�һ�����̶�Ӧ�ľ���
	ChessCoordinate* coordinateConvert(CoordinateType type, ChessCoordinate* oldPos, ChessCoordinate* newPos, Sprite* sprite);

	// �����A��ս����������,��������Ϊ��������
	void putChessInPlayerAWarZone(Vec2 posi,Chess* chess);

	// ��ȡ���A��ս�����Ӽ���
	vector<Chess*>* getPlayerAWarZoneChesses();

	// �����A��ս����������
	void putChessInPlayerAPreZone(Chess* chess);

	// ��ȡ���A��ս�����Ӽ���
	vector<Chess*>* getPlayerAPreZoneChesses();

	// �����A��ս����ȥ����
	void removeChessFromPlayerAPreZone(Chess* chess);

	// �����B��ս����������,��������Ϊ��������
	void putChessInPlayerBWarZone(Vec2 posi, Chess* chess);

	// ��ȡ���B��ս�����Ӽ���
	vector<Chess*>* getPlayerBWarZoneChesses();

	// �����B��ս����������
	void putChessInPlayerBPreZone(Chess* chess);

	// ��ȡ���B��ս�����Ӽ���
	vector<Chess*>* getPlayerBPreZoneChesses();

	// �����B��ս����ȥ����
	void removeChessFromPlayerBPreZone(Chess* chess);

	//����Ƿ������̷�Χ��
	static bool isInBoard(Vec2 posi);

	// �жϸ�λ���Ƿ���Է�������,�����������ת��Ϊ��������
	inline bool isAvailable(int row,int col);

	// TODO
	// Ѱ·
	//ȷ������Ŀ��֮���

};

#endif // !_CHESSBOARD_H_