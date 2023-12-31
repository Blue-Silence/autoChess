/*
功能：商店信息
作者：Hu Junwei
*/
#include "Market.h"

//-----------------------------------------------------//
//函数参数：空                                         //
//函数功能：初始化商店的信息                           //
//函数返回值：bool型                                   //
//函数注意事项(选)：无                                 //
//-----------------------------------------------------//
bool Market::init()
{
    RefreshMarket();
    return true;
}

//-----------------------------------------------------//
//函数参数：空                                         //
//函数功能：刷新商店信息                               //
//函数返回值：bool型                                   //
//函数注意事项(选)：无                                 //
//-----------------------------------------------------//
bool Market::RefreshMarket()
{
    srand(static_cast<unsigned int>(time(NULL)));

    // 如果原来已有棋子，清空
    chessList.clear();

    // 增加棋子
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
//函数参数：int money, int maxChessNum, int nowChessNum//
//          ClassName* A, typename price               //
//函数功能：能否购买棋子权限判断                       //
//函数返回值：bool型                                   //
//函数注意事项(选)：无                                 //
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
//函数参数：ClassName* A                               //
//函数功能：函数先根据最大棋子个数和拥有的金钱判断是否 //
//          可以买这枚棋子。然后返回不同的棋子，其中如 //
//          果返回特殊数据棋子则算为购买失败           //
//函数返回值：模板类ClassName                          //
//函数注意事项(选)：A是被购买的棋子                    //
//-----------------------------------------------------//
template<class ClassName>
ClassName Market::BuyChess(ClassName* A)
{
    ClassName candidate = *A;
    return candidate;
}

//-----------------------------------------------------//
//函数参数：无                                         //
//函数功能：购买装备                                   //
//函数返回值：int型                                    //
//函数注意事项(选)：                                   //
//-----------------------------------------------------//
int Market::BuyEquippment() const
{
    return equippment;
}

//-----------------------------------------------------//
//函数参数：ClassName piece                            //
//函数功能：出售棋子或者装备                           //
//函数返回值：int型                                    //
//函数注意事项(选)：返回出售得到的                     //
//-----------------------------------------------------//
template<class ClassName>
int Market::SellOut(ClassName chess)
{
    int price = 0;//棋子的价格
    price = static_cast<int>(chess.getPieceLevel());
    return price;
}

//-----------------------------------------------------//
//函数参数：int index                                  //
//函数功能：出售棋子                                   //
//函数返回值：int型                                    //
//函数注意事项(选)：返回出售得到的                     //
//-----------------------------------------------------//
Chess* Market::GetChessByIndex(int index)
{
    if (index >= 0 && index < chessList.size() && chessList[index] != nullptr)
    {
        // 由于使用了智能指针，需要获取原始指针
        return chessList[index].get();
    }
    return nullptr;
}