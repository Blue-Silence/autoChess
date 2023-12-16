#include "BattleLayer.h"



bool BattleLayer::init()
{
	for (Chess* chess : *playerME.getchessInBattleArea())
	{

		chess->initCondition();
	}
	for (Chess* chess : *playerOPP.getchessInBattleArea())
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

	// 调用其他您定义的方法
	// 比如：findAttackTargets(), moveCharacters(), performAttacks() 等
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
Chess* BattleLayer::findEnemy(Chess* damageMaker, PlayerInfo enemy)
{
	double distance = 999;
	Chess* targetEnemyChess;

	for (int i = 0; i < 7; i++) 
	{
		if (enemy.chessInBattleArea[i] == nullptr) 
		{//没棋子了
			break;
		}
		else
		{
			double temp = 0;
			temp = BattleLayer::getDistance(damageMaker->getChessCoordinateByType(CoordinateType::chessBoardCoordinates), 
				enemy.chessInBattleArea[i]->getChessCoordinateByType(CoordinateType::chessBoardCoordinates));
			
			if (temp < distance) 
			{
				targetEnemyChess = enemy.chessInBattleArea[i];
			}
		}
	}
	return targetEnemyChess;
}

// 将精灵移动到指定位置，坐标需为屏幕坐标
void BattleLayer::moveChess(Sprite* movingChess,Vec2 targetPosition)
{
	float duration = 0.5f; // 移动所需时间，以秒为单位
	auto moveTo = MoveTo::create(duration, targetPosition);
	movingChess->runAction(moveTo);
	
}

// 寻路函数 循环调用，只需要知道下一步怎么走即可
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

	coordinateConvert(CoordinateType::screenCoordinates, { xOfdamageMaker+dx,yOfdamageMaker+dy }, newPos);
	moveChess(damageMakerImage, Vec2{ newPos->getX(),newPos->getY() });
}


// 待添加音效
void BattleLayer::doAttack(Chess* damageMaker, Chess* targetEnemy)
{
	Sprite* damageMakerImage = damageMaker->getChessSprite();
	string damageMakerCareer = damageMaker->GetCareer();
	Sprite* targetChessImage = targetEnemy->getChessSprite();

	// 普攻计数器
	int attackNum = 0;

	while (true)
	{
		// 使用技能
		if (attackNum == damageMaker->getChessCondition()->skillCooldown)
		{

			// 技能可视化逻辑
			damageMaker->createSkill();
			Sprite* damageMakerSkillImage = damageMaker->getSkillSprite();
			// 把技能的贴图添加到当前层或场景中
			this->addChild(damageMakerSkillImage);
			// 移动到目标所需的时间，以秒为单位
			float moveDuration = 0.5f;
			// 技能移动到目标位置
			auto moveAction = MoveTo::create(moveDuration, targetChessImage->getPosition());
			// 击中对方后消失
			auto removeSprite = CallFunc::create([damageMakerSkillImage]()
				{
					damageMakerSkillImage->removeFromParent();
				});
			
			// 数值逻辑
			damageMaker->skill(*targetEnemy);
			// 更新对手生命条进度
			targetEnemy->updateHpBar();
			// 更新自己的技能进度条
			damageMaker->updateMpBar(0);
			attackNum = 0;
			// 判断自己是否死了
			if (damageMaker->isDead())
			{
				damageMakerImage->removeFromParent();
				break;
			}
			// 对方已死，停止攻击
			if (targetEnemy->isDead())
			{
				break;
			}
		}

		// 进行一次普攻 待添加音效
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
			auto moveAction = MoveTo::create(moveDuration, targetChessImage->getPosition());
			// 击中对方后消失
			auto removeSprite = CallFunc::create([damageMakerAttackImage]() 
				{
					damageMakerAttackImage->removeFromParent();
				});
		}
		else
		{
			// 坦克近战的普通攻击动作：一个简单的缩放来表示攻击
			auto attackAction = ScaleBy::create(0.2f, 1.2f); // 缩放到原来的1.2倍
			auto attackActionReverse = attackAction->reverse(); // 攻击后缩放回原始大小

			// 组合动作
			auto attackSequence = Sequence::create(attackAction, attackActionReverse, nullptr);
			// 让攻击者执行这个动作序列
			damageMakerImage->runAction(attackSequence);
		}
		
		// 数值逻辑
		damageMaker->attackOne(*targetEnemy);
		++attackNum;
		// 更新对手生命条进度
		targetEnemy->updateHpBar();
		// 更新自己的技能进度条
		damageMaker->updateMpBar(attackNum);
		// 判断自己是否死了
		if (damageMaker->isDead())
		{
			damageMakerImage->removeFromParent();
			break;
		}
		// 对方已死，停止攻击
		if (targetEnemy->isDead())
		{
			break;
		}
	}
}

































bool BattleLayer::detect(PlayerInfo A)
{
	bool over = -1;
	for (int i = 0; i < 7; i++) {
		if (A.BattleChessNum[i] == nullptr) {//没棋子了
			break;
		}
		else {
			if (A.BattleChessNum[i]->isDead()) {//棋子死了
				if (over == -1) {
					over = 1;
				}
			}
			else {
				over = 0;
			}
		}
	}
	if (over != 0) {//结束了
		return true;
	}
	else {
		return false;
	}
}


