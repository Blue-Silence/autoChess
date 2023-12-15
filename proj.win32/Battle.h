/********************************************
 * ���ܣ����ӻ���ս��
 * ���ߣ�lxy
 * �汾��1.0.0
 * ��Ϸ���棺Cocos2d-x 4.0
 * ******************************************
 * ˵����ս�������У����û�а취�����κβ�����
 * �����ӻ��๥�����ɡ�
 ********************************************/

#pragma once
#include"PlayerInfo.h"
#include"ChessBoard.h"

class Battle :public Ref
{
public:
	virtual bool init();

	//CREATE_FUNC(Battle);

	//�������
	double getDistance(ChessCoordinate* start, ChessCoordinate* end);

	void pieceBattle(Chess &damageMaker, Chess &victim);

	Chess* findEnemy(Chess* damageMaker, PlayerInfo enemy);

	//����һ����ͨ��Ѱ·����,�������������߻��������ߵ��ƶ�
	void normalTraceTrack(Chess* origin, Chess* destination);

	bool detect(PlayerInfo A);//�����ҵ�ս�������Ƿ���ʣ�࣬���У�����й������ƶ�
};