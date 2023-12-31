#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include<Network/Connect.h>
#include<Scene/PlayScene.h>

USING_NS_CC;

class NetworkConfigSetting : public cocos2d::Scene
{
private:
    cocos2d::ui::TextField* ipInput; // IP�����
    cocos2d::ui::TextField* portInput; // �˿������
    cocos2d::ui::CheckBox* hostCheckBox; // ����ѡ���
    //cocos2d::ui::CheckBox* clientCheckBox; // �ͻ���ѡ���
    cocos2d::ui::Button* confirmButton; // ȷ�����Ӱ�ť

    std::string ipAddress; // �����洢IP��ַ
    std::string portNumber; // �����洢�˿�

    ConPort* conPort = nullptr;

public:
    virtual bool init() override;

    void onEnter() override;
    void onExit() override;

    // ��������������UIԪ��
    cocos2d::ui::TextField* createInputField(const std::string& placeholder, const cocos2d::Vec2& position);
    void createHostSelector(const cocos2d::Vec2& position);

    // �ص�����
    void textFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type);
    void checkBoxEvent(cocos2d::Ref* sender, cocos2d::ui::CheckBox::EventType type);

    // �رհ�ť��ѡ�����ص�
    void menuCloseCallback(cocos2d::Ref* pSender);

    static NetworkConfigSetting* createScene()
    {
        // ����һ���µĳ�������
        NetworkConfigSetting* newScene = new (std::nothrow) NetworkConfigSetting();

        if (newScene && newScene->init())
        {
            // ��ʼ���ɹ��������Զ��ͷų������������ڴ�
            newScene->autorelease();
            return newScene;
        }
        else
        {
            // ��ʼ��ʧ�ܣ��ͷų���������ڴ�
            CC_SAFE_DELETE(newScene);
            return nullptr;
        }
    }


    void confirmButtonEvent(Ref* sender, cocos2d::ui::Widget::TouchEventType type);


    // �ֶ�ʵ��"static create()"����
    CREATE_FUNC(NetworkConfigSetting);
};