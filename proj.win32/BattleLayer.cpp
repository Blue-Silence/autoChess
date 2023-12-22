#include "BattleLayer.h"






void BattleLayer::AItest()
{
	
	

	playerOPP->putChessInBattleArea(make_shared<shooter>(HOUYI));
	playerOPP->putChessInBattleArea(make_shared<shooter>(DIRENJIE));

	ChessCoordinate* newPos = new ChessCoordinate;

	shared_ptr<Chess> daji = (*playerOPP->getBattleAreaChesses())[0];
	shared_ptr<Chess> diaochan = (*playerOPP->getBattleAreaChesses())[1];

	daji->setChessCoordinateByType(Vec2(3, 4), CoordinateType::chessBoardCoordinates);
	CoordinateConvert(CoordinateType::screenCoordinates, Vec2(3, 4), newPos);
	daji->setChessCoordinateByType(Vec2(newPos->getX(),newPos->getY()), CoordinateType::screenCoordinates);
	this->addChild(daji->createChess(Vec2 (newPos->getX(),newPos->getY())));

	diaochan->setChessCoordinateByType(Vec2(2, 3), CoordinateType::chessBoardCoordinates);
	CoordinateConvert(CoordinateType::screenCoordinates, Vec2(2, 3), newPos);
	diaochan->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);
	this->addChild(diaochan->createChess(Vec2(newPos->getX(), newPos->getY())));

	delete newPos;




	/*playerME.putChessInBattleArea(make_shared<mage>(DAJI));
	playerME.putChessInBattleArea(make_shared<mage>(DIAOCHAN));

	newPos = new ChessCoordinate;

	shared_ptr<Chess> daji1 = (*playerME.getBattleAreaChesses())[0];
	shared_ptr<Chess> diaochan1 = (*playerME.getBattleAreaChesses())[1];

	daji1->setChessCoordinateByType(Vec2(5, 0), CoordinateType::chessBoardCoordinates);
	CoordinateConvert(CoordinateType::screenCoordinates, Vec2(5, 0), newPos);
	daji1->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);
	this->addChild(daji1->createChess(Vec2(newPos->getX(), newPos->getY())));

	diaochan1->setChessCoordinateByType(Vec2(3, 1), CoordinateType::chessBoardCoordinates);
	CoordinateConvert(CoordinateType::screenCoordinates, Vec2(3, 1), newPos);
	diaochan1->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);
	this->addChild(diaochan1->createChess(Vec2(newPos->getX(), newPos->getY())));*/

	//delete newPos;


	

	



}

// 战斗前初始化棋子的战斗数据
void BattleLayer::chessInitBeforeBattle(shared_ptr<Chess> chess)
{
	chess->state = Chess::State::Idle;
	chess->isAttacking = false;
	chess->isMoving = false;
	chess->getChessCondition()->HP = chess->getChessCondition()->maxHP;
	chess->attackNum = 0;
	chess->inGameChessBoardCoordinate.setX(chess->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getX());
	chess->inGameChessBoardCoordinate.setY(chess->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getY());
	chess->inGameScreenCoordinate.setX(chess->getChessCoordinateByType(CoordinateType::screenCoordinates)->getX());
	chess->inGameScreenCoordinate.setY(chess->getChessCoordinateByType(CoordinateType::screenCoordinates)->getY());
	boardInGame[chess->inGameChessBoardCoordinate.getY()][chess->inGameChessBoardCoordinate.getX()] = 1;
}

inline bool BattleLayer::isAvailable(int row, int col)
{
	if (row < 0 || row >= BOARDMAXR || col < 0 || col >= BOARDMAXC)
		return false;

	return boardInGame[row][col] == 0;
}

bool BattleLayer::init()
{
	gameOver = false;

	memset(boardInGame, 0, sizeof boardInGame);

	AItest();


	for (shared_ptr<Chess> chess : *playerME->getBattleAreaChesses())
	{
		chessInitBeforeBattle(chess);
	}

	for (shared_ptr<Chess> chess : *playerOPP->getBattleAreaChesses())
	{
		chessInitBeforeBattle(chess);
	}
	
	// 调度启动update()函数，开始战斗
	this->scheduleUpdate();


	return true;
}

// 
void BattleLayer::update(float delta) 
{
	// 我方
	for (auto chess : *playerME->getBattleAreaChesses())
	{
		if (chess->isDead())
			continue;
		
		playGame(chess,playerOPP);
	}
	// 对方
	for (auto chess : *playerOPP->getBattleAreaChesses())
	{
		if (chess->isDead())
			continue;
		playGame(chess, playerME);
	}

	if(gameOver)
	{
		// 取消对update函数的调度
		this->unscheduleUpdate();
		
	}
}


void BattleLayer::detectWinner()
{
	
}

// 封装英雄进行战斗的全流程——寻找、寻路、攻击
void BattleLayer::playGame(shared_ptr<Chess> chess,PlayerInfo* opp)
{
	// 先确定攻击目标，确定目标一帧的时间应该是够的，findEnemy()函数执行后，将棋子的状态设为Moving
	if (chess->state == Chess::State::Idle)
	{
		shared_ptr<Chess> targetChess = findEnemy(chess, opp);
		// 没有攻击目标，游戏结束了
		if (targetChess == nullptr || gameOver)
		{
			gameOver = true;
		}
		chess->targetChess = targetChess;
	}
	// 棋子处于移动状态但没有在移动时需要调用
	else if (chess->state == Chess::State::Moving && !chess->isMoving)
	{
		// 进入攻击范围了，不需要再移动了，设置棋子状态为攻击
		if (getDistance(&chess->inGameChessBoardCoordinate,
			&chess->targetChess->inGameChessBoardCoordinate)
			< (chess->getChessCondition()->improvedAttackDistance+1))
		{
			chess->isMoving = false;
			chess->state = Chess::State::Attacking;
			// 注意，虽然进入攻击状态，但还没有开始攻击
			chess->isAttacking = false;
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


// 获取两个棋子之间在祺盘上的曼哈顿距离
double BattleLayer::getDistance(ChessCoordinate* start,ChessCoordinate* end)
{
	// 检查输入的指针是否为null
	if (start == nullptr || end == nullptr) 
	{
		// 可以选择返回一个错误值或抛出异常
		return -1.0; 
	}
	int startX = start->getX();
	int startY = start->getY();

	int endX = end->getX();
	int endY = end->getY();

	// 计算并返回曼哈顿距离
	return abs(startX-endX) + abs(startY-endY);
}



// 寻找攻击目标，选择距离最近的敌方英雄
shared_ptr<Chess> BattleLayer::findEnemy(shared_ptr<Chess> damageMaker, PlayerInfo* enemy)
{
	double distance = 999;

	shared_ptr<Chess> targetChess = nullptr;

	for (auto chess : *enemy->getBattleAreaChesses())
	{
		if (chess->isDead())
			continue;

		int tmp = getDistance(&damageMaker->inGameChessBoardCoordinate,
			(&chess->inGameChessBoardCoordinate));

		if (tmp < distance)
		{
			targetChess = chess;
			distance = tmp;
		}
	}

	// 说明对方已经没有存活的棋子了，游戏结束
	if (targetChess == nullptr)
	{
		return nullptr;
	}

	// 将棋子的状态转入移动状态
	damageMaker->state = Chess::State::Moving;
	// 注意只是进入移动状态，但还没开始移动
	damageMaker->isMoving = false;

	return targetChess;
}

// 将精灵移动到指定位置，坐标需为屏幕坐标
void BattleLayer::moveChess(shared_ptr<Chess> movingChess,Vec2 targetPosition)
{
	// 移动前先更新棋盘坐标，抢占位置，避免位置冲突
	ChessCoordinate* newPos = new ChessCoordinate;
	CoordinateConvert(CoordinateType::chessBoardCoordinates, targetPosition, newPos);
	int oldRow = movingChess->inGameChessBoardCoordinate.getY();
	int oldCol = movingChess->inGameChessBoardCoordinate.getX();
	boardInGame[oldRow][oldCol] = 0;
	movingChess->inGameChessBoardCoordinate.setX(newPos->getX());
	movingChess->inGameChessBoardCoordinate.setY(newPos->getY());
	boardInGame[newPos->getY()][newPos->getX()] = 1;
	delete newPos;

	// 开始移动
	Sprite* movingChessImage = movingChess->getChessSprite();
	float duration = 0.5f; // 移动所需时间，以秒为单位
	auto moveTo = MoveTo::create(duration, targetPosition);
	auto callback = CallFunc::create([this, movingChess,targetPosition]() 
		{
			
			// 移动完成后的处理
			// 移动完成才更新棋子在游戏中的屏幕坐标
			movingChess->inGameScreenCoordinate.setX(int(targetPosition.x));
			movingChess->inGameScreenCoordinate.setY(int(targetPosition.y));
			
			// 移动一步完毕后，设置isMoving为false
			movingChess->isMoving = false;
		});

	auto sequence = Sequence::create(moveTo, callback, nullptr);
	movingChessImage->runAction(sequence);
}

// 寻路函数 每次只走一步
void BattleLayer::findPathToEnemy(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetChess)
{
	ChessCoordinate *src, *dst;

	src = &damageMaker->inGameChessBoardCoordinate;
	dst = &targetChess->inGameChessBoardCoordinate;

	int xOfdamageMaker = src->getX();
	int yOfdamageMaker = src->getY();

	int xOftargetChess = dst->getX();
	int yOftargetChess = dst->getY();

	Sprite* damageMakerImage = damageMaker->getChessSprite();
	Sprite* targetChessImage = targetChess->getChessSprite();

	
	ChessCoordinate* newPos = new ChessCoordinate;
	
	int dx = 0;
	int dy = 0;
	
	
	// 先走y方向
	if (yOfdamageMaker < yOftargetChess && isAvailable(yOfdamageMaker + 1, xOfdamageMaker))
	{
		dy = 1;
	}
	else if (yOfdamageMaker > yOftargetChess && isAvailable(yOfdamageMaker - 1, xOfdamageMaker))
	{
		dy = -1;
	}
	// 再走x方向
	else if (xOfdamageMaker < xOftargetChess && isAvailable(yOfdamageMaker, xOfdamageMaker + 1))
	{
		dx = 1;
	}
	else if (xOfdamageMaker > xOftargetChess && isAvailable(yOfdamageMaker, xOfdamageMaker - 1))
	{
		dx = -1;
	}
	

	CoordinateConvert(CoordinateType::screenCoordinates, Vec2(int(xOfdamageMaker + dx),int(yOfdamageMaker + dy)), newPos);
	damageMaker->targetPos = Vec2(newPos->getX(),newPos->getY());
	delete newPos;
	moveChess(damageMaker, damageMaker->targetPos);
	
}


// 进行一次攻击，待添加音效
void BattleLayer::doAttack(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy)
{
	Sprite* damageMakerImage = damageMaker->getChessSprite();
	string damageMakerCareer = damageMaker->GetCareer();
	Sprite* targetChessImage = targetEnemy->getChessSprite();
	
	int targetRow = targetEnemy->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getY();
	int targetCol = targetEnemy->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getX();


	// 使用技能
	if (damageMaker->attackNum == (damageMaker->getChessCondition()->skillCooldown))
	{

		// 技能可视化逻辑
		damageMaker->createSkill();
		Sprite* damageMakerSkillImage = damageMaker->getSkillSprite();

		// 将技能贴图进行旋转
		Vec2 currentPosition = damageMakerSkillImage->getPosition();
		Vec2 targetPosition = targetChessImage->getPosition();
		// 计算目标方向的向量
		Vec2 direction = targetPosition - currentPosition;
		// 计算角度，这里使用 atan2 函数
		float angleRadians = atan2(direction.y, direction.x);
		float angleDegrees = CC_RADIANS_TO_DEGREES(-angleRadians);
		// 设置技能贴图的初始旋转角度
		damageMakerSkillImage->setRotation(angleDegrees);

		// 把技能的贴图添加到当前层或场景中
		this->addChild(damageMakerSkillImage);
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
				// 数值逻辑
				damageMaker->skill(*targetEnemy);
				// 更新对手生命条进度
				targetEnemy->updateHpBar();
				// 重置普通攻击计数器
				damageMaker->attackNum = 0;
				// 更新自己的技能进度条
				damageMaker->updateMpBar();

				// 判断自己是否死了
				if (damageMaker->isDead())
				{

					damageMaker->state = Chess::State::Dead;

				}

				// 对方已死，停止攻击,重新开始寻找攻击目标
				if (targetEnemy->isDead())
				{
					targetChessImage->setVisible(false);
					boardInGame[targetRow][targetCol] = 0;
					//targetChessImage->removeFromParent();
					damageMaker->state = Chess::State::Idle;
					damageMaker->isMoving = false;
				}

				damageMaker->isAttacking = false;
			});

			// 组合动作
			auto sequence = Sequence::create(skillAttack, callback, nullptr);
			// 执行技能动画和逻辑计算
			damageMakerSkillImage->runAction(sequence);
		
	}
	// 进行一次普通攻击，待添加音效
	else
	{
		Sequence* normalAttack;
		if (damageMakerCareer == "mage" || damageMakerCareer == "shooter")
		{
			// 普攻可视化逻辑
			damageMaker->createAttack();
			Sprite* damageMakerAttackImage = damageMaker->getAttackSprite();
			// 将远程攻击贴图进行旋转
			Vec2 currentPosition = damageMakerAttackImage->getPosition();
			Vec2 targetPosition = targetChessImage->getPosition();
			// 计算目标方向的向量
			Vec2 direction = targetPosition - currentPosition;

			// 计算角度，这里使用 atan2 函数
			float angleRadians = atan2(direction.y, direction.x);
			float angleDegrees = CC_RADIANS_TO_DEGREES(-angleRadians);
			// 设置技能贴图的初始旋转角度
			damageMakerAttackImage->setRotation(angleDegrees);

			// 把普通攻击的贴图添加到当前层或场景中
			this->addChild(damageMakerAttackImage);
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
					// 数值逻辑
					damageMaker->attackOne(*targetEnemy);
					// 更新普通攻击计数器
					damageMaker->attackNum += 1;
					// 更新对手生命条进度
					targetEnemy->updateHpBar();
					// 更新自己的技能进度条
					damageMaker->updateMpBar();
					// 判断自己是否死了
					if (damageMaker->isDead())
					{
						damageMaker->state = Chess::State::Dead;
					}
					// 对方已死，停止攻击,重新开始寻找攻击目标
					if (targetEnemy->isDead())
					{
						targetChessImage->setVisible(false);
						boardInGame[targetRow][targetCol] = 0;
						//targetChessImage->removeFromParent();

						damageMaker->state = Chess::State::Idle;
						damageMaker->isMoving = false;

					}
					// 攻击完毕后设置棋子状态为不在攻击，以便下次update可以调用新一次的doAttack()
					damageMaker->isAttacking = false;
				});
			auto sequence = Sequence::create(normalAttack, callback, nullptr);
			// 执行技能动画和逻辑计算
			damageMakerAttackImage->runAction(sequence);
			
		}
		else
		{
			// 坦克近战的普通攻击动作：一个简单的缩放来表示攻击
			auto attackAction = ScaleBy::create(0.5f, 2.0f); // 缩放到原来的1.2倍
			auto attackActionReverse = attackAction->reverse(); // 攻击后缩放回原始大小

			// 组合动作
			normalAttack = Sequence::create(attackAction, attackActionReverse, nullptr);

			// 技能移动到目标位置击中目标后的处理
			auto callback = CallFunc::create([this, targetRow, targetCol, damageMaker, targetEnemy, damageMakerImage,targetChessImage]()
				{
					// 数值逻辑
					damageMaker->attackOne(*targetEnemy);

					// 更新普通攻击计数器
					damageMaker->attackNum++;
					// 更新对手生命条进度
					targetEnemy->updateHpBar();
					// 更新自己的技能进度条
					damageMaker->updateMpBar();
					// 判断自己是否死了
					if (damageMaker->isDead())
					{
						
						damageMaker->state = Chess::State::Dead;
						damageMaker->isAttacking = false;

					}
					// 对方已死，停止攻击,重新开始寻找攻击目标
					if (targetEnemy->isDead())
					{
						
						targetChessImage->setVisible(false);
						boardInGame[targetRow][targetCol] = 0;
						//targetChessImage->removeFromParent();
						damageMaker->state = Chess::State::Idle;
						damageMaker->isAttacking = false;

					}
					damageMaker->isAttacking = false;
			});
			auto sequence = Sequence::create(normalAttack, callback, nullptr);
			// 执行技能动画和逻辑计算
			damageMakerImage->runAction(sequence);
		}
	}
}
