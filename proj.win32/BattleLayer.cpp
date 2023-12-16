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
	// ������Ϸ״̬
	// ���磺��ɫ�ƶ���Ѱ�ҹ���Ŀ�ִ꣬�й�����
	// ...

	// ��������������ķ���
	// ���磺findAttackTargets(), moveCharacters(), performAttacks() ��
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
Chess* BattleLayer::findEnemy(Chess* damageMaker, PlayerInfo enemy)
{
	double distance = 999;
	Chess* targetEnemyChess;

	for (int i = 0; i < 7; i++) 
	{
		if (enemy.chessInBattleArea[i] == nullptr) 
		{//û������
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

// �������ƶ���ָ��λ�ã�������Ϊ��Ļ����
void BattleLayer::moveChess(Sprite* movingChess,Vec2 targetPosition)
{
	float duration = 0.5f; // �ƶ�����ʱ�䣬����Ϊ��λ
	auto moveTo = MoveTo::create(duration, targetPosition);
	movingChess->runAction(moveTo);
	
}

// Ѱ·���� ѭ�����ã�ֻ��Ҫ֪����һ����ô�߼���
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

	coordinateConvert(CoordinateType::screenCoordinates, { xOfdamageMaker+dx,yOfdamageMaker+dy }, newPos);
	moveChess(damageMakerImage, Vec2{ newPos->getX(),newPos->getY() });
}


// �������Ч
void BattleLayer::doAttack(Chess* damageMaker, Chess* targetEnemy)
{
	Sprite* damageMakerImage = damageMaker->getChessSprite();
	string damageMakerCareer = damageMaker->GetCareer();
	Sprite* targetChessImage = targetEnemy->getChessSprite();

	// �չ�������
	int attackNum = 0;

	while (true)
	{
		// ʹ�ü���
		if (attackNum == damageMaker->getChessCondition()->skillCooldown)
		{

			// ���ܿ��ӻ��߼�
			damageMaker->createSkill();
			Sprite* damageMakerSkillImage = damageMaker->getSkillSprite();
			// �Ѽ��ܵ���ͼ��ӵ���ǰ��򳡾���
			this->addChild(damageMakerSkillImage);
			// �ƶ���Ŀ�������ʱ�䣬����Ϊ��λ
			float moveDuration = 0.5f;
			// �����ƶ���Ŀ��λ��
			auto moveAction = MoveTo::create(moveDuration, targetChessImage->getPosition());
			// ���жԷ�����ʧ
			auto removeSprite = CallFunc::create([damageMakerSkillImage]()
				{
					damageMakerSkillImage->removeFromParent();
				});
			
			// ��ֵ�߼�
			damageMaker->skill(*targetEnemy);
			// ���¶�������������
			targetEnemy->updateHpBar();
			// �����Լ��ļ��ܽ�����
			damageMaker->updateMpBar(0);
			attackNum = 0;
			// �ж��Լ��Ƿ�����
			if (damageMaker->isDead())
			{
				damageMakerImage->removeFromParent();
				break;
			}
			// �Է�������ֹͣ����
			if (targetEnemy->isDead())
			{
				break;
			}
		}

		// ����һ���չ� �������Ч
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
			auto moveAction = MoveTo::create(moveDuration, targetChessImage->getPosition());
			// ���жԷ�����ʧ
			auto removeSprite = CallFunc::create([damageMakerAttackImage]() 
				{
					damageMakerAttackImage->removeFromParent();
				});
		}
		else
		{
			// ̹�˽�ս����ͨ����������һ���򵥵���������ʾ����
			auto attackAction = ScaleBy::create(0.2f, 1.2f); // ���ŵ�ԭ����1.2��
			auto attackActionReverse = attackAction->reverse(); // ���������Ż�ԭʼ��С

			// ��϶���
			auto attackSequence = Sequence::create(attackAction, attackActionReverse, nullptr);
			// �ù�����ִ�������������
			damageMakerImage->runAction(attackSequence);
		}
		
		// ��ֵ�߼�
		damageMaker->attackOne(*targetEnemy);
		++attackNum;
		// ���¶�������������
		targetEnemy->updateHpBar();
		// �����Լ��ļ��ܽ�����
		damageMaker->updateMpBar(attackNum);
		// �ж��Լ��Ƿ�����
		if (damageMaker->isDead())
		{
			damageMakerImage->removeFromParent();
			break;
		}
		// �Է�������ֹͣ����
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
		if (A.BattleChessNum[i] == nullptr) {//û������
			break;
		}
		else {
			if (A.BattleChessNum[i]->isDead()) {//��������
				if (over == -1) {
					over = 1;
				}
			}
			else {
				over = 0;
			}
		}
	}
	if (over != 0) {//������
		return true;
	}
	else {
		return false;
	}
}


