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
	virtual void initBackground(CCLayer* pLayer);			// ��ʼ��������Ư����
	void updateBackground(CCLayer* pLayer, float delta);	// ���±�����Ư����λ��  

	void checkAllIsCollide(CCLayer* pLayer);				// �����ײ
	bool isCollided(CUnitSprite* a, CUnitSprite* b);		// ���2������ײ 

	void release();
	static CSceneManager* shareSceneManager();
private:
	CSceneManager();
	CCSize m_winSize;										// ���ڴ�С
	CCSprite* m_pBg;										// ����ͼ
	CCSprite* m_pBg2;
	float m_pBgHeight;										// ����ͼ�ĸ߶�
	CCTMXTiledMap* m_pFloaterMap;							// Ư����
	CCTMXTiledMap* m_pFloaterMap2;
	float m_pFloaterHeight;									// Ư����߶�
	bool m_bIsBgLoad;										// �Ƿ�����˵ڶ��ű���ͼ
	bool m_bIsFloaterLoad;									// �Ƿ�����˵ڶ���Ư����ͼ

	CCLabelTTF* m_pScore;									// ����
	CCLabelTTF*	m_pLifeCount;								// ��ҷɻ�����

	CCRect	m_screenRect;									// ��Ļ����

	CCSprite* pBackup;										// ����
};

#endif