#include "PreparationSeatLayer.h"

/*
* ����������PlayerInfo* pr, ChessBoard* board, EventListenerMouse* listener, Layer* preArea,bool* buyChess, Label* player_coin
* �������ܣ��½��㡢�õ���Ļ��С�����ó���ϯ
* ��������ֵ��
* ����ע�����
*/
bool PreparationSeat::init(PlayerInfo* pr, ChessBoard* board, EventListenerMouse* listener, Layer* preArea,bool* buyChess, Label* player_coin)
{
	//��ʼ������ָ�룬ָ��playScene��ʵ��������
	PreAreaLayer = preArea;
	cBoard = board;
	player = pr;
	listenerMouse = listener;
	canBuyChess = buyChess;

	preAreaChessMenu.resize(9);
	pageSize = Director::getInstance()->getVisibleSize();
	pageCoord = Director::getInstance()->getVisibleOrigin();
	playerCoin = player_coin;

	//���ó���ϯ
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
* ����������int index, Vec2& coord, float& scale
* �������ܣ����ñ�սϯͼ��ߴ硢λ��
* ��������ֵ��
* ����ע�����
*/
inline void PreparationSeat::GetCoordAndScale(int index, Vec2& coord, float& scale)
{
	coord.y = 0.27 * pageSize.height + pageCoord.y;
	coord.x = 0.13 * pageSize.width + pageCoord.x + index * 0.08 * pageSize.width;
	//�����������
	scale = (0.08 * pageSize.width) / scale;
}

/*
* ����������Ref* sender, int index
* �������ܣ���սϯ���Ӱ�ť�Ļص�����
* ��������ֵ��
* ����ע�����
*/
void  PreparationSeat::CallBack(Ref* sender, int index)
{
	//Ϊ��ǰ����ָ�븳ֵ
	curHero.first = static_cast<Menu*>(sender);
	curHero.second = index;

	//��ս��ʱ�����ӿ����ϳ�
	if (*canBuyChess)
	{
		//�õ���ť����ָ��
		Menu* myButton = static_cast<Menu*>(sender);
		//��갴�µĻص�����
		listenerMouse->onMouseDown = [this, myButton, index](EventMouse* Event)
			{
				//��ȡ���λ��
				auto mouseX = Event->getCursorX();
				auto mouseY = Event->getCursorY();
				Vec2 coord = Vec2(mouseX, mouseY);
				ChessCoordinate* boardCoord = new ChessCoordinate;
				//�ж�������Ƿ���������
				if (ChessBoard::isInBoard(coord))
				{
					//�õ���������
					CoordinateConvert(CoordinateType::chessBoardCoordinates, coord, boardCoord);
					Vec2 onBoardCoord;
					onBoardCoord.x = boardCoord->getY();
					onBoardCoord.y = boardCoord->getX();

					if (onBoardCoord.x <= 1)
					{
						//�ǿ�λ�������Ƿ�������
						if (this->cBoard->isAvailable(onBoardCoord.x, onBoardCoord.y))
						{
							//��ѡ��״̬�£������սϯ���ӣ������ϳ�
							if (myButton->isVisible() && numOfBoard < player->getMaxBattleChessNum())
							{
								myButton->setVisible(false);
								//�����ϳ�����¼
								onBoardSprite[static_cast<int>(onBoardCoord.x)][static_cast<int>(onBoardCoord.y)] = ChessToBoard(onBoardCoord, index);
								auto chess = player->chessInPreArea[index];
								player->putChessInBattleArea(chess);
								//���Ӽ�¼
								onBoardChess[static_cast<int>(onBoardCoord.x)][static_cast<int>(onBoardCoord.y)] = chess;
								player->chessInPreArea[index] = nullptr;
								//����������+1
								numOfBoard++;
								//���ʾ�ı�
								ChangeBuffImage();
							}
						}
						//�ǿ�λ������
						else {
							//��ѡ��״̬�£������սϯ���ӣ������ϳ�
							if (myButton->isVisible())
							{
								//���
								myButton->setVisible(false);
								onBoardSprite[static_cast<int>(onBoardCoord.x)][static_cast<int>(onBoardCoord.y)]->removeFromParent();

								//������¼
								shared_ptr<Chess> chessToBoard = player->chessInPreArea[index];
								shared_ptr<Chess> chessToPre = onBoardChess[static_cast<int>(onBoardCoord.x)][static_cast<int>(onBoardCoord.y)];
								//�����ϳ�����¼
								onBoardSprite[static_cast<int>(onBoardCoord.x)][static_cast<int>(onBoardCoord.y)] = ChessToBoard(onBoardCoord, index);
								//��սϯͼ�����
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
//��ʾ�
void PreparationSeat::ChangeBuffImage()
{
	auto config = ConfigController::getInstance();
	int n=0;//��¼�ڼ����
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
* ����������shared_ptr<Chess> curHero, int index
* �������ܣ�������սϯ���Ӱ�ť
* ��������ֵ��
* ����ע�����
*/
void  PreparationSeat::CreatePreAreaButton(shared_ptr<Chess> curHero, int index)
{
	CC_SAFE_RETAIN(PreAreaLayer);
	//�õ�ͼƬ
	std::string imageNormalPath = curHero->getChessImagePath();
	int pos = imageNormalPath.find(".png");
	//���Ǽ��õ�ͼƬ·��
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
	//�����˵�����
	auto item = StartAndLoginScene::createGameButton(imageNormalPath, imageNormalPath, CC_CALLBACK_1(PreparationSeat::CallBack, this, index));
	Vec2 originSize;
	float scale = item->getContentSize().width;
	GetCoordAndScale(index, originSize, scale);
	//���ð���λ�á��ߴ�
	item->setPosition(originSize);
	item->setScale(scale);
	item->setEnabled(true);
	//����Ϊ�˵�
	auto menu = Menu::create(item, NULL);
	menu->setPosition(Vec2::ZERO);
	menu->setName("PreAreaMenu");
	//�������
	PreAreaLayer->addChild(menu, 8);
	preAreaChessMenu[index] = menu;
}

/*
* ����������const Vec2& onBoardCoord, int index
* �������ܣ������ϳ�����
* ��������ֵ��Ӣ�۾���
* ����ע�����
*/
Sprite* PreparationSeat::ChessToBoard(const Vec2& onBoardCoord, int index)
{
	//�궨ѡ��λ��
	ChessCoordinate* sceenCoord = new ChessCoordinate;
	this->cBoard->OnBoard(onBoardCoord.x, onBoardCoord.y);
	//�õ���ǰѡ������
	shared_ptr<Chess> curHero = player->chessInPreArea[index];
	//�õ�������Ļ����
	CoordinateConvert(CoordinateType::screenCoordinates, Vec2(onBoardCoord.y,onBoardCoord.x), sceenCoord);
	Vec2 onScreenCoord;
	onScreenCoord.x = sceenCoord->getX();
	onScreenCoord.y = sceenCoord->getY();
	//����ͼ�겢��ʾ
	Sprite* Hero = curHero->createChess(onScreenCoord);
	curHero->setChessCoordinateByType(onScreenCoord, CoordinateType::screenCoordinates);
	curHero->setChessCoordinateByType(Vec2(onBoardCoord.y, onBoardCoord.x), CoordinateType::chessBoardCoordinates);
	PreAreaLayer->addChild(Hero, 9);
	delete sceenCoord;
	return Hero;
}

/*
* ����������int index
* �������ܣ���������
* ��������ֵ��
* ����ע�����
*/
void PreparationSeat::PromoteChessLevel(int index)
{
	//��սϯɾ��Ӣ��λ��
	int delLocation1 = -1;
	int delLocation2 = -1;

	bool rise = player->starRaiseLevel(index, delLocation1, delLocation2);
	shared_ptr<Chess> curHero = player->chessInPreArea[index];
	while (rise) 
	{
		int preAreaNum = 0;
		//���ȴӱ�սϯɾ��
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
		//�ӳ���ɾ��
		if (preAreaNum != 2) 
		{
			for (int i = 0; i < RSIZE; i++) 
			{
				for (int j = 0; j < CSIZE; j++) 
				{
					//�ҵ����ϵ�һ������Ҫ�������
					if (onBoardChess[i][j] != nullptr
						&& curHero->getChessName() == onBoardChess[i][j]->getChessName()
						&& curHero->getChessLevel() == onBoardChess[i][j]->getChessLevel() + 1) 
					{
						onBoardChess[i][j] = nullptr;
						onBoardSprite[i][j]->removeFromParent();
						cBoard->WithdrawBoard(i, j);
						preAreaNum++;
						//ɾ����ϣ��˳�ѭ��
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
		//���������ж�
		delLocation1 = delLocation2 = -1;
		rise = player->starRaiseLevel(index, delLocation1, delLocation2);
		curHero = player->chessInPreArea[index];
	}
	//�������Ӽ��뱸սϯ
	CreatePreAreaButton(curHero, index);
}

/*
* ����������Ref* sender
* �������ܣ����ӳ��ۺ���
* ��������ֵ��
* ����ע�����
*/
void PreparationSeat::SellOut(Ref* sender)
{
	//ѡ������
	if (curHero.first != nullptr && curHero.first->isVisible())
	{
		//��������
		player->getSellCoin(curHero.second);
		player->chessInPreArea[curHero.second] = nullptr;
		playerCoin->setString(std::to_string(player->getcoin()));
		//�Ƴ�ͼ��
		curHero.first->removeFromParent();
	}
}