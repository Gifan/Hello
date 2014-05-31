#ifndef ABOUTSCENE_H_
#define ABOUTSCENE_H_

#include "cocos2d.h"
USING_NS_CC;

/**
* 关于游戏类
*/
class CAbout : public CCLayer
{
public:
	static CCScene* scene();
	virtual bool init();
	void goBack(CCObject* pSender);

	CREATE_FUNC(CAbout);
private:
	CAbout(){}
};

#endif