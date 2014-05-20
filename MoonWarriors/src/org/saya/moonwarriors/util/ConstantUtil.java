package org.saya.moonwarriors.util;

public class ConstantUtil {

	// 触摸状态常量
	public static final int KEY_DOWN = 1;
	public static final int KEY_UP = 0;

	// 消息传递常量
	public static final int HAND_NEW_GAME = 1;
	public static final int HAND_OPTION = 2;
	public static final int HAND_ABOUT = 3;
	public static final int HAND_OVER = 4;
	public static final int HAND_MAIN = 5;

	// 屏幕大小
	public static final int SCREEN_HEIGHT = 480;
	public static final int SCREEN_WIDTH = 320;

	// 游戏难度
	public static final int LV_EASY = 3;
	public static final int LV_NORMAL = 4;
	public static final int LV_HARD = 5;
	public static final int LV_KIND = 3;

	// 游戏声音常量
	public static final int SOUND_ON = 0;
	public static final int SOUND_OFF = 1;

	// 游戏状态
	public static final int GAME_WAIT = 0;
	public static final int GAME_DOING = 1;
	public static final int GAME_FAILD = 2;

	// 游戏音乐类型
	public static final int MUSIC_AT_OTHER = 0;
	public static final int MUSIC_AT_GAME = 1;

	// 当前游戏画面状态
	public static final int MAIN_VIEW = 0;
	public static final int GAME_VIEW = 1;
	public static final int OPTION_VIEW = 2;
	public static final int ABOUT_VIEW = 3;
	public static final int FAILED_VIEW = 4;

	// 子弹
	public static final int ENEMY_BULLET = 0;
	public static final int MY_BULLET = 1;
	public static final int MY_BULLET_COUNT = 16;
	// 图片分割数
	public static final int PIC_COUNT = 72;

	// 漂浮物数量
	public static final int FLOAT_COUNT = 2;

	//爆炸声音
	public static final int ENEMY_EXPLODE = 1;
	public static final int SHIP_EXPLODE = 2;
}
