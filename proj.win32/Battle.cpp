#include "Battle.h"

/****************
���ƣ�BattleChessNum
���أ�void
������Chess 2��
˵����ģ�⹥�����������ݵĸ���
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
	//�����˹���
	damageMaker.attackOne(victim);
}

Chess* Battle::findEnemy(Chess* damageMaker, PlayerInfo enemy)
{
	double distance = 999;
	Chess* enemyPiece;
	for (int i = 0; i < 7; i++) {
		if (enemy.BattleChessNum[i] == nullptr) {//û������
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
		if (A.BattleChessNum[i] == nullptr) {//û������
			break;
		}
		else {
			if (A.BattleChessNum[i]->isDead()) {//��������
				if (over == -1) {
					over = 1;
				}
			}
			else {
				over = 0;
			}
		}
	}
	if (over != 0) {//������
		return true;
	}
	else {
		return false;
	}
}


