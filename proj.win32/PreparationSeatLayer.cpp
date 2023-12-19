#include "PreparationSeatLayer.h"

/*
* 函数参数：
* 函数功能：新建层、得到屏幕大小、
* 函数返回值：
* 函数注意事项：加载完成后显示“开始游戏”和“退出游戏”菜单
*/
bool PreparationSeat::init(PlayerInfo* pr, ChessBoard* board, EventListenerMouse* listener, Layer* preArea)
{
	PreAreaLayer = preArea;
	cBoard = board;
	player = pr;
	listenerMouse = listener;
	preAreaChessMenu.resize(9);
	pageSize = Director::getInstance()->getVisibleSize();
	pageCoord = Director::getInstance()->getVisibleOrigin();
	//设置大小
	return true;
}

inline void PreparationSeat::GetCoordAndScale(int index, Vec2& coord, float& scale)
{
	coord.y = 0.27 * pageSize.height + pageCoord.y;
	coord.x = 0.13 * pageSize.width + pageCoord.x + index * 0.08 * pageSize.width;
	//调整后的数字
	scale = (0.08 * pageSize.width) / scale;
}

void  PreparationSeat::CallBack(Ref* sender, int index)
{
	Menu* myButton = static_cast<Menu*>(sender);
	int* counter = new int;
	*counter = 0;
	auto tp = listenerMouse;
	listenerMouse->onMouseDown = [this, myButton, index, counter, tp](EventMouse* Event)
		{
			if (*counter == 1)
			{
				tp->onMouseDown = [](EventMouse* Event) {};
				delete counter;
				return;
			}
			(*counter)++;
			//获取鼠标位置
			auto mouseX = Event->getCursorX();
			auto mouseY = Event->getCursorY();
			Vec2 coord = Vec2(mouseX, mouseY);
			ChessCoordinate* sceenCoord = new ChessCoordinate;
			ChessCoordinate* boardCoord = new ChessCoordinate;
			//判断鼠标点击是否在棋盘上
			if (ChessBoard::isInBoard(coord))
			{
				//得到棋盘坐标
				CoordinateConvert(CoordinateType::chessBoardCoordinates, coord, boardCoord);
				Vec2 onBoardCoord;
				onBoardCoord.x = boardCoord->getY();
				onBoardCoord.y = boardCoord->getX();
				//前两行且可放置
				if (onBoardCoord.x <= 1 && this->cBoard->isAvailable(onBoardCoord.x, onBoardCoord.y))
				{
					//在选中状态下，清除备战席棋子，棋子上场
					if (myButton->isVisible())
					{
						myButton->setVisible(false);
						//上场
						this->cBoard->OnBoard(onBoardCoord.x, onBoardCoord.y);
						shared_ptr<Chess> curHero = player->chessInPreArea[index];
						player->chessInPreArea[index] = nullptr;
						CoordinateConvert(CoordinateType::screenCoordinates, onBoardCoord, sceenCoord);
						Vec2 onScreenCoord;
						onScreenCoord.x = sceenCoord->getY();
						onScreenCoord.y = sceenCoord->getX();
						Sprite* Hero = curHero->createChess(onScreenCoord);
						PreAreaLayer->addChild(Hero, 9);
						player->putChessInBattleArea(curHero);
					}
				}
			}
			delete sceenCoord;
			delete boardCoord;
		};
}

void  PreparationSeat::CreatePreAreaButton(shared_ptr<Chess> curHero, int index)
{
	CC_SAFE_RETAIN(PreAreaLayer);
	//得到图片
	std::string imageNormalPath = curHero->getChessImagePath();
	int pos = imageNormalPath.find(".png");
	imageNormalPath.replace(pos, 11, "Hero.png");
	//创建菜单按键
	auto item = StartAndLoginScene::createGameButton(imageNormalPath, imageNormalPath, CC_CALLBACK_1(PreparationSeat::CallBack, this, index));
	Vec2 originSize;
	float scale = item->getContentSize().width;
	GetCoordAndScale(index, originSize, scale);
	item->setPosition(originSize);
	item->setScale(scale);
	item->setEnabled(true);
	auto menu = Menu::create(item, NULL);
	menu->setPosition(Vec2::ZERO);
	menu->setName("PreAreaMenu");
	PreAreaLayer->addChild(menu, 8);
}

