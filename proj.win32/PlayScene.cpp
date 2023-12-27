#include "PlayScene.h"
#include "StartAndLoginScene.h"

#include "Hero.h"

Scene* PlayScene::createScene()
{
	return PlayScene::create();
}


bool PlayScene::init()
{
	if (!Scene::init()) // �Ը���init�������ж�
		return false;
	//���ű�������
	playSceneBGM = SimpleAudioEngine::getInstance();
	playSceneBGM->playBackgroundMusic("res/Music/playScene_bgm.wav", true);

	// ��Ҫ�õ��ĵ�������
	auto texture = Director::getInstance()->getTextureCache();
	auto config = ConfigController::getInstance();

	// ��ȡ��Ļ�ϵ������λ
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto buttonPositiony = visibleSize.height / 3;	//	The y position of two buttons

	// ��������¼�������
	mouseListener = EventListenerMouse::create();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	// ��ӱ�����
	playLayer = Layer::create();
	playLayer->setPosition(origin);
	playLayer->setContentSize(visibleSize);
	this->addChild(playLayer);
	CC_SAFE_RETAIN(playLayer);
	// ��ӱ���ͼƬ
	auto backGround = Sprite::create("/res/Background/background2.png");
	backGround->setPosition(visibleSize / 2);
	Vec2 originSize = backGround->getContentSize();
	backGround->setScale(visibleSize.height / originSize.y);
	playLayer->addChild(backGround, 1);
	//auto magebuff = cocos2d::Sprite::create("/res/Bond/mage.png");
	//magebuff->setPosition(100, 100);
	//playLayer->addChild(magebuff, 1);
	// ��������
	chessBoardModel = ChessBoard::create();
	createBoard(Vec2(config->getPx()->x * 38, config->getPx()->y * 30));
	CC_SAFE_RETAIN(chessBoardModel);
	// �������
	playerA = PlayerInfo::create();
	playerOPP = PlayerInfo::create();
	CC_SAFE_RETAIN(playerA);
	CC_SAFE_RETAIN(playerOPP);

	//���������ʾ
	player_coin = Label::createWithTTF(std::to_string(playerA->getcoin()), "fonts/Marker Felt.ttf", 30);
	player_coin->setPosition(Vec2(760, 140));
	playLayer->addChild(player_coin, 5);
	Sprite* coinImage = Sprite::create("/res/UI/coin.png");
	coinImage->setPosition(Vec2(800, 140));
	coinImage->setScale(0.07);
	playLayer->addChild(coinImage, 5);

	//��ʼ����սϯ
	preArea = PreparationSeat::create(playerA, chessBoardModel, mouseListener, playLayer, &isInBattle,player_coin);
	CC_SAFE_RETAIN(preArea);


	//�����ȼ���ʾ
	player_level = Label::createWithTTF(std::to_string(playerA->getLevel()), "fonts/Marker Felt.ttf", 30);
	player_level->setPosition(Vec2(760, 90));
	playLayer->addChild(player_level, 5);
	Sprite* levelImage = Sprite::create("res/UI/Level.png");
	levelImage->setPosition(Vec2(800, 90));
	levelImage->setScale(0.07);
	playLayer->addChild(levelImage, 5);

	// ����Ѫ����ʾ
	player_lifevalue = Label::createWithTTF(std::to_string(playerA->GetLifeValue()), "fonts/Marker Felt.ttf", 30);
	player_lifevalue->setPosition(Vec2(760, 40));
	playLayer->addChild(player_lifevalue, 5);
	Sprite* lifeValue = Sprite::create("res/UI/lifeValue.png");
	lifeValue->setPosition(Vec2(800, 40));
	lifeValue->setScale(0.07);
	playLayer->addChild(lifeValue, 5);

	// �����̵�
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

	
	// ���AI�����
	AILabel->setPosition(Vec2(240, 160)); // ������Ҫ����λ��
	AILabel->setColor(Color3B::WHITE); // ������ɫ
	// ����ǩ��ӵ����������
	this->addChild(AILabel);


	// ����˳���ť
	auto exitButton = StartAndLoginScene::createGameButton("/res/UI/ExitNormal.png", "/res/UI/ExitSelected.png", CC_CALLBACK_1(PlayScene::menuExitCallBack, this));

	exitButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	originSize = exitButton->getContentSize();
	exitButton->setScale(10 * ConfigController::getInstance()->getPx()->x / originSize.x);
	exitButton->setPosition(Vec2(50 * ConfigController::getInstance()->getPx()->y, -35 * ConfigController::getInstance()->getPx()->y));


	auto menu = Menu::create(exitButton, nullptr);
	playLayer->addChild(menu, 5);


	// ��������update()��������ʼս��
	this->scheduleUpdate();
	

	
	return true;
}

void PlayScene::update(float delta)
{
	if (gameMode == "�˻���ս")
	{
		if (AI == nullptr && AINum != 0)
			AI = new AIMode(AINum);


		if (!isInBattle && AI != nullptr)
		{
			isInBattle = true;
			AI->eachTurnOver();
			if (!AI->existLiveAI())
			{
				// ս������,��һ�ʤ
				// 
				// ȡ����update�����ĵ���
				this->unscheduleUpdate();
			}
			else if (!playerA->isAlive())
			{
				// AI��ʤ
				// 
				// ȡ����update�����ĵ���
				this->unscheduleUpdate();
			}

			//AI++;
			TargetAI = (TargetAI) % (AINum)+1;
			while (!(*AI->getPlayersVectorInfo())[TargetAI].isAlive())
			{
				TargetAI = (TargetAI) % (AINum)+1;
			}
			// ѡ���ñ��ֶ�ս��AIѡ��
			playerOPP = &(*AI->getPlayersVectorInfo())[TargetAI];
			// �����AI�����Ѫ
			AI->randomBloodLose(TargetAI);

			// ��Һ�AI��ʼ��ս��
			// ��ʱ��,��ʱ������ʼ��ս
			auto clockLayer = Clock::create(playerA, playerOPP, &isInBattle);
			this->addChild(clockLayer, 6);

			// ��AI����API����
			/*string message = "The number of my tank heroes is " + to_string(playerA->getTankNum())
				+ ", the number of mage heroes is " + to_string(playerA->getMagesNum()) + ",the number of shooter is " + to_string(playerA->getShooterNum())
				+ ", my level is" + to_string(playerA->getLevel());*/
			string message = "tank,mages,shooters,which one should I buy firstly,please speak in short english";
			string response = chatAI.performRequest(url, message);
			string content = chatAI.extractContent(response);
			AILabel->setString(content);





		}
	}
	else if (gameMode == "������ս")
	{



	}
}


void PlayScene::onBattleButtonClicked(Ref* sender) 
{
	// ��鲢�Ƴ����е� BattleLayer
	auto existingLayer = this->getChildByTag(BATTLE_LAYER_TAG);
	if (existingLayer) {
		this->removeChild(existingLayer, true);
	}

	// ����������µ� BattleLayer
	auto battleLayer = BattleLayer::create(playerA, playerOPP, &isInBattle);
	battleLayer->setTag(BATTLE_LAYER_TAG); // ���ñ�ǩ�Ա�ʶ��
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
	// ��������
	auto config = ConfigController::getInstance();
	auto texture = Director::getInstance()->getTextureCache();

	// ˢ�¼�������
	// ����
	auto shopMore = Sprite::create("/res/UI/ShoppingMore.png");
	Vec2 originSize = shopMore->getContentSize();
	float scale = 16.9 * config->getPx()->x / originSize.x;
	shopMore->setScale(scale);
	shopMore->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	shopMore->setPosition(Vec2(position.x + 80 * config->getPx()->x, position.y + 45 * config->getPx()->y));
	Vec2 originPosition = Vec2(shopMore->getPositionX(), shopMore->getPositionY());
	playLayer->addChild(shopMore, 5);
	// �����˵���
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
	

	// ���Ӽ�װ����Ƭ
	
}
//����Ҫ�ú�����
MenuItemSprite* PlayScene::createPieceCard(string pieceName, string piecePicPath, Vec2 position, const ccMenuCallback& callback)
{
	auto texture = Director::getInstance()->getTextureCache();
	auto config = ConfigController::getInstance();
	float adjust = 160 / 22;
	//CsvParser csv;
	//csv.parseWithFile("Data/PiecesData.csv");

	// ������Ƭ����
	auto cardBack = Sprite::createWithTexture(texture->getTextureForKey("/res/UI/ShoppingCard.png"));
	
	// ����һ������˵���
	auto item = MenuItemSprite::create(cardBack, cardBack, callback);
	item->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	// ��Ƭ���
	auto sprite = Sprite::createWithTexture(texture->getTextureForKey(piecePicPath));
	sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	Vec2 originSize = item->getContentSize();
	item->setPosition(position.x, position.y);
	Vec2 originPosition = Vec2(sprite->getPositionX(), sprite->getPositionY());

	// ����ͼƬ���
	originSize = sprite->getContentSize();
	sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	float scale = 8.5 * config->getPx()->x / originSize.x * adjust;
	sprite->setScale(scale);
	sprite->setPosition(originPosition.x + 38 * config->getPx()->x, originPosition.y + 67 * config->getPx()->y);
	originPosition = Vec2(sprite->getPositionX(), sprite->getPositionY());
	item->addChild(sprite, 6);

	// �������
	//Value data = Value(csv[0][2].c_str());
	auto nameLabel = Label::createWithTTF(pieceName, "fonts/Marker Felt.ttf", 30 * adjust);
	nameLabel->setColor(Color3B::BLACK);
	nameLabel->setPosition(Vec2(originPosition.x, originPosition.y - 59 * config->getPx()->y));
	item->addChild(nameLabel, 6);

	// �����������
	auto healthIcon = Sprite::createWithTexture(texture->getTextureForKey("/res/Icons/Health.png")); // the Health icon��������
	auto healthValue = Label::createWithTTF("1000", "fonts/Marker Felt.ttf", 27 * adjust); //the value of Health icon
	auto attackIcon = Sprite::createWithTexture(texture->getTextureForKey("/res/Icons/Attack.png")); //the Attack icon(����)
	auto attackValue = Label::createWithTTF("1000", "fonts/Marker Felt.ttf", 27 * adjust); //the value of Attack
	auto armorIcon = Sprite::createWithTexture(texture->getTextureForKey("/res/Icons/Armor.png")); //the Armor icon(����)
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


//ˢ���̵�Ļص�����������
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

		//�������ü���
		CC_SAFE_RETAIN(shopModel);
	}
	else {
		// ����һ�� Label����ûǮʱ��ʾ
		cocos2d::Label* label = cocos2d::Label::createWithTTF("No money!!!", "fonts/Marker Felt.ttf", 24);
		label->setPosition(cocos2d::Vec2(500, 200));
		this->addChild(label);
		auto delayAction = cocos2d::DelayTime::create(1.0f);
		auto removeLabel = cocos2d::CallFunc::create([label]() {
			label->removeFromParent();
			});

		// ����һ��˳����������ʱ��Ȼ���Ƴ� Label
		auto sequence = cocos2d::Sequence::create(delayAction, removeLabel, nullptr);

		// �� Label ���������˳����
		label->runAction(sequence);
	}
}

//�������ӵĻص�����������
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
			//��սϯ����������
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

		// ����һ��˳����������ʱ��Ȼ���Ƴ� Label
		auto sequence = cocos2d::Sequence::create(delayAction, removeLabel, nullptr);

		// �� Label ���������˳����
		label->runAction(sequence);
	}
	
}

//�����ص�����
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

		// ����һ��˳����������ʱ��Ȼ���Ƴ� Label
		auto sequence = cocos2d::Sequence::create(delayAction, removeLabel, nullptr);

		// �� Label ���������˳����
		label->runAction(sequence);
	}
}
