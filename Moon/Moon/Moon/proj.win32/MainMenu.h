#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "cocos2d.h"

USING_NS_CC;

/**
* 主菜单类
*/
class CMainMenu : public cocos2d::CCLayer
{

public:

	// 初始化资源
	virtual bool init();

	// 获取场景
	static CCScene* scene();

	// 更新飞机位置
	virtual void update(float delta);

	// 开始游戏特效
	void startEffect(CCObject* pSender);

	// 按键按钮声效
	void onButtonEffect();
	// 开始新游戏
	void newGame();

	// 其他菜单选项回调函数
	void menuCallback(CCObject* pSender);
	void menuCloseCallback(CCObject* pSender);

	CREATE_FUNC(CMainMenu);

private:
	CMainMenu(){}
	CCSprite* m_myPlane;                  // 移动的飞机
	CCSize m_winSize;					   // 窗口大小
};

#endif