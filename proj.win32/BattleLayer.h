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

	//CREATE_FUNC(Battle);

	//计算两个棋子之间的曼哈顿距离，输入参数为两个棋子的棋格坐标
	double getDistance(ChessCoordinate* start, ChessCoordinate* end);

	//void pieceBattle(Chess &damageMaker, Chess &victim);

	Chess* findEnemy(Chess* damageMaker, PlayerInfo enemy);

	//这是一个普通的寻路函数,每次只走一步
	void findPathToEnemy(Chess* damageMaker, Chess* targetEnemy,ChessBoard* chessboard);

	// 移动棋子
	void moveChess(Sprite* movingChess, Vec2 targetPoistion);

	// 攻击——动画+数据
	void doAttack(Chess* damageMaker, Chess* targetEnemy);

	bool detect(PlayerInfo A);//检查玩家的战斗棋子是否还有剩余，如有，则进行攻击或移动

private:

	PlayerInfo playerME;
	PlayerInfo playerOPP;


};