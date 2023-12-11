/********************************************
 * 功能：棋子模型cpp文件
 * 作者：郑伟丞
 * 最新修改日期：12.09
 ********************************************/

#include "Chess.h"

// ChessCoordinate类实现
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


// Chess类实现

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
//函数参数：坐标类别                                   //
//函数功能：根据坐标类别，获取对应类型的坐标，棋盘坐标 //
//          chessBoardCoordinates 或 屏幕坐标          //
//函数返回值：坐标                                     //
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

// 提升棋子星级，最多提升到三级，升级的同时棋子的属性会跟着提升
void Chess::promoteRank()
{
	if (chessLevel<3)
	{
		chessLevel++;

		// 每升一级，各项属性跟着提升
		chessCondition.maxHP *= promoteScale;
		chessCondition.basicAttack *= promoteScale;
		chessCondition.basicDefence *= promoteScale;
		chessCondition.basicAttackDistance *= promoteScale;

	}
	return;
}



//攻击函数，返回攻击值,作为攻击目标的被攻击函数的输入
int Chess::myAttack()
{
	return chessCondition.improvedAttack;
}

//被攻击函数,自身的当前生命值扣除对手伤害量和自身的防御值的一半的差值
void Chess::beenAttack(int oppAttack)
{
	int damage = oppAttack - 0.5 * chessCondition.basicDefence;
	if (damage > 0)
		if (chessCondition.HP > damage)
			chessCondition.HP -= damage;
		else
			chessCondition.HP = 0;
}


// 执行一次攻击
void Chess::attackOne(Chess& OPP)
{
	OPP.beenAttack(myAttack());
}

// 判断棋子是否死亡
bool Chess::isDead()
{
	return chessCondition.HP == 0;
}


// 可视化棋子
Sprite* Chess::createChess(Vec2 chessPosition)
{
	// 设置棋子的位置
	setChessCoordinateByType(chessPosition, CoordinateType::screenCoordinates);

	auto texture = Director::getInstance()->getTextureCache();
	auto config = ConfigController::getInstance();

	//CsvParser csv;
	//csv.parseWithFile("Data/PiecesData.csv");


	// 创建三个精灵对象
	chessImage = Sprite::createWithTexture(texture->getTextureForKey(chessImagePath));
	auto hpBar = Sprite::createWithTexture(texture->getTextureForKey("/res/UI/HpBar.png"));//生命条
	auto mpBar = Sprite::createWithTexture(texture->getTextureForKey("/res/UI/MpBar.png"));//蓝条


	// 获取三个对象的原始大小
	Vec2 chessOriginSize = chessImage->getContentSize();
	Vec2 hpBarOriginSize = hpBar->getContentSize();
	Vec2 mpBarOriginSize = mpBar->getContentSize();


	// 怎么缩放后面在调整
	// 缩放大小由config一起控制
	float chessScale = 4 * config->getPx()->x / chessOriginSize.x;
	//float chessScale = 4;
	//float hpBarScale = 2;
	//float mpBarScale = 2;
	
	// 设置三个精灵大小
	chessImage->setScale(chessScale);
	// 设置缩放，旋转的锚点
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

//返回当前精灵指针,可以通过对指针操作完成动画效果
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


//根据需要类型设定棋子的屏幕坐标或棋盘坐标
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


// 以下是具体到职业的棋子子类

//----------------- 法师类
// 初始化
mage::mage(int name)
{
	chessName = name;
	initCondition();
}

// 对目标使用技能
void mage::skill(Chess& OPP)
{
	// 技能可视化
	// 
	// 技能可视化

	OPP.beenAttack(skillScale * myAttack());
}

// 羁绊效果增益
void mage::careerBuff()
{
	// 技能伤害效果提升
	skillScale = 3;
}

Sprite* mage::createAttack()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表普通攻击，这里也写死了
	if(chessName==DAJI)
		attackImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/mage/normal/DAJI"));
	else
		attackImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/mage/normal/DIAOCHAN"));

	// 后续再调整
	// 可以根据需要设置攻击精灵的位置和缩放
	// 这里假设我们将攻击放置在棋子的旁边
	Vec2 attackPosition = chessImage->getPosition() + Vec2(100, 0); // 举例，右侧100像素
	attackImage->setPosition(attackPosition);

	// 可以设置其他属性，比如缩放、旋转等
	attackImage->setScale(1.0); // 示例缩放
	return attackImage;
}

Sprite* mage::createSkill()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表普通攻击，这里也写死了
	if (chessName == DAJI)
		skillImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/mage/skill/DAJI"));
	else
		skillImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/mage/skill/DIAOCHAN"));

	// 后续再调整
	// 可以根据需要设置攻击精灵的位置和缩放
	// 这里假设我们将攻击放置在棋子的旁边
	Vec2 attackPosition = chessImage->getPosition() + Vec2(100, 0); // 举例，右侧100像素
	skillImage->setPosition(attackPosition);

	// 可以设置其他属性，比如缩放、旋转等
	skillImage->setScale(1.0); // 示例缩放
	return skillImage;
}


//----------------- 法师类


//----------------- 射手类
// 初始化
shooter::shooter(int name)
{
	chessName = name;
	initCondition();
}
// 对目标使用技能
void shooter::skill(Chess& OPP)
{
	// 技能可视化
	// 
	// 技能可视化

	OPP.beenAttack(skillScale * myAttack());
}

// 羁绊效果增益
void shooter::careerBuff()
{
	// 现有攻击上升
	chessCondition.improvedAttack *= 1.2;
}

Sprite* shooter::createAttack()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表普通攻击，这里也写死了
	if (chessName == HOUYI)
		attackImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/shooter/normal/HOUYI"));
	else
		attackImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/shooter/normal/DIRENJIE"));

	// 后续再调整
	// 可以根据需要设置攻击精灵的位置和缩放
	// 这里假设我们将攻击放置在棋子的旁边
	Vec2 attackPosition = chessImage->getPosition() + Vec2(100, 0); // 举例，右侧100像素
	attackImage->setPosition(attackPosition);

	// 可以设置其他属性，比如缩放、旋转等
	attackImage->setScale(1.0); // 示例缩放
	return attackImage;
}

Sprite* shooter::createSkill()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表普通攻击，这里也写死了
	if (chessName == HOUYI)
		skillImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/shooter/skill/HOUYI"));
	else
		skillImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/shooter/skill/DIRENJIE"));

	// 后续再调整
	// 可以根据需要设置攻击精灵的位置和缩放
	// 这里假设我们将攻击放置在棋子的旁边
	Vec2 attackPosition = chessImage->getPosition() + Vec2(100, 0); // 举例，右侧100像素
	skillImage->setPosition(attackPosition);

	// 可以设置其他属性，比如缩放、旋转等
	skillImage->setScale(1.0); // 示例缩放
	return skillImage;
}

//----------------- 射手类


//----------------- 坦克类
tank::tank(int name)
{
	chessName = name;
	initCondition();
}
// 对目标使用技能
void tank::skill(Chess& OPP)
{
	// 技能可视化
	// 
	// 技能可视化

	OPP.beenAttack(skillScale * myAttack());
}

// 羁绊效果增益
void tank::careerBuff()
{
	// 现有防御值上升
	chessCondition.imporovedDefence *= 1.2;
}


Sprite* tank::createAttack()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表普通攻击，这里也写死了
	if (chessName == XIANGYU)
		attackImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/tank/normal/XIANGYU"));
	else
		attackImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/tank/normal/ZHNAGFEI"));

	// 后续再调整
	// 可以根据需要设置攻击精灵的位置和缩放
	// 这里假设我们将攻击放置在棋子的旁边
	Vec2 attackPosition = chessImage->getPosition() + Vec2(100, 0); // 举例，右侧100像素
	attackImage->setPosition(attackPosition);

	// 可以设置其他属性，比如缩放、旋转等
	attackImage->setScale(1.0); // 示例缩放
	return attackImage;
}

Sprite* tank::createSkill()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表普通攻击，这里也写死了
	if (chessName == XIANGYU)
		skillImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/tank/skill/XIANGYU"));
	else
		skillImage = Sprite::createWithTexture(texture->getTextureForKey("/res/Attack/tank/skill/ZHANGFEI"));

	// 后续再调整
	// 可以根据需要设置攻击精灵的位置和缩放
	// 这里假设我们将攻击放置在棋子的旁边
	Vec2 attackPosition = chessImage->getPosition() + Vec2(100, 0); // 举例，右侧100像素
	skillImage->setPosition(attackPosition);

	// 可以设置其他属性，比如缩放、旋转等
	skillImage->setScale(1.0); // 示例缩放
	return skillImage;
}
//----------------- 坦克类

