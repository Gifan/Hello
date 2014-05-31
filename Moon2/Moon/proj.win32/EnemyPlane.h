#ifndef ENEMYPLANE_H_
#define ENEMYPLANE_H_

#include "cocos2d.h"
#include "Config.h"
#include "UnitSprite.h"
#include "Bullet.h"
#include "Effect.h"

/**
* �л���
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

	void initAction(CCPoint desPos);			// ��ʼ���ɻ�����
	void repeatAction(CCNode* pSender);			// �ظ�����
	void shoot(float delta);					// �����ӵ�
	static CEnemyPlane* createPlane();
	CREATE_FUNC(CEnemyPlane);
	
private:
	bool m_bIsActive;			
	int m_iSpeed;								// �л��ٶ�
	int m_iBulletSpeed;							// �ӵ��ٶ�
	int m_iHP;									// �л�HP
	int m_iScore;								// �����л�����õķ���
};

#endif