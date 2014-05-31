#include <vld.h>
#include "GameScene.h"
#include "ConstantRes.h"
#include "Config.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
CGameLayer::CGameLayer():m_iState(sStatePlaying)
{
}

CGameLayer::~CGameLayer()
{ 
	if (CSceneManager::shareSceneManager() != NULL)
	{
		CSceneManager::shareSceneManager()->release();

	}
}

CCScene* CGameLayer::scene()
{
	CCScene* pScene = CCScene::create();
	CGameLayer* pLayer = CGameLayer::create();
	pScene->addChild(pLayer);
	return pScene;
}

bool CGameLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	// 开启触摸
	this->setTouchEnabled(true);
	
	CConfig::shareCConfig()->resetCConfig();

	this->m_winSize = CCDirector::sharedDirector()->getWinSize();
	// 初始化背景音乐
	if (CConfig::shareCConfig()->isSoundOn())
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(sz_gameMusic, true);
	// 初始化飞机
	CPlaneManager::sharePlaneManager()->initPlane(this);
	// 初始化飞机爆炸效果
	CPlaneManager::sharePlaneManager()->initExplode();
	// 初始化UI
	this->initUI();
	// 初始化背景及漂浮物
	this->initBackground();
	// 启动定时器
	this->scheduleUpdate();
	return true;
}

void CGameLayer::initUI()
{
	CSceneManager::shareSceneManager()->initUI(this);
	// 字体菜单创建
	CCLabelTTF* pGoBackLb = CCLabelTTF::create(
		"Main Menu",
		"微软雅黑",
		15,
		CCSizeMake(100, 20),
		CCTextAlignment::kCCTextAlignmentCenter,
		CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
	CCMenuItemLabel* pGoMenu = CCMenuItemLabel::create(pGoBackLb, this, menu_selector(CGameLayer::mainMenu));

	CCMenu* pMenu = CCMenu::create(pGoMenu, NULL);
	this->addChild(pMenu, 100);
	pMenu->setAnchorPoint(ccp(0, 0));
	pMenu->setPosition(this->m_winSize.width - 50, 20);
}

void CGameLayer::updateUI()
{
	CSceneManager::shareSceneManager()->updateUI();
}


void CGameLayer::mainMenu(CCObject* pSender)
{
	CCScene* pScene = CMainMenu::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.8f, pScene));

}

//更新画面
void CGameLayer::update(float delta)
{
	if (this->m_iState == sStatePlaying)
	{
		this->checkAllIsCollide();
		this->updateUI();
		CPlaneManager::sharePlaneManager()->updatePlane(this, delta);

		if (this->isGameOver())
		{
			this->m_iState = sStateGameOver;
			CCCallFunc* pGameOver = CCCallFunc::create(this, callfunc_selector(CGameLayer::gameOver));
			this->runAction(CCSequence::create(CCDelayTime::create(2.0f), pGameOver, NULL));
		}
	}
}

// 检测所有物体碰撞
void CGameLayer::checkAllIsCollide()
{
	CSceneManager::shareSceneManager()->checkAllIsCollide(this);
}

bool CGameLayer::isGameOver()
{
	if (CPlaneManager::sharePlaneManager()->getMyPlane() == NULL) return true;
	if (!(CPlaneManager::sharePlaneManager()->getMyPlane()->isActive()))
		return true;
	else
		return false;
}

void CGameLayer::initBackground()
{
	CSceneManager::shareSceneManager()->initBackground(this);
	// 设置定时器以滚动背景
	this->schedule(schedule_selector(CGameLayer::updateBackground),3);

}

void CGameLayer::updateBackground(float delta)
{
	CSceneManager::shareSceneManager()->updateBackground(this, delta);
}
// 游戏结束回调函数
void CGameLayer::gameOver()
{
	CCScene* pScene = CGameOver::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.8f, pScene));
}

bool CGameLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	return true;
}

void CGameLayer::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	if ((this->m_iState == sStatePlaying) && CPlaneManager::sharePlaneManager()->getMyPlane() != NULL)
	{
		// 修改当前飞机位置
		CCPoint pos = pTouch->getDelta();
		CCPoint currentPos = CPlaneManager::sharePlaneManager()->getMyPlane()->getPosition();
		currentPos = ccpAdd(currentPos, pos);

		// 锁住只能在屏幕区域范围移动
		currentPos = ccpClamp(currentPos, CCPointZero, ccp(this->m_winSize.width, this->m_winSize.height));
		CPlaneManager::sharePlaneManager()->setMyPlanePos(currentPos);
	}
}

void CGameLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{

}

// 分别注册和注销系统的触摸事件
void CGameLayer::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCLayer::onEnter();
}
void CGameLayer::onExit()
{
	CPlaneManager::sharePlaneManager()->release();
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}