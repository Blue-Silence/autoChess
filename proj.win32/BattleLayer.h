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

	void chessInitBeforeBattle(shared_ptr<Chess> chess);

	//������������֮��������پ��룬�������Ϊ�������ӵ��������
	double getDistance(ChessCoordinate* start, ChessCoordinate* end);

	//void pieceBattle(Chess &damageMaker, Chess &victim);

	shared_ptr<Chess> findEnemy(shared_ptr<Chess> damageMaker, PlayerInfo* enemy);

	//����һ����ͨ��Ѱ·����,ÿ��ֻ��һ��
	void findPathToEnemy(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy);

	// �ƶ�����
	void moveChess(shared_ptr<Chess> movingChess, Vec2 targetPoistion);

	// ������������+����
	void doAttack(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy);

	// �жϸ�λ���ܷ�ͨ��
	inline bool isAvailable(int row, int col);

	// ��װӢ�۽���ս����ȫ���̡���Ѱ�ҡ�Ѱ·������
	void playGame(shared_ptr<Chess> chess,PlayerInfo* opp);

	// ս������
	void AItest();



private:
	bool gameOver = false;

	int boardInGame[BOARDMAXR][BOARDMAXC] = { 0 };

	ChessBoard chessBoard;

	PlayerInfo playerME;

	PlayerInfo playerOPP;

	
};