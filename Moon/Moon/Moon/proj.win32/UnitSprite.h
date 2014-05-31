#ifndef UNITSPRITE_H_
#define UNITSPRITE_H_

#include "cocos2d.h"
USING_NS_CC;

/**
* 飞机抽象类
*/

class CUnitSprite : public CCSprite
{
public:
	CUnitSprite(){};
	~CUnitSprite(){};

	virtual void destroy() = 0;				// 摧毁了飞机
    virtual void hurt() = 0 ;				// 被击中
    virtual CCRect collideRect() = 0;		// 碰撞区域
    virtual bool isActive() = 0;			// 是否生存着
};

#endif