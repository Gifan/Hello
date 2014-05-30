#ifndef UNITSPRITE_H_
#define UNITSPRITE_H_

#include "cocos2d.h"
USING_NS_CC;

/**
* �ɻ�������
*/

class CUnitSprite : public CCSprite
{
public:
	CUnitSprite(){};
	~CUnitSprite(){};

	virtual void destroy() = 0;				// �ݻ��˷ɻ�
    virtual void hurt() = 0 ;				// ������
    virtual CCRect collideRect() = 0;		// ��ײ����
    virtual bool isActive() = 0;			// �Ƿ�������
};

#endif