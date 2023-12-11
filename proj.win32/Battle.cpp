#include "Battle.h"

/****************
名称：BattleChessNum
返回：void
参数：Chess 2个
说明：模拟攻击，进行数据的更改
*/
bool Battle::init()
{
	return true;
}
double Battle::getDistance(ChessCoordinate* start, ChessCoordinate* end)
{
	return sqrt(pow((start->getX() - end->getX()), 2) + pow((start->getY() - end->getY()), 2));
}
void Battle::pieceBattle(Chess &damageMaker, Chess &victim)
{
	//进行了攻击
	damageMaker.attackOne(victim);
}

Chess* Battle::findEnemy(Chess* damageMaker, PlayerInfo enemy)
{
	double distance = 999;
	Chess* enemyPiece;
	for (int i = 0; i < 7; i++) {
		if (enemy.BattleChessNum[i] == nullptr) {//没棋子了
			break;
		}
		else {
			double temp = 0;
			temp = Battle::getDistance(damageMaker->getChessCoordinateByType(CoordinateType::screenCoordinates), enemy.BattleChessNum[i]->getChessCoordinateByType(CoordinateType::screenCoordinates));
			if (temp < distance) {
				enemyPiece = enemy.BattleChessNum[i];
			}
		}
	}
	return enemyPiece;
}

void Battle::normalTraceTrack(Chess* origin, Chess* destination)
{
	ChessCoordinate ori, dst;
	ori = *origin->getChessCoordinateByType(CoordinateType::chessBoardCoordinates);
	dst = *destination->getChessCoordinateByType(CoordinateType::chessBoardCoordinates);
	int x, y;
	if (ori.getX() < dst.getX()) {
		x = 1;
	}
	else if(ori.getX()>dst.getX()){
		x = -1;
	}
	else {
		x = 0;
	}
	if (ori.getY() < dst.getY()) {
		y = 1;
	}
	else if (ori.getY() > dst.getY()) {
		y = -1;
	}
	else {
		y = 0;
	}
}

bool Battle::detect(PlayerInfo A)
{
	bool over = -1;
	for (int i = 0; i < 7; i++) {
		if (A.BattleChessNum[i] == nullptr) {//没棋子了
			break;
		}
		else {
			if (A.BattleChessNum[i]->isDead()) {//棋子死了
				if (over == -1) {
					over = 1;
				}
			}
			else {
				over = 0;
			}
		}
	}
	if (over != 0) {//结束了
		return true;
	}
	else {
		return false;
	}
}


