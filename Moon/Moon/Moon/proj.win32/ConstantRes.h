#ifndef CONSTANTRES_H_
#define CONSTANTRES_H_

// 资源名称
static const char sz_menu[]               = "menu.png";
static const char sz_bg01[]               = "bg01.png";
static const char sz_myPlane[]            = "plane.png";
static const char sz_logo[]               = "logo.png";
static const char sz_conLogo[]            = "menuTitle.png";
static const char sz_b01[]                = "bg01.jpg";
static const char sz_level01[]            = "level01.tmx";
static const char sz_myplane[]            = "plane.png";
static const char sz_bullet[]             = "bullet.png";
static const char sz_bulletPlist[]        = "bullet.plist";
static const char sz_hit[]                = "hit.jpg";
static const char sz_enemyPlist[]         = "Enemy.plist";
static const char sz_enemyPlane[]         = "Enemy.png";
static const char sz_explosionPlist[]     = "explosion.plist";
static const char sz_gameOver[]           = "gameOver.png";

static const char sz_gameMusic[]          = "Music/bgMusic.mp3";
static const char sz_btnEffect[]          = "Music/buttonEffect.mp3";
static const char sz_explode[]            = "Music/explodeEffect.mp3";
static const char sz_mainMusic[]          = "Music/mainMainMusic.mp3";
static const char sz_destroyPlane[]       = "Music/planeDestroyEffect.mp3";

// 菜单选项
static const int CLICK_OPTION             = 20;
static const int CLICK_ABOUT              = 21;

// 音效选项及状态
static const int CLICK_SOUND              = 1;
static const bool SOUND_ON                 = TRUE;
static const bool SOUND_OFF                = FALSE;

// 难度选项及状态
static const int CLICK_MODE               = 2;
static const int MODE_EASY                = 3;
static const int MODE_NORMAL              = 4;
static const int MODE_HARD                = 5;
static const int MODE_NUM                 = 3;

// 背景及漂浮物移动速度
static const float BG_MOVESPEED           = 48;
static const float FLOATER_MOVESPEED      = 200;

// 子弹类型
static const int ENEMY_BULLET             = 0;
static const int MYPLANE_BULLET           = 1;

#endif