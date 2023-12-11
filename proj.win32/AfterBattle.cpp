#include "AfterBattle.h"

bool AfterBattle::init()
{
	return false;
}

/*
作用：遍历两个容器，看哪个容器里还有剩余的棋子，然后根据棋子的星级计算伤害
参数列表：两个装着Chess的vector
说明：
*/
void AfterBattle::damage(vector<Chess*> A, vector<Chess*> B,PlayerInfo a,PlayerInfo b)
{
	int damage = 0;
	if (A.size() > 0) {//赢的人是A
		for (int i = 0; i < static_cast<int>(A.size()); i++) {
			damage += static_cast<int>(A[i]->getChessLevel());
		}
		a.DecreaseLifeValue(damage);
	}
	else if (B.size() > 0) {
		for (int i = 0; i < static_cast<int>(B.size()); i++) {
			damage += static_cast<int>(B[i]->getChessLevel());
		}
		b.DecreaseLifeValue(damage);
	}
}

/*
功能：检测是否有玩家死亡
参数：两个玩家类
*/
bool AfterBattle::detect(PlayerInfo a, PlayerInfo b)
{
	if (a.GetLifeValue() < 0 || b.GetLifeValue() < 0) {
		return true;
	}
	else {
		return false;
	}
}



