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
	chessCondition.basicDefence = chessCondition.imporovedDefence = defenceData[chessName];
	chessCondition.basicAttackDistance = chessCondition.improvedAttackDistance = attackDistanceData[chessName];
}

string Chess::GetCareer()
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

const ChessInfo* Chess::getChessCondition()
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
void Chess::promoteRank()
{
	if (chessLevel<3)
	{
		chessLevel++;

		// ÿ��һ�����������Ը�������
		chessCondition.maxHP *= promoteScale;
		chessCondition.basicAttack *= promoteScale;
		chessCondition.basicDefence *= promoteScale;
		chessCondition.basicAttackDistance *= promoteScale;

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


// ִ��һ�ι���
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

	auto texture = Director::getInstance()->getTextureCache();
	auto config = ConfigController::getInstance();

	//CsvParser csv;
	//csv.parseWithFile("Data/PiecesData.csv");


	// ���������������
	chessImage = Sprite::createWithTexture(texture->getTextureForKey(chessImagePath));
	auto hpBar = Sprite::createWithTexture(texture->getTextureForKey("/res/UI/HpBar.png"));//������
	auto mpBar = Sprite::createWithTexture(texture->getTextureForKey("/res/UI/MpBar.png"));//����


	// ��ȡ���������ԭʼ��С
	Vec2 chessOriginSize = chessImage->getContentSize();
	Vec2 hpBarOriginSize = hpBar->getContentSize();
	Vec2 mpBarOriginSize = mpBar->getContentSize();


	// ��ô���ź����ڵ���
	// ���Ŵ�С��configһ�����
	float chessScale = 4 * config->getPx()->x / chessOriginSize.x;
	//float chessScale = 4;
	//float hpBarScale = 2;
	//float mpBarScale = 2;
	
	// �������������С
	chessImage->setScale(chessScale);
	// �������ţ���ת��ê��
	hpBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	mpBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

	hpBar->setScale(0.7, 2);
	mpBar->setScale(0.7, 3);


	chessImage->setPosition(chessPosition);
	hpBar->setPosition(chessPosition.x + 300, chessPosition.y + 1700);
	mpBar->setPosition(chessPosition.x + 300, chessPosition.y + 2000);

	chessImage->addChild(hpBar);
	chessImage->addChild(mpBar);

	

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


// �����Ǿ��嵽ְҵ����������

//----------------- ��ʦ��
// ��ʼ��
mage::mage(int name)
{
	chessName = name;
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

Sprite* mage::createAttack()
{
	auto texture = Director::getInstance()->getTextureCache();

	// ȡ·���е�ͼƬ������ͨ����������Ҳд����
	if(chessName==DAJI)
		attackImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/mage/normal/DAJI"));
	else
		attackImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/mage/normal/DIAOCHAN"));

	// �����ٵ���
	// ���Ը�����Ҫ���ù��������λ�ú�����
	// ����������ǽ��������������ӵ��Ա�
	Vec2 attackPosition = chessImage->getPosition() + Vec2(100, 0); // �������Ҳ�100����
	attackImage->setPosition(attackPosition);

	// ���������������ԣ��������š���ת��
	attackImage->setScale(1.0); // ʾ������
	return attackImage;
}

Sprite* mage::createSkill()
{
	auto texture = Director::getInstance()->getTextureCache();

	// ȡ·���е�ͼƬ������ͨ����������Ҳд����
	if (chessName == DAJI)
		skillImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/mage/skill/DAJI"));
	else
		skillImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/mage/skill/DIAOCHAN"));

	// �����ٵ���
	// ���Ը�����Ҫ���ù��������λ�ú�����
	// ����������ǽ��������������ӵ��Ա�
	Vec2 attackPosition = chessImage->getPosition() + Vec2(100, 0); // �������Ҳ�100����
	skillImage->setPosition(attackPosition);

	// ���������������ԣ��������š���ת��
	skillImage->setScale(1.0); // ʾ������
	return skillImage;
}


//----------------- ��ʦ��


//----------------- ������
// ��ʼ��
shooter::shooter(int name)
{
	chessName = name;
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

Sprite* shooter::createAttack()
{
	auto texture = Director::getInstance()->getTextureCache();

	// ȡ·���е�ͼƬ������ͨ����������Ҳд����
	if (chessName == HOUYI)
		attackImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/shooter/normal/HOUYI"));
	else
		attackImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/shooter/normal/DIRENJIE"));

	// �����ٵ���
	// ���Ը�����Ҫ���ù��������λ�ú�����
	// ����������ǽ��������������ӵ��Ա�
	Vec2 attackPosition = chessImage->getPosition() + Vec2(100, 0); // �������Ҳ�100����
	attackImage->setPosition(attackPosition);

	// ���������������ԣ��������š���ת��
	attackImage->setScale(1.0); // ʾ������
	return attackImage;
}

Sprite* shooter::createSkill()
{
	auto texture = Director::getInstance()->getTextureCache();

	// ȡ·���е�ͼƬ������ͨ����������Ҳд����
	if (chessName == HOUYI)
		skillImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/shooter/skill/HOUYI"));
	else
		skillImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/shooter/skill/DIRENJIE"));

	// �����ٵ���
	// ���Ը�����Ҫ���ù��������λ�ú�����
	// ����������ǽ��������������ӵ��Ա�
	Vec2 attackPosition = chessImage->getPosition() + Vec2(100, 0); // �������Ҳ�100����
	skillImage->setPosition(attackPosition);

	// ���������������ԣ��������š���ת��
	skillImage->setScale(1.0); // ʾ������
	return skillImage;
}

//----------------- ������


//----------------- ̹����
tank::tank(int name)
{
	chessName = name;
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
	chessCondition.imporovedDefence *= 1.2;
}


Sprite* tank::createAttack()
{
	auto texture = Director::getInstance()->getTextureCache();

	// ȡ·���е�ͼƬ������ͨ����������Ҳд����
	if (chessName == XIANGYU)
		attackImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/tank/normal/XIANGYU"));
	else
		attackImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/tank/normal/ZHNAGFEI"));

	// �����ٵ���
	// ���Ը�����Ҫ���ù��������λ�ú�����
	// ����������ǽ��������������ӵ��Ա�
	Vec2 attackPosition = chessImage->getPosition() + Vec2(100, 0); // �������Ҳ�100����
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
		skillImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/tank/skill/XIANGYU"));
	else
		skillImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/tank/skill/ZHANGFEI"));

	// �����ٵ���
	// ���Ը�����Ҫ���ù��������λ�ú�����
	// ����������ǽ��������������ӵ��Ա�
	Vec2 attackPosition = chessImage->getPosition() + Vec2(100, 0); // �������Ҳ�100����
	skillImage->setPosition(attackPosition);

	// ���������������ԣ��������š���ת��
	skillImage->setScale(1.0); // ʾ������
	return skillImage;
}
//----------------- ̹����

