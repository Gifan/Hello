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

	// ���ر���
	CCSprite* pBg01 = CCSprite::create(sz_bg01);
	pBg01->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	this->addChild(pBg01, 0);

	// ����ѡ��logo������ʹ��Cache����һ��
	CCTexture2D* pImageCache = CCTextureCache::sharedTextureCache()->addImage(sz_conLogo); 
	CCSprite* pOptionLogo = CCSprite::createWithTexture(pImageCache, CCRectMake(0, 0, 134, 34));
	this->addChild(pOptionLogo);
	pOptionLogo->setPosition(ccp(winSize.width / 2, winSize.height - 60));

	// ����˵�����
	CCLabelTTF* pGoBackLb = CCLabelTTF::create(
		"Go Back",
		"΢���ź�",
		20,
		CCSizeMake(winSize.width * 0.3f, 40),
		CCTextAlignment::kCCTextAlignmentCenter,
		CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
	CCMenuItemLabel* pGoBack = CCMenuItemLabel::create(pGoBackLb, this, menu_selector(COption::goBack));

	// ��Ч����
	CCMenuItemFont::setFontName("΢���ź�");
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
	pToggleSound->setTag(CLICK_SOUND);  // ������ЧTag

	// ���õ�ǰ��Ч����
	bool bSelectId = CConfig::shareCConfig()->isSoundOn() ? SOUND_ON : SOUND_OFF;
	pToggleSound->setSelectedIndex(bSelectId);

	// �Ѷȱ���
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
	pToggleMode->setTag(CLICK_MODE);   // �����Ѷ�Tag

	// ���õ�ǰ�Ѷ�
	int iCurMode = CConfig::shareCConfig()->getLevelState();
	pToggleMode->setSelectedIndex(iCurMode-3);

	// ���ɲ˵�
	CCMenu* pMenu = CCMenu::create(pSoundTitle, pLevelTitle, pToggleSound, pToggleMode, pGoBack, NULL);
	this->addChild(pMenu);
	pMenu->setPosition(winSize.width / 2, winSize.height / 2);

	// ���ò˵��Ű�
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
	case CLICK_SOUND: // ������Ч״̬
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
	case CLICK_MODE: // �������ѵȼ�״̬
		{
			CConfig::shareCConfig()->updateLevel();
			/*int iCurLevel = CConfig::shareCConfig()->getLevelState();
			char a[30] = "Current Level State ";
			sprintf(a,"%s %d", a, iCurLevel-3);
			CCLog(a);*/
			break;
		}
	default:
		CCAssert(false, "�����ID");
		break;
	}
}