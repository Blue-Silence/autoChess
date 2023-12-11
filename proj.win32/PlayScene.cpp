#include "PlayScene.h"

Scene* PlayScene::createScene()
{
	return PlayScene::create();
}

bool PlayScene::init()
{
	if (!Scene::init()) // 对父类init方法的判断
		return false;

	// 需要用到的单例工具
	auto texture = Director::getInstance()->getTextureCache();
	auto config = ConfigController::getInstance();

	// 获取屏幕上的特殊点位
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto buttonPositiony = visibleSize.height / 3;	//	The y position of two buttons

	// 创建单点事件监听器
	auto clickListener = EventListenerTouchOneByOne::create();
	clickListener->setSwallowTouches(true);
	clickListener->onTouchBegan = CC_CALLBACK_2(PlayScene::onTouchBegan, this);
	clickListener->onTouchEnded = CC_CALLBACK_2(PlayScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(clickListener, this);

	// 创建移动事件监听器
	auto moveListener = EventListenerMouse::create();
	moveListener->onMouseMove = CC_CALLBACK_1(PlayScene::onMouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(moveListener, this);

	// 添加背景层
	playLayer = Layer::create();
	playLayer->setPosition(origin);
	playLayer->setContentSize(visibleSize);
	this->addChild(playLayer);

	// 添加背景图片
	auto backGround = Sprite::create("background2.png");
	backGround->setPosition(visibleSize / 2);
	Vec2 originSize = backGround->getContentSize();
	backGround->setScale(visibleSize.height / originSize.y);
	playLayer->addChild(backGround, 1);

	// 创建棋盘
	chessBoardModel = ChessBoard::create();
	createBoard(Vec2(config->getPx()->x * 38, config->getPx()->y * 30));

	// 创建商店
	shopModel = Market::create();
	createShop(Vec2(-45 * config->getPx()->x, -45 * config->getPx()->y));

	// 创建玩家
	playerA = PlayerInfo::create();



	// 添加退出按钮
	//auto exitButton = StartAndLoginScene::createGameButton("Exit!", "/res/UI/ExitNormal.png", "/res/UI/ExitSelected.png", CC_CALLBACK_1(PlayScene::menuExitCallBack, this));
	//exitButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//originSize = exitButton->getContentSize();
	//exitButton->setScale(10 * ConfigController::getInstance()->getPx()->x / originSize.x);
	//exitButton->setPosition(Vec2(70 * ConfigController::getInstance()->getPx()->y, -35 * ConfigController::getInstance()->getPx()->y));


	//auto menu = Menu::create(exitButton, shop[0], shop[1], shop[2], shop[3], shop[4], nullptr);
	//playLayer->addChild(menu, 5);

	return true;
}

void PlayScene::createBoard(Vec2 position)
{
	auto texture = Director::getInstance()->getTextureCache();
	auto config = ConfigController::getInstance();

	auto sprite = Sprite::createWithTexture(texture->getTextureForKey("/res/Background/bgwhite.png"));
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
				chessBoard[i].push_back(Sprite::createWithTexture(texture->getTextureForKey("/res/Background/bgwhite.png")));
			}
			else
			{
				chessBoard[i].push_back(Sprite::createWithTexture(texture->getTextureForKey("/res/Background/bgblack.png")));
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
	auto shopMore = Sprite::createWithTexture(texture->getTextureForKey("/res/UI/ShoppingMore.png"));
	Vec2 originSize = shopMore->getContentSize();
	float scale = 16.9 * config->getPx()->x / originSize.x;
	shopMore->setScale(scale);
	shopMore->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	shopMore->setPosition(Vec2(position.x + 80 * config->getPx()->x, position.y + 45 * config->getPx()->y));
	Vec2 originPosition = Vec2(shopMore->getPositionX(), shopMore->getPositionY());
	playLayer->addChild(shopMore, 5);
	// 两个菜单项
	//auto buyExp = StartAndLoginScene::createGameButton("", "/res/UI/UpgradeNormal.png", "/res/UI/Upgradeclick.png", CC_CALLBACK_1(PlayScene::menuBuyExpCallBack, this));
	//auto freshShop = StartAndLoginScene::createGameButton("", "/res/UI/RefreshNormal.png", "/res/UI/RefreshSelected.png", CC_CALLBACK_1(PlayScene::menuFreshShopCallBack, this));
	
	//originSize = buyExp->getContentSize();
	//scale = 13.5 * 16.9 / 15 * config->getPx()->x / originSize.x;
	//buyExp->setScale(scale);
	//freshShop->setScale(scale);
	//buyExp->setPosition(Vec2(originPosition.x - 16.9 / 2 * config->getPx()->x, originPosition.y + 10.5 * 16.9 / 15 * config->getPx()->y));
	//freshShop->setPosition(Vec2(originPosition.x - 16.9 / 2 * config->getPx()->x, originPosition.y + 4 * 16.9 / 15 * config->getPx()->y));
	//auto menu = Menu::create(buyExp,freshShop, NULL);
	//menu->setPosition(Vec2::ZERO);
	//playLayer->addChild(menu, 6);
	

	// 棋子及装备卡片
	auto pieceCard1 = PlayScene::createPieceCard("ABCD", "/res/Books/AdvancedMathematics.png", position, CC_CALLBACK_1(PlayScene::menuPieceCardCallBack, this));
	auto pieceCard2 = PlayScene::createPieceCard("EFGH", "/res/Books/AdvancedMathematics.png", Vec2(position.x + 1 * 22 * config->getPx()->x, position.y), CC_CALLBACK_1(PlayScene::menuPieceCardCallBack, this));
	auto pieceCard3 = PlayScene::createPieceCard("IJKL", "/res/Books/AdvancedMathematics.png", Vec2(position.x + 2 * 22 * config->getPx()->x, position.y), CC_CALLBACK_1(PlayScene::menuPieceCardCallBack, this));
	auto pieceCard4 = PlayScene::createPieceCard("MNOP", "/res/Books/AdvancedMathematics.png", Vec2(position.x + 3 * 22 * config->getPx()->x, position.y), CC_CALLBACK_1(PlayScene::menuPieceCardCallBack, this));
	auto pieceCard5 = PlayScene::createPieceCard("QRST", "/res/Books/AdvancedMathematics.png", Vec2(position.x + 4 * 22 * config->getPx()->x, position.y), CC_CALLBACK_1(PlayScene::menuPieceCardCallBack, this));
	shop.push_back(pieceCard1);
	shop.push_back(pieceCard2);
	shop.push_back(pieceCard3);
	shop.push_back(pieceCard4);
	shop.push_back(pieceCard5);
}

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

void PlayScene::menuFreshShopCallBack(Ref* sender)
{

}

void PlayScene::menuBuyExpCallBack(Ref* sender)
{

}

int PlayScene::onTouchBegan(Touch* touch, Event* event)
{
	Vec2 position = touch->getLocation();
	if (position.x > chessBoard[1][1]->getPositionX() && position.x < chessBoard[1][9]->getPositionX() && 
		position.y > chessBoard[1][1]->getPositionY() && position.y < chessBoard[5][1]->getPositionY()) // 鼠标在棋盘战斗区
	{
		CCLOG("WAR");
		return IN_WAR_ZONE;
	}
	else if (position.x > chessBoard[0][1]->getPosition().x && position.x < chessBoard[0][9]->getPosition().x &&
			 position.y > chessBoard[0][1]->getPosition().y && position.y < chessBoard[1][1]->getPosition().y)
	{
		CCLOG("READY");
		return IN_READY_ZONE;
	}
	else
	{
		return NOT_IN_BOARD;
	}
}

void PlayScene::onTouchEnded(Touch* touch, Event* event)
{
	Vec2 position = touch->getLocation();
	ChessCoordinate* logPosition = coordingrevert(position);

	// int clickType = PlayScene::onTouchBegan(touch, event);
	/*switch (clickType)
	{
		case IN_WAR_ZONE:
			if (board->getPlayerInfoA_WarZone_Pieces()[logPosition->getX()][logPosition->getY()] != nullptr)
			{
				chessBoard[logPosition->getY() + 1][logPosition->getX()]->setOpacity(50);
			}
			break;

		case IN_READY_ZONE:
			if (board->getPlayerInfoA_PreZone_Pieces()->at(logPosition->getX()) != nullptr)
			{
				chessBoard[0][logPosition->getX()]->setOpacity(50);
			}
			break;

		default:
			break;
	}*/

}

void PlayScene::onMouseMove(Event* event)
{
	EventMouse* e = (EventMouse*)event;
}
