#ifndef MYPLANE_H_
#define MYPLANE_H_

#include "cocos2d.h"
#include "UnitSprite.h"
USING_NS_CC;

/**
* 玩家飞机类
*/

class CMyPlane : public CUnitSprite
{
public:
	CMyPlane();
	~CMyPlane();

	virtual void update(float delta);
	virtual bool init();

	virtual void destroy();				
    virtual void hurt() ;				
    virtual CCRect collideRect();		
    virtual bool isActive();

	int getHP();
	void shoot(float delta);
	CREATE_FUNC(CMyPlane);

private:
	int m_iBulletSpeed;			// 子弹速度
	int m_iHP;					// 飞机生命
	bool m_bCanBeAttack;		// 是否能被击中
	bool m_bIsActive;			// 是否活跃状态，还没死
    CCPoint m_appearPosition;	// 初始位置
};

#endif