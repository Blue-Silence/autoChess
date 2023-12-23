/********************************************
 * 功能：棋子互相战斗
 * 作者：郑伟丞
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

	// 自定义的静态创建函数
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

	//计算两个棋子之间的曼哈顿距离，输入参数为两个棋子的棋格坐标
	double getDistance(ChessCoordinate* start, ChessCoordinate* end);

	//void pieceBattle(Chess &damageMaker, Chess &victim);

	shared_ptr<Chess> findEnemy(shared_ptr<Chess> damageMaker, PlayerInfo* enemy);

	//这是一个普通的寻路函数,每次只走一步
	void findPathToEnemy(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy);

	// 移动棋子
	void moveChess(shared_ptr<Chess> movingChess, Vec2 targetPoistion);

	// 攻击——动画+数据
	void doAttack(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy);

	// 判断该位置能否通行
	inline bool isAvailable(int row, int col);

	// 封装英雄进行战斗的全流程——寻找、寻路、攻击
	void playGame(shared_ptr<Chess> chess,PlayerInfo* opp);

	// 判断谁赢谁负
	void detectWinner();

	// 战斗测试
	void AItest();



private:

	// 私有构造函数
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