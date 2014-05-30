#include "EnemyPlane.h"
#include "ConstantRes.h"
#include "SimpleAudioEngine.h"
#include "PlaneManager.h"
using namespace CocosDenshion;

CEnemyPlane::CEnemyPlane() : m_bIsActive(true), m_iBulletSpeed(200)
{
	this->m_iHP = CConfig::shareCConfig()->getLevelState();
	this->m_iSpeed = CCRANDOM_0_1() * 400 + 200;
}

CEnemyPlane::~CEnemyPlane()
{

}

bool CEnemyPlane::init()
{
	if (!CCSprite::init())
	{
		return false;
	}

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(sz_enemyPlist, sz_enemyPlane);



	// ������ɵл�����
	int temp = (int)(CCRANDOM_0_1() * 4.5);
	char a[10] = "";
	switch(temp)
	{
	case 0:
		strcpy(a, "E0.png");
		this->m_iScore = 100 * (CConfig::shareCConfig()->getLevelState()-2);
		break;
	case 1:
		strcpy(a, "E1.png");
		this->m_iScore = 100 * (CConfig::shareCConfig()->getLevelState()-1);
		break;
	case 2:
		strcpy(a, "E2.png");
		this->m_iScore = 100 * (CConfig::shareCConfig()->getLevelState()-0);
		break;
	case 3:
		strcpy(a, "E3.png");
		this->m_iScore = 100 * (CConfig::shareCConfig()->getLevelState()+1);
		break;
	case 4:
		strcpy(a, "E4.png");
		this->m_iScore = 100 * (CConfig::shareCConfig()->getLevelState()+2);
		break;
	default:
		CCAssert(false, "�����ID");
		break;
	}

//	CCLog(a);

	this->initWithSpriteFrameName(a);

	// ��ʼ������
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	this->setPosition(
		ccp( 80 + (winSize.width - 160) * CCRANDOM_0_1(),
		winSize.height));

	this->scheduleUpdate();

	// �����ӵ���ʱ��
	float fdelyTime = 3.5f - 0.5f * CConfig::shareCConfig()->getLevelState(); 
	this->schedule(schedule_selector(CEnemyPlane::shoot), fdelyTime);

	return true;
}

void CEnemyPlane::update(float dt)
{
    if (this->m_iHP <= 0) {
        this->m_bIsActive = false;
    }
}

// ��ʼ������:�ȳ�����Ȼ������ҷɻ���������ƶ�
void CEnemyPlane::initAction(CCPoint desPos)
{
	CCPoint offset;
	CCSize winSize;
	winSize = CCDirector::sharedDirector()->getWinSize();
	offset = desPos;
	offset.x += CCRANDOM_MINUS1_1() * 50;
	offset.y += 100 + CCRANDOM_0_1() * 100;

	if (offset.y >= winSize.height) offset.y = winSize.height - 30;


	CCCallFuncN* pOnComplete = 
		CCCallFuncN::create(this, callfuncN_selector(CEnemyPlane::repeatAction));
	CCAction* pTempAction = CCSequence::create(CCMoveTo::create(1, offset), pOnComplete, NULL);
	this->runAction(pTempAction);
}

void CEnemyPlane::repeatAction(CCNode *pSender)
{
    CCDelayTime* pDelay =  CCDelayTime::create(1);
    CCMoveBy* pMv = CCMoveBy::create(1, ccp(100 + 100 * CCRANDOM_0_1(), 0));
    CCActionInterval *seq = CCSequence::create(pDelay, pMv, pDelay->copy(), pMv->reverse(), NULL);
    pSender->runAction(CCRepeatForever::create(seq));
}

void CEnemyPlane::hurt()
{
	this->m_iHP--;
	CConfig::shareCConfig()->addScoreVal(20);
}

void CEnemyPlane::destroy()
{
	CCPoint pos = this->getPosition();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	if(pos.x > 0 && pos.x < winSize.width )
	{
		// ���·���
		CConfig::shareCConfig()->addScoreVal(this->m_iScore );

		// ��ը��Ч
		CEffect* pEffect = CEffect::create();

		pEffect->explode(this->getParent(), pos);

		// ����
		if (CConfig::shareCConfig()->isSoundOn())
			SimpleAudioEngine::sharedEngine()->playEffect(sz_explode);
	}

    // ɾ������
    this->removeFromParent();
    
}

// ������ײ����
CCRect CEnemyPlane::collideRect()
{
    CCSize size = getContentSize();
    CCPoint pos = getPosition();
    
    return CCRectMake(pos.x - size.width / 2, pos.y - size.height / 4, size.width, size.height / 2);
}

bool CEnemyPlane::isActive()
{
    return  this->m_bIsActive;
}

void CEnemyPlane::shoot(float delta)
{
	CCPoint position = this->getPosition();
    CCSize contentSize = this->getContentSize();

	// �����ӵ�����������ΪCREATE_FUNC(CBullet) ʹ�ô��βι��캯��
	CBullet* pBullet = new CBullet(this->m_iBulletSpeed,ENEMY_BULLET);
    if (pBullet) 
	{
        pBullet->autorelease();
		pBullet->init();
		CPlaneManager::sharePlaneManager()->getEnemyBullets()->addObject(pBullet);
		this->getParent()->addChild(pBullet, pBullet->getZOrder(), 901);
        pBullet->setPosition(ccp(position.x, position.y));

    }
	else
	{
        delete pBullet;
        pBullet = NULL;
    }
}

CEnemyPlane* CEnemyPlane::createPlane()
{
	CEnemyPlane* pEnemyPlane = new CEnemyPlane();
	
//	CCLog("createPlane->>>>>>>>>>>>>>>>>>>");
	if (pEnemyPlane)
	{
//		CCLog("createPlane->>>>>>>>>>>>>>>>>>>2");
		if (pEnemyPlane->init())
		{
//			CCLog("createPlane->>>>>>>>>>>>>>>>>>>3");
			pEnemyPlane->autorelease();
			return pEnemyPlane;
		}
	}

	delete pEnemyPlane;
	pEnemyPlane = NULL;
	return NULL;
}