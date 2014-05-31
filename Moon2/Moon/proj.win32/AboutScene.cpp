#include "AboutScene.h"
#include "ConstantRes.h"
#include "MainMenu.h"
#include "Config.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

CCScene* CAbout::scene()
{
	CCScene* pScene = CCScene::create();

	CAbout* pLayer = CAbout::create();

	pScene->addChild(pLayer);

	return pScene;
}

bool CAbout::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	// 加载背景
	CCSprite* pBg01 = CCSprite::create(sz_bg01);

	pBg01->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	this->addChild(pBg01, 0);

	// 加载选项logo，尝试使用Cache加载一次
	CCTexture2D* pImageCache = CCTextureCache::sharedTextureCache()->addImage(sz_conLogo); 
	CCSprite* pAboutLogo = CCSprite::createWithTexture(pImageCache, CCRectMake(0, 36, 100, 34));
	this->addChild(pAboutLogo);
	pAboutLogo->setPosition(ccp(winSize.width / 2, winSize.height - 60));

	// about说明
	CCLabelTTF* pDescAbout = CCLabelTTF::create(
		"Test About",
		"Test Font",
		18,
		CCSizeMake(winSize.width * 0.85, 320),
		CCTextAlignment::kCCTextAlignmentCenter);
	this->addChild(pDescAbout, 1);
//	pDescAbout->setAnchorPoint(ccp(0.5f,0.5f));
	pDescAbout->setPosition(ccp(winSize.width/2, winSize.height/2 - 20));

	// 字体菜单创建
	CCLabelTTF* pGoBackLb = CCLabelTTF::create(
		"Go Back",
		"微软雅黑",
		20,
		CCSizeMake(winSize.width*0.3f, 40),
		CCTextAlignment::kCCTextAlignmentCenter,
		CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
    CCMenuItemLabel* pGoBack = CCMenuItemLabel::create(pGoBackLb, this, menu_selector(CAbout::goBack));

	// 加载背景音乐
	//if (CConfig::shareCConfig()->isSoundOn())
	//	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(sz_mainMusic, true);

 //   pGoBack->runAction(CCRepeatForever::create(seq));
    
    CCMenu* pMenu = CCMenu::create(pGoBack, NULL);
    this->addChild(pMenu);
	pMenu->setPosition(winSize.width / 2, 50);

	return true;
}

void CAbout::goBack(CCObject* pSender)
{
	if (CConfig::shareCConfig()->isSoundOn())
		SimpleAudioEngine::sharedEngine()->playEffect(sz_btnEffect);
	CCScene* pScene = CMainMenu::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.8f, pScene));

}
