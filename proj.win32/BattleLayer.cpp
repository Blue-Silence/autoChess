#include "BattleLayer.h"



bool BattleLayer::init()
{
	for (Chess* chess : *playerME->getBattleAreaChesses())
	{

		chess->initCondition();
	}
	for (Chess* chess : *playerOPP->getBattleAreaChesses())
	{
		chess->initCondition();
	}
	

	return true;
}

// 
void BattleLayer::update(float delta) 
{
	// 更新游戏状态
	// 例如：角色移动，寻找攻击目标，执行攻击等
	// ...

	for (auto chess : *playerME->getBattleAreaChesses())
	{
		Chess* targetChess;
		// 先确定攻击目标，确定目标一帧的时间应该是够的，findEnemy()函数执行后，将棋子的状态设为Moving
		if(chess->state == Chess::State::Idle)
			targetChess = findEnemy(chess, playerOPP);
		// 棋子处于移动状态但没有在移动时需要调用
		else if (chess->state == Chess::State::Moving && !chess->isMoving)
		{
			// 进入攻击范围了，不需要再移动了，设置棋子状态为攻击
			if (getDistance(chess->getChessCoordinateByType(CoordinateType::chessBoardCoordinates),
				targetChess->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)) 
				< (chess->getChessCondition()->improvedAttackDistance + 1))
			{
				chess->state = Chess::State::Attacking;
				chess->isMoving = false;
				// 注意，虽然进入攻击状态，但还没有开始攻击
				chess->isAttacking = false;
				continue;
			}
			// 设置为开始移动，必须等findPathToEnemy()执行完毕后在函数结尾将isMoving重新置为false才能进入本分支再次调用
			chess->isMoving = true;
			findPathToEnemy(chess, targetChess, chessBoard);
		}
		// 棋子处于攻击状态时但没有在攻击时需要调用
		else if (chess->state == Chess::State::Attacking && !chess->isAttacking)
		{

			// 设置开始攻击了，必须等doAttack()执行完毕后在函数结尾将isAttacking重新置为false才能进入本分支再次调用
			chess->isAttacking = true;
			doAttack(chess, targetChess);

		}

		// 

	}

	
}

// 获取两个棋子之间在祺盘上的曼哈顿距离
double BattleLayer::getDistance(ChessCoordinate* start, ChessCoordinate* end)
{
	// 检查输入的指针是否为null
	if (start == nullptr || end == nullptr) 
	{
		// 可以选择返回一个错误值或抛出异常
		return -1.0; 
	}

	// 计算并返回曼哈顿距离
	return abs(start->getX() - end->getX()) + abs(start->getY() - end->getY());
}


//void Battle::pieceBattle(Chess &damageMaker, Chess &victim)
//{
	//进行了攻击
	//damageMaker.attackOne(victim);
//}

// 寻找攻击目标，选择距离最近的敌方英雄
Chess* BattleLayer::findEnemy(Chess* damageMaker, PlayerInfo* enemy)
{
	double distance = 999;
	Chess* targetChess;

	for (auto chess : *enemy->getBattleAreaChesses())
	{
		if (chess->isDead())
			continue;
		int tmp = getDistance(damageMaker->getChessCoordinateByType(CoordinateType::chessBoardCoordinates),
			(chess->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)));
		if (tmp < distance)
		{
			targetChess = chess;
		}
	}

	
	// 将棋子的状态转入移动状态
	damageMaker->state = Chess::State::Moving;
	// 注意只是进入移动状态，但还没开始移动
	damageMaker->isMoving = false;

	return targetChess;
}

// 将精灵移动到指定位置，坐标需为屏幕坐标
void BattleLayer::moveChess(Chess* movingChess,Vec2 targetPosition)
{
	Sprite* movingChessImage = movingChess->getChessSprite();
	float duration = 0.2f; // 移动所需时间，以秒为单位
	auto moveTo = MoveTo::create(duration, targetPosition);
	auto callback = CallFunc::create([this, movingChess,targetPosition]() 
		{
			
		// 移动完成后的处理
		// 例如：更新棋子状态
			movingChess->setChessCoordinateByType(targetPosition, CoordinateType::screenCoordinates);
			ChessCoordinate* newPos;
			CoordinateConvert(CoordinateType::chessBoardCoordinates, targetPosition, newPos);
			movingChess->setChessCoordinateByType(Vec2(newPos->getY(), newPos->getX()), CoordinateType::chessBoardCoordinates);
			// 移动一步完毕后，设置isMoving为false
			movingChess->isMoving = false;
		});

	auto sequence = Sequence::create(moveTo, callback, nullptr);
	movingChessImage->runAction(sequence);
}

// 寻路函数 每次只走一步
void BattleLayer::findPathToEnemy(Chess* damageMaker, Chess* targetChess,ChessBoard* chessboard)
{
	ChessCoordinate src, dst;

	src = *damageMaker->getChessCoordinateByType(CoordinateType::chessBoardCoordinates);
	dst = *targetChess->getChessCoordinateByType(CoordinateType::chessBoardCoordinates);

	int xOfdamageMaker = src.getX();
	int yOfdamageMaker = src.getY();
	int xOftargetChess = dst.getX();
	int yOftargetChess = dst.getY();

	Sprite* damageMakerImage = damageMaker->getChessSprite();
	Sprite* targetChessImage = targetChess->getChessSprite();

	ChessCoordinate* newPos;
	int dx = 0;
	int dy = 0;
	
	
	// 先走y方向
	if (yOfdamageMaker < yOftargetChess && chessboard->isAvailable(yOfdamageMaker + 1, xOfdamageMaker))
	{
		dy = 1;
	}
	else if (yOfdamageMaker > yOftargetChess && chessboard->isAvailable(yOfdamageMaker - 1, xOfdamageMaker))
	{
		dy = -1;
	}
	// 再走x方向
	else if (xOfdamageMaker < xOftargetChess && chessboard->isAvailable(yOfdamageMaker, xOfdamageMaker + 1))
	{
		dx = 1;
	}
	else if (xOfdamageMaker > xOftargetChess && chessboard->isAvailable(yOfdamageMaker, xOfdamageMaker - 1))
	{
		dx = -1;
	}

	CoordinateConvert(CoordinateType::screenCoordinates, { xOfdamageMaker+dx,yOfdamageMaker+dy }, newPos);
	damageMaker->targetPos = Vec2(newPos->getX(),newPos->getY());
	moveChess(damageMaker, damageMaker->targetPos);
	
}


// 待添加音效


void BattleLayer::doAttack(Chess* damageMaker, Chess* targetEnemy)
{
	Sprite* damageMakerImage = damageMaker->getChessSprite();
	string damageMakerCareer = damageMaker->GetCareer();
	Sprite* targetChessImage = targetEnemy->getChessSprite();

	// 使用技能
	if (damageMaker->attackNum == damageMaker->getChessCondition()->skillCooldown)
	{

		// 技能可视化逻辑
		damageMaker->createSkill();
		Sprite* damageMakerSkillImage = damageMaker->getSkillSprite();
		// 把技能的贴图添加到当前层或场景中
		this->addChild(damageMakerSkillImage);
		// 移动到目标所需的时间，以秒为单位
		float moveDuration = 0.5f;
		// 技能移动到目标位置
		auto moveSkill = MoveTo::create(moveDuration, targetChessImage->getPosition());

		// 技能移动到目标位置击中目标后的处理
		auto callback = CallFunc::create([damageMaker,targetEnemy,damageMakerImage,damageMakerSkillImage]()
		{
			// 移除技能贴图
			damageMakerSkillImage->removeFromParent();
			// 数值逻辑
			damageMaker->skill(*targetEnemy);
			// 更新对手生命条进度
			targetEnemy->updateHpBar();
			// 更新自己的技能进度条
			damageMaker->updateMpBar(0);
			// 重置普通攻击计数器
			damageMaker->attackNum = 0;

			// 判断自己是否死了
			if (damageMaker->isDead())
			{
				damageMakerImage->removeFromParent();

			}
			// 对方已死，停止攻击
			if (targetEnemy->isDead())
			{

			}

		});
		auto sequence = Sequence::create(moveSkill, callback, nullptr);
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
			// 把普通攻击的贴图添加到当前层或场景中
			this->addChild(damageMakerAttackImage);
			// 移动到目标所需的时间，以秒为单位
			float moveDuration = 0.5f;
			// 远程攻击移动到目标位置
			auto moveAttack = MoveTo::create(moveDuration, targetChessImage->getPosition());
			// 击中对方后消失
			auto removeSprite = CallFunc::create([damageMakerAttackImage]()
				{
					damageMakerAttackImage->removeFromParent();
				});
			// 组合动作
			normalAttack = Sequence::create(moveAttack, removeSprite, nullptr);

			// 技能移动到目标位置击中目标后的处理
			auto callback = CallFunc::create([damageMaker, targetEnemy, damageMakerImage]()
			{
					// 数值逻辑
					damageMaker->attackOne(*targetEnemy);

					// 更新普通攻击计数器
					damageMaker->attackNum++;

					// 更新对手生命条进度
					targetEnemy->updateHpBar();
					// 更新自己的技能进度条
					damageMaker->updateMpBar(damageMaker->attackNum);
					// 判断自己是否死了
					if (damageMaker->isDead())
					{
						damageMakerImage->removeFromParent();

					}
					// 对方已死，停止攻击
					if (targetEnemy->isDead())
					{

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
			auto attackAction = ScaleBy::create(0.2f, 1.2f); // 缩放到原来的1.2倍
			auto attackActionReverse = attackAction->reverse(); // 攻击后缩放回原始大小

			// 组合动作
			normalAttack = Sequence::create(attackAction, attackActionReverse, nullptr);

			// 技能移动到目标位置击中目标后的处理
			auto callback = CallFunc::create([damageMaker, targetEnemy, damageMakerImage]()
			{
					// 数值逻辑
					damageMaker->attackOne(*targetEnemy);

					// 更新普通攻击计数器
					damageMaker->attackNum++;

					// 更新对手生命条进度
					targetEnemy->updateHpBar();
					// 更新自己的技能进度条
					damageMaker->updateMpBar(damageMaker->attackNum);
					// 判断自己是否死了
					if (damageMaker->isDead())
					{
						damageMakerImage->removeFromParent();

					}
					// 对方已死，停止攻击
					if (targetEnemy->isDead())
					{

					}
					// 攻击完毕后设置棋子状态为不在攻击，以便下次update可以调用新一次的doAttack()
					damageMaker->isAttacking = false;


			});
			auto sequence = Sequence::create(normalAttack, callback, nullptr);
			// 执行技能动画和逻辑计算
			damageMakerImage->runAction(sequence);
		}
	}
}

























bool BattleLayer::isEnd()
{
	
}


