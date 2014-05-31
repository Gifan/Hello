#include "PlaneManager.h"

CPlaneManager* g_pPlaneManager = NULL;

CPlaneManager::CPlaneManager()
{
	this->m_pPlayBullet = CCArray::create();
	this->m_pPlayBullet->retain();

	this->m_pEnemyBullet = CCArray::create();
	this->m_pEnemyBullet->retain();

	this->m_pEnemyPlanes = CCArray::create();
	this->m_pEnemyPlanes->retain();
}

CPlaneManager::~CPlaneManager()
{
	RELEASE_ARRAY(this->m_pPlayBullet);
	RELEASE_ARRAY(this->m_pEnemyBullet);
	RELEASE_ARRAY(this->m_pEnemyPlanes);

}

CPlaneManager* CPlaneManager::sharePlaneManager()
{
	if (g_pPlaneManager == NULL)
	{
		g_pPlaneManager = new CPlaneManager();
	}
	return g_pPlaneManager;
}

void CPlaneManager::initPlane(CCLayer* pLayer)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	// 加载玩家飞机
	this->m_pMyPlane = CMyPlane::create();
	pLayer->addChild(this->m_pMyPlane, 99, 1001);
	this->m_pMyPlane->setPosition(ccp(winSize.width / 2, 60));

	// 加载敌机
	for (int i = 0; i < CConfig::shareCConfig()->getLevelState(); i++)
	{
		CEnemyPlane* pEnemy = CEnemyPlane::createPlane();
		
		// 根据玩家飞机位置初始化动作
		pEnemy->initAction(this->m_pMyPlane->getPosition());
		pLayer->addChild(pEnemy, 99);
		m_pEnemyPlanes->addObject(pEnemy);
	}
}

void CPlaneManager::initExplode()
{
	// 初始化爆炸效果
	CEffect::shareExplosion();
}

void CPlaneManager::playExplode(CCLayer* pLayer, CCPoint pos)
{
    // 子弹爆炸特效
	CCSprite* pExplode = CCSprite::create(sz_hit);
    ccBlendFunc cb = {GL_SRC_ALPHA, GL_ONE };
    pExplode->setBlendFunc(cb);
    pExplode->setPosition(pos);
    pExplode->setScale(0.75f);
    pLayer->addChild(pExplode, 99);
	CCCallFuncN* pRemoveExplode =  CCCallFuncN::create(pExplode, callfuncN_selector(CBullet::removeExplode));
    pExplode->runAction(CCSequence::create(CCFadeOut::create(0.3f), pRemoveExplode, NULL));

}

void CPlaneManager::updatePlane(CCLayer* pLayer, float delta)
{
	for (unsigned int i = 0; i < this->m_pEnemyPlanes->count(); ++i) 
	{
		CEnemyPlane* pEnemy = dynamic_cast<CEnemyPlane*>(this->m_pEnemyPlanes->objectAtIndex(i));
		if (pEnemy != NULL)
		{
			if (!pEnemy->isActive())
			{
				pEnemy->destroy();
				this->m_pEnemyPlanes->removeObject(pEnemy);
				CEnemyPlane* pEnemy = CEnemyPlane::createPlane();
				pEnemy->initAction(this->m_pMyPlane->getPosition());
				pLayer->addChild(pEnemy, 99);
				this->m_pEnemyPlanes->addObject(pEnemy);
			}
		}
    }
	if (!(this->m_pMyPlane->isActive()))
	{
		this->m_pMyPlane->destroy();
		this->m_pMyPlane = NULL;
	}
}

CCArray* CPlaneManager::getPlayBullets()
{
	return this->m_pPlayBullet;
}
CCArray* CPlaneManager::getEnemyBullets()
{
	return this->m_pEnemyBullet;
}
CCArray* CPlaneManager::getEnemyPlanes()
{
	return this->m_pEnemyPlanes;
}

CMyPlane* CPlaneManager::getMyPlane()
{
	return this->m_pMyPlane;
}

void CPlaneManager::setMyPlanePos(CCPoint pos)
{
	this->m_pMyPlane->setPosition(pos);
}

void CPlaneManager::release()
{
	if (g_pPlaneManager != NULL)
	{
		delete g_pPlaneManager;
		g_pPlaneManager = NULL;
	}
}

