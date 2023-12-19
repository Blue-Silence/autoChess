#include "PreparationSeatLayer.h"

/*
* ����������
* �������ܣ��½��㡢�õ���Ļ��С��
* ��������ֵ��
* ����ע�����������ɺ���ʾ����ʼ��Ϸ���͡��˳���Ϸ���˵�
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
	//���ô�С
	return true;
}

inline void PreparationSeat::GetCoordAndScale(int index, Vec2& coord, float& scale)
{
	coord.y = 0.27 * pageSize.height + pageCoord.y;
	coord.x = 0.13 * pageSize.width + pageCoord.x + index * 0.08 * pageSize.width;
	//�����������
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
			//��ȡ���λ��
			auto mouseX = Event->getCursorX();
			auto mouseY = Event->getCursorY();
			Vec2 coord = Vec2(mouseX, mouseY);
			ChessCoordinate* sceenCoord = new ChessCoordinate;
			ChessCoordinate* boardCoord = new ChessCoordinate;
			//�ж�������Ƿ���������
			if (ChessBoard::isInBoard(coord))
			{
				//�õ���������
				CoordinateConvert(CoordinateType::chessBoardCoordinates, coord, boardCoord);
				Vec2 onBoardCoord;
				onBoardCoord.x = boardCoord->getY();
				onBoardCoord.y = boardCoord->getX();
				//ǰ�����ҿɷ���
				if (onBoardCoord.x <= 1 && this->cBoard->isAvailable(onBoardCoord.x, onBoardCoord.y))
				{
					//��ѡ��״̬�£������սϯ���ӣ������ϳ�
					if (myButton->isVisible())
					{
						myButton->setVisible(false);
						//�ϳ�
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
	//�õ�ͼƬ
	std::string imageNormalPath = curHero->getChessImagePath();
	int pos = imageNormalPath.find(".png");
	imageNormalPath.replace(pos, 11, "Hero.png");
	//�����˵�����
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

