#include "StartAndLoginScene.h"
USING_NS_CC;

Scene* StartAndLoginScene::CreateScene()
{
	return StartAndLoginScene::create();
}
/*
* 函数参数：
* 函数功能：覆盖父类init函数
* 函数返回值：
* 函数注意事项：
*/
bool StartAndLoginScene::init()
{
	//安全判断
	if (!(Scene::init()))
	{
		return false;
	}
	//得到游戏界面尺寸及坐标
	pageSize = Director::getInstance()->getVisibleSize();
	pageCoord = Director::getInstance()->getVisibleOrigin();

	//贴背景图
	auto background = Sprite::create("res/Background/InitalBackground.png");
	if (background == nullptr)
	{
		return false;
	}
	else
	{
		auto imageSize = background->getContentSize();
		background->setScaleX(pageSize.width / imageSize.width);
		background->setScaleY(pageSize.height / imageSize.height);
		background->setPosition(Vec2(pageSize.width / 2 + pageCoord.x, pageSize.height / 2 + pageCoord.y));
		this->addChild(background);
	}

	//游戏logo
	auto gameLogo = Sprite::create("res/Background/gameLogo.png");
	if (gameLogo == nullptr)
	{
		return false;
	}
	else
	{
		auto imageSize = gameLogo->getContentSize();
		gameLogo->setPosition(Vec2(pageSize.width / 2 + pageCoord.x, 2 * pageSize.height / 3 + pageCoord.y));
		gameLogo->setScale((pageSize.width / 2) / imageSize.width);
		this->addChild(gameLogo);
	}

	//播放背景音乐
	startSceneBGM = SimpleAudioEngine::getInstance();
	startSceneBGM->playBackgroundMusic("res/Music/startScene_bgm.wav",true);

	
	//加载资源
	curFileNums = 0;
	loadLabel = Label::createWithTTF("0%", "fonts/Marker Felt.ttf", 50);
	loadLabel->setPosition(Vec2(pageSize.width / 2 + pageCoord.x, 1 * pageSize.height / 3 + pageCoord.y));
	this->addChild(loadLabel, 1);
	LoadResource();
	return true;
}

/*
* 函数参数：
* 函数功能：加载res目录下各文件夹资源
* 函数返回值：
* 函数注意事项：
*/
void StartAndLoginScene::LoadResource()
{
	GetAllMusic("res/MUSIC");
	GetAllpng("res/Background");
	GetAllpng("res/Bond");
	GetAllpng("res/Hero");
	GetAllpng("res/Effect");
	GetAllpng("res/UI");
}

/*
* 函数参数：const std::string& path
* 函数功能：预加载path目录下的png文件
* 函数返回值：路径正确为1，错误为0
* 函数注意事项：
*/
bool StartAndLoginScene::GetAllpng(const std::string& path)
{
	if (!exists(path))
	{
		return 0;
	}
	directory_iterator list(path);
	for (auto& it : list)
	{
		std::string filename = it.path().stem().string();
		//得到完整路径
		std::string contName = path;
		contName += '/';
		filename = contName + filename;
		filename += ".png";
		Director::getInstance()->getTextureCache()->addImageAsync(filename, CC_CALLBACK_1(StartAndLoginScene::LoadingCallBack, this));
	}
	return 1;
}

/*
* 函数参数：const std::string& path
* 函数功能：预加载path目录下的mp3文件
* 函数返回值：路径正确为1，错误为0
* 函数注意事项：
*/
bool StartAndLoginScene::GetAllMusic(const std::string& path)
{
	if (!exists(path))
	{
		return 0;
	}
	directory_iterator list(path);
	auto audio = SimpleAudioEngine::getInstance();
	for (auto& it : list)
	{
		std::string filename = it.path().stem().string();
		std::string contName = path;
		contName += '/';
		filename = contName + filename;
		filename += ".wav";
		//加载为bgm
		if (filename.find("bgm") != -1)
		{
			audio->preloadBackgroundMusic(filename.c_str());
		}
		//加载为effect
		else if (filename.find("effect") != -1)
		{
			audio->preloadBackgroundMusic(filename.c_str());
		}
		curFileNums++;
	}
	return 1;
}

MenuItemSprite* StartAndLoginScene::createGameButton(std::string normalPicPath, std::string pressedPicPath, const ccMenuCallback& callback)
{
	//得到图片
	auto normalImage = Sprite::create(normalPicPath);
	auto pressedImage = Sprite::create(pressedPicPath);
	//创建菜单项
	auto item = MenuItemSprite::create(normalImage, pressedImage, callback);

	return item;
}



/*
* 函数参数：Ref* pSender
* 函数功能：加载图片的回调函数
* 函数返回值：
* 函数注意事项：加载完成后显示“开始游戏”和“退出游戏”菜单
*/
void StartAndLoginScene::LoadingCallBack(Ref* pSender)
{
	curFileNums++;
	float percentage = static_cast<float>(curFileNums) / static_cast<float>(totalFileNums);
	loadLabel->setString(StringUtils::format("%.2f %%", percentage * 100));
	//加载完成
	if (curFileNums == totalFileNums)
	{
		//开始按键初始化
		start = createGameButton("res/UI/PlayNormal.png", "res/UI/PlaySelected.png", CC_CALLBACK_1(StartAndLoginScene::MenuToPlayScene, this));
		Size imageSize = start->getContentSize();
		start->setPosition(Vec2(pageSize.width / 2 + pageCoord.x, 1 * pageSize.height / 4 + pageCoord.y));
		start->setScale((pageSize.width / 4) / imageSize.width);
		//退出按键初始化
		exit = createGameButton("res/UI/ExitNormal.png", "res/UI/ExitSelected.png", CC_CALLBACK_1(StartAndLoginScene::MenuToExit, this));
		imageSize = exit->getContentSize();
		exit->setScale((pageSize.width / 4) / imageSize.width);
		exit->setAnchorPoint(Vec2(1, 0));
		exit->setPosition(Vec2(pageSize.width + pageCoord.x, 1 * pageSize.height / 9 + pageCoord.y));
		//菜单化按键
		Menu* menus = Menu::create(start, exit, NULL);
		menus->setPosition(pageCoord);
		this->addChild(menus);
	}
}

/*
* 函数参数：Ref* pSender
* 函数功能：开始按键的回调函数
* 函数返回值：
* 函数注意事项：点击后跳转至游戏界面
*/
void StartAndLoginScene::MenuToPlayScene(Ref* pSender)
{
	//Director::getInstance()->end();
	startSceneBGM->stopBackgroundMusic();
	Director::getInstance()->replaceScene(PlayScene::createScene());
}

/*
* 函数参数：Ref* pSender
* 函数功能：开始按键的回调函数
* 函数返回值：
* 函数注意事项：点击后退出游戏
*/
void StartAndLoginScene::MenuToExit(Ref* pSender)
{
	Director::getInstance()->end();
}
