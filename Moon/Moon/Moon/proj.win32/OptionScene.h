#ifndef OPTIONSCENE_H_
#define OPTIONSCENE_H_

#include "cocos2d.h"
#include "Config.h"
USING_NS_CC;

/**
* —°œÓ¿‡
*/
class COption : public CCLayer
{
public:
	static CCScene* scene();
	virtual bool init();
	void goBack(CCObject* pSender);
	void setOption(CCObject* pSender);

	CREATE_FUNC(COption);
private:
	COption(){}
};
#endif