/********************************************
 * ���ܣ����е���ʱ
 * ���ߣ�
 * �汾��1.0.0
 * ��Ϸ���棺Cocos2d-x 4.0
 * ******************************************
 * ˵����
 ********************************************/
#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "ui/CocosGUI.h" 
using namespace ui;

class Clock :public	Layer {
public:
	//init����
	virtual bool init();
	void reset(float time);
	virtual void update(float dt);

private:
	Label* timeLabel = Label::createWithSystemFont("", "Arial", 60);
	float timeLeft;
};
