#ifndef GAMEOVERSCENE_H_
#define GAMEOVERSCENE_H_

#include "cocos2d.h"
USING_NS_CC;

/**
* ”Œœ∑Ω· ¯¿‡
*/
class CGameOver : public cocos2d::CCLayer
{
public:
	virtual bool init();
	static CCScene* scene();
	void playAgain(CCObject* pSender);

	CREATE_FUNC(CGameOver);
};

#endif