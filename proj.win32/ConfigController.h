/********************************************
 * 功能：游戏场景控制
 * 作者：VenusHui
 * 版本：1.0.0
 * 位置：Classes/controller(筛选器)
 * 游戏引擎：Cocos2d-x 4.0
 * ******************************************
 * 说明：
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

	// 获取当前设备单位像素的大小
	Vec2* getPx() const ;

	/********************************************
	* 功能：对精灵进行缩放及位置调整
	* Sprite* sprite：待操作精灵
	* int scale：缩放后按钮再x轴上的逻辑长度
	* float pos_x：逻辑位置横坐标
	* float pos_y：逻辑位置纵坐标
	* ******************************************
	* 说明：这里的逻辑位置参见“px”的定义
	********************************************/
	void setScaleAndPosition(Sprite* sprite, int scale, float pos_x, float pos_y);

	//用于选中棋子
	Sprite* pickChessPiece(EventListenerTouchOneByOne* listener);

	//判断鼠标点击是否在合法范围内
	bool onTouchBegan(Touch* touch, Event* event);

	Sprite* onTouchEnded(Touch* touch, Event* event);

private:
	static  ConfigController* instance;
};

#endif // !_CONFIGCONTRLLER_H_
