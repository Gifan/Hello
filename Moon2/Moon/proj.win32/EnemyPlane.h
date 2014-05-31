#ifndef ENEMYPLANE_H_
#define ENEMYPLANE_H_

#include "cocos2d.h"
#include "Config.h"
#include "UnitSprite.h"
#include "Bullet.h"
#include "Effect.h"

/**
* 敌机类
*/

class CEnemyPlane : public CUnitSprite
{
public:
	CEnemyPlane();
	~CEnemyPlane();
	virtual bool init();
	virtual void update(float delta);
	virtual void hurt();
	virtual void destroy();
	virtual CCRect collideRect();
	virtual bool isActive();

	void initAction(CCPoint desPos);			// 初始化飞机动作
	void repeatAction(CCNode* pSender);			// 重复动作
	void shoot(float delta);					// 发射子弹
	static CEnemyPlane* createPlane();
	CREATE_FUNC(CEnemyPlane);
	
private:
	bool m_bIsActive;			
	int m_iSpeed;								// 敌机速度
	int m_iBulletSpeed;							// 子弹速度
	int m_iHP;									// 敌机HP
	int m_iScore;								// 击溃敌机所获得的分数
};

#endif