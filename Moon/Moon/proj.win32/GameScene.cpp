#include "GameScene.h"
#include "ConstantRes.h"
#include "Config.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;


CGameLayer::CGameLayer():m_iState(sStatePlaying), m_bIsBgLoad(false), m_bIsFloaterLoad(false)
{

}
CGameLayer::~CGameLayer()
{ 
	pPlayBullet->release();
	pEnemyBullet->release();
	pEnemyPlanes->release();
}

CCScene* CGameLayer::scene()
{
	CCScene* pScene = CCScene::create();

	CGameLayer* pLayer = CGameLayer::create();

	pScene->addChild(pLayer);

	return pScene;
}

bool CGameLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	// 开启触摸
	this->setTouchEnabled(true);

	// 创建数组,retain增加一次引用，最后是在CConfig类中释放
	pPlayBullet = CCArray::create();
	pPlayBullet->retain();

	pEnemyBullet = CCArray::create();
	pEnemyBullet->retain();

	pEnemyPlanes = CCArray::create();
	pEnemyPlanes->retain();

	CConfig::shareCConfig()->resetCConfig();

	this->m_winSize = CCDirector::sharedDirector()->getWinSize();
	this->m_screenRect = CCRectMake(0, 0, this->m_winSize.width, this->m_winSize.height);

	// 初始化背景音乐
	if (CConfig::shareCConfig()->isSoundOn())
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(sz_gameMusic, true);

	// 初始化爆炸效果
	CEffect::shareExplosion();

	// 初始化飞机
	this->initPlane();

	// 初始化UI
	this->initUI();

	// 初始化背景及漂浮物
	this->initBackground();

	// 启动定时器
	this->scheduleUpdate();

	return true;
}

void CGameLayer::initUI()
{
	// 创建字体分数
	this->m_pScore = CCLabelTTF::create(
		"Score:0",
		"微软雅黑",
		15,
		CCSizeMake(100, 30),
		CCTextAlignment::kCCTextAlignmentCenter,
		CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
	this->m_pScore->setAnchorPoint(ccp(0.5, 0));
	this->addChild(this->m_pScore, 100);
	this->m_pScore->setPosition(ccp(this->m_winSize.width-60, this->m_winSize.height - 30));

	// 添加玩家飞机UI
	CCTexture2D* pPlaneTexture = CCTextureCache::sharedTextureCache()->addImage(sz_myplane);
	CCSprite* pLife = CCSprite::createWithTexture(pPlaneTexture, CCRectMake(0, 0, 60, 38));
	pLife->setScale(0.6f);
	pLife->setPosition(ccp(30,this->m_winSize.height-23));
	this->addChild(pLife, 100);

	// 创建飞机生命值
	char life[3];
	sprintf(life, "%d", this->m_pMyplane->getHP());
	this->m_pLifeCount = CCLabelTTF::create(
		life,
		"微软雅黑",
		15,
		CCSizeMake(30, 30),
		CCTextAlignment::kCCTextAlignmentCenter,
		CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
	this->addChild(this->m_pLifeCount, 100);
	this->m_pLifeCount->setPosition(ccp(60, this->m_winSize.height-23));
	this->m_pLifeCount->setColor(ccc3(255, 0, 0));

	// 字体菜单创建
	CCLabelTTF* pGoBackLb = CCLabelTTF::create(
		"Main Menu",
		"微软雅黑",
		15,
		CCSizeMake(100, 20),
		CCTextAlignment::kCCTextAlignmentCenter,
		CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
	CCMenuItemLabel* pGoMenu = CCMenuItemLabel::create(pGoBackLb, this, menu_selector(CGameLayer::mainMenu));

	CCMenu* pMenu = CCMenu::create(pGoMenu, NULL);
	this->addChild(pMenu, 100);
	pMenu->setAnchorPoint(ccp(0, 0));
	pMenu->setPosition(this->m_winSize.width - 50, 20);

}

void CGameLayer::updateUI()
{
	// 更新分数
	char s[30] = "Score:";
	sprintf(s, "%s%d", s, CConfig::shareCConfig()->getScoreVal());
	this->m_pScore->setString(s);

	// 更新飞机剩余生命值
	char life[3];
	sprintf(life, "%d", this->m_pMyplane->getHP());
	this->m_pLifeCount->setString(life);
}

// 碰撞检测
bool CGameLayer::isCollided(CUnitSprite* a, CUnitSprite* b)
{
	if (a == NULL || b == NULL)
	{
		return false;
	}

	CCRect aRect = a->collideRect();
	CCRect bRect = b->collideRect();

	if (!aRect.intersectsRect(bRect))
	{
		return false;
	}

	return true;
}

void CGameLayer::mainMenu(CCObject* pSender)
{
	CCScene* pScene = CMainMenu::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.8f, pScene));

}
void CGameLayer::update(float delta)
{
	if (this->m_iState == sStatePlaying)
	{
		this->checkAllIsCollide();
		this->updateUI();
		this->updatePlane(delta);
	}
}


void CGameLayer::updatePlane(float delta)
{
	for (int i = 0; i < pEnemyPlanes->count(); ++i) 
	{
		CEnemyPlane* pEnemy = dynamic_cast<CEnemyPlane*>(pEnemyPlanes->objectAtIndex(i));
		if (pEnemy != NULL)
		{
			if (!pEnemy->isActive())
			{
				pEnemy->destroy();
				CEnemyPlane* pEnemy = CEnemyPlane::createPlane();
				pEnemy->initAction(this->m_pMyplane->getPosition());
				this->addChild(pEnemy, 99);
				pEnemyPlanes->addObject(pEnemy);
			}
		}
    }

	// 玩家生命结束，游戏结束
	if (this->m_pMyplane->getHP() <= 0)
	{
		this->m_iState = sStateGameOver;
		this->m_pMyplane->destroy();
		this->m_pMyplane = NULL;
		CCCallFunc* pGameOver = CCCallFunc::create(this, callfunc_selector(CGameLayer::gameOver));
		this->runAction(CCSequence::create(CCDelayTime::create(2.0f), pGameOver, NULL));
	}
}
void CGameLayer::checkAllIsCollide()
{
	CCObject* pBullets;
	CCObject* pEnemys;

	// 循环敌机和玩家子弹和玩家飞机碰撞检测
	CCARRAY_FOREACH(pEnemyPlanes, pEnemys)
	{
		CUnitSprite* pEnemy = (CUnitSprite*)(pEnemys);

		// 子弹与敌机碰撞检测
		CCARRAY_FOREACH(pPlayBullet, pBullets)
		{	 
			CUnitSprite* pBullet = (CUnitSprite*)(pBullets);

			if (this->isCollided(pEnemy, pBullet))
			{
				pEnemy->hurt();
				if (pBullet->isActive())
					pBullet->destroy();
			}
			if (!(this->m_screenRect.intersectsRect(pBullet->boundingBox())))
			{
				if (pBullet->isActive())
					pBullet->destroy();
				//	CCLog("destroy success in playbullet");
			}
		}
		// 敌机与玩家飞机碰撞检测
		if (this->isCollided(pEnemy,this->m_pMyplane))
		{
			if (this->m_pMyplane->isActive())
			{
				pEnemy->hurt();
				this->m_pMyplane->hurt();
			}
		}
		if (!(this->m_screenRect.intersectsRect(pEnemy->boundingBox()))) 
		{
            pEnemy->hurt();
        }
	}

	CCARRAY_FOREACH(pEnemyBullet, pBullets)
	{
		CUnitSprite* pBullet = (CUnitSprite*)(pBullets);
		if (pBullet != NULL)
		{
			if (this->isCollided(pBullet, this->m_pMyplane))
			{
				if (this->m_pMyplane->isActive())
				{
					this->m_pMyplane->hurt();
					if (pBullet->isActive())
						pBullet->destroy();
				}
			}
		}
		if (!(this->m_screenRect.intersectsRect(pBullet->boundingBox())))
		{
			if (pBullet->isActive())
				pBullet->destroy();
			//	CCLog("destroy success in enemybullet");
		}
	}
}

/**
*滚动背景，这次是使用2张背景图然后循环加载滚动
*/
void CGameLayer::initBackground()
{
	// 加载背景图片
	this->m_pBg = CCSprite::create(sz_b01);
	this->m_pBg->setAnchorPoint(ccp(0, 0));
	this->m_pBgHeight = this->m_pBg->getContentSize().height;
	this->addChild(m_pBg, 0);

	// 加载漂浮物
	this->m_pFloaterMap = CCTMXTiledMap::create(sz_level01);
	this->addChild(this->m_pFloaterMap, 1);
	float height = this->m_pFloaterMap->getMapSize().height;
	this->m_pFloaterHeight = height * height;

	this->m_pBgHeight -= 48;
	this->m_pFloaterHeight -= 200;
	this->m_pBg->runAction(CCMoveBy::create(3, ccp(0, 0-BG_MOVESPEED)));
	this->m_pFloaterMap->runAction(CCMoveBy::create(3, ccp(0, 0-FLOATER_MOVESPEED)));

	// 设置定时器以滚动背景
	this->schedule(schedule_selector(CGameLayer::updateBackground),3);

}

void CGameLayer::updateBackground(float delta)
{
	this->m_pBg->runAction(CCMoveBy::create(3, ccp(0, 0-BG_MOVESPEED)));
	this->m_pFloaterMap->runAction(CCMoveBy::create(3, ccp(0, 0-FLOATER_MOVESPEED)));

	this->m_pBgHeight -= BG_MOVESPEED;
	this->m_pFloaterHeight -= FLOATER_MOVESPEED;

	// 背景图的顶部达到了屏幕顶部的时候
	if (this->m_pBgHeight <= this->m_winSize.height)
	{
		// 当前还没加载另一背景图的时候
		if (!this->m_bIsBgLoad)
		{
			// 这里尝试每次都加载一次资源。没有使用CCTextureCache
			this->m_pBg2 = CCSprite::create(sz_b01);
			this->m_pBg2->setAnchorPoint(ccp(0, 0));
			this->addChild(this->m_pBg2, 0);
			this->m_pBg2->setPosition(ccp(0, this->m_winSize.height));
			this->m_bIsBgLoad = true;
		}

		this->m_pBg2->runAction(CCMoveBy::create(3, ccp(0, 0-BG_MOVESPEED)));
	}

	// 第一张图完全离开屏幕后，移除该图，并复制到m_pBg中，置空m_pBg2。
	if (this->m_pBgHeight <= 0.0f)
	{
		this->m_pBgHeight = this->m_winSize.height;

		// 移除并clean掉
		this->removeChild(this->m_pBg, true);

		this->m_pBg = this->m_pBg2;

		this->m_pBg2 = NULL;
		this->m_bIsBgLoad = false;
	}

	// 漂浮物类似于背景图片
	if (this->m_pFloaterHeight <= this->m_winSize.height) {
		if (!this->m_bIsFloaterLoad) {
			this->m_pFloaterMap2 = CCTMXTiledMap::create(sz_level01);
			this->addChild(this->m_pFloaterMap2, 1);
			this->m_pFloaterMap2->setPosition(0, this->m_winSize.height);
			this->m_bIsFloaterLoad = true;
		}
		this->m_pFloaterMap2->runAction(CCMoveBy::create(3, ccp(0, 0-FLOATER_MOVESPEED)));
	}

	if (this->m_pFloaterHeight <= 0.0f) {
		this->m_pFloaterHeight = this->m_pFloaterMap->getMapSize().height * this->m_pFloaterMap->getTileSize().height;
		/*	char a[10] = "";
		sprintf(a, "%.2f", this->m_pFloaterHeight);
		CCLog(a);*/
		this->removeChild(this->m_pFloaterMap, true);
		this->m_pFloaterMap = this->m_pFloaterMap2;
		this->m_pFloaterMap2 = NULL;
		this->m_bIsFloaterLoad = false;
	}
}

void CGameLayer::initPlane()
{
	// 加载玩家飞机
	this->m_pMyplane = CMyPlane::create();
	this->addChild(this->m_pMyplane, 99, 1001);

	// 加载敌机
	for (int i = 0; i < CConfig::shareCConfig()->getLevelState(); i++)
	{
		CEnemyPlane* pEnemy = CEnemyPlane::createPlane();
		
		// 根据玩家飞机位置初始化动作
		pEnemy->initAction(this->m_pMyplane->getPosition());
		this->addChild(pEnemy, 99);
		pEnemyPlanes->addObject(pEnemy);
	}
}


// 游戏结束回调函数
void CGameLayer::gameOver()
{
	CCScene* pScene = CGameOver::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.8f, pScene));
}

bool CGameLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	return true;
}

void CGameLayer::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	if ((this->m_iState == sStatePlaying) && this->m_pMyplane)
	{
		// 修改当前飞机位置
		CCPoint pos = pTouch->getDelta();
		CCPoint currentPos = this->m_pMyplane->getPosition();
		currentPos = ccpAdd(currentPos, pos);

		// 锁住只能在屏幕区域范围移动
		currentPos = ccpClamp(currentPos, CCPointZero, ccp(this->m_winSize.width, this->m_winSize.height));
		this->m_pMyplane->setPosition(currentPos);
	}
}

void CGameLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{

}

// 分别注册和注销系统的触摸事件
void CGameLayer::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCLayer::onEnter();
}
void CGameLayer::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}