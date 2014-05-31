#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "cocos2d.h"

USING_NS_CC;

/**
* ���˵���
*/
class CMainMenu : public cocos2d::CCLayer
{

public:

	// ��ʼ����Դ
	virtual bool init();

	// ��ȡ����
	static CCScene* scene();

	// ���·ɻ�λ��
	virtual void update(float delta);

	// ��ʼ��Ϸ��Ч
	void startEffect(CCObject* pSender);

	// ������ť��Ч
	void onButtonEffect();
	// ��ʼ����Ϸ
	void newGame();

	// �����˵�ѡ��ص�����
	void menuCallback(CCObject* pSender);
	void menuCloseCallback(CCObject* pSender);

	CREATE_FUNC(CMainMenu);

private:
	CMainMenu(){}
	CCSprite* m_myPlane;                  // �ƶ��ķɻ�
	CCSize m_winSize;					   // ���ڴ�С
};

#endif