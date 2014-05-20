package org.saya.moonwarriors.model;

import android.graphics.Bitmap;
import android.graphics.Rect;

/**
 * Ư������
 * @author Saya
 *
 */
public class Floater {
	private Bitmap floater;			// Ư����ͼƬ	
	private int moveSpan = 2;		// �ƶ����
	private int type;				// ����
	private final Rect[] RECT_TYPE = {// ����ͼƬ����
			new Rect(0, 0, 240, 240),
			new Rect(240, 0, 480, 240),
			new Rect(0, 240, 240, 480),
			new Rect(240, 240, 480, 480) };

	public Floater(int type, Bitmap floater) {
		this.floater = floater;
		this.type = type;
	}

	// �������Ư��������
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
