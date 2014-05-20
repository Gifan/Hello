package org.saya.moonwarriors.service;

/**
 * 等级服务类
 * @author Saya
 *
 */
public class LevelService {
	private static int level;	// 当前等级
	public static int getLevel() {
		return level;
	}

	public static void setLevel(int level) {
		LevelService.level = level;
	}
	
}
