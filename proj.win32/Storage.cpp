/*
���ܣ����Ӵ洢����Ϣ
���ߣ�Hu Junwei
*/
#include "Storage.h"

//-----------------------------------------------------//
// ������������                                        //
// �������ܣ���ʼ���洢�������                        //
// ��������ֵ��bool��                                  //
// ����ע�������                                    //
//-----------------------------------------------------//
bool Storage::init()
{
    tankVector.clear();
    mageVector.clear();
    shooterVector.clear();
    return true;
}

//-----------------------------------------------------//
// ����������tank obj - �洢̹�˶����ʵ��             //
// �������ܣ���̹�˶���洢���洢������                //
// ��������ֵ���ޣ�void��                              //
// ����ע�������                                    //
//-----------------------------------------------------//
void Storage::tankMemorize(tank obj)
{
    tankVector.push_back(obj);
}

//-----------------------------------------------------//
// ����������tank* ptr - ָ��Ҫ�Ƴ���̹�˶����ָ��    //
// �������ܣ��Ӵ洢�������Ƴ��ض���̹�˶���            //
// ��������ֵ���ޣ�void��                              //
// ����ע�������                                    //
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
// ����������mage obj - �洢��ʦ�����ʵ��             //
// �������ܣ�����ʦ����洢���洢������                //
// ��������ֵ���ޣ�void��                              //
// ����ע�������                                    //
//-----------------------------------------------------//
void Storage::mageMemorize(mage obj)
{
    mageVector.push_back(obj);
}

//-----------------------------------------------------//
// ����������mage* ptr - ָ��Ҫ�Ƴ��ķ�ʦ�����ָ��    //
// �������ܣ��Ӵ洢�������Ƴ��ض��ķ�ʦ����            //
// ��������ֵ���ޣ�void��                              //
// ����ע�������                                    //
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
// ����������shooter obj - �洢���ֶ����ʵ��          //
// �������ܣ������ֶ���洢���洢������                //
// ��������ֵ���ޣ�void��                              //
// ����ע�������                                    //
//-----------------------------------------------------//
void Storage::shooterMemorize(shooter obj)
{
    shooterVector.push_back(obj);
}

//-----------------------------------------------------//
// ����������shooter* ptr - ָ��Ҫ�Ƴ������ֶ����ָ�� //
// �������ܣ��Ӵ洢�������Ƴ��ض������ֶ���            //
// ��������ֵ���ޣ�void��                              //
// ����ע�������                                    //
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
