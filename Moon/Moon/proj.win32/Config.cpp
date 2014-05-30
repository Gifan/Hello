#include "Config.h"
#include "ConstantRes.h"

USING_NS_CC;
static CConfig* g_Config = NULL;

CCArray* pPlayBullet;
CCArray* pEnemyBullet;
CCArray* pEnemyPlanes;

CConfig::CConfig():m_bIsSoundOn(true), m_iLevel(MODE_EASY), m_iScoreVal(0)
{

}

CConfig::~CConfig()
{
	if (pPlayBullet != NULL)
	{
		pPlayBullet->release();
		pPlayBullet = NULL;
	}
	if (pEnemyBullet != NULL)
	{
		pEnemyBullet->release();
		pEnemyBullet = NULL;
	}
	if (pEnemyPlanes != NULL)
	{
		pEnemyPlanes->release();
		pEnemyPlanes = NULL;
	}
}

// µ¥ÀýÄ£Ê½
CConfig* CConfig::shareCConfig()
{
	if (g_Config == NULL)
	{
		g_Config = new CConfig();
	}
	return g_Config;
}

bool CConfig::isSoundOn()
{
	return this->m_bIsSoundOn;
}

void CConfig::setSoundState(bool state)
{
	this->m_bIsSoundOn = state;
}

void CConfig::updateSound()
{
	this->m_bIsSoundOn = !this->m_bIsSoundOn;
}


int CConfig::getScoreVal()
{
	return this->m_iScoreVal;
}

void CConfig::setScoreVal(int score)
{
	this->m_iScoreVal = score;
}

void CConfig::addScoreVal(int score)
{
	this->m_iScoreVal += score;
}

int CConfig::getLevelState()
{
	return this->m_iLevel;
}

void CConfig::setLevelState(int state)
{
	this->m_iLevel = state;
}

void CConfig::updateLevel()
{
	this->m_iLevel = (this->m_iLevel+1) % MODE_NUM + 3;
}

void CConfig::resetCConfig()
{
	this->m_iScoreVal = 0;
}



