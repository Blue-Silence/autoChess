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
#include "BattleLayer.h"
#include "ui/CocosGUI.h" 
using namespace ui;

const int BATTLE_LAYER_TAG = 100;

class Clock :public	Layer {
public:
	//init函数
	virtual bool init();
	void reset(float time);
	virtual void update(float dt);
    static Clock* create(PlayerInfo* playerME,PlayerInfo* playerOPP,bool* isInBattle) 
    {
        
        Clock* pRet = new(std::nothrow) Clock(playerME,playerOPP,isInBattle);
        if (pRet && pRet->init()) {
            pRet->autorelease();
            return pRet;
        }
        else {
            delete pRet;
            pRet = nullptr;
            return nullptr;
        }
    }
    Clock(PlayerInfo* ME,PlayerInfo* OPP,bool* inBattle) :playerME(ME),playerOPP(OPP),isInBattle(inBattle) {}

private:
    
	Label* timeLabel = Label::createWithSystemFont("", "Arial", 60);
	float timeLeft=5;
    bool*  isInBattle;
    PlayerInfo* playerME;
    PlayerInfo* playerOPP;
};
