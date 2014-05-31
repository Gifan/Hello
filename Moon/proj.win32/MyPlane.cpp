#include "MyPlane.h"
#include "Config.h"
#include "ConstantRes.h"
#include "Bullet.h"
#include "Effect.h"
#include "SimpleAudioEngine.h"
#include "PlaneManager.h"

using namespace CocosDenshion;

CMyPlane::CMyPlane() : m_iBulletSpeed(900), m_iHP(1),
	m_bCanBeAttack(true), m_bIsActive(true)
{

}

CMyPlane::~CMyPlane()
{

}

bool CMyPlane::init()
{
	if (!CCSprite::init())
	{
		return false;
	}

	// ��ʼ���ɻ�
	CCTexture2D* pPlaneTextureCache = CCTextureCache::sharedTextureCache()->addImage(sz_myplane);
    CCRect rec = CCRectMake(0, 0, 60, 38);
    this->initWithTexture(pPlaneTextureCache,  rec);
    
	// ����֡
    CCSpriteFrame* pFrame0 = CCSpriteFrame::createWithTexture(pPlaneTextureCache, CCRectMake(0, 0, 60, 38));
    CCSpriteFrame* pFrame1 = CCSpriteFrame::createWithTexture(pPlaneTextureCache, CCRectMake(60, 0, 60, 38));

	CCArray* pAnimFrames = CCArray::create();
    pAnimFrames->addObject(pFrame0);
    pAnimFrames->addObject(pFrame1);

    // �ɻ�����
    CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(pAnimFrames, 0.1f);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    this->runAction(CCRepeatForever::create(pAnimate));

	// �ӵ�����
    this->schedule(schedule_selector(CMyPlane::shoot), 0.12f);

	// ��ʼ��λ��
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	this->m_appearPosition = ccp(winSize.width / 2, 60);
	this->setPosition(this->m_appearPosition);

	return true;
}

void CMyPlane::update(float delta)
{
	if (this->m_iHP <= 0)
	{
        this->m_bIsActive = false;
    }
}

int CMyPlane::getHP()
{
	return this->m_iHP;
}

void CMyPlane::shoot(float delta)
{
	CCPoint position = this->getPosition();
    CCSize contentSize = this->getContentSize();

	// �����ӵ�����������ΪCREATE_FUNC(CBullet) ʹ�ô��βι��캯��
	CBullet* pBullet = new CBullet(this->m_iBulletSpeed,MYPLANE_BULLET);
    if (pBullet) 
	{
        pBullet->autorelease();
		pBullet->init();
		CPlaneManager::sharePlaneManager()->getPlayBullets()->addObject(pBullet);
		this->getParent()->addChild(pBullet, pBullet->getZOrder(), 901);
        pBullet->setPosition(ccp(position.x, position.y + contentSize.height * 0.4));
    }
	else
	{
        delete pBullet;
        pBullet = NULL;
    }
}

// ������
void CMyPlane::hurt()
{
    if (this->m_bCanBeAttack) {
        this->m_iHP--;
    }
	if (this->m_iHP <= 0)
		this->m_bIsActive = false;
}

void CMyPlane::destroy()
{
 //  CCLOG("destroy one ship");
    
	if (!this->m_bIsActive)
	{
		CEffect* pEffect = CEffect::create();

		pEffect->explode(this->getParent(), this->getPosition());

		this->removeFromParent(); 

		if (CConfig::shareCConfig()->isSoundOn())
			SimpleAudioEngine::sharedEngine()->playEffect(sz_destroyPlane);
	}
}

CCRect CMyPlane::collideRect()
{
	// ��ȡ�ɻ���С�����ꡣ������ײ����
    CCPoint pos = this->getPosition();
    CCSize cs = this->getContentSize();
    
    return CCRectMake(pos.x - cs.width / 2 , pos.y - cs.height / 2, cs.width, cs.height / 2);
}

bool CMyPlane::isActive()
{
	return this->m_bIsActive;
}

