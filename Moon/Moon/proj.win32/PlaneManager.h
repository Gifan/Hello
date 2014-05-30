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
* �ɻ�������
*/
class CPlaneManager
{
public:
	~CPlaneManager();
	static CPlaneManager* sharePlaneManager();
	void initPlane(CCLayer* pLayer);					// ��ʼ���ɻ�
	void initExplode();									// ��ʼ����ըЧ��
	void updatePlane(CCLayer* pLayer, float delta);	// ���·ɻ�
	
	CCArray* getPlayBullets();							// ��ȡ����ӵ�
	CCArray* getEnemyBullets();							// ��ȡ�л��ӵ�
	CCArray* getEnemyPlanes();							// ��ȡ�л�
	CMyPlane* getMyPlane();								// ��ȡ��ҷɻ�
	void setMyPlanePos(CCPoint pos);
	void playExplode(CCLayer* pLayer, CCPoint pos);	    // ���ƻ���Ч��
	void release();										// �ͷ�

private:
	CPlaneManager();
	CCArray* m_pPlayBullet;								// ����ӵ�
	CCArray* m_pEnemyBullet;								// �л��ӵ�
	CCArray* m_pEnemyPlanes;								// �л�

	CMyPlane* m_pMyPlane;								// ��ҷɻ�
};

#endif