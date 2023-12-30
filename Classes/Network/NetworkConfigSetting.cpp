#include<Network/NetworkConfigSetting.h>



bool NetworkConfigSetting::init()
{
    if (!Scene::init())
    {
        return false;
    }

    // ��ʼ��IP�����
    ipInput = createInputField("Please input IP:", Vec2(480, 320)); // ������Ҫ����λ��
    this->addChild(ipInput);


    // ��ʼ���˿������
    portInput = createInputField("Please input port:", Vec2(480, 240)); // ������Ҫ����λ��
    portInput->setTextColor(Color4B::WHITE); // ����������ɫΪ��ɫ
    this->addChild(portInput);

    // ��ʼ������ѡ����
    createHostSelector(Vec2(480, 160)); // ������Ҫ����λ��

    //// ��ʼ��ȷ�����Ӱ�ť
    confirmButton = cocos2d::ui::Button::create("res/UI/confirm.png", "res/UI/confirm.png");
    confirmButton->setScale(0.1);
    confirmButton->setPosition(Vec2(480, 80)); // ������Ҫ����λ��
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


// ȷ�����Ӱ�ť���¼�������
void NetworkConfigSetting::confirmButtonEvent(Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        // �����ﴦ���û����ȷ�����Ӻ���߼�
        // �����ȡ������е�IP�Ͷ˿ڣ��������/�ͻ���ѡ������״̬��Ȼ�������ӻ�ִ����������
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
    // �����Ҫ��������Ӹ�����볡��ʱ�Ĵ���
}

void NetworkConfigSetting::onExit()
{
    Scene::onExit();
    // �����Ҫ��������Ӹ����˳�����ʱ����Ĵ���
}



void NetworkConfigSetting::createHostSelector(const cocos2d::Vec2& position)
{
    // ������ѡ��
    hostCheckBox = cocos2d::ui::CheckBox::create("res/UI/noSelected.png", "res/UI/selected.png");
    hostCheckBox->setScale(0.1);
    hostCheckBox->setPosition(position + Vec2(-50, 0)); // ������Ҫ����λ��
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

        // ���������ı�ʶ�����������ĸ�����
        if (textField == ipInput)
        {
            // ����IP��ַ����
            ipAddress = textField->getString();
        }
        else if (textField == portInput)
        {
            // ���¶˿ںű���
            portNumber = textField->getString();
        }
    }
}
   

void NetworkConfigSetting::checkBoxEvent(cocos2d::Ref * sender, cocos2d::ui::CheckBox::EventType type)
{
    if (type == cocos2d::ui::CheckBox::EventType::SELECTED)
    {
        auto checkBox = dynamic_cast<cocos2d::ui::CheckBox*>(sender);
        // ��������Դ���ѡ���ѡ��ȷ������/�ͻ���ֻ��һ����ѡ��
        if (checkBox == hostCheckBox)
        {
            hostCheckBox->setSelected(true);
            // ����Ϊ����
        }   
    }
}

void NetworkConfigSetting::menuCloseCallback(cocos2d::Ref* pSender)
{
    // ����ر��¼��������ǽ���������Ӷ�ջ�е���
    Director::getInstance()->popScene();
}