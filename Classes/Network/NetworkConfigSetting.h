#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include<Network/Connect.h>
#include<Scene/PlayScene.h>

USING_NS_CC;

class NetworkConfigSetting : public cocos2d::Scene
{
private:
    cocos2d::ui::TextField* ipInput; // IP输入框
    cocos2d::ui::TextField* portInput; // 端口输入框
    cocos2d::ui::CheckBox* hostCheckBox; // 主机选择框
    //cocos2d::ui::CheckBox* clientCheckBox; // 客户端选择框
    cocos2d::ui::Button* confirmButton; // 确定连接按钮

    std::string ipAddress; // 用来存储IP地址
    std::string portNumber; // 用来存储端口

    ConPort* conPort = nullptr;

public:
    virtual bool init() override;

    void onEnter() override;
    void onExit() override;

    // 辅助函数来创建UI元素
    cocos2d::ui::TextField* createInputField(const std::string& placeholder, const cocos2d::Vec2& position);
    void createHostSelector(const cocos2d::Vec2& position);

    // 回调函数
    void textFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type);
    void checkBoxEvent(cocos2d::Ref* sender, cocos2d::ui::CheckBox::EventType type);

    // 关闭按钮的选择器回调
    void menuCloseCallback(cocos2d::Ref* pSender);

    static NetworkConfigSetting* createScene()
    {
        // 创建一个新的场景对象
        NetworkConfigSetting* newScene = new (std::nothrow) NetworkConfigSetting();

        if (newScene && newScene->init())
        {
            // 初始化成功，启用自动释放池来管理场景的内存
            newScene->autorelease();
            return newScene;
        }
        else
        {
            // 初始化失败，释放场景对象的内存
            CC_SAFE_DELETE(newScene);
            return nullptr;
        }
    }


    void confirmButtonEvent(Ref* sender, cocos2d::ui::Widget::TouchEventType type);


    // 手动实现"static create()"方法
    CREATE_FUNC(NetworkConfigSetting);
};