#include "PreparationSeatLayer.h"

/*
* ����������
* �������ܣ��½��㡢�õ���Ļ��С��
* ��������ֵ��
* ����ע�����������ɺ���ʾ����ʼ��Ϸ���͡��˳���Ϸ���˵�
*/
bool PreparationSeat::init()
{
	PreAreaLayer = Layer::create();
	preAreaChessButton.resize(9);
	pageSize = Director::getInstance()->getVisibleSize();
	pageCoord = Director::getInstance()->getVisibleOrigin();
	//���ô�С
	return true;
}

inline void PreparationSeat::GetCoordAndScale(int index,Vec2& coord,float& scale)
{
	coord.y = 0.27 * pageSize.height + pageCoord.y;
	coord.x = 0.13 * pageSize.width + pageCoord.x + index * 0.08 * pageSize.width;
	//�����������
	scale = (0.08 * pageSize.width) / scale;
}

void  PreparationSeat::CreatePreAreaButton(Chess* curHero, int index)
{
	CC_SAFE_RETAIN(PreAreaLayer);
	//�õ�ͼƬ
	std::string imageNormalPath = curHero->getChessImagePath();
	std::string imageSeletedPath;
	int pos = imageNormalPath.find(".png");
	imageNormalPath.replace(pos, 11, "Hero.png");
	//�����˵�����
	auto item = Button::create(imageNormalPath, imageNormalPath);
	Vec2 originSize;
	float scale = item->getSize().width;
	GetCoordAndScale(index, originSize, scale);
	item->setPosition(originSize);
	item->setScale(scale);

	//��������
	preAreaChessButton[index] = item;
	PreAreaLayer->addChild(item,8);
	//item->addTouchEventListener(CC_CALLBACK_0(ChessBoard::createMouseListener, this));
	//������¼�
}
