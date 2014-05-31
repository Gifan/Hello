#include "MainMenu.h"
#include "resource.h"
#include "ConstantRes.h"
#include "SimpleAudioEngine.h"
#include "AboutScene.h"
#include "OptionScene.h"
#include "GameScene.h"

USING_NS_CC;
using namespace CocosDenshion;

CCScene* CMainMenu::scene()
{
	CCScene* pScene = CCScene::create();

	CMainMenu* pLayer = CMainMenu::create();

	pScene->addChild(pLayer);

	return pScene;
}

bool CMainMenu::init()
{

	// 初始化父类
	if (!CCLayer::init())
	{
		return false;
	}

	this->m_winSize = CCDirector::sharedDirector()->getWinSize();

	// 加载飞机
	this->m_myPlane = CCSprite::create(sz_myPlane, CCRectMake(0, 0, 60, 46));
	this->addChild(this->m_myPlane, 1);
	this->m_myPlane->setPosition(
		ccp(CCRANDOM_0_1() * this->m_winSize.width, this->m_winSize.height + 10));

	// 加载logo
	CCSprite* pSprite = CCSprite::create(sz_logo);
	this->addChild(pSprite, 2);
	pSprite->setPosition(ccp(this->m_winSize.width / 2, this->m_winSize.height - 150
		));

	// 获取可视区域原点
	CCSize visSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// 加载菜单精灵
	CCSprite* pNewGameNormal = CCSprite::create(sz_menu, CCRectMake(0 , 0, 126, 33));
	CCSprite* pNewGameSelect = CCSprite::create(sz_menu, CCRectMake(0, 33, 126, 33));
	CCSprite* pNewGameDisable = CCSprite::create(sz_menu, CCRectMake(0, 66, 126, 33 ));

	CCSprite* pGameSettingNormal = CCSprite::create(sz_menu, CCRectMake(126, 0, 126, 33));
	CCSprite* pGameSettingSelect = CCSprite::create(sz_menu, CCRectMake(126, 33, 126, 33));
	CCSprite* pGameSettingDisable = CCSprite::create(sz_menu, CCRectMake(126, 66, 126, 33));

	CCSprite* pAboutNormal = CCSprite::create(sz_menu, CCRectMake(252, 0, 126, 33));
	CCSprite* pAboutSelect = CCSprite::create(sz_menu, CCRectMake(252, 33, 126, 33));
	CCSprite* pAboutDisable = CCSprite::create(sz_menu, CCRectMake(252, 66, 126, 33));

	// 创建菜单项
	CCMenuItemSprite* pNewGame = CCMenuItemSprite::create(
		pNewGameNormal,
		pNewGameSelect,
		pNewGameDisable,
		this,
		menu_selector(CMainMenu::startEffect));

	CCMenuItemSprite* pGameSetting = CCMenuItemSprite::create(
		pGameSettingNormal,
		pGameSettingSelect,
		pGameSettingDisable,
		this,
		menu_selector(CMainMenu::menuCallback));
	pGameSetting->setTag(CLICK_OPTION);

	CCMenuItemSprite* pAbout = CCMenuItemSprite::create(
		pAboutNormal,
		pAboutSelect,
		pAboutDisable,
		this,
		menu_selector(CMainMenu::menuCallback));
	pAbout->setTag(CLICK_ABOUT);
	
	CCMenu* pMenu = CCMenu::create(pNewGame, pGameSetting, pAbout, NULL);
	// 设置垂直间隔排列
	pMenu->alignItemsVerticallyWithPadding(10);

	this->addChild(pMenu, 3, 20);

	pMenu->setPosition(this->m_winSize.width / 2, this->m_winSize.height / 2 - 40);

	// 加载背景
	CCSprite* pBg01 = CCSprite::create(sz_bg01);

	pBg01->setPosition(ccp(
		visSize.width / 2+origin.x,
		visSize.height / 2 + origin.y)
		);

	// 加载背景音乐
	if (CConfig::shareCConfig()->isSoundOn())
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(sz_mainMusic, true);


	this->addChild(pBg01, 0);

	// 启动定时器
	this->scheduleUpdate();

	return true;
}


// 根据当前声音状态播放按钮声效
void CMainMenu::onButtonEffect()
{
	if (CConfig::shareCConfig()->isSoundOn())
		SimpleAudioEngine::sharedEngine()->playEffect(sz_btnEffect);

}

// 开始游戏特效
void CMainMenu::startEffect(CCObject* pSender)
{
	this->onButtonEffect();
	this->newGame();
}

// 开始游戏
void CMainMenu::newGame()
{
	CCScene* pScene = CGameLayer::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.8f, pScene));

}

// 菜单选项回调
void CMainMenu::menuCallback(CCObject* pSender)
{
	this->onButtonEffect();

	int iTag = dynamic_cast<CCNode*>(pSender)->getTag();

	switch(iTag)
	{
	case CLICK_OPTION:  // 转向OPTION场景
		{
			CCScene* pScene = COption::scene();
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.8f, pScene));
			break;
		}
	case CLICK_ABOUT:   // 转向ABOUT场景
		{
			CCScene* pScene = CAbout::scene();
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.8f, pScene));
			break;
		}
	default:
		CCAssert(false, "错误ID");
		break;
	}
}

void CMainMenu::update(float delta)
{
	if (this->m_myPlane->getPosition().y > this->m_winSize.height)
	{
		CCPoint pos = ccp(CCRANDOM_0_1() * this->m_winSize.width, 10);
		this->m_myPlane->setPosition(pos);
		this->m_myPlane->runAction(CCMoveBy::create(
			5 * CCRANDOM_0_1() + 1,
			ccp(CCRANDOM_0_1() * this->m_winSize.width, this->m_winSize.height + pos.y)
			));
	}

}

void CMainMenu::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
}

