#include "Effect.h"
#include "ConstantRes.h"

CEffect::CEffect()
{

}

CEffect::~CEffect()
{

}

CEffect* CEffect::create()
{
	CEffect* pEffect = new CEffect();
	if (pEffect != NULL)
	{
		pEffect->autorelease();
		return pEffect;
	}
	else
	{
		delete pEffect;
		pEffect = NULL;
		return NULL;
	}
}

void CEffect::explode(CCNode* parent, CCPoint pos)
{
	CCSpriteFrame* pFrame = 
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("explosion_01.png");
	CCSprite* pExplosion = CCSprite::createWithSpriteFrame(pFrame);
    parent->addChild(pExplosion, 99);
    pExplosion->setPosition(pos);
    
    // 完成之后的回调
    CCCallFuncN* pRemoveFunc =  CCCallFuncN::create(pExplosion, callfuncN_selector(CEffect::removeSprite));
    
    // 爆炸动画
    CCAnimation* pAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("Explosion");
    pExplosion->runAction(CCSequence::create(CCAnimate::create(pAnimation), pRemoveFunc, NULL));

}

//动画加入缓存
void CEffect::shareExplosion()
{

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(sz_explosionPlist);
    CCArray* pAnimFrames = CCArray::create();
    
    char str[64] = {0};
    
	// 载入爆炸效果帧
    for (int i = 1; i < 35; ++i)
	{
        sprintf(str, "explosion_%02d.png", i);
        CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
        pAnimFrames->addObject(pFrame);
    }
    CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(pAnimFrames, 0.04f);
    
    // 帧动画命名
    CCAnimationCache::sharedAnimationCache()->addAnimation(pAnimation, "Explosion");
 
}

void CEffect::removeSprite(CCNode *pSender)
{
    pSender->removeFromParent();
}