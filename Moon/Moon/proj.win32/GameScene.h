#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "cocos2d.h"
#include "MainMenu.h"
#include "MyPlane.h"
#include "EnemyPlane.h"
#include "Effect.h"

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
	void initPlane();								// 初始化飞机
	void updatePlane(float delta);					// 更新飞机
	void checkAllIsCollide();						// 检查碰撞
	bool isCollided(CUnitSprite* a, CUnitSprite* b);// 检测2物体碰撞 
	void gameOver();								// 游戏结束
	void mainMenu(CCObject* pSender);				// 返回主菜单回调函数
	CREATE_FUNC(CGameLayer);

private:
	int m_iState;									// 当前游戏状态
	CCSize m_winSize;								// 窗口大小
	CCSprite* m_pBg;								// 背景图
	CCSprite* m_pBg2;
	float m_pBgHeight;								// 背景图的高度
	CCTMXTiledMap* m_pFloaterMap;					// 漂浮物
	CCTMXTiledMap* m_pFloaterMap2;
	float m_pFloaterHeight;							// 漂浮物高度
	bool m_bIsBgLoad;								// 是否加载了第二张背景图
	bool m_bIsFloaterLoad;							// 是否加载了第二张漂浮物图

	CCLabelTTF* m_pScore;							// 分数
	CCLabelTTF*	m_pLifeCount;						// 玩家飞机生命

	CCRect	m_screenRect;							// 屏幕区域

	CMyPlane* m_pMyplane;							// 玩家飞机
};

#endif