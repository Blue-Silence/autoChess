/******************************************************/
/*                  文件名：Chess.cpp                 
/*                  功能：棋子模型具体实现            
/*                  作者：郑伟丞                      
/******************************************************/

#include "Chess.h"

//-----------------------//
// ChessCoordinate类实现 
//-----------------------//

int ChessCoordinate::getX() const
{
	return x;
}

int ChessCoordinate::getY() const
{
	return y;
}

void ChessCoordinate::setX(const int& X) 
{
	
	x = X;
	return;
}

void ChessCoordinate::setY(const int& Y)
{
	
	y = Y;
	return;
}


//-----------------------//
// Chess类实现           
//-----------------------//

// 初始化棋子信息
void Chess::initCondition()
{
	chessCondition.maxHP = chessCondition.HP = hpData[chessName];
	chessCondition.basicAttack = chessCondition.improvedAttack = attackData[chessName];
	chessCondition.basicDefence = chessCondition.improvedDefence = defenceData[chessName];
	chessCondition.basicAttackDistance = chessCondition.improvedAttackDistance = attackDistanceData[chessName];
	chessCondition.skillCooldown = skillCooldowns[chessName];
}

// 获取棋子职业属性
const string Chess::getCareer() const
{
	return career;
 }

// 获取棋子图片路径
const string Chess::getChessImagePath() const
{
	return chessImagePath;
}

// 获取棋子名
const int Chess::getChessName() const
{
	return chessName;
}

// 获取棋子状态
ChessInfo* Chess::getChessCondition()
{
	return &chessCondition;
}

// 获取棋子等级
const int Chess::getChessLevel() const
{
	return chessLevel;
}

//-----------------------------------------------------//
//函数参数：坐标类别                                   
//函数功能：根据坐标类别，获取对应类型的坐标，棋盘坐标 
//          chessBoardCoordinates 或 屏幕坐标          
//函数返回值：坐标                                     
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

// 设置棋子等级
void Chess::setChessLevel(const int newLevel)
{
	if (newLevel > 3)
		return;
	chessLevel = newLevel;
}

// 提升棋子星级，最多提升到三级，升级的同时棋子的属性会跟着提升
void Chess::promoteRank(int dstRank)
{
	if (dstRank<=3)
	{
		chessLevel = dstRank;

		// 每升一级，各项属性跟着提升
		chessCondition.HP = chessCondition.maxHP = hpData[chessName]*pow(promoteScale, dstRank);
		chessCondition.improvedAttack = chessCondition.basicAttack = attackData[chessName] * pow(promoteScale, dstRank);
		chessCondition.improvedDefence = chessCondition.basicDefence = defenceData[chessName] * pow(promoteScale, dstRank);
		if (career != "tank")
			chessCondition.improvedAttackDistance = chessCondition.basicAttackDistance = attackDistanceData[chessName] + dstRank - 1;

	}
	return;
}

// 攻击函数，返回攻击值,作为攻击目标的被攻击函数的输入
const int Chess::myAttack() const
{
	return chessCondition.improvedAttack;
}

// 被攻击函数,自身的当前生命值扣除对手伤害量和自身的防御值的一半的差值
void Chess::beenAttack(int oppAttack)
{
	double damage = oppAttack - 0.5 * chessCondition.basicDefence;
	if (damage > 0)
		chessCondition.HP = max(0.0, chessCondition.HP - damage);
}

// 执行一次攻击，
void Chess::attackOne(Chess& OPP)
{
	OPP.beenAttack(myAttack());
}

// 判断棋子是否死亡
const bool Chess::isDead() const
{
	return chessCondition.HP == 0;
}

//-----------------------------------------------------//
// 函数参数：chessPosition（棋子在屏幕上的位置）       
// 函数功能：创建并可视化棋子，包括生命条和技能条      
// 函数返回值：Sprite*，创建的棋子精灵对象             
// 函数注意事项：设置棋子的位置、大小和进度条          
//-----------------------------------------------------//
Sprite* Chess::createChess(Vec2 chessPosition)
{
	// 设置棋子的位置
	setChessCoordinateByType(chessPosition, CoordinateType::screenCoordinates);

	auto config = ConfigController::getInstance();


	// 创建三个精灵对象
	std::string imageHeroPath = getChessImagePath();
	int pos = imageHeroPath.find(".png");
	imageHeroPath.replace(pos, 11, "Hero.png");
	chessImage = Sprite::create(imageHeroPath);

	// 创建生命条
	auto hpBar = Sprite::create("/res/UI/HpBar.png");
	hpBarProgress = ProgressTimer::create(hpBar);
	hpBarProgress->setType(ProgressTimer::Type::BAR);

	// 设置进度条的方向，从左到右
	hpBarProgress->setMidpoint(Vec2(0.0f, 0.5f));
	hpBarProgress->setBarChangeRate(Vec2(1.0f, 0.0f));
	// 设置进度条的初始百分比
	hpBarProgress->setPercentage(100); // 100% 表示完全填充

	// 创建技能条
	auto mpBar = Sprite::create("/res/UI/MpBar.png");
	mpBarProgress = ProgressTimer::create(mpBar);
	mpBarProgress->setType(ProgressTimer::Type::BAR);

	// 设置进度条的方向，从左到右
	mpBarProgress->setMidpoint(Vec2(0.0f, 0.5f));
	mpBarProgress->setBarChangeRate(Vec2(1.0f, 0.0f));

	// 设置进度条的初始百分比
	mpBarProgress->setPercentage(0); // 初始技能条进度为0


	// 获取三个对象的原始大小
	Vec2 chessOriginSize = chessImage->getContentSize();
	Vec2 hpBarOriginSize = hpBarProgress->getContentSize();
	Vec2 mpBarOriginSize = mpBarProgress->getContentSize();

	
	// 缩放大小由config一起控制
	float chessScaleX = 6 * config->getPx()->x / chessOriginSize.x;
	float chessScaleY = 7.5 * config->getPx()->y / chessOriginSize.y;
	float hpBarScaleX = 10 * config->getPx()->x / hpBarOriginSize.x;
	float hpBarScaleY = 1 * config->getPx()->y / hpBarOriginSize.y;
	float mpBarScaleX = 10 * config->getPx()->x / mpBarOriginSize.x;
	float mpBarScaleY = 1 * config->getPx()->y / mpBarOriginSize.y;
	
	// 设置三个精灵大小，缩放，旋转的锚点
	chessImage->setScale(chessScaleX,chessScaleY);
	chessImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	hpBarProgress->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	mpBarProgress->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	hpBarProgress->setScale(hpBarScaleX /chessScaleX , hpBarScaleY /chessScaleX);
	mpBarProgress->setScale(mpBarScaleX / chessScaleX, mpBarScaleY / chessScaleX);

	double nowWidth = chessImage->getContentSize().width;
	double nowHeight = chessImage->getContentSize().height;
	
	// 设置三个精灵的位置
	chessImage->setPosition(chessPosition);
	hpBarProgress->setPosition(chessPosition.x - 0.5 * nowWidth, chessPosition.y + 0.60 * nowHeight);
	mpBarProgress->setPosition(chessPosition.x - 0.5 * nowWidth, chessPosition.y + 0.80 * nowHeight);

	// 把血条和技能条添加到棋子上方
	chessImage->addChild(hpBarProgress);
	chessImage->addChild(mpBarProgress);

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

// 更新生命条进度
void Chess::updateHpBar()
{
	hpBarProgress->setPercentage(chessCondition.HP / chessCondition.maxHP * 100);
}

// 更新技能条进度
void Chess::updateMpBar()
{
	mpBarProgress->setPercentage(attackNum / chessCondition.skillCooldown * 100);
}

// 重载赋值运算符便于赋值操作
Chess& Chess::operator=(const Chess& other) 
{
	if (this == &other) 
	{
		return *this; // 检查自赋值
	}

	// 复制基本数据
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



	// 复制指针，实现共享 Sprite
	chessImage = other.chessImage;
	attackImage = other.attackImage;
	skillImage = other.skillImage;
	hpBarProgress = other.hpBarProgress;
	mpBarProgress = other.mpBarProgress;

	return *this;
}


// 以下是具体到职业的棋子子类

//----------------- 法师类 -----------------//
// 初始化
mage::mage(int name)
{
	chessName = name;
	career = "mage";
	chessImagePath = chessImagePaths[name];
	initCondition();
}

// 对目标使用技能
void mage::skill(Chess& OPP)
{
	OPP.beenAttack(1.2*skillScale * myAttack());
}

// 羁绊效果增益
void mage::careerBuff()
{
	// 技能伤害效果提升
	skillScale = 3;
}

// 去除法师羁绊效果
void mage::removeCareerBuff()
{
	skillScale = 2;
}

// 创建普攻贴图
Sprite* mage::createAttack()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表普通攻击，这里也写死了
	if(chessName==DAJI)
		attackImage = Sprite::create("/res/Effect/DAJI1.png");
	else
		attackImage = Sprite::create("/res/Effect/DIAOCHAN1.png");

	// 可以根据需要设置攻击精灵的位置和缩放
	Vec2 attackPosition = chessImage->getPosition() ;
	attackImage->setPosition(attackPosition);

	return attackImage;
}

// 创建技能贴图
Sprite* mage::createSkill()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表技能，这里也写死了
	if (chessName == DAJI)
		skillImage = Sprite::create("/res/Effect/DAJI.png");
	else
		skillImage = Sprite::create("/res/Effect/DIAOCHAN.png");

	// 可以根据需要设置攻击精灵的位置和缩放
	Vec2 attackPosition = chessImage->getPosition(); 

	skillImage->setPosition(attackPosition);

	return skillImage;
}



//----------------- 射手类 -----------------//
// 初始化
shooter::shooter(int name)
{
	chessName = name;
	career = "shooter";
	chessImagePath = chessImagePaths[name];
	initCondition();
}
// 对目标使用技能
void shooter::skill(Chess& OPP)
{
	OPP.beenAttack(skillScale * myAttack());
}

// 羁绊效果增益
void shooter::careerBuff()
{
	// 现有攻击提升
	chessCondition.improvedAttack *= 1.3;
}

// 去除射手羁绊效果
void shooter::removeCareerBuff()
{
	chessCondition.improvedAttack /= 1.3;
}

// 创建普攻贴图
Sprite* shooter::createAttack()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表普通攻击，这里也写死了
	if (chessName == HOUYI)
		attackImage = Sprite::create("/res/Effect/HOUYI1.png");
	else
		attackImage = Sprite::create("/res/Effect/DIRENJIE1.png");

	// 可以根据需要设置攻击精灵的位置和缩放
	Vec2 attackPosition = chessImage->getPosition(); 
	attackImage->setPosition(attackPosition);  
	
	return attackImage;
}

// 设置技能贴图
Sprite* shooter::createSkill()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表普通攻击，这里也写死了
	if (chessName == HOUYI)
		skillImage = Sprite::create("/res/Effect/HOUYI.png");
	else
		skillImage = Sprite::create("/res/Effect/DIRENJIE.png");


	// 可以根据需要设置攻击精灵的位置和缩放
	Vec2 attackPosition = chessImage->getPosition() ;
	skillImage->setPosition(attackPosition);

	return skillImage;
}


//----------------- 坦克类 -----------------//
tank::tank(int name)
{
	chessName = name;
	career = "tank";
	chessImagePath = chessImagePaths[name];
	initCondition();
}
// 对目标使用技能
void tank::skill(Chess& OPP)
{
	OPP.beenAttack(skillScale * myAttack());
}

// 羁绊效果增益
void tank::careerBuff()
{
	// 现有防御值上升
	chessCondition.improvedDefence *= 1.5;
}

// 去除坦克羁绊效果
void tank::removeCareerBuff()
{
	chessCondition.improvedDefence /= 1.5;
}

// 创建普攻贴图
Sprite* tank::createAttack()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表普通攻击，这里也写死了
	if (chessName == XIANGYU)
		attackImage = Sprite::create("/res/Attack/tank/normal/XIANGYU");
	else
		attackImage = Sprite::create("/res/Attack/tank/normal/ZHNAGFEI");

	// 可以根据需要设置攻击精灵的位置和缩放
	Vec2 attackPosition = chessImage->getPosition(); 
	attackImage->setPosition(attackPosition);

	return attackImage;
}

// 创建技能贴图
Sprite* tank::createSkill()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表普通攻击，这里也写死了
	if (chessName == XIANGYU)
		skillImage = Sprite::create("/res/Effect/XIANGYU.png");
	else
		skillImage = Sprite::create("/res/Effect/ZHANGFEI.png");

	// 可以根据需要设置攻击精灵的位置和缩放
	Vec2 attackPosition = chessImage->getPosition();
	skillImage->setPosition(attackPosition);

	return skillImage;
}	

