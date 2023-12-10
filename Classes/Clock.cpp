#include <string>
using std::string;
using std::to_string;

#include"clock.h"

#define STR_OUTPUT_TIME(T) ((string{"Time:"}+to_string(static_cast<int>((T))))) //ͳһ�ĵ���ʱ��ʾ��ʽ

bool Clock::init()
{
	if (!Layer::init()) {
		return false;
	}

	//��ʼ���ɹ�

	// ��ȡ��Ļ�ϵĸ������
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto clock = Layer::create();
	clock->setPosition(origin);
	clock->setContentSize(visibleSize);
	this->addChild(clock);

	//�˺��Ӧ��Ա���ڴ˴�������Ӧ�Ŀ��ӻ�ʵ�֣����������

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
		//�����Ĳ�������ʵ�֡�
	}
}
