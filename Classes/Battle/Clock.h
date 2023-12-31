
#pragma once

#include "cocos2d.h"
USING_NS_CC;
#include "Battle/BattleLayer.h"
#include "ui/CocosGUI.h" 
using namespace ui;

const int BATTLE_LAYER_TAG = 100;

class Clock :public	Layer {
public:
	//init����
	virtual bool init();
	void reset(float time);
	virtual void update(float dt);
    static Clock* create(PlayerInfo* playerME,PlayerInfo* playerOPP,bool* isInBattle,bool* canBuyChess,bool* isTransmittingInfo,string mode) 
    {
        
        Clock* pRet = new(std::nothrow) Clock(playerME,playerOPP,isInBattle,canBuyChess,isTransmittingInfo,mode);
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
    

private:
    Clock(PlayerInfo* ME, PlayerInfo* OPP, bool* inBattle, bool* BuyChess,bool* TransmittingInfo,string mode) :
        playerME(ME), playerOPP(OPP), isInBattle(inBattle), canBuyChess(BuyChess),isTransmittingInfo(TransmittingInfo),gameMode(mode) {}
	Label* timeLabel = Label::createWithSystemFont("", "Arial", 60);
	float timeLeft=10;
    bool*  isInBattle;
    bool* canBuyChess;
    bool* isTransmittingInfo;
    PlayerInfo* playerME;
    PlayerInfo* playerOPP;
    string gameMode;
    
};
