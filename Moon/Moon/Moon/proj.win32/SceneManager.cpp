#include "SceneManager.h"

CSceneManager* g_pSceneManager = NULL;

CSceneManager::CSceneManager() : m_bIsBgLoad(false), m_bIsFloaterLoad(false)
{
	this->m_winSize = CCDirector::sharedDirector()->getWinSize();
	this->m_screenRect = CCRectMake(0, 0, this->m_winSize.width, this->m_winSize.height);
}

CSceneManager::~CSceneManager()
{

}

CSceneManager* CSceneManager::shareSceneManager()
{
	if (g_pSceneManager == NULL)
	{
		g_pSceneManager = new CSceneManager();
	}
	return g_pSceneManager;
}

void CSceneManager::initUI(CCLayer* pLayer)
{
	// �����������
	this->m_pScore = CCLabelTTF::create(
		"Score:0",
		"΢���ź�",
		15,
		CCSizeMake(100, 30),
		CCTextAlignment::kCCTextAlignmentCenter,
		CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
	this->m_pScore->setAnchorPoint(ccp(0.5, 0));
	pLayer->addChild(this->m_pScore, 100);
	this->m_pScore->setPosition(ccp(this->m_winSize.width-60, this->m_winSize.height - 30));

	// �����ҷɻ�UI
	CCTexture2D* pPlaneTexture = CCTextureCache::sharedTextureCache()->addImage(sz_myplane);
	CCSprite* pLife = CCSprite::createWithTexture(pPlaneTexture, CCRectMake(0, 0, 60, 38));
	pLife->setScale(0.6f);
	pLife->setPosition(ccp(30,this->m_winSize.height-23));
	pLayer->addChild(pLife, 100);

	// �����ɻ�����ֵ
	char life[3];
	sprintf(life, "%d", CPlaneManager::sharePlaneManager()->getMyPlane()->getHP());
	this->m_pLifeCount = CCLabelTTF::create(
		life,
		"΢���ź�",
		15,
		CCSizeMake(30, 30),
		CCTextAlignment::kCCTextAlignmentCenter,
		CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
	pLayer->addChild(this->m_pLifeCount, 100);
	this->m_pLifeCount->setPosition(ccp(60, this->m_winSize.height-23));
	this->m_pLifeCount->setColor(ccc3(255, 0, 0));
}


void CSceneManager::updateUI()
{
	// ���·���
	char s[30] = "Score:";
	sprintf(s, "%s%d", s, CConfig::shareCConfig()->getScoreVal());
	this->m_pScore->setString(s);

	// ���·ɻ�ʣ������ֵ
	char life[3];
	sprintf(life, "%d", CPlaneManager::sharePlaneManager()->getMyPlane()->getHP());
	this->m_pLifeCount->setString(life);
}

void CSceneManager::initBackground(CCLayer* pLayer)
{
	// ���ر���ͼƬ
	this->m_pBg = CCSprite::create(sz_b01);
	this->m_pBg2 = CCSprite::create(sz_b01);
	this->m_pBg2->setAnchorPoint(ccp(0, 0));
	this->m_pBg->setAnchorPoint(ccp(0, 0));
	this->m_pBgHeight = this->m_pBg->getContentSize().height;
	pLayer->addChild(m_pBg, 0);

	// ����Ư����
	this->m_pFloaterMap = CCTMXTiledMap::create(sz_level01);
	pLayer->addChild(this->m_pFloaterMap, 1);
	float height = this->m_pFloaterMap->getMapSize().height;
	this->m_pFloaterHeight = height * height;

	this->m_pBgHeight -= 48;
	this->m_pFloaterHeight -= 200;
	this->m_pBg->runAction(CCMoveBy::create(3, ccp(0, 0-BG_MOVESPEED)));
	this->m_pFloaterMap->runAction(CCMoveBy::create(3, ccp(0, 0-FLOATER_MOVESPEED)));

}

void CSceneManager::updateBackground(CCLayer* pLayer, float delta)
{
	this->m_pBg->runAction(CCMoveBy::create(3, ccp(0, 0-BG_MOVESPEED)));
	this->m_pFloaterMap->runAction(CCMoveBy::create(3, ccp(0, 0-FLOATER_MOVESPEED)));

	this->m_pBgHeight -= BG_MOVESPEED;
	this->m_pFloaterHeight -= FLOATER_MOVESPEED;

	// ����ͼ�Ķ����ﵽ����Ļ������ʱ��
	if (this->m_pBgHeight <= this->m_winSize.height)
	{
		// ��ǰ��û������һ����ͼ��ʱ��
		if (!this->m_bIsBgLoad)
		{
			// ���ﳢ��ÿ�ζ�����һ����Դ��û��ʹ��CCTextureCache
			this->m_pBg2 = CCSprite::create(sz_b01);
			this->m_pBg2->setAnchorPoint(ccp(0, 0));
			pLayer->addChild(this->m_pBg2, 0);
			this->m_pBg2->setPosition(ccp(0, this->m_winSize.height));
			this->m_bIsBgLoad = true;
		}

		this->m_pBg2->runAction(CCMoveBy::create(3, ccp(0, 0-BG_MOVESPEED)));
	}

	// ��һ��ͼ��ȫ�뿪��Ļ���Ƴ���ͼ�������Ƶ�m_pBg�У��ÿ�m_pBg2��
	if (this->m_pBgHeight <= 0.0f)
	{
		this->m_pBgHeight = this->m_winSize.height;

		// �Ƴ���clean��
		pLayer->removeChild(this->m_pBg, true);

		this->m_pBg = this->m_pBg2;

		this->m_pBg2 = NULL;
		this->m_bIsBgLoad = false;
	}

	// Ư���������ڱ���ͼƬ
	if (this->m_pFloaterHeight <= this->m_winSize.height) {
		if (!this->m_bIsFloaterLoad) {
			this->m_pFloaterMap2 = CCTMXTiledMap::create(sz_level01);
			pLayer->addChild(this->m_pFloaterMap2, 1);
			this->m_pFloaterMap2->setPosition(0, this->m_winSize.height);
			this->m_bIsFloaterLoad = true;
		}
		this->m_pFloaterMap2->runAction(CCMoveBy::create(3, ccp(0, 0-FLOATER_MOVESPEED)));
	}

	if (this->m_pFloaterHeight <= 0.0f) {
		this->m_pFloaterHeight = this->m_pFloaterMap->getMapSize().height * this->m_pFloaterMap->getTileSize().height;
		/*	char a[10] = "";
		sprintf(a, "%.2f", this->m_pFloaterHeight);
		CCLog(a);*/
		pLayer->removeChild(this->m_pFloaterMap, true);
		this->m_pFloaterMap = this->m_pFloaterMap2;
		this->m_pFloaterMap2 = NULL;
		this->m_bIsFloaterLoad = false;
	}
}

void CSceneManager::checkAllIsCollide(CCLayer* pLayer)
{
	CCObject* pBullets;
	CCObject* pEnemys;

	CCArray* pEnemyPlanes = CPlaneManager::sharePlaneManager()->getEnemyPlanes();
	// ѭ���л�������ӵ�����ҷɻ���ײ���
	CCARRAY_FOREACH(pEnemyPlanes, pEnemys)
	{
		CUnitSprite* pEnemy = (CUnitSprite*)(pEnemys);

		CCArray* pPlayBullet = CPlaneManager::sharePlaneManager()->getPlayBullets();
		// �ӵ���л���ײ���
		CCARRAY_FOREACH(pPlayBullet, pBullets)
		{	 
			CUnitSprite* pBullet = (CUnitSprite*)(pBullets);

			if (this->isCollided(pEnemy, pBullet))
			{
				pEnemy->hurt();
				if (pBullet->isActive())
				{
					CPlaneManager::sharePlaneManager()->playExplode(pLayer, pBullet->getPosition());
					pBullet->destroy();
					pBullet = NULL;
				}
			}
			if (pBullet != NULL && pBullet->isActive())
			{
				if (!(this->m_screenRect.intersectsRect(pBullet->boundingBox())))
				{
					CPlaneManager::sharePlaneManager()->playExplode(pLayer, pBullet->getPosition());
					pBullet->destroy();
					pBullet = NULL;
					//	CCLog("destroy success in playbullet");
				}
			}
		}
		// �л�����ҷɻ���ײ���
		if (this->isCollided(pEnemy,CPlaneManager::sharePlaneManager()->getMyPlane()))
		{
			if (CPlaneManager::sharePlaneManager()->getMyPlane()->isActive())
			{
				pEnemy->hurt();
				CPlaneManager::sharePlaneManager()->getMyPlane()->hurt();
			}
		}
		if (!(this->m_screenRect.intersectsRect(pEnemy->boundingBox()))) 
		{
            pEnemy->hurt();
        }
	}

	CCArray* pEnemyBullet = CPlaneManager::sharePlaneManager()->getEnemyBullets();

	CCARRAY_FOREACH(pEnemyBullet, pBullets)
	{
		CUnitSprite* pBullet = (CUnitSprite*)(pBullets);

		if (pBullet != NULL && pBullet->isActive())
		{
			if (this->isCollided(pBullet, CPlaneManager::sharePlaneManager()->getMyPlane()))
			{
				if (CPlaneManager::sharePlaneManager()->getMyPlane()->isActive())
				{
					CPlaneManager::sharePlaneManager()->getMyPlane()->hurt();
					if (pBullet->isActive())
					{
						pBullet->destroy();
						pBullet = NULL;
					}
				}
			}
		}
		if (pBullet != NULL && pBullet->isActive())
		{
			if (!(this->m_screenRect.intersectsRect(pBullet->boundingBox())))
			{
					if (pBullet->isActive())
					{
						CPlaneManager::sharePlaneManager()->playExplode(pLayer, pBullet->getPosition());
						pBullet->destroy();
						pBullet = NULL;
					}
					//	CCLog("destroy success in enemybullet");
			}
		}
	}
}

bool CSceneManager::isCollided(CUnitSprite* a, CUnitSprite* b)
{
	if (a == NULL || b == NULL)
	{
		return false;
	}

	CCRect aRect = a->collideRect();
	CCRect bRect = b->collideRect();

	if (!aRect.intersectsRect(bRect))
	{
		return false;
	}

	return true;
}

void CSceneManager::release()
{
	if (g_pSceneManager != NULL)
	{
		delete g_pSceneManager;
		g_pSceneManager = NULL;
	}
}