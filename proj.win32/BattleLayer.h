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

	//CREATE_FUNC(BattleLayer);

	// �Զ���ľ�̬��������
	static BattleLayer* create(PlayerInfo* playerME, PlayerInfo* playerOPP,bool* isInBattle) 
	{
		;
		BattleLayer* layer = new (std::nothrow) BattleLayer(playerME, playerOPP,isInBattle);
		if (layer && layer->init()) 
		{
			layer->autorelease();
			return layer;
		}
		CC_SAFE_DELETE(layer);
		return nullptr;
	}

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

	// �ж�˭Ӯ˭��
	void detectWinner();

	// ս������
	void AItest();



private:

	// ˽�й��캯��
	BattleLayer(PlayerInfo* playerOfMe, PlayerInfo* playerOfOpp,bool* inBattle)
	{
		playerME = playerOfMe; playerOPP = playerOfOpp; isInBattle = inBattle;
	}

	bool gameOver = false;

	int boardInGame[BOARDMAXR][BOARDMAXC] = { 0 };

	PlayerInfo* playerME;

	PlayerInfo* playerOPP;

	bool* isInBattle;

	
};