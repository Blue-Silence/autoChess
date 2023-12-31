/******************************************************/
/*                  文件名：BattleLayer.cpp           
/*                  功能：战斗逻辑具体实现            
/*                  作者：郑伟丞                      
/******************************************************/

#include "BattleLayer.h"


//-----------------------------------------------------//
// 函数参数：无                                        
// 函数功能：设置AI对手的行为                          
// 函数返回值：无（void类型）                          
// 函数注意事项：无                                    
//-----------------------------------------------------//
void BattleLayer::AItest()
{
	// 为对手在战斗区域放置棋子
	playerOPP->putChessInBattleArea(make_shared<shooter>(HOUYI));
	playerOPP->putChessInBattleArea(make_shared<shooter>(DIRENJIE));

	// 创建新的棋盘坐标
	ChessCoordinate* newPos = new ChessCoordinate;

	// 获取对手战斗区域中的棋子
	shared_ptr<Chess> houyi = (*playerOPP->getBattleAreaChesses())[0];
	shared_ptr<Chess> direnjie = (*playerOPP->getBattleAreaChesses())[1];

	// 设置并添加棋子到棋盘
	houyi->setChessCoordinateByType(Vec2(3, 0), CoordinateType::chessBoardCoordinates);
	CoordinateConvert(CoordinateType::screenCoordinates, Vec2(3, 0), newPos);
	houyi->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);
	this->addChild(houyi->createChess(Vec2(newPos->getX(), newPos->getY())));

	// 设置并添加棋子到棋盘
	direnjie->setChessCoordinateByType(Vec2(0, 0), CoordinateType::chessBoardCoordinates);
	CoordinateConvert(CoordinateType::screenCoordinates, Vec2(0, 0), newPos);
	direnjie->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);
	this->addChild(direnjie->createChess(Vec2(newPos->getX(), newPos->getY())));

	// 释放新的棋盘坐标实例
	delete newPos;
}


//-----------------------------------------------------------//
// 函数参数：chess（要初始化的棋子），isME（是否为玩家棋子） 
// 函数功能：战斗前初始化棋子的战斗数据                      
// 函数返回值：无（void类型）                                
// 函数注意事项：无                                          
//-----------------------------------------------------------//
void BattleLayer::chessInitBeforeBattle(shared_ptr<Chess> chess, const bool &isME)
{
	// 禁止购买棋子
	*canBuyChess = false;

	// 初始化棋子状态
	chess->state = Chess::State::Idle;
	chess->isAttacking = false;
	chess->isMoving = false;

	// 设置棋子在棋盘和屏幕上的初始坐标
	chess->inGameChessBoardCoordinate.setX(chess->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getX());
	chess->inGameChessBoardCoordinate.setY(chess->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getY());
	chess->inGameScreenCoordinate.setX(chess->getChessCoordinateByType(CoordinateType::screenCoordinates)->getX());
	chess->inGameScreenCoordinate.setY(chess->getChessCoordinateByType(CoordinateType::screenCoordinates)->getY());

	// 如果是对手的棋子，则进行额外的初始化
	if (!isME)
	{
		// 设置棋子的生命值为最大值
		chess->getChessCondition()->HP = chess->getChessCondition()->maxHP;
		chess->attackNum = 0;

		// 在屏幕上创建棋子
		chess->createChess(Vec2(chess->inGameScreenCoordinate.getX(), chess->inGameScreenCoordinate.getY()));

		// 设置敌方棋子的标志
		auto chessImage = chess->getChessSprite();
		double nowWidth = chessImage->getContentSize().width;
		double nowHeight = chessImage->getContentSize().height;

		// 创建敌方标志
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

	// 在棋盘上标记棋子的位置
	boardInGame[chess->inGameChessBoardCoordinate.getY()][chess->inGameChessBoardCoordinate.getX()] = 1;
}


//-----------------------------------------------------//
// 函数参数：row（检查的行坐标），col（检查的列坐标）  
// 函数功能：判断指定行和列的棋盘位置是否可用          
// 函数返回值：bool型，可用返回true，不可用返回false   
// 函数注意事项：无                                    
//-----------------------------------------------------//
inline bool BattleLayer::isAvailable(const int &row,const int &col)
{
	// 首先检查行和列的坐标是否在棋盘的有效范围内
	if (row < 0 || row >= BOARDMAXR || col < 0 || col >= BOARDMAXC)
		return false; 

	// 如果坐标在有效范围内，则检查该位置是否已被占用（值为0表示未占用）
	return boardInGame[row][col] == 0;
}


//-------------------------------------------------------//
// 函数参数：无                                          
// 函数功能：初始化BattleLayer，设置游戏的初始状态和资源 
// 函数返回值：bool型，表示初始化是否成功                
// 函数注意事项：无                                      
//-------------------------------------------------------//
bool BattleLayer::init()
{
	// 预加载音效资源
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/zhangfei.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/houyi.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/daji.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/skillSound.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/victory.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/defeat.wav");

	// 初始化游戏结束标志
	gameOver = false;

	// 初始化棋盘状态
	memset(boardInGame, 0, sizeof boardInGame);
	
    // 初始化玩家棋子
    for (shared_ptr<Chess> chess : *playerME->getBattleAreaChesses())
    {
		if (chess != nullptr) 
		{
			chessInitBeforeBattle(chess, true);
		}
    }


    playerOPP->buffJudgment();
    // 初始化对手棋子
    for (shared_ptr<Chess> chess : *playerOPP->getBattleAreaChesses())
    {
		if (chess != nullptr)
		{
			chessInitBeforeBattle(chess, false);
		}
    }
	
	

	// 调度启动update()函数，开始战斗
	this->scheduleUpdate();

	return true;
}


//-----------------------------------------------------//
// 函数参数：delta（每帧间隔时间）                     
// 函数功能：更新函数，用于每帧处理游戏逻辑            
// 函数返回值：无（void类型）                          
// 函数注意事项：需在游戏循环中持续调用                
//-----------------------------------------------------//
void BattleLayer::update(float delta)
{
	
	
    // 处理我方的每个棋子
    for (auto chess : *playerME->getBattleAreaChesses())
    {
		if (chess != nullptr) {
			// 如果棋子已死亡，则跳过
			if (chess->isDead())
				continue;

			// 播放棋子的行动
			playGame(chess, playerOPP);
		}
    }

    // 处理对方的每个棋子
    for (auto chess : *playerOPP->getBattleAreaChesses())
    {
		if (chess != nullptr)
		{
			// 如果棋子已死亡，则跳过
			if (chess->isDead())
				continue;

			// 播放棋子的行动
			playGame(chess, playerME);
		}
    }
	
	
	
	
	

	// 如果游戏结束
	if (gameOver)
	{
		// 停止更新
		this->unscheduleUpdate();

		// 创建辅助节点用于延时操作
		auto delayNode = Node::create();
		this->addChild(delayNode); // 添加到当前层

		// 创建延时动作
		auto delayAction = DelayTime::create(2.0f); // 2秒延时

		// 游戏结束后的回调动作
		auto callbackActionForEnd = CallFunc::create([this, delayNode]() 
			{
				// 检测胜负
				detectWinner();

				// 清理音效资源
				auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
				audio->unloadEffect("/res/Music/zhangfei.wav");
				audio->unloadEffect("/res/Music/houyi.wav");
				audio->unloadEffect("/res/Music/daji.wav");
				audio->unloadEffect("/res/Music/skillSound.wav");
				audio->unloadEffect("/res/Music/victory.wav");
				audio->unloadEffect("/res/Music/defeat.wav");
			});

		// 游戏开始前的准备动作
		auto callbackActionForStart = CallFunc::create([this, delayNode]() 
			{
				// 重置我方所有棋子状态，便于战后重新显示
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

				// 更新战斗状态标志
				*isInBattle = false;
				*canBuyChess = true;
			});

		// 创建并运行动作序列
		auto sequence = Sequence::create(delayAction, callbackActionForEnd, callbackActionForStart, nullptr);
		delayNode->runAction(sequence);
	}
}


//---------------------------------------------------------//
// 函数参数：playerME（玩家信息），playerOPP（对手信息）， 
//           isInBattle（指向是否处于战斗状态的标志），    
//           canBuyChess（指向是否可以购买棋子的标志）     
// 函数功能：静态创建函数，用于创建并初始化BattleLayer对象 
// 函数返回值：BattleLayer*，创建成功返回对象指针，        
//             失败返回nullptr                             
// 函数注意事项：使用new (std::nothrow)避免抛出异常        
//---------------------------------------------------------//
BattleLayer* BattleLayer::create(PlayerInfo* playerME, PlayerInfo* playerOPP, bool* isInBattle, bool* canBuyChess)
{
	// 尝试创建新的BattleLayer实例
	BattleLayer* layer = new (std::nothrow) BattleLayer(playerME, playerOPP, isInBattle, canBuyChess);

	// 初始化layer
	if (layer && layer->init())
	{
		// 初始化成功，将layer标记为自动释放并返回
		layer->autorelease();
		return layer;
	}

	// 如果初始化失败，则安全删除layer并返回空指针
	CC_SAFE_DELETE(layer);
	return nullptr;
}


// 构造函数
BattleLayer::BattleLayer(PlayerInfo* playerOfMe, PlayerInfo* playerOfOpp, bool* inBattle, bool* BuyChess)
{
	playerME = playerOfMe; playerOPP = playerOfOpp; isInBattle = inBattle; canBuyChess = BuyChess;
}

//---------------------------------------------------------------//
// 函数参数：gameResult（游戏结果，可以是"Win"、"Lost"或"Draw"） 
// 函数功能：显示结束游戏的动画（胜利、失败或平局）              
// 函数返回值：无（void类型）                                    
// 函数注意事项：根据不同的游戏结果显示不同的动画和音效          
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
	auto endAction = ScaleBy::create(0.5f, 1.5f);
	auto endActionReverse = endAction->reverse();
	auto callback = CallFunc::create([soundEffectPath,endGameImage, this]() 
		{
			endGameImage->removeFromParent();
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundEffectPath.c_str());
		});
	Sequence* sequence = Sequence::create(endAction, endActionReverse, callback, nullptr);

	// 获取 SimpleAudioEngine 实例
	//auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();

	// 设置音效音量为最大
	//audioEngine->setEffectsVolume(1.0f);
	// 播放音效
	
	//audioEngine->playEffect(soundEffectPath.c_str());
	endGameImage->runAction(sequence);
}

// 检测游戏胜负并执行相应的动作
void BattleLayer::detectWinner()
{
	// 玩家存活的棋子数量
	int surviveNumMe = 0;
	// 对手存活的棋子数量
	int surviveNumOPP = 0;

	// 计算玩家存活的棋子数量
	for (auto chess : *playerME->getBattleAreaChesses())
	{
		if (chess != nullptr && !chess->isDead())
		{
			surviveNumMe++;
		}
	}

	// 计算对手存活的棋子数量，并移除其棋子的显示
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

	// 根据存活棋子数量判断游戏结果，并展示相应动画
	if (surviveNumMe > 0) // 玩家胜利
	{
		playerME->gainCoin();
		playerOPP->DecreaseLifeValue(playerME->getLevel() + surviveNumMe);
		showEndGameAnimation("Win");
	}
	else if (surviveNumOPP > 0) // 对手胜利
	{
		playerME->DecreaseLifeValue(playerOPP->getLevel() + surviveNumOPP);
		showEndGameAnimation("Lost");
	}
	else // 平局
	{
		showEndGameAnimation("Draw");
	}
}


//---------------------------------------------------------//
// 函数参数：chess（参与游戏的棋子），opp（对手信息）      
// 函数功能：封装英雄进行战斗的全流程——寻找、寻路、攻击  
// 函数返回值：无（void类型）                              
// 函数注意事项：根据棋子的状态和位置执行相应的行动        
//---------------------------------------------------------//
void BattleLayer::playGame(shared_ptr<Chess> chess, PlayerInfo* opp)
{
	// 先确定攻击目标，确定目标一帧的时间应该是够的，findEnemy()函数执行后，将棋子的状态设为Moving
	if (chess->state == Chess::State::Idle)
	{
		shared_ptr<Chess> targetChess = findEnemy(chess, opp);
		// 没有攻击目标，游戏结束了
		if (targetChess == nullptr)
		{
			gameOver = true;
		}
		chess->targetChess = targetChess;
	}
	// 棋子处于移动状态但没有在移动时需要调用
	else if (chess->state == Chess::State::Moving && !chess->isMoving)
	{
		double distance = getDistance(&(chess->inGameChessBoardCoordinate), &(chess->targetChess->inGameChessBoardCoordinate));

		// 进入攻击范围了，不需要再移动了，设置棋子状态为攻击
		if (distance < (chess->getChessCondition()->improvedAttackDistance + 1))
		{
			chess->isMoving = false;
			chess->state = Chess::State::Attacking;
			// 注意，虽然进入攻击状态，但还没有开始攻击
			chess->isAttacking = false;
			return;
		}
		// 设置为开始移动，必须等findPathToEnemy()执行完毕后在函数结尾将isMoving重新置为false才能进入本分支再次调用
		chess->isMoving = true;
		findPathToEnemy(chess, chess->targetChess);
	}
	// 棋子处于攻击状态时但没有在攻击时需要调用
	else if (chess->state == Chess::State::Attacking && !chess->isAttacking)
	{

		// 设置开始攻击了，必须等doAttack()执行完毕后在函数结尾将isAttacking重新置为false才能进入本分支再次调用
		chess->isAttacking = true;
		doAttack(chess, chess->targetChess);
	}

}

//-----------------------------------------------------//
// 函数参数：start（起始棋子坐标），end（结束棋子坐标）
// 函数功能：获取两个棋子之间在棋盘上的曼哈顿距离      
// 函数返回值：double型，表示两个坐标之间的曼哈顿距离  
// 函数注意事项：当输入的指针为null时返回-1.0          
//-----------------------------------------------------//
double BattleLayer::getDistance(const ChessCoordinate* start,const ChessCoordinate* end)
{
	// 检查输入的指针是否为null
	if (start == nullptr || end == nullptr)
	{
		// 可以选择返回一个错误值或抛出异常
		return -1.0;
	}

	// 获取起始和结束位置的坐标
	int startX = start->getX();
	int startY = start->getY();
	int endX = end->getX();
	int endY = end->getY();

	// 计算并返回曼哈顿距离
	return abs(startX - endX) + abs(startY - endY);
}




//--------------------------------------------------------//
// 函数参数：damageMaker（攻击发起者），enemy（敌方信息） 
// 函数功能：寻找攻击目标，选择距离最近的敌方棋子         
// 函数返回值：shared_ptr<Chess>，找到的目标棋子指针      
// 函数注意事项：如果未找到目标棋子，则可能意味着游戏结束 
//--------------------------------------------------------//
shared_ptr<Chess> BattleLayer::findEnemy(shared_ptr<Chess> damageMaker, PlayerInfo* enemy)
{
	double minDistance = 9999; // 初始化一个很大的距离值
	shared_ptr<Chess> targetChess = nullptr; // 存储最近的敌方棋子

	// 遍历敌方所有棋子
	for (auto chess : *enemy->getBattleAreaChesses())
	{
		if (chess != nullptr)
		{
			// 跳过已死的棋子
			if (chess->isDead())
				continue;

			// 计算当前棋子与攻击者的距离
			double currentDistance = getDistance(&damageMaker->inGameChessBoardCoordinate, &chess->inGameChessBoardCoordinate);

			// 如果当前棋子更近，则更新目标棋子
			if (currentDistance <= minDistance)
			{
				targetChess = chess;
				minDistance = currentDistance;
			}
		}
	}

	// 如果没有找到目标棋子（即所有敌方棋子都已死亡）
	if (targetChess == nullptr)
	{
		return nullptr; // 游戏可能结束
	}

	// 设置攻击者的状态为移动状态，准备接近目标
	damageMaker->state = Chess::State::Moving;
	damageMaker->isMoving = false; // 还未开始移动

	return targetChess; // 返回找到的目标棋子
}


//-------------------------------------------------------------------//
// 函数参数：movingChess（要移动的棋子），targetPosition（目标位置） 
// 函数功能：移动指定的棋子到目标位置								 
// 函数返回值：无（void类型）										 
// 函数注意事项：移动完成后更新棋子的屏幕坐标						 
//-------------------------------------------------------------------//
void BattleLayer::moveChess(shared_ptr<Chess> movingChess,const Vec2 &targetPosition)
{
	// 开始移动
	Sprite* movingChessImage = movingChess->getChessSprite();
	float duration = 0.5f; // 移动所需时间，以秒为单位
	auto moveTo = MoveTo::create(duration, targetPosition);
	auto callback = CallFunc::create([this, movingChess,targetPosition]() 
		{
			movingChess->inGameScreenCoordinate.setX(int(targetPosition.x));
			movingChess->inGameScreenCoordinate.setY(int(targetPosition.y));
			
			// 移动一步完毕后，设置isMoving为false
			movingChess->isMoving = false;
		});

	auto sequence = Sequence::create(moveTo, callback, nullptr);
	movingChessImage->runAction(sequence);
}

//------------------------------------------------------------------//
// 函数参数：damageMaker（攻击发起者棋子），targetChess（目标棋子） 
// 函数功能：寻找到敌人的路径，使用A*寻路算法					    
// 函数返回值：bool型，找到路径返回true，否则返回false				
// 函数注意事项：路径寻找失败可能意味着无法达到目标				    
//------------------------------------------------------------------//
bool BattleLayer:: findPathToEnemy(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetChess) 
{
	// 开放集和关闭集
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
		// 找到开放集中fCost最小的节点
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

		// 如果当前节点是终点
		if (abs(currentNode->x- endNode->x)+ abs(currentNode->y - endNode->y)<damageMaker->getChessCondition()->improvedAttackDistance+1)
		{
			// 回溯找到路径的第一步
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

			// 清理内存
			for (auto node : openSet) delete node;
			for (auto node : closedSet) delete node;
			delete endNode;
			return true;
		}

		// 检查相邻节点
		for (int dx = -1; dx <= 1; dx++) 
		{
			for (int dy = -1; dy <= 1; dy++) 
			{
				if (dx == 0 && dy == 0) continue; // 跳过自身
				if (dx != 0 && dy != 0) continue; // 只允许垂直或水平移动

				int newX = currentNode->x + dx;
				int newY = currentNode->y + dy;

				// 检查是否可移动
				if (!isAvailable(newY, newX)) continue;

				pathNode* neighbour = new pathNode(newX, newY, currentNode);
				neighbour->gCost = currentNode->gCost + 1;
				neighbour->hCost = abs(newX - endNode->x) + abs(newY - endNode->y);
				neighbour->calculateFCost();

				// 检查节点是否已在关闭集中
				if (std::find_if(closedSet.begin(), closedSet.end(),
					[neighbour](pathNode* node) { return node->x == neighbour->x && node->y == neighbour->y; }) != closedSet.end()) {
					delete neighbour;
					continue;
				}

				// 检查是否已在开放集中
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

	// 清理内存
	for (auto node : openSet) delete node;
	for (auto node : closedSet) delete node;
	delete endNode;
	return false;
}


//-----------------------------------------------------------------------//
// 函数参数：damageMaker（攻击发起者棋子），targetEnemy（目标敌人棋子）, 
//           targetRow（目标棋子所在行），targetCol（目标棋子所在列）,   
//           targetChessImage（目标棋子的图像），useSkill（是否使用技能）
// 函数功能：共用的攻击后处理逻辑，包括技能攻击和普通攻击				 
// 函数返回值：无（void类型）											 
// 函数注意事项：根据是否使用技能进行不同的处理							 
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
		// 更新普通攻击计数器和技能进度条
		damageMaker->attackNum++;
		damageMaker->updateMpBar();
	}
	
	// 更新对手生命条进度
	targetEnemy->updateHpBar();

	// 检查攻击者和目标的生命状态
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

	// 设置攻击完成
	damageMaker->isAttacking = false;
}

// 播放相应的音效
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
// 函数参数：damageMaker（攻击发起者棋子），targetEnemy（目标敌人棋子） 
//			 useSkill（是否使用技能）									
// 函数功能：执行远程攻击的可视化动画，包括技能和普通远程攻击		    
// 函数返回值：无（void类型）										    
// 函数注意事项：设置攻击动画的角度和位置							    
//----------------------------------------------------------------------//
void BattleLayer::remoteAttackAnimation(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy,const bool &useSkill)
{
	Sprite* targetChessImage = targetEnemy->getChessSprite();
	// 远程攻击图像
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

	// 将贴图进行旋转
	Vec2 currentPosition = remoteAttackImage->getPosition();
	Vec2 targetPosition = targetChessImage->getPosition();
	// 计算目标方向的向量
	Vec2 direction = targetPosition - currentPosition;
	// 计算角度，这里使用 atan2 函数
	float angleRadians = atan2(direction.y, direction.x);
	float angleDegrees = CC_RADIANS_TO_DEGREES(-angleRadians);
	// 设置技能贴图的初始旋转角度
	remoteAttackImage->setRotation(angleDegrees);
	Vec2 OriginSize = remoteAttackImage->getContentSize();
	// 缩放大小由config一起控制
	auto config = ConfigController::getInstance();
	float ScaleX = 8 * config->getPx()->x / OriginSize.x;
	float ScaleY = OriginSize.y * 8 * config->getPx()->x / (OriginSize.y * OriginSize.x);
	remoteAttackImage->setScale(ScaleX, ScaleY);
	// 把技能的贴图添加到当前层或场景中
	this->addChild(remoteAttackImage);

}

//---------------------------------------------------------------------//
// 函数参数：damageMaker（攻击发起者棋子），targetEnemy（目标敌人棋子）
//           useSkill（是否使用技能）								   
// 函数功能：执行远程攻击的可视化动画，包括技能和普通远程攻击		   
// 函数返回值：无（void类型）										   
// 函数注意事项：设置攻击动画的角度和位置							   
//---------------------------------------------------------------------//
void BattleLayer::doAttack(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy)
{
	Sprite* damageMakerImage = damageMaker->getChessSprite();
	string damageMakerCareer = damageMaker->getCareer();
	Sprite* targetChessImage = targetEnemy->getChessSprite();
	Vec2 targetPosition = targetChessImage->getPosition();
	
	int targetRow = targetEnemy->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getY();
	int targetCol = targetEnemy->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getX();


	// 使用技能
	if (damageMaker->attackNum == (damageMaker->getChessCondition()->skillCooldown))
	{
		// 技能可视化
		remoteAttackAnimation(damageMaker, targetEnemy,true);
		auto damageMakerSkillImage=damageMaker->getSkillSprite();
		// 移动到目标所需的时间，以秒为单位
		float moveDuration = 0.5f;
		// 技能移动到目标位置
		auto moveSkill = MoveTo::create(moveDuration, targetPosition);
		// 技能击中对方后消失
		auto removeSkill = CallFunc::create([damageMakerSkillImage]()
			{
				damageMakerSkillImage->removeFromParent();
			});

		// 组合动作
		Sequence* skillAttack = Sequence::create(moveSkill, removeSkill, nullptr);

		// 击中目标后的数值处理
		auto callback = CallFunc::create([this, targetRow, targetCol, damageMaker, targetEnemy, damageMakerImage, targetChessImage]()
			{
				processAttackResult(damageMaker, targetEnemy, targetRow, targetCol, targetChessImage,true);
			});
			
		// 组合动作
		auto sequence = Sequence::create(skillAttack, callback, nullptr);
			
		// 执行技能动画和逻辑计算
		playSkillSound(damageMaker);
		damageMakerSkillImage->runAction(sequence);

	}
	// 进行一次普通攻击
	else
	{
		Sequence* normalAttack;
		if (damageMakerCareer == "mage" || damageMakerCareer == "shooter")
		{
			remoteAttackAnimation(damageMaker, targetEnemy,false);
			auto damageMakerAttackImage = damageMaker->getAttackSprite();
			// 移动到目标所需的时间，以秒为单位
			float moveDuration = 0.5f;
			// 远程攻击移动到目标位置
			auto moveAttack = MoveTo::create(moveDuration, targetPosition);
			// 击中对方后消失
			auto removeSprite = CallFunc::create([damageMakerAttackImage]()
				{
					damageMakerAttackImage->removeFromParent();
				});
			// 组合动作
			normalAttack = Sequence::create(moveAttack, removeSprite, nullptr);

			// 攻击移动到目标位置击中目标后的处理
			auto callback = CallFunc::create([this, targetRow, targetCol, damageMaker, targetEnemy, damageMakerImage, targetChessImage]()
				{
					processAttackResult(damageMaker, targetEnemy, targetRow, targetCol, targetChessImage,false);	
				});
			auto sequence = Sequence::create(normalAttack, callback, nullptr);
			
			
			// 执行技能动画和逻辑计算
			damageMakerAttackImage->runAction(sequence);
			if (damageMaker->getChessName() == HOUYI)
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/houyi.wav");
			else
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/skillSound.wav");
			
		}
		else
		{
			// 坦克近战的普通攻击动作：一个简单的缩放来表示攻击
			auto attackAction = ScaleBy::create(0.5f, 1.75f); // 缩放到原来的1.75倍
			auto attackActionReverse = attackAction->reverse(); // 攻击后缩放回原始大小
			// 组合动作
			normalAttack = Sequence::create(attackAction, attackActionReverse, nullptr);

			// 技能移动到目标位置击中目标后的处理
			auto callback = CallFunc::create([this, targetRow, targetCol, damageMaker, targetEnemy, damageMakerImage,targetChessImage]()
				{
					processAttackResult(damageMaker, targetEnemy, targetRow, targetCol, targetChessImage,false);
				});
			auto sequence = Sequence::create(normalAttack, callback, nullptr);
			
			
			// 执行技能动画和逻辑计算
			damageMakerImage->runAction(sequence);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/skillSound.wav");
		}
	}
}

// 路径节点的构造函数
pathNode::pathNode(int x, int y, pathNode* parent)
{
	this->x = x;
	this->y = y;
	this->parent = parent;
	gCost = hCost = fCost = 0;
}

// 更新路径成本
void pathNode::calculateFCost()
{
	fCost = gCost + hCost;
}