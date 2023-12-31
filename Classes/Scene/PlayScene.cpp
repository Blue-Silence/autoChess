/******************************************************/
/*               文件名：PlayScene.cpp                */
/*               功能：游戏场景                       */
/******************************************************/
#include "PlayScene.h"
#include "StartAndLoginScene.h"
#include "Hero.h"


Scene* PlayScene::createScene(bool isServer,ConPort* cone, const string& gameMode)
{
	auto p=PlayScene::create();
	p->server = isServer;
	p->connection = cone;    
	p->changeMode(gameMode);
	
	return p;
}

Scene* PlayScene::createScene()
{
	auto p = PlayScene::create();
	return p;
}

bool PlayScene::init()
{
	if (!Scene::init()) // 对父类init方法的判断
		return false;

	//播放背景音乐
	playSceneBGM = SimpleAudioEngine::getInstance();	
	playSceneBGM->playBackgroundMusic("res/Music/playScene_bgm.wav", true);

	// 需要用到的单例工具
	auto texture = Director::getInstance()->getTextureCache();
	auto config = ConfigController::getInstance();

	// 获取屏幕上的特殊点位
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto buttonPositiony = visibleSize.height / 3;	//	The y position of two buttons

	// 创建鼠标事件监听器
	mouseListener = EventListenerMouse::create();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	// 添加背景层
	playLayer = Layer::create();
	playLayer->setPosition(origin);
	playLayer->setContentSize(visibleSize);
	this->addChild(playLayer);
	CC_SAFE_RETAIN(playLayer);

	// 添加背景图片
	auto backGround = Sprite::create("/res/Background/background2.png");
	backGround->setPosition(visibleSize / 2);
	Vec2 originSize = backGround->getContentSize();
	backGround->setScale(visibleSize.height / originSize.y);
	playLayer->addChild(backGround, 1);

	// 创建棋盘
	chessBoardModel = ChessBoard::create();
	createBoard(Vec2(config->getPx()->x * 38, config->getPx()->y * 30));
	CC_SAFE_RETAIN(chessBoardModel);

	// 创建玩家
	playerME = PlayerInfo::create();
	playerOPP = PlayerInfo::create();
	CC_SAFE_RETAIN(playerME);
	CC_SAFE_RETAIN(playerOPP);

	playerME->isServer = server;

	//创建金币显示
	player_coin = Label::createWithTTF(std::to_string(playerME->getcoin()), "fonts/Marker Felt.ttf", 30);
	player_coin->setPosition(Vec2(760, 140));
	playLayer->addChild(player_coin, 5);
	Sprite* coinImage = Sprite::create("/res/UI/coin.png");
	coinImage->setPosition(Vec2(800, 140));
	coinImage->setScale(0.07f);
	playLayer->addChild(coinImage, 5);

	//初始化备战席
	preArea = PreparationSeat::create(playerME, chessBoardModel, mouseListener, playLayer, &canBuyChess,player_coin);
	CC_SAFE_RETAIN(preArea);


	//创建等级显示
	player_level = Label::createWithTTF(std::to_string(playerME->getLevel()), "fonts/Marker Felt.ttf", 30);
	player_level->setPosition(Vec2(760, 90));
	playLayer->addChild(player_level, 5);
	Sprite* levelImage = Sprite::create("res/UI/Level.png");
	levelImage->setPosition(Vec2(800, 90));
	levelImage->setScale(0.07f);
	playLayer->addChild(levelImage, 5);

	// 创建血量显示
	player_lifevalue = Label::createWithTTF(std::to_string(playerME->GetLifeValue()), "fonts/Marker Felt.ttf", 30);
	player_lifevalue->setPosition(Vec2(760, 40));
	playLayer->addChild(player_lifevalue, 5);
	Sprite* lifeValue = Sprite::create("res/UI/lifeValue.png");
	lifeValue->setPosition(Vec2(800, 40));
	lifeValue->setScale(0.07f);
	playLayer->addChild(lifeValue, 5);

	// 创建小小英雄
	createShop(Vec2(-45 * config->getPx()->x, -45 * config->getPx()->y));
	{
		auto littleHero = new Hero("/res/UI/LittleHero.png");
		if (littleHero == nullptr)
			throw "";
		auto pos = Vec2(0, 0);
		littleHero->init(pos);
		littleHero->setPosition(pos);
		littleHero->startListen();
		this->addChild(littleHero, 6);
	}

	
	// 添加AI建议文本栏
	AILabel->setPosition(Vec2(visibleSize.width*0.25, visibleSize.height*0.78)); // 根据需要调整位置
	AILabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	AILabel->setColor(Color3B::WHITE); // 设置颜色
	// 将标签添加到场景或层中
	this->addChild(AILabel);

	// 添加AI对话助手图像
	Sprite* chatGPT = Sprite::create("/res/Background/ChatGPT_logo.png");
	chatGPT->setPosition(Vec2(visibleSize.width *0.15, visibleSize.height * 0.75));
	chatGPT->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	originSize = chatGPT->getContentSize();
	float scaleX = 13 * config->getPx()->x / originSize.x;
	float scaleY = 10 * config->getPx()->y / originSize.y;
	chatGPT->setScale(scaleX, scaleY);
	this->addChild(chatGPT, 5);

	// 添加时钟图像
    Sprite* clock = Sprite::create("/res/Background/clock.png");
    clock->setPosition(Vec2(visibleSize.width / 22, visibleSize.height * 9 / 10));
	originSize = clock->getContentSize();
	scaleX = 10 * config->getPx()->x / originSize.x;
	scaleY = 10 * config->getPx()->y / originSize.y;
	clock->setScale(scaleX,scaleY);
	this->addChild(clock, 5);

	//添加设置按钮
	auto settingButton = StartAndLoginScene::createGameButton("/res/UI/setting.png", "/res/UI/setting.png",
		CC_CALLBACK_1(PlayScene::menuSettingsCallBack, this));
	originSize = settingButton->getContentSize();
	settingButton->setScale(10 * ConfigController::getInstance()->getPx()->x / originSize.x);
	settingButton->setPosition(Vec2(visibleSize.width * 8 / 9, visibleSize.height * 9 / 10));

	// 添加退出按钮
	auto exitButton = StartAndLoginScene::createGameButton("/res/UI/ExitNormal.png", "/res/UI/ExitSelected.png",
		CC_CALLBACK_1(PlayScene::menuExitCallBack, this));
	exitButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	originSize = exitButton->getContentSize();
	exitButton->setScale(30 * ConfigController::getInstance()->getPx()->x / originSize.x);
	exitButton->setPosition(Vec2(visibleSize.width, visibleSize.height / 10));

    auto menu = Menu::create(exitButton, settingButton, nullptr);
	menu->setPosition(Vec2(0, 0));
    playLayer->addChild(menu, 5);


	// 调度启动update()函数，开始战斗
	this->scheduleUpdate();
	
	return true;
}

void PlayScene::showEndGameAnimation(const string& gameResult)
{
	string imagePath = "";
	string soundEffectPath = "";
	if (gameResult == "Win")
	{
		imagePath = "/res/Victory.png";
		soundEffectPath = "/res/Music/victory.wav";
	}
	else if (gameResult == "Lost")
	{
		imagePath = "/res/Defeat.png";
		soundEffectPath = "/res/Music/defeat.wav";
	}
	else if (gameResult == "Draw")
	{
		imagePath = "/res/Draw.png";
		soundEffectPath = "/res/Music/draw.wav";
	}

	auto config = ConfigController::getInstance();

	// 获取当前视图的大小和原点
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 创建并设置图片
	auto endGameImage = Sprite::create(imagePath);
	endGameImage->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	Vec2 OriginSize = endGameImage->getContentSize();
	float chessScaleX = 88.9 * config->getPx()->x / OriginSize.x;
	float chessScaleY = 50 * config->getPx()->y / OriginSize.y;
	endGameImage->setScale(chessScaleX, chessScaleY);

	// 添加图片到当前场景并执行动画
	this->addChild(endGameImage);
	auto endAction = ScaleBy::create(5.0f, 2.0f);
	auto endActionReverse = endAction->reverse();
	auto callback = CallFunc::create([soundEffectPath, endGameImage, this]()
		{
			endGameImage->removeFromParent();
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundEffectPath.c_str());
		});
	Sequence* sequence = Sequence::create(endAction, endActionReverse, callback, nullptr);

	endGameImage->runAction(sequence);
}

void PlayScene::update(float delta)
{
	if (gameMode == "人机对战")
	{
		if (AI == nullptr && AINum != 0)
			AI = new AIMode(AINum);

		if (!isInBattle && AI != nullptr)
		{
			player_coin->setString(std::to_string(playerME->getcoin()));
			player_lifevalue->setString(to_string(playerME->GetLifeValue()));
			isInBattle = true;
			AI->eachTurnOver();
			if (!AI->existLiveAI())
			{

				// 战斗结束,玩家获胜
				showEndGameAnimation("Win");

				// 取消对update函数的调度
				this->unscheduleUpdate();
				return;
			}
			else if (!playerME->isAlive())
			{
				// AI获胜
				showEndGameAnimation("Lost");
				// 取消对update函数的调度
				this->unscheduleUpdate();
				return;
			}

			//AI++;
			TargetAI = (TargetAI+1) % (AINum)+1;
			while (!(*AI->getPlayersVectorInfo())[TargetAI].isAlive())
			{
				TargetAI = (TargetAI+1) % (AINum)+1;
			}
			// 选定好本轮对战的AI选手
			playerOPP = &(*AI->getPlayersVectorInfo())[TargetAI];
			// 其余的AI随机扣血
			AI->randomBloodLose(TargetAI);

			// 玩家和AI开始对战！
			// 计时器,计时结束后开始对战
			auto clockLayer = Clock::create(playerME, playerOPP, &isInBattle,&canBuyChess,&isTransmittingInfo,gameMode);
			this->addChild(clockLayer, 6);

			// 向AI发送API请求
			string message = "The number of my tank heroes is " + to_string(playerME->getTankNum())
				+ ", the number of mage heroes is " + to_string(playerME->getMagesNum()) + ",the number of shooter is " + to_string(playerME->getShooterNum())
				+ ", my level is " + to_string(playerME->getLevel())+",my lifevalue is "+to_string(playerME->GetLifeValue());

			////string message = "tank,mages,shooters,which one should I buy firstly,please speak in short english";
			string response = chatAI.performRequest(url, message);
			string content = chatAI.extractContent(response);
			AILabel->setString(content);
		}
	}
	else if (gameMode == "联机对战")
	{
		if(!isInBattle)
		{
			isInBattle = true;
			player_coin->setString(std::to_string(playerME->getcoin()));
			player_lifevalue->setString(to_string(playerME->GetLifeValue()));
			// 向AI发送API请求
			string message = "The number of my tank heroes is " + to_string(playerME->getTankNum())
				+ ", the number of mage heroes is " + to_string(playerME->getMagesNum()) + ",the number of shooter is " + to_string(playerME->getShooterNum())
				+ ", my level is " + to_string(playerME->getLevel()) + ",my lifevalue is " + to_string(playerME->GetLifeValue());

			////string message = "tank,mages,shooters,which one should I buy firstly,please speak in short english";
			string response = chatAI.performRequest(url, message);
			string content = chatAI.extractContent(response);
			AILabel->setString(content);
			// 计时器,计时结束后打包传送信息
			auto clockLayer = Clock::create(playerME, playerOPP, &isInBattle, &canBuyChess,&isTransmittingInfo,gameMode);
			this->addChild(clockLayer, 6);
			
		}
		else if (isTransmittingInfo)
		{
			isTransmittingInfo = false;
			canBuyChess = false;
			// 1-打包信息
			packageInfo();
			// 2-发送信息
			/////////////
			if(!this->connection)
				throw "Connection lost";
			if(!this->connection->checkValid())
				throw "Connection lost";
			this->connection->sendPack((const char*)&(this->myInfo), sizeof(this->myInfo));
			// 
			// 3-接收信息
			// 把接收到的信息赋值给成员变量oppInfo
			// 
			for (;;)
			{
				if (!this->connection->checkValid())
					throw "Connection lost";
				Packet* packRecv = (Packet*)this->connection->getPack(sizeof(this->oppInfo));
				if (packRecv)
				{
					this->oppInfo = *packRecv;
					delete[] (char *)packRecv;
					break;
				}
				Sleep(200); //等待200ms再试

			}
			// 
			// 4-提取信息
			extractInfo();
			// 接收成功后,才执行下面这一句
			beginFighting = true;

		}
		else if (beginFighting)
		{
			beginFighting = false;
			// 检查并移除现有的 BattleLayer
			auto existingLayer = this->getChildByTag(BATTLE_LAYER_TAG);
			if (existingLayer) {
				this->removeChild(existingLayer, true);
			}

			// 创建并添加新的 BattleLayer,开始战斗
			auto battleLayer = BattleLayer::create(playerME, playerOPP, &isInBattle, &canBuyChess);
			battleLayer->setTag(BATTLE_LAYER_TAG); // 设置标签以便识别
			this->addChild(battleLayer, 6);

			
		}
	}
}

// 联网下：打包我方场上棋子信息
void PlayScene::packageInfo()
{
	myInfo.totalChessNum = 0;
	for (auto chess : *playerME->getBattleAreaChesses())
	{
		if (chess != nullptr)
		{
			string career = chess->getCareer();
			int name = chess->getChessName();
			int level = chess->getChessLevel();
			int posX = chess->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getX();
			int posY = chess->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getY();

			myInfo.chesses[myInfo.totalChessNum++] = { career, name, level, posX, posY };
		}
	}
}

// 联网下：提取对方场上棋子信息
void PlayScene::extractInfo()
{
	playerOPP->clearVector();
	shared_ptr<Chess> chess = nullptr;
	for (int i = 0; i < oppInfo.totalChessNum; ++i)
	{
		string career = oppInfo.chesses[i].chessCareer;
		int name = oppInfo.chesses[i].chessName;
		int level = oppInfo.chesses[i].chessLevel;
		int posX = COL_BOARD-1-oppInfo.chesses[i].chessBoardPosX;
		int posY = ROW_BOARD -1-oppInfo.chesses[i].chessBoardPosY;


		if (career == "shooter")
		{
			chess = make_shared<shooter>(name);
			playerOPP->putChessInBattleArea(chess);
		}
		else if (career == "mage")
		{
			chess = make_shared<mage>(name);
			playerOPP->putChessInBattleArea(chess);
		}
		else
		{
			chess = make_shared<tank>(name);
			playerOPP->putChessInBattleArea(chess);
		}
		chess->setChessLevel(level);
		chess->setChessCoordinateByType(Vec2(posX, posY), CoordinateType::chessBoardCoordinates);
		ChessCoordinate* newPos = new ChessCoordinate;
		CoordinateConvert(CoordinateType::screenCoordinates, Vec2(posX, posY), newPos);
		chess->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);
		delete newPos;
	}
}



// 设置按钮的回调函数
void PlayScene::menuSettingsCallBack(cocos2d::Ref* pSender) 
{
	// 替换当前场景为 ConfigScene
	auto configScene = ConfigScene::createScene();
	configScene->setConfigPortal(&AINum);
	CC_SAFE_RETAIN(configScene);
	Director::getInstance()->pushScene(configScene);
}

// 设置按钮的回调函数
void PlayScene::menuNetworkSettingsCallBack(cocos2d::Ref* pSender)
{
	// 替换当前场景为 ConfigScene
	auto configScene = NetworkConfigSetting::createScene();
	//configScene->setConfigPortal(&AINum);
	CC_SAFE_RETAIN(configScene);
	Director::getInstance()->pushScene(configScene);
}


void PlayScene::onBattleButtonClicked(Ref* sender) 
{
	// 检查并移除现有的 BattleLayer
	auto existingLayer = this->getChildByTag(BATTLE_LAYER_TAG);
	if (existingLayer) {
		this->removeChild(existingLayer, true);
	}

	// 创建并添加新的 BattleLayer
	auto battleLayer = BattleLayer::create(playerME, playerOPP, &isInBattle,&canBuyChess);
	battleLayer->setTag(BATTLE_LAYER_TAG); // 设置标签以便识别
	this->addChild(battleLayer, 6);
}

void PlayScene::createBoard(Vec2 position)
{
	auto texture = Director::getInstance()->getTextureCache();
	auto config = ConfigController::getInstance();

	auto sprite = Sprite::create("res/Background/bgwhite.png");
	sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	Vec2 originSize = sprite->getContentSize();
	float scale = 10 * config->getPx()->x / originSize.x;
	sprite->setScale(scale);
	originSize.x *= (scale);
	originSize.y *= (scale);

	for (int i = 0; i < ROW_BOARD; i++)
	{
		for (int j = 0; j < COL_BOARD; j++)
		{
			if ((i+j)%2==0)
			{
				chessBoard[i].push_back(Sprite::create("res/Background/bgwhite.png"));
			}
			else
			{
				chessBoard[i].push_back(Sprite::create("res/Background/bgblack.png"));
			}
			chessBoard[i][j]->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			chessBoard[i][j]->setScale(scale);
			chessBoard[i][j]->setPosition(Vec2(position.x + originSize.x * j, position.y + originSize.y * i));
			playLayer->addChild(chessBoard[i][j], 2);
		}
	}

}

void PlayScene::createShop(Vec2 position)
{
	// 单例工具
	auto config = ConfigController::getInstance();
	auto texture = Director::getInstance()->getTextureCache();

	// 刷新及购买经验
	// 背景
	auto shopMore = Sprite::create("/res/Background/shop.png");
	Vec2 originSize = shopMore->getContentSize();
	shopMore->setScale(0.36f);
	auto shopSize = shopMore->getContentSize();
	auto pageSize = Director::getInstance()->getVisibleSize();
	Vec2 originPosition;
	originPosition.x = pageSize.width * 0.42;
	originPosition.y = shopSize.height * 0.36 / 2;
	shopMore->setPosition(originPosition);
	playLayer->addChild(shopMore, 5);
	// 两个菜单项
	auto buyExp = StartAndLoginScene::createGameButton("/res/UI/UpgradeNormal.png", "/res/UI/Upgradeclick.png", CC_CALLBACK_1(PlayScene::menuBuyExpCallBack, this));
	auto freshShop = StartAndLoginScene::createGameButton("/res/UI/RefreshNormal.png", "/res/UI/RefreshSelected.png", CC_CALLBACK_1(PlayScene::menuFreshShopCallBack, this));

	originSize = buyExp->getContentSize();
	double scale = 13.5 * 16.9 / 15 * config->getPx()->x / originSize.x;
	buyExp->setScale(scale);
	freshShop->setScale(scale);
	originPosition.x = pageSize.width * 0.17;
	originPosition.y = shopSize.height * 0.45 / 2;
	buyExp->setPosition(originPosition);
	originPosition.y = shopSize.height * 0.2 / 2;
	freshShop->setPosition(originPosition);
	auto menu = Menu::create(buyExp, freshShop, NULL);
	menu->setPosition(Vec2::ZERO);
	playLayer->addChild(menu, 6);
}

ChessCoordinate PlayScene::coordingrevert(Vec2 realPosition)
{
	auto config = ConfigController::getInstance();
	realPosition.x -= config->getPx()->x * 47.5;
	realPosition.y -= config->getPx()->y * 16;

	ChessCoordinate logPosition;
	float perLength = 6.5 * config->getPx()->x;
	logPosition.setX(static_cast<int>(realPosition.x) % static_cast<int>(perLength));
	logPosition.setY(static_cast<int>(realPosition.y) % static_cast<int>(perLength));

	return logPosition;
}

void PlayScene::menuExitCallBack(Ref* sender)
{
	Director::getInstance()->end();
}

//刷新商店的回调函数！！！
void PlayScene::menuFreshShopCallBack(Ref* sender)
{
	auto config = ConfigController::getInstance();
	if(playerME->getcoin()>=2)
	{
		playerME->PayForRefresh();
		player_coin->setString(std::to_string(playerME->getcoin()));
		shopModel = Market::create();
		shopModel->RefreshMarket();
		if (previousMenu) {
			previousMenu->removeFromParent();
		}


		auto chess1 = StartAndLoginScene::createGameButton(shopModel->chessList[0]->getChessImagePath(), shopModel->chessList[0]->getChessImagePath(), CC_CALLBACK_1(PlayScene::BuyChess, this, 0));
		auto chess2 = StartAndLoginScene::createGameButton(shopModel->chessList[1]->getChessImagePath(), shopModel->chessList[1]->getChessImagePath(), CC_CALLBACK_1(PlayScene::BuyChess, this, 1));
		auto chess3 = StartAndLoginScene::createGameButton(shopModel->chessList[2]->getChessImagePath(), shopModel->chessList[2]->getChessImagePath(), CC_CALLBACK_1(PlayScene::BuyChess, this, 2));
		auto chess4 = StartAndLoginScene::createGameButton(shopModel->chessList[3]->getChessImagePath(), shopModel->chessList[3]->getChessImagePath(), CC_CALLBACK_1(PlayScene::BuyChess, this, 3));
		auto chess5 = StartAndLoginScene::createGameButton(shopModel->chessList[4]->getChessImagePath(), shopModel->chessList[4]->getChessImagePath(), CC_CALLBACK_1(PlayScene::BuyChess, this, 4));
		chess1->setName("chess1");
		chess2->setName("chess2");
		chess3->setName("chess3");
		chess4->setName("chess4");
		chess5->setName("chess5");

		Vec2 originSize1 = chess1->getContentSize();
		float scale1 = 13.5 * 16.9 / 15 * config->getPx()->x / originSize1.x;
		Vec2 originSize2 = chess2->getContentSize();
		float scale2 = 13.5 * 16.9 / 15 * config->getPx()->x / originSize2.x;
		Vec2 originSize3 = chess3->getContentSize();
		float scale3 = 13.5 * 16.9 / 15 * config->getPx()->x / originSize3.x;
		Vec2 originSize4 = chess4->getContentSize();
		float scale4 = 13.5 * 16.9 / 15 * config->getPx()->x / originSize4.x;
		Vec2 originSize5 = chess5->getContentSize();
		float scale5 = 13.5 * 16.9 / 15 * config->getPx()->x / originSize5.x;
		chess1->setScale(scale1);
		chess2->setScale(scale2);
		chess3->setScale(scale3);
		chess4->setScale(scale4);
		chess5->setScale(scale5);
		chess1->setPosition(280, 80);
		chess2->setPosition(280 + 100, 80);
		chess3->setPosition(280 + 200, 80);
		chess4->setPosition(280 + 300, 80);
		chess5->setPosition(280 + 400, 80);
		auto menu = Menu::create(chess1, chess2, chess3, chess4, chess5, NULL);
		menu->setPosition(Vec2::ZERO);
		menu->setName("ChessMenu");
		playLayer->addChild(menu, 7);
		previousMenu = menu;

		//增加引用计数
		CC_SAFE_RETAIN(shopModel);
	}
	else {
		// 创建一个 Label，当没钱时显示
		cocos2d::Label* label = cocos2d::Label::createWithTTF("No money!!!", "fonts/Marker Felt.ttf", 24);
		label->setPosition(cocos2d::Vec2(500, 200));
		this->addChild(label);
		auto delayAction = cocos2d::DelayTime::create(1.0f);
		auto removeLabel = cocos2d::CallFunc::create([label]() {
			label->removeFromParent();
			});

		// 创建一个顺序动作，先延时，然后移除 Label
		auto sequence = cocos2d::Sequence::create(delayAction, removeLabel, nullptr);

		// 在 Label 上运行这个顺序动作
		label->runAction(sequence);
	}
}

//购买棋子的回调函数！！！
void PlayScene::BuyChess(Ref* sender, int index)
{
	if (canBuyChess)
	{
		if (playerME->getcoin() >= 3)
		{
			int location = playerME->GetMinIndex();
			int delLoc_1 = -1;
			int delLoc_2 = -1;
			int& ref_delLoc_1 = delLoc_1;
			int& ref_delLoc_2 = delLoc_2;
			if (sender && location != -1) {
				MenuItemImage* myButton = static_cast<MenuItemImage*>(sender);
				myButton->removeFromParent();
				playerME->payForHero();
				player_coin->setString(std::to_string(playerME->getcoin()));
				//备战席加入新棋子
				shared_ptr<Chess> purchasedChess = shopModel->chessList[index];
				playerME->chessInPreArea[location] = (purchasedChess);
				preArea->PromoteChessLevel(location);

			}
		}
		else
		{
			cocos2d::Label* label = cocos2d::Label::createWithTTF("No money!!!", "fonts/Marker Felt.ttf", 24);
			label->setPosition(cocos2d::Vec2(500, 200));
			this->addChild(label);
			auto delayAction = cocos2d::DelayTime::create(1.0f);
			auto removeLabel = cocos2d::CallFunc::create([label]() {
				label->removeFromParent();
				});

			// 创建一个顺序动作，先延时，然后移除 Label
			auto sequence = cocos2d::Sequence::create(delayAction, removeLabel, nullptr);

			// 在 Label 上运行这个顺序动作
			label->runAction(sequence);
		}
	}
}

//升级回调函数
void PlayScene::menuBuyExpCallBack(Ref* sender)
{
	if (playerME->getcoin() >= 4) {
		playerME->ChangeLevel();
		player_level->setString(std::to_string(playerME->getLevel()));
		player_coin->setString(std::to_string(playerME->getcoin()));
	}
	else {
		cocos2d::Label* label = cocos2d::Label::createWithTTF("No money!!!", "fonts/Marker Felt.ttf", 24);
		label->setPosition(cocos2d::Vec2(500, 200));
		this->addChild(label);
		auto delayAction = cocos2d::DelayTime::create(1.0f);
		auto removeLabel = cocos2d::CallFunc::create([label]() {
			label->removeFromParent();
			});

		// 创建一个顺序动作，先延时，然后移除 Label
		auto sequence = cocos2d::Sequence::create(delayAction, removeLabel, nullptr);

		// 在 Label 上运行这个顺序动作
		label->runAction(sequence);
	}
}
