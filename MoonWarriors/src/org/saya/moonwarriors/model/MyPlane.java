package org.saya.moonwarriors.model;

import org.saya.moonwarriors.util.ConstantUtil;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;

/**
 * 玩家飞机类
 * @author Saya
 *
 */
public class MyPlane {
	private Bitmap bitPlane;		// 飞机图片
	private Bullet[] bullets;		// 飞机发射的子弹
	private Rect[] rect_bull;		// 飞机子弹位置
	private int life;				// 飞机生命，简化游戏设置为1
	private int cur_frame;			// 飞机当前帧
	private int dely;				// 飞机帧延迟
	private int bull_count;			// 子弹数量
	private boolean isDead;			// 飞机是否被击溃
	private Rect[] src_plane = {    // 每帧飞机图片区域 
			new Rect(80, 32, 144, 80),
			new Rect(121, 112, 185, 160) };

	public MyPlane(Bitmap plane) {
		this.bitPlane = plane;
		this.cur_frame = 0;
		this.life = 1;
		this.dely = 0;
		this.bullets = new Bullet[ConstantUtil.MY_BULLET_COUNT];
		this.rect_bull = new Rect[ConstantUtil.MY_BULLET_COUNT];
		this.bull_count = 0;
		this.dely = 0;
		this.isDead = false;
	}

	// 绘制子弹，当玩家飞机还存在的时候才绘制
	public void drawBullet(Canvas bakCanvas, Paint paint, Rect planePos) {
		if (this.life > 0) {
			for (int i = 0; i < ConstantUtil.MY_BULLET_COUNT; i++) {
				if (this.bullets[i].isLive())
					bakCanvas.drawBitmap(this.bullets[i].getBulletBitmap(),
							this.bullets[i].getrBullet(), this.rect_bull[i],
							paint);
			}
		}
		this.updateBullet(planePos);
	}

	// 更新子弹位置
	public void updateBullet(Rect planePos) {
		for (int i = 0; i < ConstantUtil.MY_BULLET_COUNT; i++) {
			if (this.rect_bull[i].bottom < 0 && this.bull_count < 1) {
				this.rect_bull[i].left = planePos.centerX();
				this.rect_bull[i].right = planePos.centerX() + 8;
				this.rect_bull[i].bottom = planePos.top;
				this.rect_bull[i].top = planePos.top - 21;
				this.bullets[i].setLive(true);
				this.bull_count++;

			} else {
				this.rect_bull[i].top -= this.bullets[i].getSpeed();
				this.rect_bull[i].bottom -= this.bullets[i].getSpeed();
			}
		}
		if (this.bull_count >= 1)
			this.bull_count--;
	}

	
	// get set 方法
	public Rect getBulletByindex(int index) {
		if (index < 0 || index >= ConstantUtil.MY_BULLET_COUNT)
			return null;
		return this.rect_bull[index];
	}

	public void setBulletLive(int index, boolean b) {
		this.bullets[index].setLive(b);
	}

	public void setBullet(Bullet bullet, int i) {
		this.bullets[i] = bullet;
	}

	public void setBulletPos(Rect rect, int i) {
		this.rect_bull[i] = rect;
	}

	public Rect getPlaneRect() {
		this.dely = (this.dely + 1) % 2;
		if (this.dely == 0)
			this.cur_frame ^= 1;
		return this.src_plane[this.cur_frame];
	}

	public Rect getPlaneRect(int indexFrame) {
		if (indexFrame > 1 || indexFrame < 0)
			return null;
		return this.src_plane[indexFrame];
	}

	public Bullet[] getBullets() {
		return bullets;
	}

	public void setBullets(Bullet[] bullets) {
		this.bullets = bullets;
	}

	public Rect[] getRect_bull() {
		return rect_bull;
	}

	public void setRect_bull(Rect[] rect_bull) {
		this.rect_bull = rect_bull;
	}

	public Bullet getBullet(int index) {
		if (index >= ConstantUtil.MY_BULLET_COUNT || index < 0)
			return null;
		return this.bullets[index];
	}

	public int getLife() {
		return life;
	}

	public void setLife(int life) {
		this.life = life;
	}

	public boolean isDead() {
		return isDead;
	}

	public void setDead(boolean isDead) {
		this.isDead = isDead;
	}

}
