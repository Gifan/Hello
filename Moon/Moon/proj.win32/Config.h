#ifndef CONFIG_H_
#define CONFIG_H_

#include "cocos2d.h"

extern cocos2d::CCArray* pPlayBullet;				// 引用Config.cpp中的
extern cocos2d::CCArray* pEnemyBullet;				// 分别为玩家子弹数组、敌人飞机子弹数组
extern cocos2d::CCArray* pEnemyPlanes;				// 敌人飞机数组

/**
* 配置设置类
*/
class CConfig
{
public:
	~CConfig();
	static CConfig* shareCConfig();

	/** 声效处理*/
	bool isSoundOn();
	void setSoundState(bool state);
	void updateSound();

	/** 分数处理*/
	int getScoreVal();
	void setScoreVal(int score);
	void addScoreVal(int score);

	/** 难度处理*/
	int getLevelState();
	void setLevelState(int state);
	void updateLevel();

	void resetCConfig();

private:
	bool m_bIsSoundOn;			// 是否开启了声效
	int m_iLevel;				// 当前难度
	int m_iScoreVal;			// 当前获得分数

	CConfig();
};

#endif