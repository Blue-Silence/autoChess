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

	void chessInitBeforeBattle(Chess* chess);

	//计算两个棋子之间的曼哈顿距离，输入参数为两个棋子的棋格坐标
	double getDistance(ChessCoordinate* start, ChessCoordinate* end);

	//void pieceBattle(Chess &damageMaker, Chess &victim);

	Chess* findEnemy(Chess* damageMaker, PlayerInfo* enemy);

	//这是一个普通的寻路函数,每次只走一步
	void findPathToEnemy(Chess* damageMaker, Chess* targetEnemy,ChessBoard* chessboard);

	// 移动棋子
	void moveChess(Chess* movingChess, Vec2 targetPoistion);

	// 攻击——动画+数据
	void doAttack(Chess* damageMaker, Chess* targetEnemy);

	// 战斗测试
	void AItest();

private:
	bool gameOver = false;
	ChessBoard* chessBoard;
	PlayerInfo* playerME;
	PlayerInfo* playerOPP;

	
};