/**********
���ܣ��������г������
���ߣ�����Դ-2053291
λ�ã�Class/Controller��ɸѡ����
�汾��1.0.0
��Ϸ���棺cocos2d-x 4.0

************/
#pragma once

#include<cocos2d.h>
#include"PlayerInfo.h"
#include"ChessBoard.h"
#include<vector>
#include"Chess.h"
USING_NS_CC;
using namespace std;

#include <string>
using std::string;

class AfterBattle :public Ref
{
public:

	virtual bool init();

	CREATE_FUNC(AfterBattle);

    void damage(vector<Chess*> A, vector<Chess*> B,PlayerInfo a,PlayerInfo b);//���ڼ����һ����ս���л�ʤ,����ȥ��Ӧ��hp

	bool detect(PlayerInfo a, PlayerInfo b);//����Ƿ����������

private:
	
};
