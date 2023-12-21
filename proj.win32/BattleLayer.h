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

	CREATE_FUNC(BattleLayer);

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

	// 战斗测试
	void AItest();



private:
	bool gameOver = false;

	int boardInGame[BOARDMAXR][BOARDMAXC] = { 0 };

	ChessBoard chessBoard;

	PlayerInfo playerME;

	PlayerInfo playerOPP;

	
};