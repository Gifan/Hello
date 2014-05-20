package org.saya.moonwarriors.model;

import org.saya.moonwarriors.service.LevelService;
import org.saya.moonwarriors.util.ConstantUtil;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;

/**
 * 敌人飞机类
 * @author Saya
 *
 */
public class EnemyPlane {
	private int type;					// 飞机类型		
	private int life;					// 飞机生命
	private Bitmap bitmapPlane;			// 飞机图片
	private Rect des_bullet;			// 子弹目标位置
	private int speed;					// 飞机速度
	public Bullet bullet = null;		// 飞机的子弹，目前只包含1粒
	private int des_x, des_y;			// 飞机刚需要到达的位置坐标
	private Rect[] rect_type = { 		// 飞机类型图片区域
			new Rect(0, 0, 80, 46),
			new Rect(0, 48, 64, 88), 
			new Rect(0, 88, 48, 112),
			new Rect(80, 0, 72 + 80, 32) };
	
	private boolean isDead;				// 飞机是否阵亡

	public EnemyPlane(int type, Bitmap plane) {
		this.type = type;
		this.bitmapPlane = plane;
		this.life = LevelService.getLevel() + 5;
		this.isDead = false;
		this.des_x = (int) (Math.random() * ConstantUtil.SCREEN_WIDTH - 80);
		this.des_y = (int) (Math.random() * ConstantUtil.SCREEN_HEIGHT / 2) + 50;
		this.speed = 7;
		this.des_bullet = new Rect(0, 0, 0, 0);
	}

	// 绘制子弹,子弹存在的时候绘制
	public void drawBullet(Canvas canvas, Paint paint) {
		if (this.bullet.isLive()) {
			canvas.drawBitmap(this.bullet.getBulletBitmap(),
					this.bullet.getrBullet(), this.des_bullet, paint);
			this.updateBullet();
		}

	}

	// 更新子弹的位置
	private void updateBullet() {
		if (this.des_bullet.top >= ConstantUtil.SCREEN_HEIGHT) {
			this.bullet.setLive(false);
		} else {
			this.des_bullet.top += this.bullet.getSpeed();
			this.des_bullet.bottom += this.bullet.getSpeed();
		}
	}

	
	// get和set方法
	public Rect getDes_bullet() {
		return des_bullet;
	}

	public void setDes_bullet(Rect des_bullet) {
		this.des_bullet = des_bullet;
	}

	public Bullet getBullet() {
		return bullet;
	}

	public void setBullet(Bullet bullet) {
		this.bullet = bullet;
	}

	public int getType() {
		return type;
	}

	public void setType(int type) {
		this.type = type;
	}

	public int getLife() {
		return life;
	}

	public void setLife(int life) {
		this.life = life;
	}

	public Rect[] getRect_type() {
		return rect_type;
	}

	public void setRect_type(Rect[] rect_type) {
		this.rect_type = rect_type;
	}

	public boolean isDead() {
		return isDead;
	}

	public void setDead(boolean isDead) {
		this.isDead = isDead;
	}

	public Rect getRectByType() {
		return this.rect_type[type];
	}

	public int getBitmapWidth(int type) {
		if (type > 3 && type < 0)
			return 0;
		return (rect_type[type].right - rect_type[type].left);
	}

	public int getBitmapHeight(int type) {
		if (type > 3 && type < 0)
			return 0;
		return (rect_type[type].bottom - rect_type[type].top);
	}

	public int getDes_x() {
		return des_x;
	}

	public void setDes_x(int des_x) {
		this.des_x = des_x;
	}

	public int getDes_y() {
		return des_y;
	}

	public void setDes_y(int des_y) {
		this.des_y = des_y;
	}

	public int getSpeed() {
		return speed;
	}

	public void setSpeed(int speed) {
		this.speed = speed;
	}

}
