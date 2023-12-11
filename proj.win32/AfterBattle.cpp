#include "AfterBattle.h"

bool AfterBattle::init()
{
	return false;
}

/*
���ã������������������ĸ������ﻹ��ʣ������ӣ�Ȼ��������ӵ��Ǽ������˺�
�����б�����װ��Chess��vector
˵����
*/
void AfterBattle::damage(vector<Chess*> A, vector<Chess*> B,PlayerInfo a,PlayerInfo b)
{
	int damage = 0;
	if (A.size() > 0) {//Ӯ������A
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
���ܣ�����Ƿ����������
���������������
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



