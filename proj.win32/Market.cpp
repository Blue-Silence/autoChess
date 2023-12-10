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
bool Market::InitMarket()
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
		for (int i = 0; i < 4; i++)
		{
			delete chessList[i];
		}
		chessList.clear();
	}

	// ��������
	for (int i = 0; i < 4; i++) 
	{
		int flag = rand() % HEROCATAGORY;
		if (flag == 0)
		{
			chessList.push_back(new tank);
			continue;
		}
		if (flag == 1)
		{
			chessList.push_back(new shooter);
			continue;
		}
		if (flag == 2)
		{
			chessList.push_back(new mage);
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
