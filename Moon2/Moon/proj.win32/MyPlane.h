#ifndef MYPLANE_H_
#define MYPLANE_H_

#include "cocos2d.h"
#include "UnitSprite.h"
USING_NS_CC;

/**
* ��ҷɻ���
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
	int m_iBulletSpeed;			// �ӵ��ٶ�
	int m_iHP;					// �ɻ�����
	bool m_bCanBeAttack;		// �Ƿ��ܱ�����
	bool m_bIsActive;			// �Ƿ��Ծ״̬����û��
    CCPoint m_appearPosition;	// ��ʼλ��
};

#endif