#ifndef CONFIG_H_
#define CONFIG_H_

#include "cocos2d.h"

extern cocos2d::CCArray* pPlayBullet;				// ����Config.cpp�е�
extern cocos2d::CCArray* pEnemyBullet;				// �ֱ�Ϊ����ӵ����顢���˷ɻ��ӵ�����
extern cocos2d::CCArray* pEnemyPlanes;				// ���˷ɻ�����

/**
* ����������
*/
class CConfig
{
public:
	~CConfig();
	static CConfig* shareCConfig();

	/** ��Ч����*/
	bool isSoundOn();
	void setSoundState(bool state);
	void updateSound();

	/** ��������*/
	int getScoreVal();
	void setScoreVal(int score);
	void addScoreVal(int score);

	/** �Ѷȴ���*/
	int getLevelState();
	void setLevelState(int state);
	void updateLevel();

	void resetCConfig();

private:
	bool m_bIsSoundOn;			// �Ƿ�������Ч
	int m_iLevel;				// ��ǰ�Ѷ�
	int m_iScoreVal;			// ��ǰ��÷���

	CConfig();
};

#endif