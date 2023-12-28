/********************************************
 * ���ܣ����ӻ���ս��
 * ���ߣ�֣ΰة
 ********************************************/

#pragma once
#include <atomic>
#include"PlayerInfo.h"
#include"ChessBoard.h"
#include "audio/include/SimpleAudioEngine.h"

class BattleLayer :public Layer
{
public:

	bool init();

	void update(float delta);

	//CREATE_FUNC(BattleLayer);

	// �Զ���ľ�̬��������
	static BattleLayer* create(PlayerInfo* playerME, PlayerInfo* playerOPP,bool* isInBattle,bool* canBuyChess) 
	{
		;
		BattleLayer* layer = new (std::nothrow) BattleLayer(playerME, playerOPP,isInBattle,canBuyChess);
		if (layer && layer->init()) 
		{
			layer->autorelease();
			return layer;
		}
		CC_SAFE_DELETE(layer);
		return nullptr;
	}

	void chessInitBeforeBattle(shared_ptr<Chess> chess,bool isME);

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
	BattleLayer(PlayerInfo* playerOfMe, PlayerInfo* playerOfOpp,bool* inBattle,bool* BuyChess)
	{
		playerME = playerOfMe; playerOPP = playerOfOpp; isInBattle = inBattle; canBuyChess = BuyChess;
	}

	bool gameOver = false;

	std::atomic<int> boardInGame[BOARDMAXR][BOARDMAXC] = { 0 };

	PlayerInfo* playerME;

	PlayerInfo* playerOPP;

	bool* isInBattle;
	bool* canBuyChess;

	
};