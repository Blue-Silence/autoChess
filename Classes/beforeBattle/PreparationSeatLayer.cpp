#include "PreparationSeatLayer.h"

/*
* 函数参数：PlayerInfo* pr, ChessBoard* board, EventListenerMouse* listener, Layer* preArea,bool* buyChess, Label* player_coin
* 函数功能：新建层、得到屏幕大小、设置出售席
* 函数返回值：
* 函数注意事项：
*/
bool PreparationSeat::init(PlayerInfo* pr, ChessBoard* board, EventListenerMouse* listener, Layer* preArea,bool* buyChess, Label* player_coin)
{
	//初始化所有指针，指向playScene的实例化对象
	PreAreaLayer = preArea;
	cBoard = board;
	player = pr;
	listenerMouse = listener;
	canBuyChess = buyChess;

	preAreaChessMenu.resize(9);
	pageSize = Director::getInstance()->getVisibleSize();
	pageCoord = Director::getInstance()->getVisibleOrigin();
	playerCoin = player_coin;

	//设置出售席
	auto sellOutArea = StartAndLoginScene::createGameButton("/res/UI/sellOut.png", "/res/UI/sellOut.png", CC_CALLBACK_1(PreparationSeat::SellOut, this));
	sellOutArea->setPosition(Vec2(900, 200));
	sellOutArea->setScale(0.16f);
	auto sellMenu = Menu::create(sellOutArea, NULL);
	sellMenu->setPosition(Vec2::ZERO);
	sellMenu->setName("PreAreaMenu");
	PreAreaLayer->addChild(sellMenu, 8);

	return true;
}

/*
* 函数参数：int index, Vec2& coord, float& scale
* 函数功能：设置备战席图像尺寸、位置
* 函数返回值：
* 函数注意事项：
*/
inline void PreparationSeat::GetCoordAndScale(int index, Vec2& coord, float& scale)
{
	coord.y = 0.27 * pageSize.height + pageCoord.y;
	coord.x = 0.13 * pageSize.width + pageCoord.x + index * 0.08 * pageSize.width;
	//调整后的数字
	scale = (0.08 * pageSize.width) / scale;
}

/*
* 函数参数：Ref* sender, int index
* 函数功能：备战席棋子按钮的回调函数
* 函数返回值：
* 函数注意事项：
*/
void  PreparationSeat::CallBack(Ref* sender, int index)
{
	//为当前棋子指针赋值
	curHero.first = static_cast<Menu*>(sender);
	curHero.second = index;

	//非战斗时间棋子可以上场
	if (*canBuyChess)
	{
		//得到按钮对象指针
		Menu* myButton = static_cast<Menu*>(sender);
		//鼠标按下的回调函数
		listenerMouse->onMouseDown = [this, myButton, index](EventMouse* Event)
			{
				//获取鼠标位置
				auto mouseX = Event->getCursorX();
				auto mouseY = Event->getCursorY();
				Vec2 coord = Vec2(mouseX, mouseY);
				ChessCoordinate* boardCoord = new ChessCoordinate;
				//判断鼠标点击是否在棋盘上
				if (ChessBoard::isInBoard(coord))
				{
					//得到棋盘坐标
					CoordinateConvert(CoordinateType::chessBoardCoordinates, coord, boardCoord);
					Vec2 onBoardCoord;
					onBoardCoord.x = boardCoord->getY();
					onBoardCoord.y = boardCoord->getX();

					if (onBoardCoord.x <= 1)
					{
						//是空位，考虑是否最大个数
						if (this->cBoard->isAvailable(onBoardCoord.x, onBoardCoord.y))
						{
							//在选中状态下，清除备战席棋子，棋子上场
							if (myButton->isVisible() && numOfBoard < player->getMaxBattleChessNum())
							{
								myButton->setVisible(false);
								//精灵上场、记录
								onBoardSprite[static_cast<int>(onBoardCoord.x)][static_cast<int>(onBoardCoord.y)] = ChessToBoard(onBoardCoord, index);
								auto chess = player->chessInPreArea[index];
								player->putChessInBattleArea(chess);
								//棋子记录
								onBoardChess[static_cast<int>(onBoardCoord.x)][static_cast<int>(onBoardCoord.y)] = chess;
								player->chessInPreArea[index] = nullptr;
								//场上棋子数+1
								numOfBoard++;
								//羁绊显示改变
								ChangeBuffImage();
							}
						}
						//非空位，交换
						else {
							//在选中状态下，清除备战席棋子，棋子上场
							if (myButton->isVisible())
							{
								//清除
								myButton->setVisible(false);
								onBoardSprite[static_cast<int>(onBoardCoord.x)][static_cast<int>(onBoardCoord.y)]->removeFromParent();

								//交换记录
								shared_ptr<Chess> chessToBoard = player->chessInPreArea[index];
								shared_ptr<Chess> chessToPre = onBoardChess[static_cast<int>(onBoardCoord.x)][static_cast<int>(onBoardCoord.y)];
								//精灵上场、记录
								onBoardSprite[static_cast<int>(onBoardCoord.x)][static_cast<int>(onBoardCoord.y)] = ChessToBoard(onBoardCoord, index);
								//备战席图像更新
								player->chessInPreArea[index] = chessToPre;
								CreatePreAreaButton(onBoardChess[static_cast<int>(onBoardCoord.x)][static_cast<int>(onBoardCoord.y)], index);
								onBoardChess[static_cast<int>(onBoardCoord.x)][static_cast<int>(onBoardCoord.y)] = chessToBoard;
								player->ReBattleChess(chessToPre, chessToBoard);
								ChangeBuffImage();
							}

						}
					}
					delete boardCoord;
				}
			};
	}
}
//显示羁绊
void PreparationSeat::ChangeBuffImage()
{
	auto config = ConfigController::getInstance();
	int n=0;//记录第几个羁绊
	int* Buff= player->buffJudgment();
	if (PreviousBuffmage) 
	{
		PreviousBuffmage->removeFromParent();
	}
	if (PreviousBufftank)
	{
		PreviousBufftank->removeFromParent();
	}
	if (PreviousBuffshooter)
	{
		PreviousBuffshooter->removeFromParent();
	}
	if (Buff[0]) 
	{
		n++;
		Sprite* magebuff = Sprite::create("/res/Bond/mage.png");
		Vec2 originSize1 = magebuff->getContentSize();
		float scale1 = 13.5 * 16.9 / 15 * config->getPx()->x / originSize1.x;
		magebuff->setScale(scale1);
		magebuff->setPosition(50, 500);
		PreAreaLayer->addChild(magebuff,5);
		PreviousBuffmage = magebuff;
	}
	if (Buff[1])
	{
		Sprite* shooterbuff = Sprite::create("/res/Bond/shooter.png");
		Vec2 originSize1 = shooterbuff->getContentSize();
		float scale1 = 13.5 * 16.9 / 15 * config->getPx()->x / originSize1.x;
		shooterbuff->setScale(scale1);
		shooterbuff->setPosition(50, 500-n*50);
		PreAreaLayer->addChild(shooterbuff, 5);
		PreviousBuffshooter = shooterbuff;
		n++;
	}
	if (Buff[2]) 
	{
		Sprite* tankbuff = Sprite::create("/res/Bond/tank.png");
		Vec2 originSize1 = tankbuff->getContentSize();
		float scale1 = 13.5 * 16.9 / 15 * config->getPx()->x / originSize1.x;
		tankbuff->setScale(scale1);
		tankbuff->setPosition(50, 500-n*50);
		PreAreaLayer->addChild(tankbuff, 5);
		PreviousBufftank = tankbuff;
		n++;
	}
}

/*
* 函数参数：shared_ptr<Chess> curHero, int index
* 函数功能：创建备战席棋子按钮
* 函数返回值：
* 函数注意事项：
*/
void  PreparationSeat::CreatePreAreaButton(shared_ptr<Chess> curHero, int index)
{
	CC_SAFE_RETAIN(PreAreaLayer);
	//得到图片
	std::string imageNormalPath = curHero->getChessImagePath();
	int pos = imageNormalPath.find(".png");
	//按星级得到图片路径
	switch (curHero->getChessLevel())
	{
	case 1:
		imageNormalPath.replace(pos, 11, "HeroOneStar.png");
		break;
	case 2:
		imageNormalPath.replace(pos, 11, "HeroTwoStar.png");
		break;
	case 3:
		imageNormalPath.replace(pos, 11, "HeroThreeStar.png");
		break;
	default:
		break;
	}
	//创建菜单按键
	auto item = StartAndLoginScene::createGameButton(imageNormalPath, imageNormalPath, CC_CALLBACK_1(PreparationSeat::CallBack, this, index));
	Vec2 originSize;
	float scale = item->getContentSize().width;
	GetCoordAndScale(index, originSize, scale);
	//设置按键位置、尺寸
	item->setPosition(originSize);
	item->setScale(scale);
	item->setEnabled(true);
	//设置为菜单
	auto menu = Menu::create(item, NULL);
	menu->setPosition(Vec2::ZERO);
	menu->setName("PreAreaMenu");
	//加入界面
	PreAreaLayer->addChild(menu, 8);
	preAreaChessMenu[index] = menu;
}

/*
* 函数参数：const Vec2& onBoardCoord, int index
* 函数功能：棋子上场函数
* 函数返回值：英雄精灵
* 函数注意事项：
*/
Sprite* PreparationSeat::ChessToBoard(const Vec2& onBoardCoord, int index)
{
	//标定选中位置
	ChessCoordinate* sceenCoord = new ChessCoordinate;
	this->cBoard->OnBoard(onBoardCoord.x, onBoardCoord.y);
	//得到当前选中棋子
	shared_ptr<Chess> curHero = player->chessInPreArea[index];
	//得到棋子屏幕坐标
	CoordinateConvert(CoordinateType::screenCoordinates, Vec2(onBoardCoord.y,onBoardCoord.x), sceenCoord);
	Vec2 onScreenCoord;
	onScreenCoord.x = sceenCoord->getX();
	onScreenCoord.y = sceenCoord->getY();
	//创建图标并显示
	Sprite* Hero = curHero->createChess(onScreenCoord);
	curHero->setChessCoordinateByType(onScreenCoord, CoordinateType::screenCoordinates);
	curHero->setChessCoordinateByType(Vec2(onBoardCoord.y, onBoardCoord.x), CoordinateType::chessBoardCoordinates);
	PreAreaLayer->addChild(Hero, 9);
	delete sceenCoord;
	return Hero;
}

/*
* 函数参数：int index
* 函数功能：棋子升星
* 函数返回值：
* 函数注意事项：
*/
void PreparationSeat::PromoteChessLevel(int index)
{
	//备战席删除英雄位置
	int delLocation1 = -1;
	int delLocation2 = -1;

	bool rise = player->starRaiseLevel(index, delLocation1, delLocation2);
	shared_ptr<Chess> curHero = player->chessInPreArea[index];
	while (rise) 
	{
		int preAreaNum = 0;
		//优先从备战席删除
		if (delLocation1 != -1) 
		{
			preAreaNum++;
			preAreaChessMenu[delLocation1]->removeFromParent();
		}
		if (delLocation2 != -1) 
		{
			preAreaNum++;
			preAreaChessMenu[delLocation2]->removeFromParent();
		}
		//从场上删除
		if (preAreaNum != 2) 
		{
			for (int i = 0; i < RSIZE; i++) 
			{
				for (int j = 0; j < CSIZE; j++) 
				{
					//找到场上第一个符合要求的棋子
					if (onBoardChess[i][j] != nullptr
						&& curHero->getChessName() == onBoardChess[i][j]->getChessName()
						&& curHero->getChessLevel() == onBoardChess[i][j]->getChessLevel() + 1) 
					{
						onBoardChess[i][j] = nullptr;
						onBoardSprite[i][j]->removeFromParent();
						cBoard->WithdrawBoard(i, j);
						preAreaNum++;
						//删除完毕，退出循环
						if (preAreaNum == 2) 
						{
							i = RSIZE;
							j = CSIZE;
							numOfBoard--;
							break;
						}
					}
				}
			}
		}
		//二次升星判断
		delLocation1 = delLocation2 = -1;
		rise = player->starRaiseLevel(index, delLocation1, delLocation2);
		curHero = player->chessInPreArea[index];
	}
	//购买棋子加入备战席
	CreatePreAreaButton(curHero, index);
}

/*
* 函数参数：Ref* sender
* 函数功能：棋子出售函数
* 函数返回值：
* 函数注意事项：
*/
void PreparationSeat::SellOut(Ref* sender)
{
	//选中棋子
	if (curHero.first != nullptr && curHero.first->isVisible())
	{
		//出售棋子
		player->getSellCoin(curHero.second);
		player->chessInPreArea[curHero.second] = nullptr;
		playerCoin->setString(std::to_string(player->getcoin()));
		//移除图像
		curHero.first->removeFromParent();
	}
}