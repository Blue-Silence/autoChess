/********************************************
 * ���ܣ����ӻ���ս��
 * ���ߣ�֣ΰة
 ********************************************/

#pragma once
#include"PlayerInfo.h"
#include"ChessBoard.h"

class BattleLayer :public Layer
{
public:
	bool init();

	void update(float delta);

	CREATE_FUNC(BattleLayer);

	void chessInitBeforeBattle(Chess* chess);

	//������������֮��������پ��룬�������Ϊ�������ӵ��������
	double getDistance(ChessCoordinate* start, ChessCoordinate* end);

	//void pieceBattle(Chess &damageMaker, Chess &victim);

	Chess* findEnemy(Chess* damageMaker, PlayerInfo* enemy);

	//����һ����ͨ��Ѱ·����,ÿ��ֻ��һ��
	void findPathToEnemy(Chess* damageMaker, Chess* targetEnemy,ChessBoard* chessboard);

	// �ƶ�����
	void moveChess(Chess* movingChess, Vec2 targetPoistion);

	// ������������+����
	void doAttack(Chess* damageMaker, Chess* targetEnemy);

	// ս������
	void AItest();

private:
	bool gameOver = false;
	ChessBoard* chessBoard;
	PlayerInfo* playerME;
	PlayerInfo* playerOPP;

	
};