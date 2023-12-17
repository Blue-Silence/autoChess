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
	// ������Ϸ״̬
	// ���磺��ɫ�ƶ���Ѱ�ҹ���Ŀ�ִ꣬�й�����
	// ...

	for (auto chess : *playerME->getBattleAreaChesses())
	{
		Chess* targetChess;
		// ��ȷ������Ŀ�꣬ȷ��Ŀ��һ֡��ʱ��Ӧ���ǹ��ģ�findEnemy()����ִ�к󣬽����ӵ�״̬��ΪMoving
		if(chess->state == Chess::State::Idle)
			targetChess = findEnemy(chess, playerOPP);
		// ���Ӵ����ƶ�״̬��û�����ƶ�ʱ��Ҫ����
		else if (chess->state == Chess::State::Moving && !chess->isMoving)
		{
			// ���빥����Χ�ˣ�����Ҫ���ƶ��ˣ���������״̬Ϊ����
			if (getDistance(chess->getChessCoordinateByType(CoordinateType::chessBoardCoordinates),
				targetChess->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)) 
				< (chess->getChessCondition()->improvedAttackDistance + 1))
			{
				chess->state = Chess::State::Attacking;
				chess->isMoving = false;
				// ע�⣬��Ȼ���빥��״̬������û�п�ʼ����
				chess->isAttacking = false;
				continue;
			}
			// ����Ϊ��ʼ�ƶ��������findPathToEnemy()ִ����Ϻ��ں�����β��isMoving������Ϊfalse���ܽ��뱾��֧�ٴε���
			chess->isMoving = true;
			findPathToEnemy(chess, targetChess, chessBoard);
		}
		// ���Ӵ��ڹ���״̬ʱ��û���ڹ���ʱ��Ҫ����
		else if (chess->state == Chess::State::Attacking && !chess->isAttacking)
		{

			// ���ÿ�ʼ�����ˣ������doAttack()ִ����Ϻ��ں�����β��isAttacking������Ϊfalse���ܽ��뱾��֧�ٴε���
			chess->isAttacking = true;
			doAttack(chess, targetChess);

		}

		// 

	}

	
}

// ��ȡ��������֮���������ϵ������پ���
double BattleLayer::getDistance(ChessCoordinate* start, ChessCoordinate* end)
{
	// ��������ָ���Ƿ�Ϊnull
	if (start == nullptr || end == nullptr) 
	{
		// ����ѡ�񷵻�һ������ֵ���׳��쳣
		return -1.0; 
	}

	// ���㲢���������پ���
	return abs(start->getX() - end->getX()) + abs(start->getY() - end->getY());
}


//void Battle::pieceBattle(Chess &damageMaker, Chess &victim)
//{
	//�����˹���
	//damageMaker.attackOne(victim);
//}

// Ѱ�ҹ���Ŀ�꣬ѡ���������ĵз�Ӣ��
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

	
	// �����ӵ�״̬ת���ƶ�״̬
	damageMaker->state = Chess::State::Moving;
	// ע��ֻ�ǽ����ƶ�״̬������û��ʼ�ƶ�
	damageMaker->isMoving = false;

	return targetChess;
}

// �������ƶ���ָ��λ�ã�������Ϊ��Ļ����
void BattleLayer::moveChess(Chess* movingChess,Vec2 targetPosition)
{
	Sprite* movingChessImage = movingChess->getChessSprite();
	float duration = 0.2f; // �ƶ�����ʱ�䣬����Ϊ��λ
	auto moveTo = MoveTo::create(duration, targetPosition);
	auto callback = CallFunc::create([this, movingChess,targetPosition]() 
		{
			
		// �ƶ���ɺ�Ĵ���
		// ���磺��������״̬
			movingChess->setChessCoordinateByType(targetPosition, CoordinateType::screenCoordinates);
			ChessCoordinate* newPos;
			CoordinateConvert(CoordinateType::chessBoardCoordinates, targetPosition, newPos);
			movingChess->setChessCoordinateByType(Vec2(newPos->getY(), newPos->getX()), CoordinateType::chessBoardCoordinates);
			// �ƶ�һ����Ϻ�����isMovingΪfalse
			movingChess->isMoving = false;
		});

	auto sequence = Sequence::create(moveTo, callback, nullptr);
	movingChessImage->runAction(sequence);
}

// Ѱ·���� ÿ��ֻ��һ��
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
	
	
	// ����y����
	if (yOfdamageMaker < yOftargetChess && chessboard->isAvailable(yOfdamageMaker + 1, xOfdamageMaker))
	{
		dy = 1;
	}
	else if (yOfdamageMaker > yOftargetChess && chessboard->isAvailable(yOfdamageMaker - 1, xOfdamageMaker))
	{
		dy = -1;
	}
	// ����x����
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


// �������Ч


void BattleLayer::doAttack(Chess* damageMaker, Chess* targetEnemy)
{
	Sprite* damageMakerImage = damageMaker->getChessSprite();
	string damageMakerCareer = damageMaker->GetCareer();
	Sprite* targetChessImage = targetEnemy->getChessSprite();

	// ʹ�ü���
	if (damageMaker->attackNum == damageMaker->getChessCondition()->skillCooldown)
	{

		// ���ܿ��ӻ��߼�
		damageMaker->createSkill();
		Sprite* damageMakerSkillImage = damageMaker->getSkillSprite();
		// �Ѽ��ܵ���ͼ��ӵ���ǰ��򳡾���
		this->addChild(damageMakerSkillImage);
		// �ƶ���Ŀ�������ʱ�䣬����Ϊ��λ
		float moveDuration = 0.5f;
		// �����ƶ���Ŀ��λ��
		auto moveSkill = MoveTo::create(moveDuration, targetChessImage->getPosition());

		// �����ƶ���Ŀ��λ�û���Ŀ���Ĵ���
		auto callback = CallFunc::create([damageMaker,targetEnemy,damageMakerImage,damageMakerSkillImage]()
		{
			// �Ƴ�������ͼ
			damageMakerSkillImage->removeFromParent();
			// ��ֵ�߼�
			damageMaker->skill(*targetEnemy);
			// ���¶�������������
			targetEnemy->updateHpBar();
			// �����Լ��ļ��ܽ�����
			damageMaker->updateMpBar(0);
			// ������ͨ����������
			damageMaker->attackNum = 0;

			// �ж��Լ��Ƿ�����
			if (damageMaker->isDead())
			{
				damageMakerImage->removeFromParent();

			}
			// �Է�������ֹͣ����
			if (targetEnemy->isDead())
			{

			}

		});
		auto sequence = Sequence::create(moveSkill, callback, nullptr);
		// ִ�м��ܶ������߼�����
		damageMakerSkillImage->runAction(sequence);
	}
	// ����һ����ͨ�������������Ч
	else
	{
		Sequence* normalAttack;
		if (damageMakerCareer == "mage" || damageMakerCareer == "shooter")
		{
			// �չ����ӻ��߼�
			damageMaker->createAttack();
			Sprite* damageMakerAttackImage = damageMaker->getAttackSprite();
			// ����ͨ��������ͼ��ӵ���ǰ��򳡾���
			this->addChild(damageMakerAttackImage);
			// �ƶ���Ŀ�������ʱ�䣬����Ϊ��λ
			float moveDuration = 0.5f;
			// Զ�̹����ƶ���Ŀ��λ��
			auto moveAttack = MoveTo::create(moveDuration, targetChessImage->getPosition());
			// ���жԷ�����ʧ
			auto removeSprite = CallFunc::create([damageMakerAttackImage]()
				{
					damageMakerAttackImage->removeFromParent();
				});
			// ��϶���
			normalAttack = Sequence::create(moveAttack, removeSprite, nullptr);

			// �����ƶ���Ŀ��λ�û���Ŀ���Ĵ���
			auto callback = CallFunc::create([damageMaker, targetEnemy, damageMakerImage]()
			{
					// ��ֵ�߼�
					damageMaker->attackOne(*targetEnemy);

					// ������ͨ����������
					damageMaker->attackNum++;

					// ���¶�������������
					targetEnemy->updateHpBar();
					// �����Լ��ļ��ܽ�����
					damageMaker->updateMpBar(damageMaker->attackNum);
					// �ж��Լ��Ƿ�����
					if (damageMaker->isDead())
					{
						damageMakerImage->removeFromParent();

					}
					// �Է�������ֹͣ����
					if (targetEnemy->isDead())
					{

					}
					// ������Ϻ���������״̬Ϊ���ڹ������Ա��´�update���Ե�����һ�ε�doAttack()
					damageMaker->isAttacking = false;
			});
			auto sequence = Sequence::create(normalAttack, callback, nullptr);
			// ִ�м��ܶ������߼�����
			damageMakerAttackImage->runAction(sequence);

		}
		else
		{
			// ̹�˽�ս����ͨ����������һ���򵥵���������ʾ����
			auto attackAction = ScaleBy::create(0.2f, 1.2f); // ���ŵ�ԭ����1.2��
			auto attackActionReverse = attackAction->reverse(); // ���������Ż�ԭʼ��С

			// ��϶���
			normalAttack = Sequence::create(attackAction, attackActionReverse, nullptr);

			// �����ƶ���Ŀ��λ�û���Ŀ���Ĵ���
			auto callback = CallFunc::create([damageMaker, targetEnemy, damageMakerImage]()
			{
					// ��ֵ�߼�
					damageMaker->attackOne(*targetEnemy);

					// ������ͨ����������
					damageMaker->attackNum++;

					// ���¶�������������
					targetEnemy->updateHpBar();
					// �����Լ��ļ��ܽ�����
					damageMaker->updateMpBar(damageMaker->attackNum);
					// �ж��Լ��Ƿ�����
					if (damageMaker->isDead())
					{
						damageMakerImage->removeFromParent();

					}
					// �Է�������ֹͣ����
					if (targetEnemy->isDead())
					{

					}
					// ������Ϻ���������״̬Ϊ���ڹ������Ա��´�update���Ե�����һ�ε�doAttack()
					damageMaker->isAttacking = false;


			});
			auto sequence = Sequence::create(normalAttack, callback, nullptr);
			// ִ�м��ܶ������߼�����
			damageMakerImage->runAction(sequence);
		}
	}
}

























bool BattleLayer::isEnd()
{
	
}


