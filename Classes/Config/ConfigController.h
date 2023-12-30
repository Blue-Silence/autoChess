/********************************************
 * ���ܣ���Ϸ��������
 * ���ߣ�VenusHui
 * �汾��1.0.0
 * λ�ã�Classes/controller(ɸѡ��)
 * ��Ϸ���棺Cocos2d-x 4.0
 * ******************************************
 * ˵����
 ********************************************/
#ifndef _CONFIGCONTRLLER_H_
#define _CONFIGCONTRLLER_H_

//#include"ChessBoard.h"
#include <cocos2d.h>
USING_NS_CC;
#include <ui/CocosGUI.h>
using namespace ui;
#include<vector>
using namespace std;

#define HORIZONTAL_PIXEL_NUM 160
#define VERTICAL_PIXEL_NUM 90

class ConfigController
{
public:
	static ConfigController* getInstance();

	static void destroyInstance();

	~ConfigController();

	// ��ȡ��ǰ�豸��λ���صĴ�С
	Vec2* getPx() const ;

	/********************************************
	* ���ܣ��Ծ���������ż�λ�õ���
	* Sprite* sprite������������
	* int scale�����ź�ť��x���ϵ��߼�����
	* float pos_x���߼�λ�ú�����
	* float pos_y���߼�λ��������
	* ******************************************
	* ˵����������߼�λ�òμ���px���Ķ���
	********************************************/
	void setScaleAndPosition(Sprite* sprite, int scale, float pos_x, float pos_y);

	//����ѡ������
	Sprite* pickChessPiece(EventListenerTouchOneByOne* listener);

	//�ж�������Ƿ��ںϷ���Χ��
	bool onTouchBegan(Touch* touch, Event* event);

	Sprite* onTouchEnded(Touch* touch, Event* event);

private:
	static  ConfigController* instance;
};

#endif // !_CONFIGCONTRLLER_H_
