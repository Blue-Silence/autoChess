/*
功能：棋子存储器信息
作者：Hu Junwei
*/
#include "Storage.h"

//-----------------------------------------------------//
// 函数参数：无                                        //
// 函数功能：初始化存储器，清空                        //
// 函数返回值：bool型                                  //
// 函数注意事项：无                                    //
//-----------------------------------------------------//
bool Storage::init()
{
    tankVector.clear();
    mageVector.clear();
    shooterVector.clear();
    return true;
}

//-----------------------------------------------------//
// 函数参数：tank obj - 存储坦克对象的实例             //
// 函数功能：将坦克对象存储到存储容器中                //
// 函数返回值：无（void）                              //
// 函数注意事项：无                                    //
//-----------------------------------------------------//
void Storage::tankMemorize(tank obj)
{
    tankVector.push_back(obj);
}

//-----------------------------------------------------//
// 函数参数：tank* ptr - 指向要移除的坦克对象的指针    //
// 函数功能：从存储容器中移除特定的坦克对象            //
// 函数返回值：无（void）                              //
// 函数注意事项：无                                    //
//-----------------------------------------------------//
void Storage::tankDelete(tank* ptr)
{
    for (vector<tank>::iterator it = tankVector.begin(); it != tankVector.end(); it++)
    {
        if (it->storageNum == ptr->storageNum)
        {
            tankVector.erase(it);
            break;
        }
    }
}

//-----------------------------------------------------//
// 函数参数：mage obj - 存储法师对象的实例             //
// 函数功能：将法师对象存储到存储容器中                //
// 函数返回值：无（void）                              //
// 函数注意事项：无                                    //
//-----------------------------------------------------//
void Storage::mageMemorize(mage obj)
{
    mageVector.push_back(obj);
}

//-----------------------------------------------------//
// 函数参数：mage* ptr - 指向要移除的法师对象的指针    //
// 函数功能：从存储容器中移除特定的法师对象            //
// 函数返回值：无（void）                              //
// 函数注意事项：无                                    //
//-----------------------------------------------------//
void Storage::mageDelete(mage* ptr)
{
    for (vector<mage>::iterator it = mageVector.begin(); it != mageVector.end(); it++)
    {
        if (it->storageNum == ptr->storageNum)
        {
            mageVector.erase(it);
            break;
        }
    }
}

//-----------------------------------------------------//
// 函数参数：shooter obj - 存储射手对象的实例          //
// 函数功能：将射手对象存储到存储容器中                //
// 函数返回值：无（void）                              //
// 函数注意事项：无                                    //
//-----------------------------------------------------//
void Storage::shooterMemorize(shooter obj)
{
    shooterVector.push_back(obj);
}

//-----------------------------------------------------//
// 函数参数：shooter* ptr - 指向要移除的射手对象的指针 //
// 函数功能：从存储容器中移除特定的射手对象            //
// 函数返回值：无（void）                              //
// 函数注意事项：无                                    //
//-----------------------------------------------------//
void Storage::shooterDelete(shooter* ptr)
{
    for (vector<shooter>::iterator it = shooterVector.begin(); it != shooterVector.end(); it++)
    {
        if (it->storageNum == ptr->storageNum)
        {
            shooterVector.erase(it);
            break;
        }
    }
}
