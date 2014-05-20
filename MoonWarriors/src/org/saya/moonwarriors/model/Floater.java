package org.saya.moonwarriors.model;

import android.graphics.Bitmap;
import android.graphics.Rect;

/**
 * 漂浮物类
 * @author Saya
 *
 */
public class Floater {
	private Bitmap floater;			// 漂浮物图片	
	private int moveSpan = 2;		// 移动间隔
	private int type;				// 类型
	private final Rect[] RECT_TYPE = {// 类型图片区域
			new Rect(0, 0, 240, 240),
			new Rect(240, 0, 480, 240),
			new Rect(0, 240, 240, 480),
			new Rect(240, 240, 480, 480) };

	public Floater(int type, Bitmap floater) {
		this.floater = floater;
		this.type = type;
	}

	// 随机生成漂浮物类型
	public void randomType() {
		this.type = (int) (Math.random() * 4);
	}

	public Rect getFloaterType() {
		return this.RECT_TYPE[type];
	}

	public int getMoveSpan() {
		return moveSpan;
	}

	public Bitmap getBitmap() {
		return floater;
	}
}
