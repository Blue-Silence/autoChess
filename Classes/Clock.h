/********************************************
 * 功能：进行倒计时
 * 作者：
 * 版本：1.0.0
 * 游戏引擎：Cocos2d-x 4.0
 * ******************************************
 * 说明：
 ********************************************/
#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "ui/CocosGUI.h" 
using namespace ui;

class Clock :public	Layer {
public:
	//init函数
	virtual bool init();
	void reset(float time);
	virtual void update(float dt);

private:
	Label* timeLabel = Label::createWithSystemFont("", "Arial", 60);
	float timeLeft;
};
