#include "ConfigController.h"

ConfigController* ConfigController::instance = nullptr;
ConfigController* ConfigController::getInstance()
{
	if (instance == nullptr)
	{
		instance = new ConfigController();
	}
	return instance;
}

void ConfigController::destroyInstance()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

ConfigController::~ConfigController()
{
}

Vec2* ConfigController::getPx() const
{
	Vec2* px = &Vec2(Director::getInstance()->getVisibleSize().width / HORIZONTAL_PIXEL_NUM, Director::getInstance()->getVisibleSize().height / VERTICAL_PIXEL_NUM);
	return px;
}

void ConfigController::setScaleAndPosition(Sprite* sprite, int scale, float pos_x, float pos_y)
{
	auto originSize = sprite->getContentSize();
	sprite->setScale(scale / originSize.width);
	sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sprite->setPosition(getPx()->x * pos_x, getPx()->y * pos_y);
}

Sprite* ConfigController::onTouchEnded(Touch* touch, Event* event)
{
	Vec2 posi = touch->getLocation();

	return nullptr;
}
