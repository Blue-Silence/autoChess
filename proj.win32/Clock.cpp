#include <string>
using std::string;
using std::to_string;

#include"clock.h"

#define STR_OUTPUT_TIME(T) ((string{""}+to_string(static_cast<int>((T))))) //统一的倒计时显示格式

bool Clock::init()
{
	if (!Layer::init()) {
		return false;
	}

	//初始化成功

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto clock = Layer::create();

	// 设置位置为屏幕左上角
	clock->setPosition(Vec2(origin.x, origin.y + visibleSize.height ));

	clock->setContentSize(visibleSize);
	this->addChild(clock);


	//此后对应人员请在此处加入相应的可视化实现，如进度条等

	timeLabel->setPosition(Vec2(origin.x+ 0.1*visibleSize.width, origin.y + 0.9*visibleSize.height ));
	this->addChild(timeLabel);
	this->scheduleUpdate();
	return true;
}

void Clock::reset(float time) {
	timeLeft = time;
	timeLabel->setString(STR_OUTPUT_TIME(timeLeft));
}

void Clock::update(float dt)
{
	if (timeLeft > 0.5f) 
	{
		timeLeft -= dt;
		timeLabel->setString(STR_OUTPUT_TIME(timeLeft));
	}
	else
	{
		this->unscheduleUpdate();
		timeLabel->removeFromParent();
		
		//归零后的操作。待实现。
		auto existingLayer = this->getChildByTag(BATTLE_LAYER_TAG);
		if (existingLayer) {
			this->removeChild(existingLayer, true);
		}

		// 创建并添加新的 BattleLayer
		auto battleLayer = BattleLayer::create(playerME, playerOPP, isInBattle);
		battleLayer->setTag(BATTLE_LAYER_TAG); // 设置标签以便识别
		this->addChild(battleLayer, 7);
	}
}
