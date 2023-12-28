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
void Chess::promoteRank(int dstRank)
{
	if (dstRank<=3)
	{
		chessLevel = dstRank;

		// 每升一级，各项属性跟着提升
		chessCondition.HP = chessCondition.maxHP = hpData[chessName]*pow(promoteScale, dstRank);
		chessCondition.improvedAttack = chessCondition.basicAttack = attackData[chessName] * pow(promoteScale, dstRank);
		chessCondition.improvedDefence = chessCondition.basicDefence = defenceData[chessName] * pow(promoteScale, dstRank);
		chessCondition.improvedAttackDistance = chessCondition.basicAttackDistance = attackDistanceData[chessName] * pow(promoteScale, dstRank);
		

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


// 执行一次攻击，包含动画
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

	
	auto config = ConfigController::getInstance();

	//CsvParser csv;
	//csv.parseWithFile("Data/PiecesData.csv");

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
	
	// 设置三个精灵大小
	chessImage->setScale(chessScaleX,chessScaleY);

	// 设置缩放，旋转的锚点
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

//----------------- 法师类
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

void mage::removeCareerBuff()
{
	skillScale = 2;
}

Sprite* mage::createAttack()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表普通攻击，这里也写死了
	if(chessName==DAJI)
		attackImage = Sprite::create("/res/Effect/DAJI1.png");
	else
		attackImage = Sprite::create("/res/Effect/DIAOCHAN1.png");

	// 后续再调整
	// 可以根据需要设置攻击精灵的位置和缩放
	// 这里假设我们将攻击放置在棋子的旁边
	Vec2 attackPosition = chessImage->getPosition() ; // 举例，右侧100像素
	attackImage->setPosition(attackPosition);

	// 可以设置其他属性，比如缩放、旋转等
	attackImage->setScale(0.1); // 示例缩放
	return attackImage;
}

Sprite* mage::createSkill()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表普通攻击，这里也写死了
	if (chessName == DAJI)
		skillImage = Sprite::create("/res/Effect/DAJI.png");
	else
		skillImage = Sprite::create("/res/Effect/DIAOCHAN.png");

	// 后续再调整
	// 可以根据需要设置攻击精灵的位置和缩放
	// 这里假设我们将攻击放置在棋子的旁边
	Vec2 attackPosition = chessImage->getPosition(); // 举例，右侧100像素

	skillImage->setPosition(attackPosition);

	// 可以设置其他属性，比如缩放、旋转等
	skillImage->setScale(0.1); // 示例缩放
	return skillImage;
}


//----------------- 法师类


//----------------- 射手类
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

void shooter::removeCareerBuff()
{
	chessCondition.improvedAttack /= 1.2;
}

Sprite* shooter::createAttack()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表普通攻击，这里也写死了
	if (chessName == HOUYI)
		attackImage = Sprite::create("/res/Effect/HOUYI1.png");
	else
		attackImage = Sprite::create("/res/Effect/DIRENJIE1.png");

	// 后续再调整
	// 可以根据需要设置攻击精灵的位置和缩放
	// 这里假设我们将攻击放置在棋子的旁边
	Vec2 attackPosition = chessImage->getPosition(); // 举例，右侧100像素
	attackImage->setPosition(attackPosition);

	// 可以设置其他属性，比如缩放、旋转等
	attackImage->setScale(0.05); // 示例缩放
	return attackImage;
}

Sprite* shooter::createSkill()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表普通攻击，这里也写死了
	if (chessName == HOUYI)
		skillImage = Sprite::create("/res/Effect/HOUYI.png");
	else
		skillImage = Sprite::create("/res/Effect/DIRENJIE.png");

	// 后续再调整
	// 可以根据需要设置攻击精灵的位置和缩放
	// 这里假设我们将攻击放置在棋子的旁边
	Vec2 attackPosition = chessImage->getPosition() ; // 举例，右侧100像素
	skillImage->setPosition(attackPosition);

	// 可以设置其他属性，比如缩放、旋转等
	skillImage->setScale(0.1); // 示例缩放
	return skillImage;
}

//----------------- 射手类


//----------------- 坦克类
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
	// 技能可视化
	// 
	// 技能可视化

	OPP.beenAttack(skillScale * myAttack());
}

// 羁绊效果增益
void tank::careerBuff()
{
	// 现有防御值上升
	chessCondition.improvedDefence *= 1.2;
}

void tank::removeCareerBuff()
{
	chessCondition.improvedDefence /= 1.2;
}


Sprite* tank::createAttack()
{
	auto texture = Director::getInstance()->getTextureCache();

	// 取路径中的图片代表普通攻击，这里也写死了
	if (chessName == XIANGYU)
		attackImage = Sprite::create("/res/Attack/tank/normal/XIANGYU");
	else
		attackImage = Sprite::create("/res/Attack/tank/normal/ZHNAGFEI");

	// 后续再调整
	// 可以根据需要设置攻击精灵的位置和缩放
	// 这里假设我们将攻击放置在棋子的旁边
	Vec2 attackPosition = chessImage->getPosition(); // 举例，右侧100像素
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
		skillImage = Sprite::create("/res/Effect/XIANGYU.png");
	else
		skillImage = Sprite::create("/res/Effect/ZHANGFEI.png");

	// 后续再调整
	// 可以根据需要设置攻击精灵的位置和缩放
	// 这里假设我们将攻击放置在棋子的旁边
	Vec2 attackPosition = chessImage->getPosition();// 举例，右侧100像素
	skillImage->setPosition(attackPosition);

	// 可以设置其他属性，比如缩放、旋转等
	skillImage->setScale(1.0); // 示例缩放
	return skillImage;
}
//----------------- 坦克类

