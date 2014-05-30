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
* ��Ϸ������
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
	void initBackground();							// ��ʼ��������Ư����
	void updateBackground(float delta);				// ���±�����Ư����λ��  
	void initPlane();								// ��ʼ���ɻ�
	void updatePlane(float delta);					// ���·ɻ�
	void checkAllIsCollide();						// �����ײ
	bool isCollided(CUnitSprite* a, CUnitSprite* b);// ���2������ײ 
	void gameOver();								// ��Ϸ����
	void mainMenu(CCObject* pSender);				// �������˵��ص�����
	CREATE_FUNC(CGameLayer);

private:
	int m_iState;									// ��ǰ��Ϸ״̬
	CCSize m_winSize;								// ���ڴ�С
	CCSprite* m_pBg;								// ����ͼ
	CCSprite* m_pBg2;
	float m_pBgHeight;								// ����ͼ�ĸ߶�
	CCTMXTiledMap* m_pFloaterMap;					// Ư����
	CCTMXTiledMap* m_pFloaterMap2;
	float m_pFloaterHeight;							// Ư����߶�
	bool m_bIsBgLoad;								// �Ƿ�����˵ڶ��ű���ͼ
	bool m_bIsFloaterLoad;							// �Ƿ�����˵ڶ���Ư����ͼ

	CCLabelTTF* m_pScore;							// ����
	CCLabelTTF*	m_pLifeCount;						// ��ҷɻ�����

	CCRect	m_screenRect;							// ��Ļ����

	CMyPlane* m_pMyplane;							// ��ҷɻ�
};

#endif