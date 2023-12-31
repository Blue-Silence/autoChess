/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/





#include "ConfigScene.h"
#include "SimpleAudioEngine.h"


#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;


void ConfigScene::setConfigPortal(int* playerNum) {
    this->playerNum = playerNum;
}


ConfigScene* ConfigScene::createScene()
{
    return ConfigScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in ConfigScene.cpp\n");
}

// on "init" you need to initialize your instance
bool ConfigScene::init()
{


    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float unitH = visibleSize.height / 1000;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(ConfigScene::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Config", "fonts/Marker Felt.ttf", 80*unitH);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }



    {

        auto label = Label::createWithTTF("Player Number", "fonts/Marker Felt.ttf", 52*unitH);
        if (label == nullptr)
        {
            problemLoading("'fonts/Marker Felt.ttf'");
        }
        else
        {
            // position the label on the center of the screen
            label->setPosition(Vec2(origin.x + visibleSize.width / 2,
                origin.y + visibleSize.height - label->getContentSize().height));

            // add the label as a child to this layer
            this->addChild(label, 1);
        }
        label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 313*unitH));
    }


    {
        auto slider = Slider::create();
        slider->loadBarTexture("Slider_Back.png"); // what the slider looks like
        slider->loadSlidBallTextures("SliderNode_Normal.png", "SliderNode_Press.png", "SliderNode_Disable.png");
        slider->loadProgressBarTexture("Slider_PressBar.png");


        slider->addTouchEventListener([&, slider, this](Ref* sender, Widget::TouchEventType type) {
            int opt = slider->getPercent() / 50 + (slider->getPercent() % 50 > 25 ? 1 : 0);
            switch (type)
            {
             case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                slider->setPercent(50 * opt);
                //if (this->playerNum != nullptr)
                //    *this->playerNum = opt+1;
                this->playerNumTMP = opt+1;
                break;
            default:
                break;
            }
            });

        if (slider == nullptr)
            throw "";
        slider->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 247*unitH));

        this->addChild(slider);
    }

    {
        auto label = Label::createWithTTF("1                           2                           4", "fonts/Marker Felt.ttf", 40 * unitH);
        if (label == nullptr)
        {
            problemLoading("'fonts/Marker Felt.ttf'");
        }
        else
        {
            // position the label on the center of the screen
            label->setPosition(Vec2(origin.x + visibleSize.width / 2,
                origin.y + visibleSize.height - label->getContentSize().height));

            // add the label as a child to this layer
            this->addChild(label, 1);
        }
        label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 180 * unitH));
    }




    {

        auto label = Label::createWithTTF("Music volume", "fonts/Marker Felt.ttf", 52*unitH);
        if (label == nullptr)
        {
            problemLoading("'fonts/Marker Felt.ttf'");
        }
        else
        {
            // position the label on the center of the screen
            label->setPosition(Vec2(origin.x + visibleSize.width / 2,
                origin.y + visibleSize.height - label->getContentSize().height));

            // add the label as a child to this layer
            this->addChild(label, 1);
        }
        label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 0*unitH));
    }




    {
        auto slider = Slider::create();
        slider->loadBarTexture("Slider_Back.png"); // what the slider looks like
        slider->loadSlidBallTextures("SliderNode_Normal.png", "SliderNode_Press.png", "SliderNode_Disable.png");
        slider->loadProgressBarTexture("Slider_PressBar.png");


        slider->addTouchEventListener([&, slider, this](Ref* sender, Widget::TouchEventType type) {
            switch (type)
            {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                //if (this->musicPercent != nullptr)
                //    *this->musicPercent = slider->getPercent();
                this->musicPercentTMP = slider->getPercent();
                break;
            default:
                break;
            }
            });

        if (slider == nullptr)
            throw "";
        slider->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + -77*unitH));

        this->addChild(slider);
    }

    return true;
}


void ConfigScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->popScene();


    using namespace CocosDenshion;

    auto audio = SimpleAudioEngine::getInstance();

    // setting the volume specifying value as a float
    // set default volume
    audio->setEffectsVolume(double(musicPercentTMP)/100.0);
    audio->setBackgroundMusicVolume(double(musicPercentTMP) / 100.0);

    if (this->playerNum != nullptr)
        *this->playerNum = this->playerNumTMP;
 
}

