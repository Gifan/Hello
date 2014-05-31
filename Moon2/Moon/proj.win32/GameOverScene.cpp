#include "GameOverScene.h"
#include "ConstantRes.h"
#include "Config.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

CCScene* CGameOver::scene()
{
	CCScene* pScene = CCScene::create();

	CGameOver* pGameOver = CGameOver::create();

	pScene->addChild(pGameOver);


	return pScene;
}

bool CGameOver::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	// ¼ÓÔØ±³¾°
	CCSprite* pBg01 = CCSprite::create(sz_bg01);

	pBg01->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	this->addChild(pBg01, 0);

	// ¼ÓÔØOver logo
	CCSprite* pLogo = CCSprite::create(sz_gameOver);
    pLogo->setAnchorPoint(ccp(0, 0));
    pLogo->setPosition(ccp(0, 300));
    this->addChild(pLogo, 1);

	// ¼ÓÔØAgain²Ëµ¥
	CCSprite* pPlayAgainNormal = CCSprite::create(
		sz_menu, CCRectMake(378, 0, 126, 33));
	CCSprite* pPlayAgainSelect = CCSprite::create(
		sz_menu, CCRectMake(378, 33, 126, 33));

	CCMenuItemSprite* pPlayAgain = CCMenuItemSprite::create(
		pPlayAgainNormal,
		pPlayAgainSelect,
		NULL,
		this, menu_selector(CGameOver::playAgain));

	CCMenu* pMenu = CCMenu::create(pPlayAgain, NULL);
	this->addChild(pMenu, 1);
	pMenu->setPosition(winSize.width / 2, 220);

	// »æÖÆ·ÖÊý
	char score[40] = "Your Score:";
	sprintf(score, "%s%d", score, CConfig::shareCConfig()->getScoreVal());

	CCLabelTTF *pScore = CCLabelTTF::create(score, "Î¢ÈíÑÅºÚ", 16);
	pScore->setPosition(ccp(winSize.width / 2, 290));
	pScore->setColor(ccc3(250, 189, 0));
	this->addChild(pScore, 1);

	// ¼ÓÔØ±³¾°ÒôÀÖ
	if (CConfig::shareCConfig()->isSoundOn())
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(sz_mainMusic, true);

	return true;
}

void CGameOver::playAgain(CCObject* pSender)
{
	if (CConfig::shareCConfig()->isSoundOn())
		SimpleAudioEngine::sharedEngine()->playEffect(sz_btnEffect);
    CCScene* pScene = CCScene::create();
    pScene->addChild(CGameLayer::scene());
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.8f, pScene));
}