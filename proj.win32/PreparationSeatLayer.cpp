#include "PreparationSeatLayer.h"

/*
* 函数参数：
* 函数功能：新建层、得到屏幕大小、
* 函数返回值：
* 函数注意事项：加载完成后显示“开始游戏”和“退出游戏”菜单
*/
bool PreparationSeat::init()
{
	PreAreaLayer = Layer::create();
	preAreaChessButton.resize(9);
	pageSize = Director::getInstance()->getVisibleSize();
	pageCoord = Director::getInstance()->getVisibleOrigin();
	//设置大小
	return true;
}

inline void PreparationSeat::GetCoordAndScale(int index,Vec2& coord,float& scale)
{
	coord.y = 0.27 * pageSize.height + pageCoord.y;
	coord.x = 0.13 * pageSize.width + pageCoord.x + index * 0.08 * pageSize.width;
	//调整后的数字
	scale = (0.08 * pageSize.width) / scale;
}

void  PreparationSeat::CreatePreAreaButton(Chess* curHero, int index)
{
	CC_SAFE_RETAIN(PreAreaLayer);
	//得到图片
	std::string imageNormalPath = curHero->getChessImagePath();
	std::string imageSeletedPath;
	int pos = imageNormalPath.find(".png");
	imageNormalPath.replace(pos, 11, "Hero.png");
	//创建菜单按键
	auto item = Button::create(imageNormalPath, imageNormalPath);
	Vec2 originSize;
	float scale = item->getSize().width;
	GetCoordAndScale(index, originSize, scale);
	item->setPosition(originSize);
	item->setScale(scale);

	//加入数组
	preAreaChessButton[index] = item;
	PreAreaLayer->addChild(item,8);
	//item->addTouchEventListener(CC_CALLBACK_0(ChessBoard::createMouseListener, this));
	//绑定鼠标事件
}
