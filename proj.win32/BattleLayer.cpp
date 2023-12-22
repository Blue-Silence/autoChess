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

// ս��ǰ��ʼ�����ӵ�ս������
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
	
	// ��������update()��������ʼս��
	this->scheduleUpdate();


	return true;
}

// 
void BattleLayer::update(float delta) 
{
	// �ҷ�
	for (auto chess : *playerME->getBattleAreaChesses())
	{
		if (chess->isDead())
			continue;
		
		playGame(chess,playerOPP);
	}
	// �Է�
	for (auto chess : *playerOPP->getBattleAreaChesses())
	{
		if (chess->isDead())
			continue;
		playGame(chess, playerME);
	}

	if(gameOver)
	{
		// ȡ����update�����ĵ���
		this->unscheduleUpdate();
		
	}
}


void BattleLayer::detectWinner()
{
	
}

// ��װӢ�۽���ս����ȫ���̡���Ѱ�ҡ�Ѱ·������
void BattleLayer::playGame(shared_ptr<Chess> chess,PlayerInfo* opp)
{
	// ��ȷ������Ŀ�꣬ȷ��Ŀ��һ֡��ʱ��Ӧ���ǹ��ģ�findEnemy()����ִ�к󣬽����ӵ�״̬��ΪMoving
	if (chess->state == Chess::State::Idle)
	{
		shared_ptr<Chess> targetChess = findEnemy(chess, opp);
		// û�й���Ŀ�꣬��Ϸ������
		if (targetChess == nullptr || gameOver)
		{
			gameOver = true;
		}
		chess->targetChess = targetChess;
	}
	// ���Ӵ����ƶ�״̬��û�����ƶ�ʱ��Ҫ����
	else if (chess->state == Chess::State::Moving && !chess->isMoving)
	{
		// ���빥����Χ�ˣ�����Ҫ���ƶ��ˣ���������״̬Ϊ����
		if (getDistance(&chess->inGameChessBoardCoordinate,
			&chess->targetChess->inGameChessBoardCoordinate)
			< (chess->getChessCondition()->improvedAttackDistance+1))
		{
			chess->isMoving = false;
			chess->state = Chess::State::Attacking;
			// ע�⣬��Ȼ���빥��״̬������û�п�ʼ����
			chess->isAttacking = false;
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


// ��ȡ��������֮���������ϵ������پ���
double BattleLayer::getDistance(ChessCoordinate* start,ChessCoordinate* end)
{
	// ��������ָ���Ƿ�Ϊnull
	if (start == nullptr || end == nullptr) 
	{
		// ����ѡ�񷵻�һ������ֵ���׳��쳣
		return -1.0; 
	}
	int startX = start->getX();
	int startY = start->getY();

	int endX = end->getX();
	int endY = end->getY();

	// ���㲢���������پ���
	return abs(startX-endX) + abs(startY-endY);
}



// Ѱ�ҹ���Ŀ�꣬ѡ���������ĵз�Ӣ��
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

	// ˵���Է��Ѿ�û�д��������ˣ���Ϸ����
	if (targetChess == nullptr)
	{
		return nullptr;
	}

	// �����ӵ�״̬ת���ƶ�״̬
	damageMaker->state = Chess::State::Moving;
	// ע��ֻ�ǽ����ƶ�״̬������û��ʼ�ƶ�
	damageMaker->isMoving = false;

	return targetChess;
}

// �������ƶ���ָ��λ�ã�������Ϊ��Ļ����
void BattleLayer::moveChess(shared_ptr<Chess> movingChess,Vec2 targetPosition)
{
	// �ƶ�ǰ�ȸ����������꣬��ռλ�ã�����λ�ó�ͻ
	ChessCoordinate* newPos = new ChessCoordinate;
	CoordinateConvert(CoordinateType::chessBoardCoordinates, targetPosition, newPos);
	int oldRow = movingChess->inGameChessBoardCoordinate.getY();
	int oldCol = movingChess->inGameChessBoardCoordinate.getX();
	boardInGame[oldRow][oldCol] = 0;
	movingChess->inGameChessBoardCoordinate.setX(newPos->getX());
	movingChess->inGameChessBoardCoordinate.setY(newPos->getY());
	boardInGame[newPos->getY()][newPos->getX()] = 1;
	delete newPos;

	// ��ʼ�ƶ�
	Sprite* movingChessImage = movingChess->getChessSprite();
	float duration = 0.5f; // �ƶ�����ʱ�䣬����Ϊ��λ
	auto moveTo = MoveTo::create(duration, targetPosition);
	auto callback = CallFunc::create([this, movingChess,targetPosition]() 
		{
			
			// �ƶ���ɺ�Ĵ���
			// �ƶ���ɲŸ�����������Ϸ�е���Ļ����
			movingChess->inGameScreenCoordinate.setX(int(targetPosition.x));
			movingChess->inGameScreenCoordinate.setY(int(targetPosition.y));
			
			// �ƶ�һ����Ϻ�����isMovingΪfalse
			movingChess->isMoving = false;
		});

	auto sequence = Sequence::create(moveTo, callback, nullptr);
	movingChessImage->runAction(sequence);
}

// Ѱ·���� ÿ��ֻ��һ��
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
	
	
	// ����y����
	if (yOfdamageMaker < yOftargetChess && isAvailable(yOfdamageMaker + 1, xOfdamageMaker))
	{
		dy = 1;
	}
	else if (yOfdamageMaker > yOftargetChess && isAvailable(yOfdamageMaker - 1, xOfdamageMaker))
	{
		dy = -1;
	}
	// ����x����
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


// ����һ�ι������������Ч
void BattleLayer::doAttack(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy)
{
	Sprite* damageMakerImage = damageMaker->getChessSprite();
	string damageMakerCareer = damageMaker->GetCareer();
	Sprite* targetChessImage = targetEnemy->getChessSprite();
	
	int targetRow = targetEnemy->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getY();
	int targetCol = targetEnemy->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getX();


	// ʹ�ü���
	if (damageMaker->attackNum == (damageMaker->getChessCondition()->skillCooldown))
	{

		// ���ܿ��ӻ��߼�
		damageMaker->createSkill();
		Sprite* damageMakerSkillImage = damageMaker->getSkillSprite();

		// ��������ͼ������ת
		Vec2 currentPosition = damageMakerSkillImage->getPosition();
		Vec2 targetPosition = targetChessImage->getPosition();
		// ����Ŀ�귽�������
		Vec2 direction = targetPosition - currentPosition;
		// ����Ƕȣ�����ʹ�� atan2 ����
		float angleRadians = atan2(direction.y, direction.x);
		float angleDegrees = CC_RADIANS_TO_DEGREES(-angleRadians);
		// ���ü�����ͼ�ĳ�ʼ��ת�Ƕ�
		damageMakerSkillImage->setRotation(angleDegrees);

		// �Ѽ��ܵ���ͼ��ӵ���ǰ��򳡾���
		this->addChild(damageMakerSkillImage);
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
				// ��ֵ�߼�
				damageMaker->skill(*targetEnemy);
				// ���¶�������������
				targetEnemy->updateHpBar();
				// ������ͨ����������
				damageMaker->attackNum = 0;
				// �����Լ��ļ��ܽ�����
				damageMaker->updateMpBar();

				// �ж��Լ��Ƿ�����
				if (damageMaker->isDead())
				{

					damageMaker->state = Chess::State::Dead;

				}

				// �Է�������ֹͣ����,���¿�ʼѰ�ҹ���Ŀ��
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

			// ��϶���
			auto sequence = Sequence::create(skillAttack, callback, nullptr);
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
			// ��Զ�̹�����ͼ������ת
			Vec2 currentPosition = damageMakerAttackImage->getPosition();
			Vec2 targetPosition = targetChessImage->getPosition();
			// ����Ŀ�귽�������
			Vec2 direction = targetPosition - currentPosition;

			// ����Ƕȣ�����ʹ�� atan2 ����
			float angleRadians = atan2(direction.y, direction.x);
			float angleDegrees = CC_RADIANS_TO_DEGREES(-angleRadians);
			// ���ü�����ͼ�ĳ�ʼ��ת�Ƕ�
			damageMakerAttackImage->setRotation(angleDegrees);

			// ����ͨ��������ͼ��ӵ���ǰ��򳡾���
			this->addChild(damageMakerAttackImage);
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
					// ��ֵ�߼�
					damageMaker->attackOne(*targetEnemy);
					// ������ͨ����������
					damageMaker->attackNum += 1;
					// ���¶�������������
					targetEnemy->updateHpBar();
					// �����Լ��ļ��ܽ�����
					damageMaker->updateMpBar();
					// �ж��Լ��Ƿ�����
					if (damageMaker->isDead())
					{
						damageMaker->state = Chess::State::Dead;
					}
					// �Է�������ֹͣ����,���¿�ʼѰ�ҹ���Ŀ��
					if (targetEnemy->isDead())
					{
						targetChessImage->setVisible(false);
						boardInGame[targetRow][targetCol] = 0;
						//targetChessImage->removeFromParent();

						damageMaker->state = Chess::State::Idle;
						damageMaker->isMoving = false;

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
			auto attackAction = ScaleBy::create(0.5f, 2.0f); // ���ŵ�ԭ����1.2��
			auto attackActionReverse = attackAction->reverse(); // ���������Ż�ԭʼ��С

			// ��϶���
			normalAttack = Sequence::create(attackAction, attackActionReverse, nullptr);

			// �����ƶ���Ŀ��λ�û���Ŀ���Ĵ���
			auto callback = CallFunc::create([this, targetRow, targetCol, damageMaker, targetEnemy, damageMakerImage,targetChessImage]()
				{
					// ��ֵ�߼�
					damageMaker->attackOne(*targetEnemy);

					// ������ͨ����������
					damageMaker->attackNum++;
					// ���¶�������������
					targetEnemy->updateHpBar();
					// �����Լ��ļ��ܽ�����
					damageMaker->updateMpBar();
					// �ж��Լ��Ƿ�����
					if (damageMaker->isDead())
					{
						
						damageMaker->state = Chess::State::Dead;
						damageMaker->isAttacking = false;

					}
					// �Է�������ֹͣ����,���¿�ʼѰ�ҹ���Ŀ��
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
			// ִ�м��ܶ������߼�����
			damageMakerImage->runAction(sequence);
		}
	}
}
