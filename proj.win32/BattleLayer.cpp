#include "BattleLayer.h"




void BattleLayer::AItest()
{
	
	

	playerOPP->putChessInBattleArea(make_shared<shooter>(HOUYI));
	playerOPP->putChessInBattleArea(make_shared<shooter>(DIRENJIE));

	ChessCoordinate* newPos = new ChessCoordinate;

	shared_ptr<Chess> daji = (*playerOPP->getBattleAreaChesses())[0];
	shared_ptr<Chess> diaochan = (*playerOPP->getBattleAreaChesses())[1];

	daji->setChessCoordinateByType(Vec2(3, 0), CoordinateType::chessBoardCoordinates);
	CoordinateConvert(CoordinateType::screenCoordinates, Vec2(3, 0), newPos);
	daji->setChessCoordinateByType(Vec2(newPos->getX(),newPos->getY()), CoordinateType::screenCoordinates);
	this->addChild(daji->createChess(Vec2 (newPos->getX(),newPos->getY())));

	diaochan->setChessCoordinateByType(Vec2(0, 0), CoordinateType::chessBoardCoordinates);
	CoordinateConvert(CoordinateType::screenCoordinates, Vec2(0, 0), newPos);
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
void BattleLayer::chessInitBeforeBattle(shared_ptr<Chess> chess,bool isME)
{
	*canBuyChess = false;
	chess->state = Chess::State::Idle;
	chess->isAttacking = false;
	chess->isMoving = false;
	chess->getChessCondition()->HP = chess->getChessCondition()->maxHP;
	chess->attackNum = 0;
	chess->inGameChessBoardCoordinate.setX(chess->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getX());
	chess->inGameChessBoardCoordinate.setY(chess->getChessCoordinateByType(CoordinateType::chessBoardCoordinates)->getY());
	chess->inGameScreenCoordinate.setX(chess->getChessCoordinateByType(CoordinateType::screenCoordinates)->getX());
	chess->inGameScreenCoordinate.setY(chess->getChessCoordinateByType(CoordinateType::screenCoordinates)->getY());
	
	if (!isME)
	{
		
		chess->createChess(Vec2(chess->inGameScreenCoordinate.getX(), chess->inGameScreenCoordinate.getY()));
		auto chessImage = chess->getChessSprite();
		double nowWidth = chessImage->getContentSize().width;
		double nowHeight = chessImage->getContentSize().height;
		// �����з���־
		auto config = ConfigController::getInstance();
		auto enemy = Sprite::create("/res/enemy.png");
		Vec2 originSize = enemy->getContentSize();
		float scaleX = 20 * config->getPx()->x / originSize.x;
		float scaleY = 20 * config->getPx()->y / originSize.y;
		enemy->setScale(scaleX, scaleY);
		enemy->setPosition(Vec2(chess->inGameScreenCoordinate.getX()+0.5*nowWidth, chess->inGameScreenCoordinate.getY()+0.8*nowHeight));
		chessImage->addChild(enemy);
		addChild(chessImage);
		
	}
	
	chess->getChessSprite()->setVisible(true);
	

	boardInGame[chess->inGameChessBoardCoordinate.getY()][chess->inGameChessBoardCoordinate.getX()] = 1;
	// ���º�����
	chess->updateHpBar();
	chess->updateMpBar();
}

inline bool BattleLayer::isAvailable(int row, int col)
{
	if (row < 0 || row >= BOARDMAXR || col < 0 || col >= BOARDMAXC)
		return false;

	return boardInGame[row][col] == 0;
}

bool BattleLayer::init()
{

	// ������Ч 
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/�ŷɴ���.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/����.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/daji.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/onlinedo-output(1).wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/onlinedo-output(2).wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/victory.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("/res/Music/defeat.wav");
	gameOver = false;

	memset(boardInGame, 0, sizeof boardInGame);

	//AItest();


	for (shared_ptr<Chess> chess : *playerME->getBattleAreaChesses())
	{
		
		chessInitBeforeBattle(chess,true);
	}

	for (shared_ptr<Chess> chess : *playerOPP->getBattleAreaChesses())
	{
		chessInitBeforeBattle(chess,false);
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
		

		// ս������
		//detectWinner();

		// ����һ�������ڵ�
		auto delayNode = Node::create();
		this->addChild(delayNode); // ��ӵ���ǰ��

		// ������ʱ����
		auto delayAction = DelayTime::create(2.0f); // 2����ʱ

		auto callbackActionForEnd = CallFunc::create([this, delayNode]() {
			detectWinner();
			// ������Ч
			auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
			audio->unloadEffect("/res/Music/�ŷɴ���.wav");
			audio->unloadEffect("/res/Music/����.wav");
			audio->unloadEffect("/res/Music/daji.wav");
			audio->unloadEffect("/res/Music/onlinedo-output (1).wav");
			audio->unloadEffect("/res/Music/onlinedo-output (2).wav");
			audio->unloadEffect("/res/Music/victory.wav");
			audio->unloadEffect("/res/Music/defeat.wav");
			

			});

		auto callbackActionForStart = CallFunc::create([this, delayNode]() {

			for (auto chess : *playerME->getBattleAreaChesses())
			{
				//chess->getChessSprite()->removeFromParent();
				auto chessImage = chess->getChessSprite();
				auto chessScreenPos = chess->getChessCoordinateByType(CoordinateType::screenCoordinates);
				chessImage->setPosition(Vec2(chessScreenPos->getX(), chessScreenPos->getY()));
				chessImage->setVisible(true);
			}
			*isInBattle = false;
			*canBuyChess = true;
			});
		

		// ������������
		auto sequence = Sequence::create(delayAction,callbackActionForEnd,callbackActionForStart, nullptr);

		// ���ж���
		delayNode->runAction(sequence);

	}
}


void BattleLayer::detectWinner()
{
	auto config = ConfigController::getInstance();

	int surviveNumMe = 0;
	int surviveNumOPP = 0;

	for (auto chess : *playerME->getBattleAreaChesses())
	{	
		if (chess->isDead())
			continue;
		surviveNumMe++;	
	}

	for (auto chess : *playerOPP->getBattleAreaChesses())
	{
		chess->getChessSprite()->removeFromParent();
		//chessImage->setVisible(false);
		if (chess->isDead())
			continue;
		surviveNumOPP++;
	}

	// ��ȡ��ǰ��ͼ�Ĵ�С��ԭ��
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (surviveNumMe > 0)
	{
		playerOPP->DecreaseLifeValue(playerME->getLevel() + surviveNumMe);
		auto victoryImage = Sprite::create("/res/Victory.png");
		// ���������м�
		victoryImage->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2));
		// ��ȡԭʼ��С
		Vec2 OriginSize = victoryImage->getContentSize();

		// ���Ŵ�С��configһ�����
		float chessScaleX = 88.9 * config->getPx()->x / OriginSize.x;
		float chessScaleY = 50 * config->getPx()->y / OriginSize.y;

		// ���ô�С
		victoryImage->setScale(chessScaleX, chessScaleY);

		// �� Label ��ӵ���ǰ����
		this->addChild(victoryImage);
		auto endAction = ScaleBy::create(0.5f, 1.5f); // ���ŵ�ԭ����1.5��
		auto endActionReverse = endAction->reverse(); // ���Ż�ԭʼ��С

		auto callback = CallFunc::create([victoryImage,this]()
			{
				victoryImage->removeFromParent();
				
			});

		// ��϶���
		Sequence* sequence = Sequence::create(endAction, endActionReverse, callback,nullptr);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/victory.wav");
		victoryImage->runAction(sequence);
		
	}
	else if (surviveNumOPP > 0)
	{
		playerME->DecreaseLifeValue(playerOPP->getLevel() + surviveNumOPP);
		auto defeatImage = Sprite::create("/res/Defeat.png");
		defeatImage->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2));

		// ��ȡԭʼ��С
		Vec2 OriginSize = defeatImage->getContentSize();

		// ���Ŵ�С��configһ�����
		float chessScaleX = 88.9 * config->getPx()->x / OriginSize.x;
		float chessScaleY = 50 * config->getPx()->y / OriginSize.y;

		// ���ô�С
		defeatImage->setScale(chessScaleX, chessScaleY);

		// �� Label ��ӵ���ǰ����
		this->addChild(defeatImage);
		auto endAction = ScaleBy::create(0.5f, 1.5f); // ���ŵ�ԭ����1.5��
		auto endActionReverse = endAction->reverse(); // ���Ż�ԭʼ��С
		auto callback = CallFunc::create([defeatImage,this]()
			{
				defeatImage->removeFromParent();
				
			});
		// ��϶���
		Sequence* sequence = Sequence::create(endAction, endActionReverse,callback, nullptr);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/defeat.wav");
		defeatImage->runAction(sequence);
	}
	else 
	{
		auto drawImage = Sprite::create("/res/Draw.png");
		drawImage->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2));

		// ��ȡԭʼ��С
		Vec2 OriginSize = drawImage->getContentSize();

		// ���Ŵ�С��configһ�����
		float chessScaleX = 88.9 * config->getPx()->x / OriginSize.x;
		float chessScaleY = 50 * config->getPx()->y / OriginSize.y;

		// ���ô�С
		drawImage->setScale(chessScaleX, chessScaleY);
		// �� Label ��ӵ���ǰ����
		this->addChild(drawImage);
		auto endAction = ScaleBy::create(0.5f, 1.5f); // ���ŵ�ԭ����1.5��
		auto endActionReverse = endAction->reverse(); // ���Ż�ԭʼ��С
		auto callback = CallFunc::create([drawImage,this]()
			{
				drawImage->removeFromParent();
				
			});
		// ��϶���
		Sequence* sequence = Sequence::create(endAction, endActionReverse,callback, nullptr);
		drawImage->runAction(sequence);
	}

}

// ��װӢ�۽���ս����ȫ���̡���Ѱ�ҡ�Ѱ·������
void BattleLayer::playGame(shared_ptr<Chess> chess,PlayerInfo* opp)
{
	// ��ȷ������Ŀ�꣬ȷ��Ŀ��һ֡��ʱ��Ӧ���ǹ��ģ�findEnemy()����ִ�к󣬽����ӵ�״̬��ΪMoving
	if (chess->state == Chess::State::Idle)
	{
		shared_ptr<Chess> targetChess = findEnemy(chess, opp);
		// û�й���Ŀ�꣬��Ϸ������
		if (targetChess == nullptr )
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
	double distance = 9999;

	shared_ptr<Chess> targetChess = nullptr;

	for (auto chess : *enemy->getBattleAreaChesses())
	{
		if (chess->isDead())
			continue;

		double tmp = getDistance(&damageMaker->inGameChessBoardCoordinate,
			(&chess->inGameChessBoardCoordinate));

		if (tmp <= distance)
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
	

	// ��ʼ�ƶ�
	Sprite* movingChessImage = movingChess->getChessSprite();
	float duration = 0.5f; // �ƶ�����ʱ�䣬����Ϊ��λ
	auto moveTo = MoveTo::create(duration, targetPosition);
	auto callback = CallFunc::create([this, movingChess,targetPosition]() 
		{
			
			// �ƶ���ɺ�Ĵ���
			// �ƶ���ɲŸ�����������Ϸ�е�����
			// �ƶ�ǰ�ȸ����������꣬��ռλ�ã�����λ�ó�ͻ
			
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
		boardInGame[yOfdamageMaker][xOfdamageMaker] = 0;
		boardInGame[yOfdamageMaker + 1][xOfdamageMaker] = 1;
		dy = 1;
	}
	else if (yOfdamageMaker > yOftargetChess && isAvailable(yOfdamageMaker - 1, xOfdamageMaker))
	{
		boardInGame[yOfdamageMaker][xOfdamageMaker] = 0;
		boardInGame[yOfdamageMaker - 1][xOfdamageMaker] = 1;
		dy = -1;
	}
	// ����x����
	else if (xOfdamageMaker < xOftargetChess && isAvailable(yOfdamageMaker, xOfdamageMaker + 1))
	{
		boardInGame[yOfdamageMaker][xOfdamageMaker] = 0;
		boardInGame[yOfdamageMaker][xOfdamageMaker + 1] = 1;
		dx = 1;
	}
	else if (xOfdamageMaker > xOftargetChess && isAvailable(yOfdamageMaker, xOfdamageMaker - 1))
	{
		boardInGame[yOfdamageMaker][xOfdamageMaker] = 0;
		boardInGame[yOfdamageMaker][xOfdamageMaker - 1] = 1;
		dx = -1;
	}

	
	CoordinateConvert(CoordinateType::screenCoordinates, Vec2(int(xOfdamageMaker + dx),int(yOfdamageMaker + dy)), newPos);

	damageMaker->inGameChessBoardCoordinate.setX(int(xOfdamageMaker + dx));
	damageMaker->inGameChessBoardCoordinate.setY(int(yOfdamageMaker + dy));
	damageMaker->targetPos = Vec2(newPos->getX(),newPos->getY());
	delete newPos;
	moveChess(damageMaker, damageMaker->targetPos);
	
}


// ����һ�ι������������Ч
void BattleLayer::doAttack(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy)
{
	Sprite* damageMakerImage = damageMaker->getChessSprite();
	string damageMakerCareer = damageMaker->getCareer();
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
		Vec2 OriginSize = damageMakerSkillImage->getContentSize();
		// ���Ŵ�С��configһ�����
		auto config = ConfigController::getInstance();
		float ScaleX = 8 * config->getPx()->x / OriginSize.x;
		float ScaleY = OriginSize.y * 8 * config->getPx()->x / (OriginSize.y * OriginSize.x);
		damageMakerSkillImage->setScale(ScaleX, ScaleY);
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
			if (damageMaker->getChessName() == ZHANGFEI)
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/�ŷɴ���.wav");
			else if (damageMaker->getChessName() == DAJI)
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/daji.wav");
			else
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/onlinedo-output (1).wav");
		
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
			Vec2 OriginSize = damageMakerAttackImage->getContentSize();
			// ���Ŵ�С��configһ�����
			auto config = ConfigController::getInstance();
			float ScaleX = 8 * config->getPx()->x / OriginSize.x;
			float ScaleY = OriginSize.y * 8 * config->getPx()->x / (OriginSize.y * OriginSize.x);
			damageMakerAttackImage->setScale(ScaleX, ScaleY);
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
			if (damageMaker->getChessName() == HOUYI)
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/����.wav");
			else
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/onlinedo-output (2).wav");
			
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
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("/res/Music/onlinedo-output (2).wav");
		}
	}
}
