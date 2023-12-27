#include "PlayScene.h"
#include "StartAndLoginScene.h"

#include "Hero.h"

Scene* PlayScene::createScene()
{
	return PlayScene::create();
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
	//auto magebuff = cocos2d::Sprite::create("/res/Bond/mage.png");
	//magebuff->setPosition(100, 100);
	//playLayer->addChild(magebuff, 1);
	// 创建棋盘
	chessBoardModel = ChessBoard::create();
	createBoard(Vec2(config->getPx()->x * 38, config->getPx()->y * 30));
	CC_SAFE_RETAIN(chessBoardModel);
	// 创建玩家
	playerA = PlayerInfo::create();
	playerOPP = PlayerInfo::create();
	CC_SAFE_RETAIN(playerA);
	CC_SAFE_RETAIN(playerOPP);

	//创建金币显示
	player_coin = Label::createWithTTF(std::to_string(playerA->getcoin()), "fonts/Marker Felt.ttf", 30);
	player_coin->setPosition(Vec2(760, 140));
	playLayer->addChild(player_coin, 5);
	Sprite* coinImage = Sprite::create("/res/UI/coin.png");
	coinImage->setPosition(Vec2(800, 140));
	coinImage->setScale(0.07);
	playLayer->addChild(coinImage, 5);

	//初始化备战席
	preArea = PreparationSeat::create(playerA, chessBoardModel, mouseListener, playLayer, &isInBattle,player_coin);
	CC_SAFE_RETAIN(preArea);


	//创建等级显示
	player_level = Label::createWithTTF(std::to_string(playerA->getLevel()), "fonts/Marker Felt.ttf", 30);
	player_level->setPosition(Vec2(760, 90));
	playLayer->addChild(player_level, 5);
	Sprite* levelImage = Sprite::create("res/UI/Level.png");
	levelImage->setPosition(Vec2(800, 90));
	levelImage->setScale(0.07);
	playLayer->addChild(levelImage, 5);

	// 创建血量显示
	player_lifevalue = Label::createWithTTF(std::to_string(playerA->GetLifeValue()), "fonts/Marker Felt.ttf", 30);
	player_lifevalue->setPosition(Vec2(760, 40));
	playLayer->addChild(player_lifevalue, 5);
	Sprite* lifeValue = Sprite::create("res/UI/lifeValue.png");
	lifeValue->setPosition(Vec2(800, 40));
	lifeValue->setScale(0.07);
	playLayer->addChild(lifeValue, 5);

	// 创建商店
	createShop(Vec2(-45 * config->getPx()->x, -45 * config->getPx()->y));
	{
		auto littleHero = new Hero("/res/UI/LittleHero.png");
		if (littleHero == nullptr)
			throw "";
		auto pos = Vec2(visibleSize.width / 2, visibleSize.height / 2);
		littleHero->init(pos);
		littleHero->setPosition(pos);
		littleHero->startListen();
		this->addChild(littleHero, 6);
	}

	
	// 添加AI建议框
	AILabel->setPosition(Vec2(240, 160)); // 根据需要调整位置
	AILabel->setColor(Color3B::WHITE); // 设置颜色
	// 将标签添加到场景或层中
	this->addChild(AILabel);


	// 添加退出按钮
	auto exitButton = StartAndLoginScene::createGameButton("/res/UI/ExitNormal.png", "/res/UI/ExitSelected.png", CC_CALLBACK_1(PlayScene::menuExitCallBack, this));

	exitButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	originSize = exitButton->getContentSize();
	exitButton->setScale(10 * ConfigController::getInstance()->getPx()->x / originSize.x);
	exitButton->setPosition(Vec2(50 * ConfigController::getInstance()->getPx()->y, -35 * ConfigController::getInstance()->getPx()->y));


	auto menu = Menu::create(exitButton, nullptr);
	playLayer->addChild(menu, 5);


	// 调度启动update()函数，开始战斗
	this->scheduleUpdate();
	

	
	return true;
}

void PlayScene::update(float delta)
{
	if (gameMode == "人机对战")
	{
		if (AI == nullptr && AINum != 0)
			AI = new AIMode(AINum);


		if (!isInBattle && AI != nullptr)
		{
			isInBattle = true;
			AI->eachTurnOver();
			if (!AI->existLiveAI())
			{
				// 战斗结束,玩家获胜
				// 
				// 取消对update函数的调度
				this->unscheduleUpdate();
			}
			else if (!playerA->isAlive())
			{
				// AI获胜
				// 
				// 取消对update函数的调度
				this->unscheduleUpdate();
			}

			//AI++;
			TargetAI = (TargetAI) % (AINum)+1;
			while (!(*AI->getPlayersVectorInfo())[TargetAI].isAlive())
			{
				TargetAI = (TargetAI) % (AINum)+1;
			}
			// 选定好本轮对战的AI选手
			playerOPP = &(*AI->getPlayersVectorInfo())[TargetAI];
			// 其余的AI随机扣血
			AI->randomBloodLose(TargetAI);

			// 玩家和AI开始对战！
			// 计时器,计时结束后开始对战
			auto clockLayer = Clock::create(playerA, playerOPP, &isInBattle);
			this->addChild(clockLayer, 6);

			// 向AI发送API请求
			/*string message = "The number of my tank heroes is " + to_string(playerA->getTankNum())
				+ ", the number of mage heroes is " + to_string(playerA->getMagesNum()) + ",the number of shooter is " + to_string(playerA->getShooterNum())
				+ ", my level is" + to_string(playerA->getLevel());*/
			string message = "tank,mages,shooters,which one should I buy firstly,please speak in short english";
			string response = chatAI.performRequest(url, message);
			string content = chatAI.extractContent(response);
			AILabel->setString(content);





		}
	}
	else if (gameMode == "联机对战")
	{



	}
}


void PlayScene::onBattleButtonClicked(Ref* sender) 
{
	// 检查并移除现有的 BattleLayer
	auto existingLayer = this->getChildByTag(BATTLE_LAYER_TAG);
	if (existingLayer) {
		this->removeChild(existingLayer, true);
	}

	// 创建并添加新的 BattleLayer
	auto battleLayer = BattleLayer::create(playerA, playerOPP, &isInBattle);
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
	auto shopMore = Sprite::create("/res/UI/ShoppingMore.png");
	Vec2 originSize = shopMore->getContentSize();
	float scale = 16.9 * config->getPx()->x / originSize.x;
	shopMore->setScale(scale);
	shopMore->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	shopMore->setPosition(Vec2(position.x + 80 * config->getPx()->x, position.y + 45 * config->getPx()->y));
	Vec2 originPosition = Vec2(shopMore->getPositionX(), shopMore->getPositionY());
	playLayer->addChild(shopMore, 5);
	// 两个菜单项
	auto buyExp = StartAndLoginScene::createGameButton("/res/UI/UpgradeNormal.png", "/res/UI/Upgradeclick.png", CC_CALLBACK_1(PlayScene::menuBuyExpCallBack, this));
	auto freshShop = StartAndLoginScene::createGameButton("/res/UI/RefreshNormal.png", "/res/UI/RefreshSelected.png", CC_CALLBACK_1(PlayScene::menuFreshShopCallBack, this));
	
	originSize = buyExp->getContentSize();
	scale = 13.5 * 16.9 / 15 * config->getPx()->x / originSize.x;
	buyExp->setScale(scale);
	freshShop->setScale(scale);
	buyExp->setPosition(Vec2(originPosition.x - 16.9 / 2 * config->getPx()->x, originPosition.y + 10.5 * 16.9 / 15 * config->getPx()->y));
	freshShop->setPosition(Vec2(originPosition.x - 16.9 / 2 * config->getPx()->x, originPosition.y + 4 * 16.9 / 15 * config->getPx()->y));
	auto menu = Menu::create(buyExp,freshShop, NULL);
	menu->setPosition(Vec2::ZERO);
	playLayer->addChild(menu, 6);
	

	// 棋子及装备卡片
	
}
//不需要该函数了
MenuItemSprite* PlayScene::createPieceCard(string pieceName, string piecePicPath, Vec2 position, const ccMenuCallback& callback)
{
	auto texture = Director::getInstance()->getTextureCache();
	auto config = ConfigController::getInstance();
	float adjust = 160 / 22;
	//CsvParser csv;
	//csv.parseWithFile("Data/PiecesData.csv");

	// 创建卡片精灵
	auto cardBack = Sprite::createWithTexture(texture->getTextureForKey("/res/UI/ShoppingCard.png"));
	
	// 创建一个精灵菜单项
	auto item = MenuItemSprite::create(cardBack, cardBack, callback);
	item->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	// 卡片相关
	auto sprite = Sprite::createWithTexture(texture->getTextureForKey(piecePicPath));
	sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	Vec2 originSize = item->getContentSize();
	item->setPosition(position.x, position.y);
	Vec2 originPosition = Vec2(sprite->getPositionX(), sprite->getPositionY());

	// 棋子图片相关
	originSize = sprite->getContentSize();
	sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	float scale = 8.5 * config->getPx()->x / originSize.x * adjust;
	sprite->setScale(scale);
	sprite->setPosition(originPosition.x + 38 * config->getPx()->x, originPosition.y + 67 * config->getPx()->y);
	originPosition = Vec2(sprite->getPositionX(), sprite->getPositionY());
	item->addChild(sprite, 6);

	// 名称相关
	//Value data = Value(csv[0][2].c_str());
	auto nameLabel = Label::createWithTTF(pieceName, "fonts/Marker Felt.ttf", 30 * adjust);
	nameLabel->setColor(Color3B::BLACK);
	nameLabel->setPosition(Vec2(originPosition.x, originPosition.y - 59 * config->getPx()->y));
	item->addChild(nameLabel, 6);

	// 各项属性相关
	auto healthIcon = Sprite::createWithTexture(texture->getTextureForKey("/res/Icons/Health.png")); // the Health icon（生命）
	auto healthValue = Label::createWithTTF("1000", "fonts/Marker Felt.ttf", 27 * adjust); //the value of Health icon
	auto attackIcon = Sprite::createWithTexture(texture->getTextureForKey("/res/Icons/Attack.png")); //the Attack icon(攻击)
	auto attackValue = Label::createWithTTF("1000", "fonts/Marker Felt.ttf", 27 * adjust); //the value of Attack
	auto armorIcon = Sprite::createWithTexture(texture->getTextureForKey("/res/Icons/Armor.png")); //the Armor icon(防御)
	auto armorValue = Label::createWithTTF("1000", "fonts/Marker Felt.ttf", 27 * adjust); //the value of Armor
	originSize = healthIcon->getContentSize();
	scale = 3 * config->getPx()->x / originSize.x * adjust;
	healthIcon->setScale(scale);
	attackIcon->setScale(scale);
	armorIcon->setScale(scale);
	healthValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	attackValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	armorValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	healthValue->setColor(Color3B::BLACK);
	attackValue->setColor(Color3B::BLACK);
	armorValue->setColor(Color3B::BLACK);
	healthIcon->setPosition(Vec2(originPosition.x + 10 * adjust * config->getPx()->x, originPosition.y + 3 * adjust * config->getPx()->y));
	attackIcon->setPosition(Vec2(originPosition.x + 10 * adjust * config->getPx()->x, originPosition.y - 1 * adjust * config->getPx()->y));
	armorIcon->setPosition(Vec2(originPosition.x + 10 * adjust * config->getPx()->x, originPosition.y - 5 * adjust * config->getPx()->y));
	healthValue->setPosition(Vec2(originPosition.x + 12 * adjust * config->getPx()->x, originPosition.y + 3 * adjust * config->getPx()->y));
	attackValue->setPosition(Vec2(originPosition.x + 12 * adjust * config->getPx()->x, originPosition.y - 1 * adjust * config->getPx()->y));
	armorValue->setPosition(Vec2(originPosition.x + 12 * adjust * config->getPx()->x, originPosition.y - 5 * adjust * config->getPx()->y));
	item->addChild(healthIcon, 6);
	item->addChild(attackIcon, 6);
	item->addChild(armorIcon, 6);
	item->addChild(healthValue, 6);
	item->addChild(attackValue, 6);
	item->addChild(armorValue, 6);

	originSize = item->getContentSize();
	scale = 22 * config->getPx()->x / originSize.x;
	item->setScale(scale);

	return item;
}

ChessCoordinate* PlayScene::coordingrevert(Vec2 realPosition)
{
	auto config = ConfigController::getInstance();
	realPosition.x -= config->getPx()->x * 47.5;
	realPosition.y -= config->getPx()->y * 16;

	ChessCoordinate logPosition;
	float perLength = 6.5 * config->getPx()->x;
	logPosition.setX(static_cast<int>(realPosition.x) % static_cast<int>(perLength));
	logPosition.setY(static_cast<int>(realPosition.y) % static_cast<int>(perLength));

	return &logPosition;
}

void PlayScene::menuExitCallBack(Ref* sender)
{
	Director::getInstance()->end();
}

void PlayScene::menuPieceCardCallBack(Ref* sender)
{
	
}


//刷新商店的回调函数！！！
void PlayScene::menuFreshShopCallBack(Ref* sender)
{
	auto config = ConfigController::getInstance();
	if(playerA->getcoin()>=2)
	{
		playerA->PayForRefresh();
		player_coin->setString(std::to_string(playerA->getcoin()));
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
	if (playerA->getcoin() >= 3)
	{
		int location = playerA->GetMinIndex();
		int delLoc_1 = -1;
		int delLoc_2 = -1;
		int& ref_delLoc_1 = delLoc_1;
		int& ref_delLoc_2 = delLoc_2;
		if (sender && location != -1) {
			MenuItemImage* myButton = static_cast<MenuItemImage*>(sender);
			myButton->removeFromParent();
			playerA->payForHero();
			player_coin->setString(std::to_string(playerA->getcoin()));
			//备战席加入新棋子
			shared_ptr<Chess> purchasedChess = shopModel->chessList[index];
			playerA->chessInPreArea[location] = (purchasedChess);
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

//升级回调函数
void PlayScene::menuBuyExpCallBack(Ref* sender)
{
	if (playerA->getcoin() >= 4) {
		playerA->ChangeLevel();
		player_level->setString(std::to_string(playerA->getLevel()));
		player_coin->setString(std::to_string(playerA->getcoin()));
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
