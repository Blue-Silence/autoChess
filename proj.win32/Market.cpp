/*
���ܣ��̵���Ϣ
���ߣ�Hu Junwei
*/
#include "Market.h"

//-----------------------------------------------------//
//������������                                         //
//�������ܣ���ʼ���̵����Ϣ                           //
//��������ֵ��bool��                                   //
//����ע������(ѡ)����                                 //
//-----------------------------------------------------//
bool Market::init()
{
	RefreshMarket();
	return true;
}

//-----------------------------------------------------//
//������������                                         //
//�������ܣ�ˢ���̵���Ϣ                               //
//��������ֵ��bool��                                   //
//����ע������(ѡ)����                                 //
//-----------------------------------------------------//
bool Market::RefreshMarket()
{
	srand(time(NULL));
	// ���ԭ���������ӣ����
	if (!chessList.empty())
	{
		for (int i = 0; i < 5; i++)
		{
			delete chessList[i];
		}
		chessList.clear();
	}

	// ��������
	for (int i = 0; i < 5; i++) 
	{
		int flag = rand() % HEROCATAGORY;
		if (flag == 0)
		{
			chessList.push_back(new tank(ZHANGFEI));
			continue;
		}
		if (flag == 1)
		{
			chessList.push_back(new shooter(HOUYI));
			continue;
		}
		if (flag == 2)
		{
			chessList.push_back(new mage(DAJI));
			continue;
		}
		if (flag == 3)
		{
			chessList.push_back(new tank(XIANGYU));
			continue;
		}
		if (flag == 4)
		{
			chessList.push_back(new shooter(DIRENJIE));
			continue;
		}
		if (flag == 5)
		{
			chessList.push_back(new mage(DIAOCHAN));
			continue;
		}

	}

	equippment = rand() % 3;
	return true;
}

//-----------------------------------------------------//
//����������int money, int maxChessNum, int nowChessNum//
//          ClassName* A, typename price               //
//�������ܣ��ܷ�������Ȩ���ж�                       //
//��������ֵ��bool��                                   //
//����ע������(ѡ)����                                 //
//-----------------------------------------------------//
template<class ClassName, typename price>
bool Market::BoughtPermission(int money, int maxChessNum, int nowChessNum, ClassName* A, typename price)
{
	if (money >= price && nowChessNum < maxChessNum)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//-----------------------------------------------------//
//����������ClassName* A                               //
//�������ܣ������ȸ���������Ӹ�����ӵ�еĽ�Ǯ�ж��Ƿ� //
//          ��������ö���ӡ�Ȼ�󷵻ز�ͬ�����ӣ������� //
//          ����������������������Ϊ����ʧ��           //
//��������ֵ��ģ����ClassName                          //
//����ע������(ѡ)��A�Ǳ����������                    //
//-----------------------------------------------------//
template<class ClassName>
ClassName Market::BuyChess(ClassName* A)
{
	ClassName candidate = *A;
	return candidate;
}

//-----------------------------------------------------//
//������������                                         //
//�������ܣ�����װ��                                   //
//��������ֵ��int��                                    //
//����ע������(ѡ)��                                   //
//-----------------------------------------------------//
int Market::BuyEquippment() const
{
	return equippment;
}

//-----------------------------------------------------//
//����������ClassName piece                            //
//�������ܣ��������ӻ���װ��                           //
//��������ֵ��int��                                    //
//����ע������(ѡ)�����س��۵õ���                     //
//-----------------------------------------------------//
template<class ClassName>
int Market::SellOut(ClassName chess)
{
	int price = 0;//���ӵļ۸�
	price = static_cast<int>(chess.getPieceLevel());
	return price;
}
