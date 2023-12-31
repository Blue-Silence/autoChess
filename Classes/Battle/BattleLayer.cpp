/******************************************************/
/*                  �ļ�����BattleLayer.cpp           
/*                  ���ܣ�ս���߼�����ʵ��            
/*                  ���ߣ�֣ΰة                      
/******************************************************/

#include "BattleLayer.h"


//-----------------------------------------------------//
// ������������                                        
// �������ܣ�����AI���ֵ���Ϊ                          
// ��������ֵ���ޣ�void���ͣ�                          
// ����ע�������                                    
//-----------------------------------------------------//
void BattleLayer::AItest()
{
	// Ϊ������ս�������������
	playerOPP->putChessInBattleArea(make_shared<shooter>(HOUYI));
	playerOPP->putChessInBattleArea(make_shared<shooter>(DIRENJIE));

	// �����µ���������
	ChessCoordinate* newPos = new ChessCoordinate;

	// ��ȡ����ս�������е�����
	shared_ptr<Chess> houyi = (*playerOPP->getBattleAreaChesses())[0];
	shared_ptr<Chess> direnjie = (*playerOPP->getBattleAreaChesses())[1];

	// ���ò�������ӵ�����
	houyi->setChessCoordinateByType(Vec2(3, 0), CoordinateType::chessBoardCoordinates);
	CoordinateConvert(CoordinateType::screenCoordinates, Vec2(3, 0), newPos);
	houyi->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);
	this->addChild(houyi->createChess(Vec2(newPos->getX(), newPos->getY())));

	// ���ò�������ӵ�����
	direnjie->setChessCoordinateByType(Vec2(0, 0), CoordinateType::chessBoardCoordinates);
	CoordinateConvert(CoordinateType::screenCoordinates, Vec2(0, 0), newPos);
	direnjie->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);
	this->addChild(direnjie->createChess(Vec2(newPos->getX(), newPos->getY())));

	// �ͷ��µ���������ʵ��
	delete newPos;
}


//-----------------------------------------------------------//
// ����������chess��Ҫ��ʼ�������ӣ���isME���Ƿ�Ϊ������ӣ� 
// �������ܣ�ս��ǰ��ʼ�����ӵ�ս������                      
// ��������ֵ���ޣ�void���ͣ�                                
// ����ע�������                                          
//-----------------------------------------------------------//
void BattleLayer::chessInitBeforeBattle(shared_ptr<Chess> chess, const bool &isME)
{
	// ��ֹ��������
	*canBuyChess = false;

	// ��ʼ������״̬
	chess->state = Chess::State::Idle;
	chess->isAttacking = false;
	chess->isMoving = false;

	// �������������̺���Ļ�ϵĳ�ʼ����
	chess->inGameChessBoardCoordinate.setX(chess->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getX());
	chess->inGameChessBoardCoordinate.setY(chess->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getY());
	chess->inGameScreenCoordinate.setX(chess->getChessCoordinateByType(CoordinateType::screenCoordinates)->getX());
	chess->inGameScreenCoordinate.setY(chess->getChessCoordinateByType(CoordinateType::screenCoordinates)->getY());

	// ����Ƕ��ֵ����ӣ�����ж���ĳ�ʼ��
	if (!isME)
	{
		// �������ӵ�����ֵΪ���ֵ
		chess->getChessCondition()->HP = chess->getChessCondition()->maxHP;
		chess->attackNum = 0;

		// ����Ļ�ϴ�������
		chess->createChess(Vec2(chess->inGameScreenCoordinate.getX(), chess->inGameScreenCoordinate.getY()));

		// ���õз����ӵı�־
		auto chessImage = chess->getChessSprite();
		double nowWidth = chessImage->getContentSize().width;
		double nowHeight = chessImage->getContentSize().height;

		// �����з���־
		auto config = ConfigController::getInstance();
		auto enemy = Sprite::create("/res/UI/enemy.png");
		Vec2 originSize = enemy->getContentSize();
		float scaleX = 20 * config->getPx()->x / originSize.x;
		float scaleY = 20 * config->getPx()->y / originSize.y;
		enemy->setScale(scaleX, scaleY);
		enemy->setPosition(Vec2(chess->inGameScreenCoordinate.getX() + 0.5 * nowWidth, chess->inGameScreenCoordinate.getY() + 0.8 * nowHeight));
		chessImage->addChild(enemy);
		addChild(chessImage);
	}

	// �������ϱ�����ӵ�λ��
	boardInGame[chess->inGameChessBoardCoordinate.getY()][chess->inGameChessBoardCoordinate.getX()] = 1;
}


//-----------------------------------------------------//
// ����������row�����������꣩��col�����������꣩  
// �������ܣ��ж�ָ���к��е�����λ���Ƿ����          
// ��������ֵ��bool�ͣ����÷���true�������÷���false   
// ����ע�������                                    
//-----------------------------------------------------//
inline bool BattleLayer::isAvailable(const int &row,const int &col)
{
	// ���ȼ���к��е������Ƿ������̵���Ч��Χ��
	if (row < 0 || row >= BOARDMAXR || col < 0 || col >= BOARDMAXC)
		return false; 

	// �����������Ч��Χ�ڣ������λ���Ƿ��ѱ�ռ�ã�ֵΪ0��ʾδռ�ã�
	return boardInGame[row][col] == 0;
}


//-------------------------------------------------------//
// ������������                                          
// �������ܣ���ʼ��BattleLayer��������Ϸ�ĳ�ʼ״̬����Դ 
// ��������ֵ��bool�ͣ���ʾ��ʼ���Ƿ�ɹ�                
// ����ע�������                                      
//-------------------------------------------------------//
bool BattleLayer::init()
{
	// Ԥ������Ч��Դ
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/zhangfei.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/houyi.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/daji.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/skillSound.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/victory.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/defeat.wav");

	// ��ʼ����Ϸ������־
	gameOver = false;

	// ��ʼ������״̬
	memset(boardInGame, 0, sizeof boardInGame);
	
    // ��ʼ���������
    for (shared_ptr<Chess> chess : *playerME->getBattleAreaChesses())
    {
		if (chess != nullptr) 
		{
			chessInitBeforeBattle(chess, true);
		}
    }


    playerOPP->buffJudgment();
    // ��ʼ����������
    for (shared_ptr<Chess> chess : *playerOPP->getBattleAreaChesses())
    {
		if (chess != nullptr)
		{
			chessInitBeforeBattle(chess, false);
		}
    }
	
	

	// ��������update()��������ʼս��
	this->scheduleUpdate();

	return true;
}


//-----------------------------------------------------//
// ����������delta��ÿ֡���ʱ�䣩                     
// �������ܣ����º���������ÿ֡������Ϸ�߼�            
// ��������ֵ���ޣ�void���ͣ�                          
// ����ע�����������Ϸѭ���г�������                
//-----------------------------------------------------//
void BattleLayer::update(float delta)
{
	
	
    // �����ҷ���ÿ������
    for (auto chess : *playerME->getBattleAreaChesses())
    {
		if (chess != nullptr) {
			// ���������������������
			if (chess->isDead())
				continue;

			// �������ӵ��ж�
			playGame(chess, playerOPP);
		}
    }

    // ����Է���ÿ������
    for (auto chess : *playerOPP->getBattleAreaChesses())
    {
		if (chess != nullptr)
		{
			// ���������������������
			if (chess->isDead())
				continue;

			// �������ӵ��ж�
			playGame(chess, playerME);
		}
    }
	
	
	
	
	

	// �����Ϸ����
	if (gameOver)
	{
		// ֹͣ����
		this->unscheduleUpdate();

		// ���������ڵ�������ʱ����
		auto delayNode = Node::create();
		this->addChild(delayNode); // ��ӵ���ǰ��

		// ������ʱ����
		auto delayAction = DelayTime::create(2.0f); // 2����ʱ

		// ��Ϸ������Ļص�����
		auto callbackActionForEnd = CallFunc::create([this, delayNode]() 
			{
				// ���ʤ��
				detectWinner();

				// ������Ч��Դ
				auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
				audio->unloadEffect("/res/Music/zhangfei.wav");
				audio->unloadEffect("/res/Music/houyi.wav");
				audio->unloadEffect("/res/Music/daji.wav");
				audio->unloadEffect("/res/Music/skillSound.wav");
				audio->unloadEffect("/res/Music/victory.wav");
				audio->unloadEffect("/res/Music/defeat.wav");
			});

		// ��Ϸ��ʼǰ��׼������
		auto callbackActionForStart = CallFunc::create([this, delayNode]() 
			{
				// �����ҷ���������״̬������ս��������ʾ
				for (auto chess : *playerME->getBattleAreaChesses())
				{
					if (chess != nullptr)
					{
						int posX = chess->getChessCoordinateByType(CoordinateType::screenCoordinates)->getX();
						int posY = chess->getChessCoordinateByType(CoordinateType::screenCoordinates)->getY();
						Sprite* chessImage = chess->getChessSprite();
						chessImage->setPosition(posX, posY);
						chessImage->setVisible(true);
						chess->getChessCondition()->HP = chess->getChessCondition()->maxHP;
						chess->attackNum = 0;
						chess->updateHpBar();
						chess->updateMpBar();
					}
				}

				// ����ս��״̬��־
				*isInBattle = false;
				*canBuyChess = true;
			});

		// ���������ж�������
		auto sequence = Sequence::create(delayAction, callbackActionForEnd, callbackActionForStart, nullptr);
		delayNode->runAction(sequence);
	}
}


//---------------------------------------------------------//
// ����������playerME�������Ϣ����playerOPP��������Ϣ���� 
//           isInBattle��ָ���Ƿ���ս��״̬�ı�־����    
//           canBuyChess��ָ���Ƿ���Թ������ӵı�־��     
// �������ܣ���̬�������������ڴ�������ʼ��BattleLayer���� 
// ��������ֵ��BattleLayer*�������ɹ����ض���ָ�룬        
//             ʧ�ܷ���nullptr                             
// ����ע�����ʹ��new (std::nothrow)�����׳��쳣        
//---------------------------------------------------------//
BattleLayer* BattleLayer::create(PlayerInfo* playerME, PlayerInfo* playerOPP, bool* isInBattle, bool* canBuyChess)
{
	// ���Դ����µ�BattleLayerʵ��
	BattleLayer* layer = new (std::nothrow) BattleLayer(playerME, playerOPP, isInBattle, canBuyChess);

	// ��ʼ��layer
	if (layer && layer->init())
	{
		// ��ʼ���ɹ�����layer���Ϊ�Զ��ͷŲ�����
		layer->autorelease();
		return layer;
	}

	// �����ʼ��ʧ�ܣ���ȫɾ��layer�����ؿ�ָ��
	CC_SAFE_DELETE(layer);
	return nullptr;
}


// ���캯��
BattleLayer::BattleLayer(PlayerInfo* playerOfMe, PlayerInfo* playerOfOpp, bool* inBattle, bool* BuyChess)
{
	playerME = playerOfMe; playerOPP = playerOfOpp; isInBattle = inBattle; canBuyChess = BuyChess;
}

//---------------------------------------------------------------//
// ����������gameResult����Ϸ�����������"Win"��"Lost"��"Draw"�� 
// �������ܣ���ʾ������Ϸ�Ķ�����ʤ����ʧ�ܻ�ƽ�֣�              
// ��������ֵ���ޣ�void���ͣ�                                    
// ����ע��������ݲ�ͬ����Ϸ�����ʾ��ͬ�Ķ�������Ч          
//---------------------------------------------------------------//
void BattleLayer::showEndGameAnimation(const string& gameResult)
{
	string imagePath = "";
	string soundEffectPath = "";
	if (gameResult == "Win")
	{
		imagePath = "/res/UI/Victory.png";
		soundEffectPath = "/res/Music/victory.wav";
	}
	else if (gameResult == "Lost")
	{
		imagePath = "/res/UI/Defeat.png";
		soundEffectPath = "/res/Music/defeat.wav";
	}
	else if (gameResult == "Draw")
	{
		imagePath = "/res/UI/Draw.png";
		soundEffectPath = "/res/Music/draw.wav";
	}

	auto config = ConfigController::getInstance();

	// ��ȡ��ǰ��ͼ�Ĵ�С��ԭ��
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ����������ͼƬ
	auto endGameImage = Sprite::create(imagePath);
	endGameImage->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	Vec2 OriginSize = endGameImage->getContentSize();
	float chessScaleX = 88.9 * config->getPx()->x / OriginSize.x;
	float chessScaleY = 50 * config->getPx()->y / OriginSize.y;
	endGameImage->setScale(chessScaleX, chessScaleY);

	// ���ͼƬ����ǰ������ִ�ж���
	this->addChild(endGameImage);
	auto endAction = ScaleBy::create(0.5f, 1.5f);
	auto endActionReverse = endAction->reverse();
	auto callback = CallFunc::create([soundEffectPath,endGameImage, this]() 
		{
			endGameImage->removeFromParent();
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundEffectPath.c_str());
		});
	Sequence* sequence = Sequence::create(endAction, endActionReverse, callback, nullptr);

	// ��ȡ SimpleAudioEngine ʵ��
	//auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();

	// ������Ч����Ϊ���
	//audioEngine->setEffectsVolume(1.0f);
	// ������Ч
	
	//audioEngine->playEffect(soundEffectPath.c_str());
	endGameImage->runAction(sequence);
}

// �����Ϸʤ����ִ����Ӧ�Ķ���
void BattleLayer::detectWinner()
{
	// ��Ҵ�����������
	int surviveNumMe = 0;
	// ���ִ�����������
	int surviveNumOPP = 0;

	// ������Ҵ�����������
	for (auto chess : *playerME->getBattleAreaChesses())
	{
		if (chess != nullptr && !chess->isDead())
		{
			surviveNumMe++;
		}
	}

	// ������ִ����������������Ƴ������ӵ���ʾ
	for (auto chess : *playerOPP->getBattleAreaChesses())
	{
		if (chess != nullptr)
		{
			chess->getChessSprite()->removeFromParent();
			if (!chess->isDead())
			{
				surviveNumOPP++;
			}
		}
	}

	// ���ݴ�����������ж���Ϸ�������չʾ��Ӧ����
	if (surviveNumMe > 0) // ���ʤ��
	{
		playerME->gainCoin();
		playerOPP->DecreaseLifeValue(playerME->getLevel() + surviveNumMe);
		showEndGameAnimation("Win");
	}
	else if (surviveNumOPP > 0) // ����ʤ��
	{
		playerME->DecreaseLifeValue(playerOPP->getLevel() + surviveNumOPP);
		showEndGameAnimation("Lost");
	}
	else // ƽ��
	{
		showEndGameAnimation("Draw");
	}
}


//---------------------------------------------------------//
// ����������chess��������Ϸ�����ӣ���opp��������Ϣ��      
// �������ܣ���װӢ�۽���ս����ȫ���̡���Ѱ�ҡ�Ѱ·������  
// ��������ֵ���ޣ�void���ͣ�                              
// ����ע������������ӵ�״̬��λ��ִ����Ӧ���ж�        
//---------------------------------------------------------//
void BattleLayer::playGame(shared_ptr<Chess> chess, PlayerInfo* opp)
{
	// ��ȷ������Ŀ�꣬ȷ��Ŀ��һ֡��ʱ��Ӧ���ǹ��ģ�findEnemy()����ִ�к󣬽����ӵ�״̬��ΪMoving
	if (chess->state == Chess::State::Idle)
	{
		shared_ptr<Chess> targetChess = findEnemy(chess, opp);
		// û�й���Ŀ�꣬��Ϸ������
		if (targetChess == nullptr)
		{
			gameOver = true;
		}
		chess->targetChess = targetChess;
	}
	// ���Ӵ����ƶ�״̬��û�����ƶ�ʱ��Ҫ����
	else if (chess->state == Chess::State::Moving && !chess->isMoving)
	{
		double distance = getDistance(&(chess->inGameChessBoardCoordinate), &(chess->targetChess->inGameChessBoardCoordinate));

		// ���빥����Χ�ˣ�����Ҫ���ƶ��ˣ���������״̬Ϊ����
		if (distance < (chess->getChessCondition()->improvedAttackDistance + 1))
		{
			chess->isMoving = false;
			chess->state = Chess::State::Attacking;
			// ע�⣬��Ȼ���빥��״̬������û�п�ʼ����
			chess->isAttacking = false;
			return;
		}
		// ����Ϊ��ʼ�ƶ��������findPathToEnemy()ִ����Ϻ��ں�����β��isMoving������Ϊfalse���ܽ��뱾��֧�ٴε���
		chess->isMoving = true;
		findPathToEnemy(chess, chess->targetChess);
	}
	// ���Ӵ��ڹ���״̬ʱ��û���ڹ���ʱ��Ҫ����
	else if (chess->state == Chess::State::Attacking && !chess->isAttacking)
	{

		// ���ÿ�ʼ�����ˣ������doAttack()ִ����Ϻ��ں�����β��isAttacking������Ϊfalse���ܽ��뱾��֧�ٴε���
		chess->isAttacking = true;
		doAttack(chess, chess->targetChess);
	}

}

//-----------------------------------------------------//
// ����������start����ʼ�������꣩��end�������������꣩
// �������ܣ���ȡ��������֮���������ϵ������پ���      
// ��������ֵ��double�ͣ���ʾ��������֮��������پ���  
// ����ע������������ָ��Ϊnullʱ����-1.0          
//-----------------------------------------------------//
double BattleLayer::getDistance(const ChessCoordinate* start,const ChessCoordinate* end)
{
	// ��������ָ���Ƿ�Ϊnull
	if (start == nullptr || end == nullptr)
	{
		// ����ѡ�񷵻�һ������ֵ���׳��쳣
		return -1.0;
	}

	// ��ȡ��ʼ�ͽ���λ�õ�����
	int startX = start->getX();
	int startY = start->getY();
	int endX = end->getX();
	int endY = end->getY();

	// ���㲢���������پ���
	return abs(startX - endX) + abs(startY - endY);
}




//--------------------------------------------------------//
// ����������damageMaker�����������ߣ���enemy���з���Ϣ�� 
// �������ܣ�Ѱ�ҹ���Ŀ�꣬ѡ���������ĵз�����         
// ��������ֵ��shared_ptr<Chess>���ҵ���Ŀ������ָ��      
// ����ע��������δ�ҵ�Ŀ�����ӣ��������ζ����Ϸ���� 
//--------------------------------------------------------//
shared_ptr<Chess> BattleLayer::findEnemy(shared_ptr<Chess> damageMaker, PlayerInfo* enemy)
{
	double minDistance = 9999; // ��ʼ��һ���ܴ�ľ���ֵ
	shared_ptr<Chess> targetChess = nullptr; // �洢����ĵз�����

	// �����з���������
	for (auto chess : *enemy->getBattleAreaChesses())
	{
		if (chess != nullptr)
		{
			// ��������������
			if (chess->isDead())
				continue;

			// ���㵱ǰ�����빥���ߵľ���
			double currentDistance = getDistance(&damageMaker->inGameChessBoardCoordinate, &chess->inGameChessBoardCoordinate);

			// �����ǰ���Ӹ����������Ŀ������
			if (currentDistance <= minDistance)
			{
				targetChess = chess;
				minDistance = currentDistance;
			}
		}
	}

	// ���û���ҵ�Ŀ�����ӣ������ез����Ӷ���������
	if (targetChess == nullptr)
	{
		return nullptr; // ��Ϸ���ܽ���
	}

	// ���ù����ߵ�״̬Ϊ�ƶ�״̬��׼���ӽ�Ŀ��
	damageMaker->state = Chess::State::Moving;
	damageMaker->isMoving = false; // ��δ��ʼ�ƶ�

	return targetChess; // �����ҵ���Ŀ������
}


//-------------------------------------------------------------------//
// ����������movingChess��Ҫ�ƶ������ӣ���targetPosition��Ŀ��λ�ã� 
// �������ܣ��ƶ�ָ�������ӵ�Ŀ��λ��								 
// ��������ֵ���ޣ�void���ͣ�										 
// ����ע������ƶ���ɺ�������ӵ���Ļ����						 
//-------------------------------------------------------------------//
void BattleLayer::moveChess(shared_ptr<Chess> movingChess,const Vec2 &targetPosition)
{
	// ��ʼ�ƶ�
	Sprite* movingChessImage = movingChess->getChessSprite();
	float duration = 0.5f; // �ƶ�����ʱ�䣬����Ϊ��λ
	auto moveTo = MoveTo::create(duration, targetPosition);
	auto callback = CallFunc::create([this, movingChess,targetPosition]() 
		{
			movingChess->inGameScreenCoordinate.setX(int(targetPosition.x));
			movingChess->inGameScreenCoordinate.setY(int(targetPosition.y));
			
			// �ƶ�һ����Ϻ�����isMovingΪfalse
			movingChess->isMoving = false;
		});

	auto sequence = Sequence::create(moveTo, callback, nullptr);
	movingChessImage->runAction(sequence);
}

//------------------------------------------------------------------//
// ����������damageMaker���������������ӣ���targetChess��Ŀ�����ӣ� 
// �������ܣ�Ѱ�ҵ����˵�·����ʹ��A*Ѱ·�㷨					    
// ��������ֵ��bool�ͣ��ҵ�·������true�����򷵻�false				
// ����ע�����·��Ѱ��ʧ�ܿ�����ζ���޷��ﵽĿ��				    
//------------------------------------------------------------------//
bool BattleLayer:: findPathToEnemy(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetChess) 
{
	// ���ż��͹رռ�
	std::list<pathNode*> openSet;
	std::list<pathNode*> closedSet;

	ChessCoordinate* src, * dst;

	src = &damageMaker->inGameChessBoardCoordinate;
	dst = &targetChess->inGameChessBoardCoordinate;

	pathNode* startNode = new pathNode(src->getX(), src->getY());
	pathNode* endNode = new pathNode(dst->getX(), dst->getY());

	openSet.push_back(startNode);

	while (!openSet.empty()) 
	{
		// �ҵ����ż���fCost��С�Ľڵ�
		pathNode* currentNode = *openSet.begin();
		for (auto node : openSet) 
		{
			if (node->fCost < currentNode->fCost ||
				(node->fCost == currentNode->fCost && node->hCost < currentNode->hCost)) 
			{
				currentNode = node;
			}
		}

		openSet.remove(currentNode);
		closedSet.push_back(currentNode);

		// �����ǰ�ڵ����յ�
		if (abs(currentNode->x- endNode->x)+ abs(currentNode->y - endNode->y)<damageMaker->getChessCondition()->improvedAttackDistance+1)
		{
			// �����ҵ�·���ĵ�һ��
			pathNode* temp = currentNode;
			while (temp->parent != nullptr && temp->parent != startNode) 
			{
				temp = temp->parent;
			}
			boardInGame[src->getY()][src->getX()] = 0;
			src->setX(temp->x);
			src->setY(temp->y);
			boardInGame[temp->y][temp->x] = 1;
			ChessCoordinate* newPos = new ChessCoordinate;
			CoordinateConvert(CoordinateType::screenCoordinates, Vec2(int(temp->x), int(temp->y)), newPos);
			damageMaker->targetPos = Vec2(newPos->getX(), newPos->getY());
			delete newPos;

			moveChess(damageMaker, damageMaker->targetPos);

			// �����ڴ�
			for (auto node : openSet) delete node;
			for (auto node : closedSet) delete node;
			delete endNode;
			return true;
		}

		// ������ڽڵ�
		for (int dx = -1; dx <= 1; dx++) 
		{
			for (int dy = -1; dy <= 1; dy++) 
			{
				if (dx == 0 && dy == 0) continue; // ��������
				if (dx != 0 && dy != 0) continue; // ֻ����ֱ��ˮƽ�ƶ�

				int newX = currentNode->x + dx;
				int newY = currentNode->y + dy;

				// ����Ƿ���ƶ�
				if (!isAvailable(newY, newX)) continue;

				pathNode* neighbour = new pathNode(newX, newY, currentNode);
				neighbour->gCost = currentNode->gCost + 1;
				neighbour->hCost = abs(newX - endNode->x) + abs(newY - endNode->y);
				neighbour->calculateFCost();

				// ���ڵ��Ƿ����ڹرռ���
				if (std::find_if(closedSet.begin(), closedSet.end(),
					[neighbour](pathNode* node) { return node->x == neighbour->x && node->y == neighbour->y; }) != closedSet.end()) {
					delete neighbour;
					continue;
				}

				// ����Ƿ����ڿ��ż���
				auto openNode = std::find_if(openSet.begin(), openSet.end(),
					[neighbour](pathNode* node) { return node->x == neighbour->x && node->y == neighbour->y; });
				if (openNode == openSet.end()) 
				{
					openSet.push_back(neighbour);
				}
				else if (neighbour->gCost < (*openNode)->gCost) 
				{
					(*openNode)->gCost = neighbour->gCost;
					(*openNode)->parent = currentNode;
				}
				else 
				{
					delete neighbour;
				}
			}
		}
	}

	// �����ڴ�
	for (auto node : openSet) delete node;
	for (auto node : closedSet) delete node;
	delete endNode;
	return false;
}


//-----------------------------------------------------------------------//
// ����������damageMaker���������������ӣ���targetEnemy��Ŀ��������ӣ�, 
//           targetRow��Ŀ�����������У���targetCol��Ŀ�����������У�,   
//           targetChessImage��Ŀ�����ӵ�ͼ�񣩣�useSkill���Ƿ�ʹ�ü��ܣ�
// �������ܣ����õĹ��������߼����������ܹ�������ͨ����				 
// ��������ֵ���ޣ�void���ͣ�											 
// ����ע����������Ƿ�ʹ�ü��ܽ��в�ͬ�Ĵ���							 
//-----------------------------------------------------------------------//
void BattleLayer::processAttackResult(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy, 
									const int &targetRow,const int &targetCol, Sprite* targetChessImage,const bool &useSkill)
{
	if (useSkill)
	{
		damageMaker->skill(*targetEnemy);
		damageMaker->attackNum = 0;
		damageMaker->updateMpBar();
	}
	else
	{
		damageMaker->attackOne(*targetEnemy);
		// ������ͨ�����������ͼ��ܽ�����
		damageMaker->attackNum++;
		damageMaker->updateMpBar();
	}
	
	// ���¶�������������
	targetEnemy->updateHpBar();

	// ��鹥���ߺ�Ŀ�������״̬
	if (damageMaker->isDead())
	{
		damageMaker->state = Chess::State::Dead;
	}
	if (targetEnemy->isDead())
	{
		targetChessImage->setVisible(false);
		boardInGame[targetRow][targetCol] = 0;
		damageMaker->state = Chess::State::Idle;
	}

	// ���ù������
	damageMaker->isAttacking = false;
}

// ������Ӧ����Ч
void BattleLayer::playSkillSound(shared_ptr<Chess> damageMaker)
{
	int chessName = damageMaker->getChessName();
	if (chessName == ZHANGFEI)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/zhangfei.wav");
	else if (chessName == DAJI)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/daji.wav");
	else if (chessName == HOUYI)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/houyi.wav");
	else
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/skillSound.wav");
}

//----------------------------------------------------------------------//
// ����������damageMaker���������������ӣ���targetEnemy��Ŀ��������ӣ� 
//			 useSkill���Ƿ�ʹ�ü��ܣ�									
// �������ܣ�ִ��Զ�̹����Ŀ��ӻ��������������ܺ���ͨԶ�̹���		    
// ��������ֵ���ޣ�void���ͣ�										    
// ����ע��������ù��������ĽǶȺ�λ��							    
//----------------------------------------------------------------------//
void BattleLayer::remoteAttackAnimation(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy,const bool &useSkill)
{
	Sprite* targetChessImage = targetEnemy->getChessSprite();
	// Զ�̹���ͼ��
	Sprite* remoteAttackImage;
	if(useSkill)
	{
		
		damageMaker->createSkill();
		remoteAttackImage = damageMaker->getSkillSprite();
	}
	else
	{
		
		damageMaker->createAttack();
		remoteAttackImage = damageMaker->getAttackSprite();
	}

	// ����ͼ������ת
	Vec2 currentPosition = remoteAttackImage->getPosition();
	Vec2 targetPosition = targetChessImage->getPosition();
	// ����Ŀ�귽�������
	Vec2 direction = targetPosition - currentPosition;
	// ����Ƕȣ�����ʹ�� atan2 ����
	float angleRadians = atan2(direction.y, direction.x);
	float angleDegrees = CC_RADIANS_TO_DEGREES(-angleRadians);
	// ���ü�����ͼ�ĳ�ʼ��ת�Ƕ�
	remoteAttackImage->setRotation(angleDegrees);
	Vec2 OriginSize = remoteAttackImage->getContentSize();
	// ���Ŵ�С��configһ�����
	auto config = ConfigController::getInstance();
	float ScaleX = 8 * config->getPx()->x / OriginSize.x;
	float ScaleY = OriginSize.y * 8 * config->getPx()->x / (OriginSize.y * OriginSize.x);
	remoteAttackImage->setScale(ScaleX, ScaleY);
	// �Ѽ��ܵ���ͼ��ӵ���ǰ��򳡾���
	this->addChild(remoteAttackImage);

}

//---------------------------------------------------------------------//
// ����������damageMaker���������������ӣ���targetEnemy��Ŀ��������ӣ�
//           useSkill���Ƿ�ʹ�ü��ܣ�								   
// �������ܣ�ִ��Զ�̹����Ŀ��ӻ��������������ܺ���ͨԶ�̹���		   
// ��������ֵ���ޣ�void���ͣ�										   
// ����ע��������ù��������ĽǶȺ�λ��							   
//---------------------------------------------------------------------//
void BattleLayer::doAttack(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy)
{
	Sprite* damageMakerImage = damageMaker->getChessSprite();
	string damageMakerCareer = damageMaker->getCareer();
	Sprite* targetChessImage = targetEnemy->getChessSprite();
	Vec2 targetPosition = targetChessImage->getPosition();
	
	int targetRow = targetEnemy->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getY();
	int targetCol = targetEnemy->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getX();


	// ʹ�ü���
	if (damageMaker->attackNum == (damageMaker->getChessCondition()->skillCooldown))
	{
		// ���ܿ��ӻ�
		remoteAttackAnimation(damageMaker, targetEnemy,true);
		auto damageMakerSkillImage=damageMaker->getSkillSprite();
		// �ƶ���Ŀ�������ʱ�䣬����Ϊ��λ
		float moveDuration = 0.5f;
		// �����ƶ���Ŀ��λ��
		auto moveSkill = MoveTo::create(moveDuration, targetPosition);
		// ���ܻ��жԷ�����ʧ
		auto removeSkill = CallFunc::create([damageMakerSkillImage]()
			{
				damageMakerSkillImage->removeFromParent();
			});

		// ��϶���
		Sequence* skillAttack = Sequence::create(moveSkill, removeSkill, nullptr);

		// ����Ŀ������ֵ����
		auto callback = CallFunc::create([this, targetRow, targetCol, damageMaker, targetEnemy, damageMakerImage, targetChessImage]()
			{
				processAttackResult(damageMaker, targetEnemy, targetRow, targetCol, targetChessImage,true);
			});
			
		// ��϶���
		auto sequence = Sequence::create(skillAttack, callback, nullptr);
			
		// ִ�м��ܶ������߼�����
		playSkillSound(damageMaker);
		damageMakerSkillImage->runAction(sequence);

	}
	// ����һ����ͨ����
	else
	{
		Sequence* normalAttack;
		if (damageMakerCareer == "mage" || damageMakerCareer == "shooter")
		{
			remoteAttackAnimation(damageMaker, targetEnemy,false);
			auto damageMakerAttackImage = damageMaker->getAttackSprite();
			// �ƶ���Ŀ�������ʱ�䣬����Ϊ��λ
			float moveDuration = 0.5f;
			// Զ�̹����ƶ���Ŀ��λ��
			auto moveAttack = MoveTo::create(moveDuration, targetPosition);
			// ���жԷ�����ʧ
			auto removeSprite = CallFunc::create([damageMakerAttackImage]()
				{
					damageMakerAttackImage->removeFromParent();
				});
			// ��϶���
			normalAttack = Sequence::create(moveAttack, removeSprite, nullptr);

			// �����ƶ���Ŀ��λ�û���Ŀ���Ĵ���
			auto callback = CallFunc::create([this, targetRow, targetCol, damageMaker, targetEnemy, damageMakerImage, targetChessImage]()
				{
					processAttackResult(damageMaker, targetEnemy, targetRow, targetCol, targetChessImage,false);	
				});
			auto sequence = Sequence::create(normalAttack, callback, nullptr);
			
			
			// ִ�м��ܶ������߼�����
			damageMakerAttackImage->runAction(sequence);
			if (damageMaker->getChessName() == HOUYI)
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/houyi.wav");
			else
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/skillSound.wav");
			
		}
		else
		{
			// ̹�˽�ս����ͨ����������һ���򵥵���������ʾ����
			auto attackAction = ScaleBy::create(0.5f, 1.75f); // ���ŵ�ԭ����1.75��
			auto attackActionReverse = attackAction->reverse(); // ���������Ż�ԭʼ��С
			// ��϶���
			normalAttack = Sequence::create(attackAction, attackActionReverse, nullptr);

			// �����ƶ���Ŀ��λ�û���Ŀ���Ĵ���
			auto callback = CallFunc::create([this, targetRow, targetCol, damageMaker, targetEnemy, damageMakerImage,targetChessImage]()
				{
					processAttackResult(damageMaker, targetEnemy, targetRow, targetCol, targetChessImage,false);
				});
			auto sequence = Sequence::create(normalAttack, callback, nullptr);
			
			
			// ִ�м��ܶ������߼�����
			damageMakerImage->runAction(sequence);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/skillSound.wav");
		}
	}
}

// ·���ڵ�Ĺ��캯��
pathNode::pathNode(int x, int y, pathNode* parent)
{
	this->x = x;
	this->y = y;
	this->parent = parent;
	gCost = hCost = fCost = 0;
}

// ����·���ɱ�
void pathNode::calculateFCost()
{
	fCost = gCost + hCost;
}