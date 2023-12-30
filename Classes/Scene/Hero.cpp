#include <string>
using std::string;
using std::to_string;

#include"Hero.h"

#define TIME_PER_DIS 0.005 //用于控制速度


bool Hero::init(Vec2 location)
{
	if (!Layer::init()) {
		return false;
	}

	CCLOG("\n\n\nHell yes!!!\n\n\n");
	//throw "";

	//初始化成功

	// 获取屏幕参数
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();


	hero->setPosition(0, 0);
	hero->setScale(0.2);
	this->addChild(hero);

	this->location = location;

	return true;
}


void Hero::update(float dt)
{
}

void Hero::startListen()
{
	this->mouseListener->onMouseDown = [this](EventMouse* event) {

		if (this->currentMove)
			this->hero->stopAction(currentMove); //停止先前的运动
		//auto p = Vec2{ event->getCursorX() - this->location.x,event->getCursorY() - this->location.y };
		
		auto dx = (event->getCursorX() - this->location.x) - this->hero->getPosition().x;
		auto dy = (event->getCursorY() - this->location.y) - this->hero->getPosition().y;
		auto dis = std::sqrt(dx * dx + dy * dy);
		
		//auto a = p.x;
		//log("%f %f %f %f\n", p.x, p.y, event->getLocation().x, event->getLocation().y);
		//p.x -= this->location.x;
		//p.y -= this->location.y; 
		//auto moveBy = MoveTo::create(dis * TIME_PER_DIS, p);
		this->currentMove = MoveBy::create(dis * TIME_PER_DIS, Vec2{ dx,dy });
		if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
			this->hero->runAction(this->currentMove);
		};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(this->mouseListener, this);
	

}
