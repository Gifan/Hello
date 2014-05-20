package org.saya.moonwarriors.model;

import org.saya.moonwarriors.util.ConstantUtil;
import org.saya.moonwarriors.view.GameView;

import android.graphics.Bitmap;
import android.graphics.Rect;

/**
 * 子弹类
 * @author Saya
 * 
 */
public class Bullet {
	private int x;      			// 子弹x坐标（备用）
	private int y;					// 子弹y坐标（备用）
	private Rect rBullet; 			// 子弹图片区域
	private int bullet_type;		// 子弹类型
	private Bitmap bulletBitmap;	// 子弹图片
	private boolean isLive;			// 子弹是否存在
	private int speed;				// 子弹飞行速度

	public Bullet(int type, Bitmap bitmap) {
		this.bullet_type = type;
		this.bulletBitmap = bitmap;
		this.speed = type * 20 + 2;
		if (type == ConstantUtil.MY_BULLET) {
			rBullet = new Rect(0, 8, 8, this.bulletBitmap.getHeight());
			this.isLive = true;
		} else {
			rBullet = new Rect(0, 0, 8, 8);
			this.isLive = false;
		}
	}

	public boolean isLive() {
		return isLive;
	}

	public void setLive(boolean isLive) {
		this.isLive = isLive;
	}

	public int getX() {
		return x;
	}

	public void setX(int x) {
		this.x = x;
	}

	public int getY() {
		return y;
	}

	public void setY(int y) {
		this.y = y;
	}

	public Rect getrBullet() {
		return rBullet;
	}

	public void setrBullet(Rect rBullet) {
		this.rBullet = rBullet;
	}

	public int getBullet_type() {
		return bullet_type;
	}

	public void setBullet_type(int bullet_type) {
		this.bullet_type = bullet_type;
	}

	public Bitmap getBulletBitmap() {
		return bulletBitmap;
	}

	public void setBulletBitmap(Bitmap bulletBitmap) {
		this.bulletBitmap = bulletBitmap;
	}

	public int getSpeed() {
		return speed;
	}

	public void setSpeed(int speed) {
		this.speed = speed;
	}

}
