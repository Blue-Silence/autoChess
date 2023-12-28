/********************************************
 * ���ܣ�����ģ��cpp�ļ�
 * ���ߣ�֣ΰة
 * �����޸����ڣ�12.09
 ********************************************/

#include "Chess.h"

// ChessCoordinate��ʵ��
int ChessCoordinate::getX() const
{
	return x;
}

int ChessCoordinate::getY() const
{
	return y;
}

void ChessCoordinate::setX(const int X) 
{
	
	x = X;
	return;
}

void ChessCoordinate::setY(const int Y)
{
	
	y = Y;
	return;
}


// Chess��ʵ��

void Chess::initCondition()
{
	chessCondition.maxHP = chessCondition.HP = hpData[chessName];
	chessCondition.basicAttack = chessCondition.improvedAttack = attackData[chessName];
	chessCondition.basicDefence = chessCondition.improvedDefence = defenceData[chessName];
	chessCondition.basicAttackDistance = chessCondition.improvedAttackDistance = attackDistanceData[chessName];
	chessCondition.skillCooldown = skillCooldowns[chessName];
}

string Chess::getCareer()
{
	return career;
 }

const string Chess::getChessImagePath()
{
	return chessImagePath;
}

const int Chess::getChessName()
{
	return chessName;
}

ChessInfo* Chess::getChessCondition()
{
	return &chessCondition;
}

const int Chess::getChessLevel()
{
	return chessLevel;
}

//-----------------------------------------------------//
//�����������������                                   //
//�������ܣ�����������𣬻�ȡ��Ӧ���͵����꣬�������� //
//          chessBoardCoordinates �� ��Ļ����          //
//��������ֵ������                                     //
//-----------------------------------------------------//
ChessCoordinate* Chess::getChessCoordinateByType(CoordinateType type)
{
	if (type == CoordinateType::chessBoardCoordinates)
	{
		return &chessBoardCoordinate;
	}
	else
	{
		return &screenCoordinate;
	}
}


void Chess::setChessLevel(const int newLevel)
{
	if (newLevel > 3)
		return;
	chessLevel = newLevel;
}

// ���������Ǽ������������������������ͬʱ���ӵ����Ի��������
void Chess::promoteRank(int dstRank)
{
	if (dstRank<=3)
	{
		chessLevel = dstRank;

		// ÿ��һ�����������Ը�������
		chessCondition.HP = chessCondition.maxHP = hpData[chessName]*pow(promoteScale, dstRank);
		chessCondition.improvedAttack = chessCondition.basicAttack = attackData[chessName] * pow(promoteScale, dstRank);
		chessCondition.improvedDefence = chessCondition.basicDefence = defenceData[chessName] * pow(promoteScale, dstRank);
		chessCondition.improvedAttackDistance = chessCondition.basicAttackDistance = attackDistanceData[chessName] * pow(promoteScale, dstRank);
		

	}
	return;
}



//�������������ع���ֵ,��Ϊ����Ŀ��ı���������������
int Chess::myAttack()
{
	return chessCondition.improvedAttack;
}

//����������,����ĵ�ǰ����ֵ�۳������˺���������ķ���ֵ��һ��Ĳ�ֵ
void Chess::beenAttack(int oppAttack)
{
	int damage = oppAttack - 0.5 * chessCondition.basicDefence;
	if (damage > 0)
		if (chessCondition.HP > damage)
			chessCondition.HP -= damage;
		else
			chessCondition.HP = 0;
}


// ִ��һ�ι�������������
void Chess::attackOne(Chess& OPP)
{
	OPP.beenAttack(myAttack());
}

// �ж������Ƿ�����
bool Chess::isDead()
{
	return chessCondition.HP == 0;
}


// ���ӻ�����
Sprite* Chess::createChess(Vec2 chessPosition)
{
	// �������ӵ�λ��
	setChessCoordinateByType(chessPosition, CoordinateType::screenCoordinates);

	
	auto config = ConfigController::getInstance();

	//CsvParser csv;
	//csv.parseWithFile("Data/PiecesData.csv");

	// ���������������
	std::string imageHeroPath = getChessImagePath();
	int pos = imageHeroPath.find(".png");
	imageHeroPath.replace(pos, 11, "Hero.png");
	chessImage = Sprite::create(imageHeroPath);

	// ����������
	auto hpBar = Sprite::create("/res/UI/HpBar.png");
	hpBarProgress = ProgressTimer::create(hpBar);
	hpBarProgress->setType(ProgressTimer::Type::BAR);

	// ���ý������ķ��򣬴�����
	hpBarProgress->setMidpoint(Vec2(0.0f, 0.5f));
	hpBarProgress->setBarChangeRate(Vec2(1.0f, 0.0f));
	// ���ý������ĳ�ʼ�ٷֱ�
	hpBarProgress->setPercentage(100); // 100% ��ʾ��ȫ���

	// ����������
	auto mpBar = Sprite::create("/res/UI/MpBar.png");
	mpBarProgress = ProgressTimer::create(mpBar);
	mpBarProgress->setType(ProgressTimer::Type::BAR);

	// ���ý������ķ��򣬴�����
	mpBarProgress->setMidpoint(Vec2(0.0f, 0.5f));
	mpBarProgress->setBarChangeRate(Vec2(1.0f, 0.0f));

	// ���ý������ĳ�ʼ�ٷֱ�
	mpBarProgress->setPercentage(0); // ��ʼ����������Ϊ0


	// ��ȡ���������ԭʼ��С
	Vec2 chessOriginSize = chessImage->getContentSize();
	Vec2 hpBarOriginSize = hpBarProgress->getContentSize();
	Vec2 mpBarOriginSize = mpBarProgress->getContentSize();


	
	// ���Ŵ�С��configһ�����
	float chessScaleX = 6 * config->getPx()->x / chessOriginSize.x;
	float chessScaleY = 7.5 * config->getPx()->y / chessOriginSize.y;

	
	
	float hpBarScaleX = 10 * config->getPx()->x / hpBarOriginSize.x;
	float hpBarScaleY = 1 * config->getPx()->y / hpBarOriginSize.y;
	float mpBarScaleX = 10 * config->getPx()->x / mpBarOriginSize.x;
	float mpBarScaleY = 1 * config->getPx()->y / mpBarOriginSize.y;
	
	// �������������С
	chessImage->setScale(chessScaleX,chessScaleY);

	// �������ţ���ת��ê��
	chessImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	hpBarProgress->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	mpBarProgress->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	hpBarProgress->setScale(hpBarScaleX /chessScaleX , hpBarScaleY /chessScaleX);
	mpBarProgress->setScale(mpBarScaleX / chessScaleX, mpBarScaleY / chessScaleX);

	double nowWidth = chessImage->getContentSize().width;
	double nowHeight = chessImage->getContentSize().height;
	

	chessImage->setPosition(chessPosition);
	hpBarProgress->setPosition(chessPosition.x - 0.5 * nowWidth, chessPosition.y + 0.60 * nowHeight);
	mpBarProgress->setPosition(chessPosition.x - 0.5 * nowWidth, chessPosition.y + 0.80 * nowHeight);

	chessImage->addChild(hpBarProgress);
	chessImage->addChild(mpBarProgress);

	

	return chessImage;
}

//���ص�ǰ����ָ��,����ͨ����ָ�������ɶ���Ч��
Sprite* Chess::getChessSprite()
{
	return chessImage;
}

Sprite* Chess::getAttackSprite()
{
	return attackImage;
}

Sprite* Chess::getSkillSprite()
{
	return skillImage;
}


//������Ҫ�����趨���ӵ���Ļ�������������
void Chess::setChessCoordinateByType(Vec2 position, CoordinateType type)
{
	if (type == CoordinateType::chessBoardCoordinates)
	{
		chessBoardCoordinate.setX(position.x);
		chessBoardCoordinate.setY(position.y);
	}
	else
	{
		screenCoordinate.setX(position.x);
		screenCoordinate.setY(position.y);
	}
}

// ��������������
void Chess::updateHpBar()
{
	hpBarProgress->setPercentage(chessCondition.HP / chessCondition.maxHP * 100);
}

// ���¼���������
void Chess::updateMpBar()
{
	mpBarProgress->setPercentage(attackNum / chessCondition.skillCooldown * 100);
}


Chess& Chess::operator=(const Chess& other) 
{
	if (this == &other) 
	{
		return *this; // ����Ը�ֵ
	}

	// ���ƻ�������
	chessName = other.chessName;
	career = other.career;
	chessImagePath = other.chessImagePath;
	chessLevel = other.chessLevel;
	chessPrice = other.chessPrice;

	chessCondition.HP = other.chessCondition.HP; 
	chessCondition.maxHP = other.chessCondition.maxHP;
	chessCondition.basicAttack = other.chessCondition.basicAttack;
	chessCondition.improvedAttack = other.chessCondition.improvedAttack;
	chessCondition.basicDefence = other.chessCondition.basicDefence;
	chessCondition.improvedDefence = other.chessCondition.improvedDefence;
	chessCondition.basicAttackDistance = other.chessCondition.basicAttackDistance;
	chessCondition.improvedAttackDistance = other.chessCondition.improvedAttackDistance;
	chessCondition.skillCooldown = other.chessCondition.skillCooldown; 


	skillScale = other.skillScale;

	chessBoardCoordinate.setX(other.chessBoardCoordinate.getX());
	chessBoardCoordinate.setY(other.chessBoardCoordinate.getY());

	screenCoordinate.setX(other.screenCoordinate.getX());
	screenCoordinate.setY(other.screenCoordinate.getY());



	// ����ָ�룬ʵ�ֹ��� Sprite
	chessImage = other.chessImage;
	attackImage = other.attackImage;
	skillImage = other.skillImage;
	hpBarProgress = other.hpBarProgress;
	mpBarProgress = other.mpBarProgress;

	return *this;
}


// �����Ǿ��嵽ְҵ����������

//----------------- ��ʦ��
// ��ʼ��
mage::mage(int name)
{
	chessName = name;
	career = "mage";
	chessImagePath = chessImagePaths[name];


	initCondition();
}

// ��Ŀ��ʹ�ü���
void mage::skill(Chess& OPP)
{
	// ���ܿ��ӻ�
	// 
	// ���ܿ��ӻ�

	OPP.beenAttack(skillScale * myAttack());
}

// �Ч������
void mage::careerBuff()
{
	// �����˺�Ч������
	skillScale = 3;
}

void mage::removeCareerBuff()
{
	skillScale = 2;
}

Sprite* mage::createAttack()
{
	auto texture = Director::getInstance()->getTextureCache();

	// ȡ·���е�ͼƬ������ͨ����������Ҳд����
	if(chessName==DAJI)
		attackImage = Sprite::create("/res/Effect/DAJI1.png");
	else
		attackImage = Sprite::create("/res/Effect/DIAOCHAN1.png");

	// �����ٵ���
	// ���Ը�����Ҫ���ù��������λ�ú�����
	// ����������ǽ��������������ӵ��Ա�
	Vec2 attackPosition = chessImage->getPosition() ; // �������Ҳ�100����
	attackImage->setPosition(attackPosition);

	// ���������������ԣ��������š���ת��
	attackImage->setScale(0.1); // ʾ������
	return attackImage;
}

Sprite* mage::createSkill()
{
	auto texture = Director::getInstance()->getTextureCache();

	// ȡ·���е�ͼƬ������ͨ����������Ҳд����
	if (chessName == DAJI)
		skillImage = Sprite::create("/res/Effect/DAJI.png");
	else
		skillImage = Sprite::create("/res/Effect/DIAOCHAN.png");

	// �����ٵ���
	// ���Ը�����Ҫ���ù��������λ�ú�����
	// ����������ǽ��������������ӵ��Ա�
	Vec2 attackPosition = chessImage->getPosition(); // �������Ҳ�100����

	skillImage->setPosition(attackPosition);

	// ���������������ԣ��������š���ת��
	skillImage->setScale(0.1); // ʾ������
	return skillImage;
}


//----------------- ��ʦ��


//----------------- ������
// ��ʼ��
shooter::shooter(int name)
{
	chessName = name;
	career = "shooter";
	chessImagePath = chessImagePaths[name];
	initCondition();
}
// ��Ŀ��ʹ�ü���
void shooter::skill(Chess& OPP)
{
	// ���ܿ��ӻ�
	// 
	// ���ܿ��ӻ�

	OPP.beenAttack(skillScale * myAttack());
}

// �Ч������
void shooter::careerBuff()
{
	// ���й�������
	chessCondition.improvedAttack *= 1.2;
}

void shooter::removeCareerBuff()
{
	chessCondition.improvedAttack /= 1.2;
}

Sprite* shooter::createAttack()
{
	auto texture = Director::getInstance()->getTextureCache();

	// ȡ·���е�ͼƬ������ͨ����������Ҳд����
	if (chessName == HOUYI)
		attackImage = Sprite::create("/res/Effect/HOUYI1.png");
	else
		attackImage = Sprite::create("/res/Effect/DIRENJIE1.png");

	// �����ٵ���
	// ���Ը�����Ҫ���ù��������λ�ú�����
	// ����������ǽ��������������ӵ��Ա�
	Vec2 attackPosition = chessImage->getPosition(); // �������Ҳ�100����
	attackImage->setPosition(attackPosition);

	// ���������������ԣ��������š���ת��
	attackImage->setScale(0.05); // ʾ������
	return attackImage;
}

Sprite* shooter::createSkill()
{
	auto texture = Director::getInstance()->getTextureCache();

	// ȡ·���е�ͼƬ������ͨ����������Ҳд����
	if (chessName == HOUYI)
		skillImage = Sprite::create("/res/Effect/HOUYI.png");
	else
		skillImage = Sprite::create("/res/Effect/DIRENJIE.png");

	// �����ٵ���
	// ���Ը�����Ҫ���ù��������λ�ú�����
	// ����������ǽ��������������ӵ��Ա�
	Vec2 attackPosition = chessImage->getPosition() ; // �������Ҳ�100����
	skillImage->setPosition(attackPosition);

	// ���������������ԣ��������š���ת��
	skillImage->setScale(0.1); // ʾ������
	return skillImage;
}

//----------------- ������


//----------------- ̹����
tank::tank(int name)
{
	chessName = name;
	career = "tank";
	chessImagePath = chessImagePaths[name];
	initCondition();
}
// ��Ŀ��ʹ�ü���
void tank::skill(Chess& OPP)
{
	// ���ܿ��ӻ�
	// 
	// ���ܿ��ӻ�

	OPP.beenAttack(skillScale * myAttack());
}

// �Ч������
void tank::careerBuff()
{
	// ���з���ֵ����
	chessCondition.improvedDefence *= 1.2;
}

void tank::removeCareerBuff()
{
	chessCondition.improvedDefence /= 1.2;
}


Sprite* tank::createAttack()
{
	auto texture = Director::getInstance()->getTextureCache();

	// ȡ·���е�ͼƬ������ͨ����������Ҳд����
	if (chessName == XIANGYU)
		attackImage = Sprite::create("/res/Attack/tank/normal/XIANGYU");
	else
		attackImage = Sprite::create("/res/Attack/tank/normal/ZHNAGFEI");

	// �����ٵ���
	// ���Ը�����Ҫ���ù��������λ�ú�����
	// ����������ǽ��������������ӵ��Ա�
	Vec2 attackPosition = chessImage->getPosition(); // �������Ҳ�100����
	attackImage->setPosition(attackPosition);

	// ���������������ԣ��������š���ת��
	attackImage->setScale(1.0); // ʾ������
	return attackImage;
}

Sprite* tank::createSkill()
{
	auto texture = Director::getInstance()->getTextureCache();

	// ȡ·���е�ͼƬ������ͨ����������Ҳд����
	if (chessName == XIANGYU)
		skillImage = Sprite::create("/res/Effect/XIANGYU.png");
	else
		skillImage = Sprite::create("/res/Effect/ZHANGFEI.png");

	// �����ٵ���
	// ���Ը�����Ҫ���ù��������λ�ú�����
	// ����������ǽ��������������ӵ��Ա�
	Vec2 attackPosition = chessImage->getPosition();// �������Ҳ�100����
	skillImage->setPosition(attackPosition);

	// ���������������ԣ��������š���ת��
	skillImage->setScale(1.0); // ʾ������
	return skillImage;
}
//----------------- ̹����

