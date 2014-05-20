package org.saya.moonwarriors.model;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.Log;

/**
 * 爆炸效果类
 * @author Saya
 *
 */
public class Explosion {
	private int dely;				// 帧延迟
	private Bitmap bit_explosion;   // 爆炸效果图片
	private Rect[] src_explosion = {// 爆炸图片区域
			new Rect(0, 0, 70, 93),
			new Rect(0, 93, 70, 186),
			new Rect(0, 186, 70, 279),
			new Rect(0, 279, 70, 372),
			new Rect(0, 372, 70, 465) };
	private Rect des_explosion;     // 爆炸目标位置
	private int cur_frame;			// 当前帧

	public Explosion(Bitmap bitmap, Rect des) {
		this.bit_explosion = bitmap;
		this.des_explosion = new Rect(des);
		this.cur_frame = 0;
		this.dely = 0;
	}

	// 绘制爆炸效果 每5帧才换1帧图片
	public void drawExplosion(Canvas canvas, Paint paint) {
		if (this.cur_frame < 5) {
			canvas.drawBitmap(this.bit_explosion,
					this.src_explosion[this.cur_frame], this.des_explosion,
					paint);
			this.dely++;
			if (this.dely > 5) {
				this.cur_frame++;
				this.dely = 0;
			}
		}
	}

	public int getCur_frame() {
		return cur_frame;
	}

	public void setCur_frame(int cur_frame) {
		this.cur_frame = cur_frame;
	}

}
