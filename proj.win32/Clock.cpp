#include <string>
using std::string;
using std::to_string;

#include"clock.h"

#define STR_OUTPUT_TIME(T) ((string{"Time:"}+to_string(static_cast<int>((T))))) //统一的倒计时显示格式

bool Clock::init()
{
	if (!Layer::init()) {
		return false;
	}

	//初始化成功

	// 获取屏幕上的各项参数
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto clock = Layer::create();
	clock->setPosition(origin);
	clock->setContentSize(visibleSize);
	this->addChild(clock);

	//此后对应人员请在此处加入相应的可视化实现，如进度条等

	timeLabel->setPosition(0,0);
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
		//归零后的操作。待实现。
	}
}
