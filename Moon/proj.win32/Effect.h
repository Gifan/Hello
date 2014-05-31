#ifndef EFFECT_H_
#define EFFECT_H_

#include "cocos2d.h"

USING_NS_CC;

/**
* ±¬Õ¨Ð§¹ûÀà
*/

class CEffect : public cocos2d::CCNode
{
public:
	CEffect();
	~CEffect();

	void explode(CCNode* parent, CCPoint pos);

	void spark(CCPoint pos, CCNode *parent, float scale, float duration);
    
    void removeSprite(CCNode *pSender);

	static void shareExplosion();

	static CEffect* create();

private:
	CCSprite* m_pSprite;
};

#endif