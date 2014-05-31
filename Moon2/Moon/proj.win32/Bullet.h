#ifndef BULLET_H_
#define BULLET_H_

#include "cocos2d.h"
#include "ConstantRes.h"
#include "UnitSprite.h"
USING_NS_CC;

class CBullet : public CUnitSprite
{
public:
	CBullet();
	CBullet(int speed, int bulletType = ENEMY_BULLET);
	virtual bool init();
	virtual void update(float delta);
	virtual void destroy();
	virtual CCRect collideRect();
	virtual bool isActive();
	virtual void hurt();
	void setActive(bool active);
	int getZOrder();
	void removeExplode(CCNode* pSender);
	CREATE_FUNC(CBullet);


private:
	bool m_bIsActive;		// 死了没
	int m_iBulletType;		// 子弹类型
	int m_iSpeed;			// 子弹速度
	int m_iZOrder;          // 子弹z轴大小
};


#endif