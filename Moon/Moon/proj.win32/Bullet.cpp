#include "Bullet.h"
#include "Config.h"
#include "PlaneManager.h"

CBullet::CBullet():m_bIsActive(true), m_iSpeed(200), m_iBulletType(ENEMY_BULLET), m_iZOrder(99)
{

}

CBullet::CBullet(int speed, int bulletType)
{
	this->m_bIsActive = true;
    this->m_iSpeed = speed;
	this->m_iBulletType = bulletType;
	this->m_iZOrder = 99;
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(sz_bulletPlist);
	if (this->m_iBulletType == MYPLANE_BULLET)
		this->initWithSpriteFrameName("W1.png");
	else this->initWithSpriteFrameName("W2.png");

	// Èá¹âÐ§¹û
    ccBlendFunc cbl = {GL_SRC_ALPHA, GL_ONE};
    this->setBlendFunc(cbl);
}

bool CBullet::init()
{
	this->scheduleUpdate();
	return true;
}

void CBullet::update(float delta)
{
	float posY = this->getPositionY();
	if (this->m_iBulletType == MYPLANE_BULLET)
		posY += this->m_iSpeed * delta;
	else 
		posY -= this->m_iSpeed * delta;
	this->setPositionY(posY);
}

void CBullet::hurt()
{

}

void CBullet::destroy()
{

	this->m_bIsActive = false;
	this->removeFromParent();
	if (this->m_iBulletType == ENEMY_BULLET)
		CPlaneManager::sharePlaneManager()->getEnemyBullets()->removeObject(this);
	else CPlaneManager::sharePlaneManager()->getPlayBullets()->removeObject(this);
}

void CBullet::removeExplode(CCNode *pSender)
{
    pSender->removeFromParent();
}

bool CBullet::isActive()
{
    return m_bIsActive;
}

void CBullet::setActive(bool active)
{
	this->m_bIsActive = active;
}

CCRect CBullet::collideRect()
{
    CCPoint pos = this->getPosition();
    return CCRectMake(pos.x - 3, pos.y - 3, 6, 6);
}

int CBullet::getZOrder()
{
	return this->m_iZOrder;
}