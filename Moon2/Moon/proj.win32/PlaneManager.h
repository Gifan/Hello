#ifndef PLANEMANAGER_H_
#define PLANEMANAGER_H_

#include "cocos2d.h"
#include "MyPlane.h"
#include "Config.h"
#include "Bullet.h"
#include "EnemyPlane.h"


USING_NS_CC;

#define RELEASE_ARRAY(_ARRAY_) if (_ARRAY_ != NULL){ _ARRAY_->release(); _ARRAY_ = NULL;}

/**
* 飞机管理类
*/
class CPlaneManager
{
public:
	~CPlaneManager();
	static CPlaneManager* sharePlaneManager();
	void initPlane(CCLayer* pLayer);					// 初始化飞机
	void initExplode();									// 初始化爆炸效果
	void updatePlane(CCLayer* pLayer, float delta);	// 更新飞机
	
	CCArray* getPlayBullets();							// 获取玩家子弹
	CCArray* getEnemyBullets();							// 获取敌机子弹
	CCArray* getEnemyPlanes();							// 获取敌机
	CMyPlane* getMyPlane();								// 获取玩家飞机
	void setMyPlanePos(CCPoint pos);
	void playExplode(CCLayer* pLayer, CCPoint pos);	    // 绘制击中效果
	void release();										// 释放

private:
	CPlaneManager();
	CCArray* m_pPlayBullet;								// 玩家子弹
	CCArray* m_pEnemyBullet;								// 敌机子弹
	CCArray* m_pEnemyPlanes;								// 敌机

	CMyPlane* m_pMyPlane;								// 玩家飞机
};

#endif