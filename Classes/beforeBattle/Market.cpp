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
    srand(static_cast<unsigned int>(time(NULL)));

    // ���ԭ���������ӣ����
    chessList.clear();

    // ��������
    for (int i = 0; i < 5; i++)
    {
        int flag = rand() % HEROCATAGORY;
        switch (flag)
        {
            case 0:
                chessList.push_back(std::make_shared<tank>(ZHANGFEI));
                break;
            case 1:
                chessList.push_back(std::make_shared<shooter>(HOUYI));
                break;
            case 2:
                chessList.push_back(std::make_shared<mage>(DAJI));
                break;
            case 3:
                chessList.push_back(std::make_shared<tank>(XIANGYU));
                break;
            case 4:
                chessList.push_back(std::make_shared<shooter>(DIRENJIE));
                break;
            case 5:
                chessList.push_back(std::make_shared<mage>(DIAOCHAN));
                break;
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

//-----------------------------------------------------//
//����������int index                                  //
//�������ܣ���������                                   //
//��������ֵ��int��                                    //
//����ע������(ѡ)�����س��۵õ���                     //
//-----------------------------------------------------//
Chess* Market::GetChessByIndex(int index)
{
    if (index >= 0 && index < chessList.size() && chessList[index] != nullptr)
    {
        // ����ʹ��������ָ�룬��Ҫ��ȡԭʼָ��
        return chessList[index].get();
    }
    return nullptr;
}