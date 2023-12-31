#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "ui/CocosGUI.h" 
using namespace ui;

class Hero :public	Layer {

private:
	Sprite* hero;

	EventListenerMouse* mouseListener = EventListenerMouse::create();

	Action* currentMove = nullptr; //��ǰ�ƶ�

	Vec2 location;

public:
	Hero(const char *name) { hero = Sprite::create(name); }
	//init����
	virtual bool init(Vec2 location);
	virtual void update(float dt);
	void startListen();


};