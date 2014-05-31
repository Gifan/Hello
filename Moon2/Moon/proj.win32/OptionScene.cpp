#include "OptionScene.h"
#include "MainMenu.h"
#include "ConstantRes.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CCScene* COption::scene()
{
	CCScene* pScene = CCScene::create();

	COption* pOption = COption::create();

	pScene->addChild(pOption);

	return pScene;
}

bool COption::init()
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
	CCSprite* pOptionLogo = CCSprite::createWithTexture(pImageCache, CCRectMake(0, 0, 134, 34));
	this->addChild(pOptionLogo);
	pOptionLogo->setPosition(ccp(winSize.width / 2, winSize.height - 60));

	// 字体菜单创建
	CCLabelTTF* pGoBackLb = CCLabelTTF::create(
		"Go Back",
		"微软雅黑",
		20,
		CCSizeMake(winSize.width * 0.3f, 40),
		CCTextAlignment::kCCTextAlignmentCenter,
		CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
	CCMenuItemLabel* pGoBack = CCMenuItemLabel::create(pGoBackLb, this, menu_selector(COption::goBack));

	// 音效标题
	CCMenuItemFont::setFontName("微软雅黑");
	CCMenuItemFont::setFontSize(18);
	CCMenuItemFont *pSoundTitle = CCMenuItemFont::create("Sound");
	pSoundTitle->setEnabled(false);

	CCMenuItemFont::setFontSize(26);
	CCMenuItemToggle* pToggleSound = CCMenuItemToggle::createWithTarget(
		this,
		menu_selector(COption::setOption),
		CCMenuItemFont::create("Off"),
		CCMenuItemFont::create("On"),
		NULL);
	pToggleSound->setTag(CLICK_SOUND);  // 设置声效Tag

	// 设置当前声效开关
	bool bSelectId = CConfig::shareCConfig()->isSoundOn() ? SOUND_ON : SOUND_OFF;
	pToggleSound->setSelectedIndex(bSelectId);

	// 难度标题
	CCMenuItemFont::setFontSize(18);
	CCMenuItemFont *pLevelTitle = CCMenuItemFont::create("Mode");
    pLevelTitle->setFontSize(18);
	pLevelTitle->setEnabled(false);

	CCMenuItemFont::setFontSize(26);
	CCMenuItemToggle* pToggleMode = CCMenuItemToggle::createWithTarget(
		this,
		menu_selector(COption::setOption),
		CCMenuItemFont::create("Easy"),
		CCMenuItemFont::create("Normal"),
		CCMenuItemFont::create("Hard"),
		NULL);
	pToggleMode->setTag(CLICK_MODE);   // 设置难度Tag

	// 设置当前难度
	int iCurMode = CConfig::shareCConfig()->getLevelState();
	pToggleMode->setSelectedIndex(iCurMode-3);

	// 生成菜单
	CCMenu* pMenu = CCMenu::create(pSoundTitle, pLevelTitle, pToggleSound, pToggleMode, pGoBack, NULL);
	this->addChild(pMenu);
	pMenu->setPosition(winSize.width / 2, winSize.height / 2);

	// 设置菜单排版
	pMenu->alignItemsInColumns(2, 2, 1);
	pGoBack->setPositionY(pGoBack->getPositionY() - 150);

	return true;
}

void COption::goBack(CCObject* pSender)
{
	if (CConfig::shareCConfig()->isSoundOn())
		SimpleAudioEngine::sharedEngine()->playEffect(sz_btnEffect);
	CCScene* pScene = CMainMenu::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.8f, pScene));

}

void COption::setOption(CCObject* pSender)
{
	int iTag = dynamic_cast<CCMenuItemToggle*>(pSender)->getTag();

	switch(iTag)
	{
	case CLICK_SOUND: // 更新声效状态
		{
			CConfig::shareCConfig()->updateSound();
			/*if (CConfig::shareCConfig()->isSoundOn())
				CCLog("current sound state On");
			else CCLog("current sound state Off");*/
			if (CConfig::shareCConfig()->isSoundOn())
			{
				SimpleAudioEngine::sharedEngine()->resumeAllEffects();
				SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
			}
			else
			{
				SimpleAudioEngine::sharedEngine()->pauseAllEffects();
				SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
			}
			break;
		}
	case CLICK_MODE: // 更新困难等级状态
		{
			CConfig::shareCConfig()->updateLevel();
			/*int iCurLevel = CConfig::shareCConfig()->getLevelState();
			char a[30] = "Current Level State ";
			sprintf(a,"%s %d", a, iCurLevel-3);
			CCLog(a);*/
			break;
		}
	default:
		CCAssert(false, "错误的ID");
		break;
	}
}