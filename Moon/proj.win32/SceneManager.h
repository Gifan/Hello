#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include "Config.h"
#include "ConstantRes.h"
#include "cocos2d.h"
#include "PlaneManager.h"
USING_NS_CC;

class CSceneManager
{
public:
	
	~CSceneManager();
	virtual void initUI(CCLayer* pLayer);
	virtual void updateUI();
	virtual void initBackground(CCLayer* pLayer);			// 初始哈背景及漂浮物
	void updateBackground(CCLayer* pLayer, float delta);	// 更新背景及漂浮物位置  

	void checkAllIsCollide(CCLayer* pLayer);				// 检查碰撞
	bool isCollided(CUnitSprite* a, CUnitSprite* b);		// 检测2物体碰撞 

	void release();
	static CSceneManager* shareSceneManager();
private:
	CSceneManager();
	CCSize m_winSize;										// 窗口大小
	CCSprite* m_pBg;										// 背景图
	CCSprite* m_pBg2;
	float m_pBgHeight;										// 背景图的高度
	CCTMXTiledMap* m_pFloaterMap;							// 漂浮物
	CCTMXTiledMap* m_pFloaterMap2;
	float m_pFloaterHeight;									// 漂浮物高度
	bool m_bIsBgLoad;										// 是否加载了第二张背景图
	bool m_bIsFloaterLoad;									// 是否加载了第二张漂浮物图

	CCLabelTTF* m_pScore;									// 分数
	CCLabelTTF*	m_pLifeCount;								// 玩家飞机生命

	CCRect	m_screenRect;									// 屏幕区域

	CCSprite* pBackup;										// 备用
};

#endif