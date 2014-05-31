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
	bool isGameOver();								// �ж��Ƿ���Ϸ����

	void checkAllIsCollide();						// �����ײ
	void gameOver();								// ��Ϸ����
	void mainMenu(CCObject* pSender);				// �������˵��ص�����
	CREATE_FUNC(CGameLayer);

private:
	int m_iState;									// ��ǰ��Ϸ״̬
	CCSize m_winSize;
};

#endif