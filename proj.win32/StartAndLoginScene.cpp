#include "StartAndLoginScene.h"
USING_NS_CC;

Scene* StartAndLoginScene::CreateScene()
{
	return StartAndLoginScene::create();
}
/*
* ����������
* �������ܣ����Ǹ���init����
* ��������ֵ��
* ����ע�����
*/
bool StartAndLoginScene::init()
{
	//��ȫ�ж�
	if (!(Scene::init()))
	{
		return false;
	}
	//�õ���Ϸ����ߴ缰����
	pageSize = Director::getInstance()->getVisibleSize();
	pageCoord = Director::getInstance()->getVisibleOrigin();

	//������ͼ
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

	//��Ϸlogo
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

	//���ű�������
	startSceneBGM = SimpleAudioEngine::getInstance();
	startSceneBGM->playBackgroundMusic("res/Music/startScene_bgm.wav",true);

	
	//������Դ
	curFileNums = 0;
	loadLabel = Label::createWithTTF("0%", "fonts/Marker Felt.ttf", 50);
	loadLabel->setPosition(Vec2(pageSize.width / 2 + pageCoord.x, 1 * pageSize.height / 3 + pageCoord.y));
	this->addChild(loadLabel, 1);
	LoadResource();
	return true;
}

/*
* ����������
* �������ܣ�����resĿ¼�¸��ļ�����Դ
* ��������ֵ��
* ����ע�����
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
* ����������const std::string& path
* �������ܣ�Ԥ����pathĿ¼�µ�png�ļ�
* ��������ֵ��·����ȷΪ1������Ϊ0
* ����ע�����
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
		//�õ�����·��
		std::string contName = path;
		contName += '/';
		filename = contName + filename;
		filename += ".png";
		Director::getInstance()->getTextureCache()->addImageAsync(filename, CC_CALLBACK_1(StartAndLoginScene::LoadingCallBack, this));
	}
	return 1;
}

/*
* ����������const std::string& path
* �������ܣ�Ԥ����pathĿ¼�µ�mp3�ļ�
* ��������ֵ��·����ȷΪ1������Ϊ0
* ����ע�����
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
		//����Ϊbgm
		if (filename.find("bgm") != -1)
		{
			audio->preloadBackgroundMusic(filename.c_str());
		}
		//����Ϊeffect
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
	//�õ�ͼƬ
	auto normalImage = Sprite::create(normalPicPath);
	auto pressedImage = Sprite::create(pressedPicPath);
	//�����˵���
	auto item = MenuItemSprite::create(normalImage, pressedImage, callback);

	return item;
}



/*
* ����������Ref* pSender
* �������ܣ�����ͼƬ�Ļص�����
* ��������ֵ��
* ����ע�����������ɺ���ʾ����ʼ��Ϸ���͡��˳���Ϸ���˵�
*/
void StartAndLoginScene::LoadingCallBack(Ref* pSender)
{
	curFileNums++;
	float percentage = static_cast<float>(curFileNums) / static_cast<float>(totalFileNums);
	loadLabel->setString(StringUtils::format("%.2f %%", percentage * 100));
	//�������
	if (curFileNums == totalFileNums)
	{
		//��ʼ������ʼ��
		start = createGameButton("res/UI/PlayNormal.png", "res/UI/PlaySelected.png", CC_CALLBACK_1(StartAndLoginScene::MenuToPlayScene, this));
		Size imageSize = start->getContentSize();
		start->setPosition(Vec2(pageSize.width / 2 + pageCoord.x, 1 * pageSize.height / 4 + pageCoord.y));
		start->setScale((pageSize.width / 4) / imageSize.width);
		//�˳�������ʼ��
		exit = createGameButton("res/UI/ExitNormal.png", "res/UI/ExitSelected.png", CC_CALLBACK_1(StartAndLoginScene::MenuToExit, this));
		imageSize = exit->getContentSize();
		exit->setScale((pageSize.width / 4) / imageSize.width);
		exit->setAnchorPoint(Vec2(1, 0));
		exit->setPosition(Vec2(pageSize.width + pageCoord.x, 1 * pageSize.height / 9 + pageCoord.y));
		//�˵�������
		Menu* menus = Menu::create(start, exit, NULL);
		menus->setPosition(pageCoord);
		this->addChild(menus);
	}
}

/*
* ����������Ref* pSender
* �������ܣ���ʼ�����Ļص�����
* ��������ֵ��
* ����ע������������ת����Ϸ����
*/
void StartAndLoginScene::MenuToPlayScene(Ref* pSender)
{
	//Director::getInstance()->end();
	startSceneBGM->stopBackgroundMusic();
	Director::getInstance()->replaceScene(PlayScene::createScene());
}

/*
* ����������Ref* pSender
* �������ܣ���ʼ�����Ļص�����
* ��������ֵ��
* ����ע�����������˳���Ϸ
*/
void StartAndLoginScene::MenuToExit(Ref* pSender)
{
	Director::getInstance()->end();
}
