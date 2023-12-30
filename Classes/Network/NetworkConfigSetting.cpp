#include<Network/NetworkConfigSetting.h>



bool NetworkConfigSetting::init()
{
    if (!Scene::init())
    {
        return false;
    }

    // 初始化IP输入框
    ipInput = createInputField("Please input IP:", Vec2(480, 320)); // 根据需要调整位置
    this->addChild(ipInput);


    // 初始化端口输入框
    portInput = createInputField("Please input port:", Vec2(480, 240)); // 根据需要调整位置
    portInput->setTextColor(Color4B::WHITE); // 设置文字颜色为白色
    this->addChild(portInput);

    // 初始化主机选择器
    createHostSelector(Vec2(480, 160)); // 根据需要调整位置

    //// 初始化确定连接按钮
    confirmButton = cocos2d::ui::Button::create("res/UI/confirm.png", "res/UI/confirm.png");
    confirmButton->setScale(0.1);
    confirmButton->setPosition(Vec2(480, 80)); // 根据需要调整位置
    confirmButton->addTouchEventListener(CC_CALLBACK_2(NetworkConfigSetting::confirmButtonEvent, this));
    this->addChild(confirmButton);


    return true;
}

cocos2d::ui::TextField* NetworkConfigSetting::createInputField(const std::string& placeholder, const cocos2d::Vec2& position)
{
    auto textField = cocos2d::ui::TextField::create(placeholder, "Arial", 24);
    textField->setPosition(position);
    textField->addEventListener(CC_CALLBACK_2(NetworkConfigSetting::textFieldEvent, this));
    return textField;
}


// 确定连接按钮的事件处理函数
void NetworkConfigSetting::confirmButtonEvent(Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        // 在这里处理用户点击确认连接后的逻辑
        // 比如获取输入框中的IP和端口，检查主机/客户端选择器的状态，然后尝试连接或执行其他操作
        delete this->conPort;
        this->conPort = new  ConPort(ipInput->getString(), std::atoi(portInput->getString().c_str()));
        this->conPort->connect(hostCheckBox->getSelectedState());
        if (!this->conPort->checkValid())
            throw "";
    }
}

void NetworkConfigSetting::onEnter()
{
    Scene::onEnter();
    // 如果需要，这里添加更多进入场景时的代码
}

void NetworkConfigSetting::onExit()
{
    Scene::onExit();
    // 如果需要，这里添加更多退出场景时清理的代码
}



void NetworkConfigSetting::createHostSelector(const cocos2d::Vec2& position)
{
    // 主机复选框
    hostCheckBox = cocos2d::ui::CheckBox::create("res/UI/noSelected.png", "res/UI/selected.png");
    hostCheckBox->setScale(0.1);
    hostCheckBox->setPosition(position + Vec2(-50, 0)); // 根据需要调整位置
    hostCheckBox->addEventListener(CC_CALLBACK_2(NetworkConfigSetting::checkBoxEvent, this));
    this->addChild(hostCheckBox);
}

void NetworkConfigSetting::textFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type)
{
    if (type == cocos2d::ui::TextField::EventType::INSERT_TEXT ||
        type == cocos2d::ui::TextField::EventType::DELETE_BACKWARD)
    {
        auto textField = dynamic_cast<cocos2d::ui::TextField*>(sender);
        if (textField == nullptr)
            return;

        // 根据输入框的标识来决定更新哪个变量
        if (textField == ipInput)
        {
            // 更新IP地址变量
            ipAddress = textField->getString();
        }
        else if (textField == portInput)
        {
            // 更新端口号变量
            portNumber = textField->getString();
        }
    }
}
   

void NetworkConfigSetting::checkBoxEvent(cocos2d::Ref * sender, cocos2d::ui::CheckBox::EventType type)
{
    if (type == cocos2d::ui::CheckBox::EventType::SELECTED)
    {
        auto checkBox = dynamic_cast<cocos2d::ui::CheckBox*>(sender);
        // 在这里可以处理复选框的选择，确保主机/客户端只有一个被选中
        if (checkBox == hostCheckBox)
        {
            hostCheckBox->setSelected(true);
            // 设置为主机
        }   
    }
}

void NetworkConfigSetting::menuCloseCallback(cocos2d::Ref* pSender)
{
    // 处理关闭事件，可能是将这个场景从堆栈中弹出
    Director::getInstance()->popScene();
}