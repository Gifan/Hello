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
	bool m_bIsActive;		// ����û
	int m_iBulletType;		// �ӵ�����
	int m_iSpeed;			// �ӵ��ٶ�
	int m_iZOrder;          // �ӵ�z���С
};


#endif