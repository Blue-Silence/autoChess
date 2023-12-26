#include <string>
using std::string;
using std::to_string;

#include"clock.h"

#define STR_OUTPUT_TIME(T) ((string{""}+to_string(static_cast<int>((T))))) //ͳһ�ĵ���ʱ��ʾ��ʽ

bool Clock::init()
{
	if (!Layer::init()) {
		return false;
	}

	//��ʼ���ɹ�

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto clock = Layer::create();

	// ����λ��Ϊ��Ļ���Ͻ�
	clock->setPosition(Vec2(origin.x, origin.y + visibleSize.height ));

	clock->setContentSize(visibleSize);
	this->addChild(clock);


	//�˺��Ӧ��Ա���ڴ˴�������Ӧ�Ŀ��ӻ�ʵ�֣����������

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
		
		//�����Ĳ�������ʵ�֡�
		auto existingLayer = this->getChildByTag(BATTLE_LAYER_TAG);
		if (existingLayer) {
			this->removeChild(existingLayer, true);
		}

		// ����������µ� BattleLayer
		auto battleLayer = BattleLayer::create(playerME, playerOPP, isInBattle);
		battleLayer->setTag(BATTLE_LAYER_TAG); // ���ñ�ǩ�Ա�ʶ��
		this->addChild(battleLayer, 7);
	}
}
