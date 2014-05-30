#include "GameScene.h"
#include "ConstantRes.h"
#include "Config.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;


CGameLayer::CGameLayer():m_iState(sStatePlaying), m_bIsBgLoad(false), m_bIsFloaterLoad(false)
{

}
CGameLayer::~CGameLayer()
{ 
	pPlayBullet->release();
	pEnemyBullet->release();
	pEnemyPlanes->release();
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

	// ��������,retain����һ�����ã��������CConfig�����ͷ�
	pPlayBullet = CCArray::create();
	pPlayBullet->retain();

	pEnemyBullet = CCArray::create();
	pEnemyBullet->retain();

	pEnemyPlanes = CCArray::create();
	pEnemyPlanes->retain();

	CConfig::shareCConfig()->resetCConfig();

	this->m_winSize = CCDirector::sharedDirector()->getWinSize();
	this->m_screenRect = CCRectMake(0, 0, this->m_winSize.width, this->m_winSize.height);

	// ��ʼ����������
	if (CConfig::shareCConfig()->isSoundOn())
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(sz_gameMusic, true);

	// ��ʼ����ըЧ��
	CEffect::shareExplosion();

	// ��ʼ���ɻ�
	this->initPlane();

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
	// �����������
	this->m_pScore = CCLabelTTF::create(
		"Score:0",
		"΢���ź�",
		15,
		CCSizeMake(100, 30),
		CCTextAlignment::kCCTextAlignmentCenter,
		CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
	this->m_pScore->setAnchorPoint(ccp(0.5, 0));
	this->addChild(this->m_pScore, 100);
	this->m_pScore->setPosition(ccp(this->m_winSize.width-60, this->m_winSize.height - 30));

	// �����ҷɻ�UI
	CCTexture2D* pPlaneTexture = CCTextureCache::sharedTextureCache()->addImage(sz_myplane);
	CCSprite* pLife = CCSprite::createWithTexture(pPlaneTexture, CCRectMake(0, 0, 60, 38));
	pLife->setScale(0.6f);
	pLife->setPosition(ccp(30,this->m_winSize.height-23));
	this->addChild(pLife, 100);

	// �����ɻ�����ֵ
	char life[3];
	sprintf(life, "%d", this->m_pMyplane->getHP());
	this->m_pLifeCount = CCLabelTTF::create(
		life,
		"΢���ź�",
		15,
		CCSizeMake(30, 30),
		CCTextAlignment::kCCTextAlignmentCenter,
		CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
	this->addChild(this->m_pLifeCount, 100);
	this->m_pLifeCount->setPosition(ccp(60, this->m_winSize.height-23));
	this->m_pLifeCount->setColor(ccc3(255, 0, 0));

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
	// ���·���
	char s[30] = "Score:";
	sprintf(s, "%s%d", s, CConfig::shareCConfig()->getScoreVal());
	this->m_pScore->setString(s);

	// ���·ɻ�ʣ������ֵ
	char life[3];
	sprintf(life, "%d", this->m_pMyplane->getHP());
	this->m_pLifeCount->setString(life);
}

// ��ײ���
bool CGameLayer::isCollided(CUnitSprite* a, CUnitSprite* b)
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

void CGameLayer::mainMenu(CCObject* pSender)
{
	CCScene* pScene = CMainMenu::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.8f, pScene));

}
void CGameLayer::update(float delta)
{
	if (this->m_iState == sStatePlaying)
	{
		this->checkAllIsCollide();
		this->updateUI();
		this->updatePlane(delta);
	}
}


void CGameLayer::updatePlane(float delta)
{
	for (int i = 0; i < pEnemyPlanes->count(); ++i) 
	{
		CEnemyPlane* pEnemy = dynamic_cast<CEnemyPlane*>(pEnemyPlanes->objectAtIndex(i));
		if (pEnemy != NULL)
		{
			if (!pEnemy->isActive())
			{
				pEnemy->destroy();
				CEnemyPlane* pEnemy = CEnemyPlane::createPlane();
				pEnemy->initAction(this->m_pMyplane->getPosition());
				this->addChild(pEnemy, 99);
				pEnemyPlanes->addObject(pEnemy);
			}
		}
    }

	// ���������������Ϸ����
	if (this->m_pMyplane->getHP() <= 0)
	{
		this->m_iState = sStateGameOver;
		this->m_pMyplane->destroy();
		this->m_pMyplane = NULL;
		CCCallFunc* pGameOver = CCCallFunc::create(this, callfunc_selector(CGameLayer::gameOver));
		this->runAction(CCSequence::create(CCDelayTime::create(2.0f), pGameOver, NULL));
	}
}
void CGameLayer::checkAllIsCollide()
{
	CCObject* pBullets;
	CCObject* pEnemys;

	// ѭ���л�������ӵ�����ҷɻ���ײ���
	CCARRAY_FOREACH(pEnemyPlanes, pEnemys)
	{
		CUnitSprite* pEnemy = (CUnitSprite*)(pEnemys);

		// �ӵ���л���ײ���
		CCARRAY_FOREACH(pPlayBullet, pBullets)
		{	 
			CUnitSprite* pBullet = (CUnitSprite*)(pBullets);

			if (this->isCollided(pEnemy, pBullet))
			{
				pEnemy->hurt();
				if (pBullet->isActive())
					pBullet->destroy();
			}
			if (!(this->m_screenRect.intersectsRect(pBullet->boundingBox())))
			{
				if (pBullet->isActive())
					pBullet->destroy();
				//	CCLog("destroy success in playbullet");
			}
		}
		// �л�����ҷɻ���ײ���
		if (this->isCollided(pEnemy,this->m_pMyplane))
		{
			if (this->m_pMyplane->isActive())
			{
				pEnemy->hurt();
				this->m_pMyplane->hurt();
			}
		}
		if (!(this->m_screenRect.intersectsRect(pEnemy->boundingBox()))) 
		{
            pEnemy->hurt();
        }
	}

	CCARRAY_FOREACH(pEnemyBullet, pBullets)
	{
		CUnitSprite* pBullet = (CUnitSprite*)(pBullets);
		if (pBullet != NULL)
		{
			if (this->isCollided(pBullet, this->m_pMyplane))
			{
				if (this->m_pMyplane->isActive())
				{
					this->m_pMyplane->hurt();
					if (pBullet->isActive())
						pBullet->destroy();
				}
			}
		}
		if (!(this->m_screenRect.intersectsRect(pBullet->boundingBox())))
		{
			if (pBullet->isActive())
				pBullet->destroy();
			//	CCLog("destroy success in enemybullet");
		}
	}
}

/**
*���������������ʹ��2�ű���ͼȻ��ѭ�����ع���
*/
void CGameLayer::initBackground()
{
	// ���ر���ͼƬ
	this->m_pBg = CCSprite::create(sz_b01);
	this->m_pBg->setAnchorPoint(ccp(0, 0));
	this->m_pBgHeight = this->m_pBg->getContentSize().height;
	this->addChild(m_pBg, 0);

	// ����Ư����
	this->m_pFloaterMap = CCTMXTiledMap::create(sz_level01);
	this->addChild(this->m_pFloaterMap, 1);
	float height = this->m_pFloaterMap->getMapSize().height;
	this->m_pFloaterHeight = height * height;

	this->m_pBgHeight -= 48;
	this->m_pFloaterHeight -= 200;
	this->m_pBg->runAction(CCMoveBy::create(3, ccp(0, 0-BG_MOVESPEED)));
	this->m_pFloaterMap->runAction(CCMoveBy::create(3, ccp(0, 0-FLOATER_MOVESPEED)));

	// ���ö�ʱ���Թ�������
	this->schedule(schedule_selector(CGameLayer::updateBackground),3);

}

void CGameLayer::updateBackground(float delta)
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
			this->addChild(this->m_pBg2, 0);
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
		this->removeChild(this->m_pBg, true);

		this->m_pBg = this->m_pBg2;

		this->m_pBg2 = NULL;
		this->m_bIsBgLoad = false;
	}

	// Ư���������ڱ���ͼƬ
	if (this->m_pFloaterHeight <= this->m_winSize.height) {
		if (!this->m_bIsFloaterLoad) {
			this->m_pFloaterMap2 = CCTMXTiledMap::create(sz_level01);
			this->addChild(this->m_pFloaterMap2, 1);
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
		this->removeChild(this->m_pFloaterMap, true);
		this->m_pFloaterMap = this->m_pFloaterMap2;
		this->m_pFloaterMap2 = NULL;
		this->m_bIsFloaterLoad = false;
	}
}

void CGameLayer::initPlane()
{
	// ������ҷɻ�
	this->m_pMyplane = CMyPlane::create();
	this->addChild(this->m_pMyplane, 99, 1001);

	// ���صл�
	for (int i = 0; i < CConfig::shareCConfig()->getLevelState(); i++)
	{
		CEnemyPlane* pEnemy = CEnemyPlane::createPlane();
		
		// ������ҷɻ�λ�ó�ʼ������
		pEnemy->initAction(this->m_pMyplane->getPosition());
		this->addChild(pEnemy, 99);
		pEnemyPlanes->addObject(pEnemy);
	}
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
	if ((this->m_iState == sStatePlaying) && this->m_pMyplane)
	{
		// �޸ĵ�ǰ�ɻ�λ��
		CCPoint pos = pTouch->getDelta();
		CCPoint currentPos = this->m_pMyplane->getPosition();
		currentPos = ccpAdd(currentPos, pos);

		// ��סֻ������Ļ����Χ�ƶ�
		currentPos = ccpClamp(currentPos, CCPointZero, ccp(this->m_winSize.width, this->m_winSize.height));
		this->m_pMyplane->setPosition(currentPos);
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
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}