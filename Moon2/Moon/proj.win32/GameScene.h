#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "cocos2d.h"
#include "PlaneManager.h"
#include "SceneManager.h"
#include "MainMenu.h"
#include "UnitSprite.h"


USING_NS_CC;

enum GameState
{
    sStatePlaying = 0,
    sStateGameOver= 1
};

/**
* 游戏场景类
*/
class CGameLayer : public CCLayer
{
public:
	CGameLayer();
	~CGameLayer();
	static CCScene* scene();
	virtual bool init();
	virtual void update(float delta);
	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual	void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
	virtual void onEnter();
	virtual void onExit();

	void initUI();
	void updateUI();
	void initBackground();							// 初始哈背景及漂浮物
	void updateBackground(float delta);				// 更新背景及漂浮物位置  
	bool isGameOver();								// 判断是否游戏结束

	void checkAllIsCollide();						// 检查碰撞
	void gameOver();								// 游戏结束
	void mainMenu(CCObject* pSender);				// 返回主菜单回调函数
	CREATE_FUNC(CGameLayer);

private:
	int m_iState;									// 当前游戏状态
	CCSize m_winSize;
};

#endif