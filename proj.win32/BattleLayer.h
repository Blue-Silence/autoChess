/******************************************************/
/*                  �ļ�����BattleLayer.h             */
/*                  ���ܣ�ս���߼�ͷ�ļ�              */
/*                  ���ߣ�֣ΰة                      */
/******************************************************/

#pragma once
#include <atomic>
#include"PlayerInfo.h"
#include"ChessBoard.h"
#include "audio/include/SimpleAudioEngine.h"

class BattleLayer :public Layer
{
public:

	// ��ʼ������
	bool init();

	// ���º���������ÿ֡����ս��״̬
	void update(float delta);

	// ��̬�������������ڴ���BattleLayerʵ��
	static BattleLayer* create(PlayerInfo* playerME, PlayerInfo* playerOPP, bool* isInBattle, bool* canBuyChess);

	// ս��ǰ���ӳ�ʼ��
	void chessInitBeforeBattle(shared_ptr<Chess> chess, const bool &isME);

	// �������Ӽ�������پ���
	double getDistance(const ChessCoordinate* start, const ChessCoordinate* end);

	// Ѱ�ҵ���
	shared_ptr<Chess> findEnemy(shared_ptr<Chess> damageMaker, PlayerInfo* enemy);

	// Ѱ·������λ��
	bool findPathToEnemy(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy);

	// �ƶ�����
	void moveChess(shared_ptr<Chess> movingChess, const Vec2 &targetPosition);

	// ִ�й�������
	void doAttack(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy);

	// ���ָ��λ���Ƿ��ͨ��
	inline bool isAvailable(const int &row,const int &col);

	// ִ��ս��ȫ����
	void playGame(shared_ptr<Chess> chess, PlayerInfo* opp);

	// �ж�ʤ��
	void detectWinner();

	// ��ʾ������Ϸ�Ķ�����ʤ����ʧ�ܻ�ƽ�֣�
	void BattleLayer::showEndGameAnimation(const string& gameResult);

	// ���õ�ս�����߼�
	void BattleLayer::processAttackResult(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy,
								const int &targetRow, const int &targetCol, Sprite* targetChessImage,const bool &useSkill);

	// ������Ӧ����Ч
	void BattleLayer::playSkillSound(shared_ptr<Chess> damageMaker);

	// ���ܹ��ö����߼�
	void remoteAttackAnimation(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy,const bool &useSkill);

	// AI ���Ժ���
	void AItest();

private:

	// ˽�й��캯��
	BattleLayer(PlayerInfo* playerOfMe, PlayerInfo* playerOfOpp, bool* inBattle, bool* BuyChess);
	
	// ��Ϸ������־
	bool gameOver = false;

	// ��Ϸ�е�����״̬
	std::atomic<int> boardInGame[BOARDMAXR][BOARDMAXC] = { 0 };

	// �����Ϣ
	PlayerInfo* playerME;

	// ������Ϣ
	PlayerInfo* playerOPP;

	// �Ƿ���ս��״̬
	bool* isInBattle;

	// �Ƿ���Թ�������
	bool* canBuyChess;

	
};

// A*Ѱ·�㷨��ʹ�õ�·���ڵ���
class pathNode
{
public:
	// ���캯��
	pathNode(int x, int y, pathNode* parent = nullptr);

	// ����Fֵ
	void calculateFCost();

	// �ڵ�λ��
	int x, y;

	// Gֵ��Hֵ��Fֵ
	int gCost, hCost, fCost;

	// ���ڵ�
	pathNode* parent;
};