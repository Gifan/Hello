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
	// ��������
	this->setTouchEnabled(true);
	
	CConfig::shareCConfig()->resetCConfig();

	this->m_winSize = CCDirector::sharedDirector()->getWinSize();
	// ��ʼ����������
	if (CConfig::shareCConfig()->isSoundOn())
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(sz_gameMusic, true);
	// ��ʼ���ɻ�
	CPlaneManager::sharePlaneManager()->initPlane(this);
	// ��ʼ���ɻ���ըЧ��
	CPlaneManager::sharePlaneManager()->initExplode();
	// ��ʼ��UI
	this->initUI();
	// ��ʼ��������Ư����
	this->initBackground();
	// ������ʱ��
	this->scheduleUpdate();
	return true;
}

void CGameLayer::initUI()
{
	CSceneManager::shareSceneManager()->initUI(this);
	// ����˵�����
	CCLabelTTF* pGoBackLb = CCLabelTTF::create(
		"Main Menu",
		"΢���ź�",
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

//���»���
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

// �������������ײ
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
	// ���ö�ʱ���Թ�������
	this->schedule(schedule_selector(CGameLayer::updateBackground),3);

}

void CGameLayer::updateBackground(float delta)
{
	CSceneManager::shareSceneManager()->updateBackground(this, delta);
}
// ��Ϸ�����ص�����
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
		// �޸ĵ�ǰ�ɻ�λ��
		CCPoint pos = pTouch->getDelta();
		CCPoint currentPos = CPlaneManager::sharePlaneManager()->getMyPlane()->getPosition();
		currentPos = ccpAdd(currentPos, pos);

		// ��סֻ������Ļ����Χ�ƶ�
		currentPos = ccpClamp(currentPos, CCPointZero, ccp(this->m_winSize.width, this->m_winSize.height));
		CPlaneManager::sharePlaneManager()->setMyPlanePos(currentPos);
	}
}

void CGameLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{

}

// �ֱ�ע���ע��ϵͳ�Ĵ����¼�
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