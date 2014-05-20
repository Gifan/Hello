package org.saya.moonwarriors.view;

import java.util.ArrayList;

import org.saya.moonwarriors.MainActivity;
import org.saya.moonwarriors.model.Bullet;
import org.saya.moonwarriors.model.EnemyPlane;
import org.saya.moonwarriors.model.Explosion;
import org.saya.moonwarriors.model.Floater;
import org.saya.moonwarriors.model.MyPlane;
import org.saya.moonwarriors.service.LevelService;
import org.saya.moonwarriors.service.MusicService;
import org.saya.moonwarriors.thread.GameViewThread;
import org.saya.moonwarriors.util.ConstantUtil;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.Rect;
import android.os.Message;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.Toast;

import com.example.moonwarriors.R;

/**
 * 游戏主视图类
 * @author Saya
 *
 */
public class GameView extends SurfaceView implements SurfaceHolder.Callback {

	private MainActivity mainActivity;				// 游戏主面板
	private GameViewThread gameThread;				// 游戏线程
	private MyPlane myPlane;						// 玩家飞机
	private EnemyPlane[] enemyPlanes;				// 敌机
	private Floater[] floater;						// 漂浮物
	private ArrayList<Explosion> exs;				// 爆炸效果数组
	private ArrayList<Rect> hints;					// 击中效果数组
	private Bitmap bitmapExplosion;					// 爆炸图片
	private Bitmap gameRes;							// 游戏资源
	private Bitmap hint;							// 击中效果图片
	private Bitmap bitmapFloater;					// 漂浮物图片
	private Bitmap bitmapPlane;						// 飞机图片
	private Bitmap bitmapBullet;					// 子弹图片
	private Paint paint;							// 画笔
	private Rect[] src_bg;							// 背景源区域
	private Rect[] des_bg;							// 背景目标区域
	private Rect[] des_floater;						// 漂浮物目标区域
	private Rect[] des_enemy;						// 敌机目标区域
	private Rect des_myPlane;						// 玩家飞机目标区域
	private Rect src_hint;							// 击中效果源区域
	private static int cycle = 0;					// 背景滚动循环辅助变量
	private int cycle_start;						//
	private int pict_split;							// 图片分割后 大小 
	private boolean isDownMove;						// 是否触摸屏幕移动飞机
	private Point temp;								// 移动飞机辅助坐标
	private int score;								// 记录分数
	private String fps = "FPS:0.0";					

	public GameView(MainActivity activity) {
		super(activity);
		this.mainActivity = activity;
		getHolder().addCallback(this);
		this.init();
		this.gameThread = new GameViewThread(getHolder(), this);
	}

	public void init() {
		this.exs = new ArrayList<Explosion>();
		this.hints = new ArrayList<Rect>();
		this.isDownMove = false;
		this.temp = new Point();
		this.paint = new Paint();
		this.score = 0;
		this.src_hint = new Rect(0, 0, this.hint.getWidth(),
				this.hint.getHeight());
		
		// 初始化图片资源
		this.initBitmap();
		
		// 初始化玩家飞机
		this.initMyPlane();
		
		// 初始化敌机
		this.initEnemyPlane();
		
		// 初始化背景
		this.initBackGround();

	}

	private void initBitmap() {
		this.gameRes = BitmapFactory.decodeResource(getResources(),
				R.drawable.texturetransparentpack);
		this.bitmapFloater = BitmapFactory.decodeResource(getResources(),
				R.drawable.b01);
		this.bitmapPlane = BitmapFactory.decodeResource(getResources(),
				R.drawable.plane);
		this.bitmapBullet = BitmapFactory.decodeResource(getResources(),
				R.drawable.bullet);
		this.bitmapExplosion = BitmapFactory.decodeResource(getResources(),
				R.drawable.explosion1);
		this.hint = BitmapFactory.decodeResource(getResources(),
				R.drawable.hint);

	}

	private void initMyPlane() {
		this.myPlane = new MyPlane(this.bitmapPlane);
		this.des_myPlane = new Rect(ConstantUtil.SCREEN_WIDTH - 194,
				ConstantUtil.SCREEN_HEIGHT - 70,
				ConstantUtil.SCREEN_WIDTH - 130,
				ConstantUtil.SCREEN_HEIGHT - 22);
		
		// 初始化玩家飞机子弹
		int left = this.des_myPlane.centerX();
		
		for (int i = 0; i < ConstantUtil.MY_BULLET_COUNT; i++) {
			Bullet b = new Bullet(ConstantUtil.MY_BULLET, this.bitmapBullet);
			this.myPlane.setBullet(b, i);

			Rect t = new Rect(left, (this.des_myPlane.top - 21) - (i * 32),
					left + 8, this.des_myPlane.top - (i * 32));
			this.myPlane.setBulletPos(t, i);
		}
	}

	private void initEnemyPlane() {
		this.enemyPlanes = new EnemyPlane[LevelService.getLevel()];
		this.des_enemy = new Rect[LevelService.getLevel()];

		for (int i = 0; i < LevelService.getLevel(); i++) {
			
			// 创建敌机
			this.createEnemyPlane(i);
		}
	}

	// 创建敌机，随机生成位置
	private void createEnemyPlane(int i) {
		int type = (int) (Math.random() * 3);
		int x = (int) (Math.random() * ConstantUtil.SCREEN_WIDTH);
		int y = 0;
		this.enemyPlanes[i] = new EnemyPlane(type, this.bitmapPlane);
		this.enemyPlanes[i].setBullet(new Bullet(ConstantUtil.ENEMY_BULLET,
				this.bitmapBullet));

		int h = this.enemyPlanes[i].getBitmapHeight(type);
		int w = this.enemyPlanes[i].getBitmapWidth(type);

		this.des_enemy[i] = new Rect(x, y, x + w, y + h);
	}

	// 把背景图分割成数块，用于制作滚动效果
	private void initBackGround() {

		this.src_bg = new Rect[ConstantUtil.PIC_COUNT];
		this.des_bg = new Rect[ConstantUtil.PIC_COUNT];
		int h = this.gameRes.getHeight();
		int w = ConstantUtil.SCREEN_WIDTH;
		this.pict_split = h / ConstantUtil.PIC_COUNT;
		for (int i = 0; i < ConstantUtil.PIC_COUNT; i++) {
			this.src_bg[i] = new Rect(0, i * this.pict_split, w, (i + 1)
					* this.pict_split);
		}
		for (int i = 0; i < ConstantUtil.PIC_COUNT - 4; i++) {
			this.des_bg[i] = new Rect(this.src_bg[i]);
		}
		this.cycle_start = ConstantUtil.PIC_COUNT;

		// 创建漂浮物
		this.des_floater = new Rect[ConstantUtil.FLOAT_COUNT];

		h = this.bitmapFloater.getHeight() / 2;
		w = this.bitmapFloater.getWidth() / 2;
		floater = new Floater[2];
		for (int i = 0; i < 2; i++) {
			floater[i] = new Floater(i, this.bitmapFloater);
		}

		for (int i = 0; i < 2; i++) {
			this.des_floater[i] = new Rect(0, i * h, w / 2, (i + 1) * h);
		}

	}

	// 绘制游戏画面 先绘制到一个Bitmap上，然后整个绘制到画布上防止画面闪烁
	public void draw(Canvas canvas) {

		try {

			Bitmap bak = Bitmap.createBitmap(ConstantUtil.SCREEN_WIDTH,
					ConstantUtil.SCREEN_HEIGHT, Bitmap.Config.ARGB_8888);
			Canvas bakCanvas = new Canvas(bak);

			this.drawBG(bakCanvas);					// 绘制背景
			this.drawExplosion(bakCanvas, paint);	// 绘制爆炸效果
			this.drawPlane(bakCanvas);				// 绘制飞机
			this.drawBullets(bakCanvas);			// 绘制子弹
			this.drawHUD(bakCanvas);				// 绘制HUD
			canvas.drawBitmap(bak, 0, 0, paint);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// 绘制爆炸效果，检测碰撞的时候敌机毁坏后加入一个爆炸效果
	// ，绘制完成移除爆炸效果，击中效果类似
	private void drawExplosion(Canvas canvas, Paint paint) {

		for (int i = 0; i < this.exs.size(); i++) {
			Explosion e1 = this.exs.get(i);
			if (e1.getCur_frame() >= 5) {
				this.exs.remove(e1);
			} else {
				e1.drawExplosion(canvas, paint);
			}
		}
		for (int i = 0; i < this.hints.size(); i++) {
			Rect r = this.hints.get(i);
			canvas.drawBitmap(this.hint, this.src_hint, r, paint);
			this.hints.remove(i);
		}
	}

	// 绘制飞机 玩家飞机生命存在时候绘制，敌机一直绘制
	private void drawPlane(Canvas bakCanvas) {

		if (this.myPlane.getLife() > 0)
			bakCanvas.drawBitmap(this.bitmapPlane, this.myPlane.getPlaneRect(),
					this.des_myPlane, paint);

		for (int i = 0; i < LevelService.getLevel(); i++) {
			bakCanvas.drawBitmap(this.bitmapPlane,
					this.enemyPlanes[i].getRectByType(), this.des_enemy[i],
					paint);
		}
	}

	// 绘制子弹 调用飞机内部绘制函数
	private void drawBullets(Canvas bakCanvas) {
		this.myPlane.drawBullet(bakCanvas, paint, this.des_myPlane);
		for (int i = 0; i < LevelService.getLevel(); i++) {
			this.enemyPlanes[i].drawBullet(bakCanvas, paint);
		}
	}

	// 绘制滚动背景
	private void drawBG(Canvas bakCanvas) {
		for (int i = 0; i < ConstantUtil.PIC_COUNT - 4; i++) {
			Rect des = new Rect(this.des_bg[i]);
			des.bottom += cycle;
			des.top += cycle;
			bakCanvas
					.drawBitmap(this.gameRes,
							this.src_bg[(i + this.cycle_start)
									% ConstantUtil.PIC_COUNT], des, paint);
		}

		for (int i = 0; i < ConstantUtil.FLOAT_COUNT; i++)
			bakCanvas.drawBitmap(this.bitmapFloater,
					this.floater[i].getFloaterType(), this.des_floater[i],
					paint);
	}

	// 绘制fps、玩家飞机生命、分数、主菜单
	private void drawHUD(Canvas bakCanvas) {
		int w = 32;
		int h = 24;
		paint.setAntiAlias(true);
		paint.setColor(Color.WHITE);
		paint.setTextAlign(Paint.Align.CENTER);
		paint.setTextSize(13.0f);
		bakCanvas.drawText("score:" + score, ConstantUtil.SCREEN_WIDTH - 40,
				20, paint);
		paint.setTextSize(15.0f);
		bakCanvas.drawText("Main Menu", ConstantUtil.SCREEN_WIDTH - 40,
				ConstantUtil.SCREEN_HEIGHT, paint);
		bakCanvas.drawBitmap(this.bitmapPlane, this.myPlane.getPlaneRect(0),
				new Rect(0, 0, w, h), paint);
		paint.setColor(Color.RED);
		paint.setTextSize(17.0f);
		bakCanvas.drawText(this.myPlane.getLife() + "", w + 10, h - 10, paint);
		bakCanvas.drawText(this.fps, w + 20, ConstantUtil.SCREEN_HEIGHT - 20,
				paint);
	}

	// 更新游戏
	public void update() {
		this.updateBg();
		this.updatePlane();
		this.collision();
	}

	/**
	 * 简单矩形碰撞检测：
	 * 循环玩家飞机子弹，如果敌机区域完全包含子弹或者部分包含就是碰撞
	 * 部分包含根据重叠面积计算是否是发生碰撞
	 */
	private void collision() {
		if (this.myPlane.getLife() > 0) {
			for (int i = 0; i < LevelService.getLevel(); i++) {
				if (this.isIntersect(this.des_enemy[i], this.des_myPlane, 150)
						|| this.des_myPlane.contains(this.enemyPlanes[i]
								.getDes_bullet())) {
					
					// 发生碰撞 产生玩家飞机爆炸效果加入队列
					Explosion e1 = new Explosion(this.bitmapExplosion,
							this.des_myPlane);
					this.exs.add(e1);
					
					// 飞机生命-1  简化游戏，生命只有1
					this.myPlane.setLife(this.myPlane.getLife() - 1);

					// 敌机击中玩家飞机，子弹消失
					if (this.des_myPlane.contains(this.enemyPlanes[i]
							.getDes_bullet())) {
						this.enemyPlanes[i].bullet.setLive(false);
					}
					
					// 玩家生命没有了就向主面板发送消息切换到游戏结束画面
					if (this.myPlane.getLife() <= 0) {
						MusicService.playRating(ConstantUtil.SHIP_EXPLODE);
						Message msg = this.mainActivity.getMyHandler()
								.obtainMessage(ConstantUtil.HAND_OVER);
						msg.arg1 = this.score;
						this.mainActivity.getMyHandler().sendMessageDelayed(
								msg, 3000);   // 延迟3秒发送消息
					}
				}
			}
			
			// 玩家子弹与敌机碰撞检测，玩家飞机子弹不存在的不进行检测 
			for (int i = 0; i < ConstantUtil.MY_BULLET_COUNT; i++) {
				if (!this.myPlane.getBullet(i).isLive())
					continue;
				Rect r = this.myPlane.getBulletByindex(i);
				
				// 循环敌机，击中则产生击中效果加入队列，击溃敌机则爆咋效果加入爆炸队列
				for (int j = 0; j < LevelService.getLevel(); j++) {
					if (this.des_enemy[j].contains(r)
							|| this.isIntersect(this.des_enemy[j], r, 50)) {
						this.score += 10;
						this.hints.add(this.des_enemy[j]);
						this.myPlane.setBulletLive(i, false);
						
						if (this.enemyPlanes[j].getLife() < 1) {
							MusicService.playRating(ConstantUtil.ENEMY_EXPLODE);
							this.score += 100;
							Explosion e1 = new Explosion(this.bitmapExplosion,
									this.des_enemy[j]);
							this.exs.add(e1);
							this.createEnemyPlane(j);
						} else {
							this.enemyPlanes[j].setLife(this.enemyPlanes[j]
									.getLife() - 1);
						}
						break;
					}
					
				}
			}
		}
	}

	/**
	 * 更新背景滚动
	 * 把背景图片分割成ConstantUtil.PIC_COUNT份，0-ConstantUtil.PIC_COUNT-1
	 * 当背景滚动了1份后即pict_split 就换下一张。屏幕显示份数少于背景图片分割数。
	 * 漂浮物随机生成。造成远近效果
	 */
	private void updateBg() {
		cycle++;
		cycle %= this.pict_split;
		if (cycle == 0) {
			this.cycle_start--;
			if (this.cycle_start < 0)
				this.cycle_start = ConstantUtil.PIC_COUNT - 1;
		}

		for (int i = 0; i < 2; i++) {
			this.des_floater[i].top += this.floater[i].getMoveSpan();
			this.des_floater[i].bottom += this.floater[i].getMoveSpan();
			if (this.des_floater[i].top > ConstantUtil.SCREEN_HEIGHT) {
				this.des_floater[i].bottom = 0;
				this.des_floater[i].top = 0 - this.floater[i].getBitmap()
						.getHeight();
				this.des_floater[i].left = (int) (Math.random()
						* ConstantUtil.SCREEN_WIDTH - 100);
				this.des_floater[i].right = this.des_floater[i].left
						+ this.floater[i].getBitmap().getWidth() / 2;
			}
		}
	}

	/**
	 * 初始化敌机按照难度选择有不同数量的敌机和血量。
	 * 到达场景后根据玩家坐标做范围移动，并发射一颗子弹。
	 */
	private void updatePlane() {
		for (int i = 0; i < LevelService.getLevel(); i++) {
			int desx = this.enemyPlanes[i].getDes_x();
			int desy = this.enemyPlanes[i].getDes_y();
			int speed = this.enemyPlanes[i].getSpeed();
			int spanx = Math.abs(desx - this.des_enemy[i].left);
			int spany = Math.abs(desy - this.des_enemy[i].top);
			if (spanx <= speed && spany <= speed) {
				this.enemyPlanes[i].setSpeed(1);
				this.enemyPlanes[i].setDes_x(this.des_myPlane.left
						+ (int) (Math.random() * 100));
				if (!this.enemyPlanes[i].bullet.isLive()) {
					this.enemyPlanes[i].bullet.setLive(true);
					this.enemyPlanes[i].setDes_bullet(new Rect(
							this.des_enemy[i].centerX(),
							this.des_enemy[i].bottom, this.des_enemy[i]
									.centerX() + 8,
							this.des_enemy[i].bottom + 8));
				}

			} else {
				if (spanx > speed) {
					if (this.des_enemy[i].left < desx) {
						this.des_enemy[i].left += speed;
						this.des_enemy[i].right += speed;
					} else if (this.des_enemy[i].left > desx) {
						this.des_enemy[i].left -= speed;
						this.des_enemy[i].right -= speed;
					}
				}
				if (spany > speed) {
					if (this.des_enemy[i].top < desy) {
						this.des_enemy[i].top += speed;
						this.des_enemy[i].bottom += speed;
					} else if (this.des_enemy[i].top > desy) {
						this.des_enemy[i].top -= speed;
						this.des_enemy[i].bottom -= speed;
					}
				}
			}
		}
	}

	/**
	 * 
	 * @param a1 碰撞检测矩形1
	 * @param a2 碰撞检测矩形2
	 * @param inter 面积重叠值
	 * @return true 发生了碰撞  false没有发生碰撞
	 */
	public boolean isIntersect(Rect a1, Rect a2, int inter) {
		int w1 = a1.right - a1.left;
		int w2 = a2.right - a2.left;
		int h1 = a1.bottom - a1.top;
		int h2 = a2.bottom - a2.top;
		if (a1.left < a2.left + w2 && a2.left < a1.left + w1
				&& a1.top < a2.top + h2 && a2.top < a1.top + h1) {

			int minx = Math.min(a1.left, a2.left);
			int maxx = Math.max(a1.left, a2.left);
			int miny = Math.min(a1.top, a2.top);
			int maxy = Math.max(a1.top, a2.top);

			int s = (minx - maxx + w1) * (miny - maxy + h1);
			if (s > inter)
				return true;
		}
		return false;
	}

	//触摸事件 实现玩家飞机移动
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int x = (int) event.getX();
		int y = (int) event.getY();

		if (event.getAction() == MotionEvent.ACTION_UP) {
			this.isDownMove = false;
			if (x > ConstantUtil.SCREEN_WIDTH - 80
					&& x < ConstantUtil.SCREEN_WIDTH
					&& y < ConstantUtil.SCREEN_HEIGHT
					&& y > ConstantUtil.SCREEN_HEIGHT - 20) {
				Message msg = this.mainActivity.getMyHandler().obtainMessage(
						ConstantUtil.HAND_MAIN);
				this.mainActivity.getMyHandler().sendMessage(msg);
			}
		}
		if (event.getAction() == MotionEvent.ACTION_DOWN
				|| event.getAction() == MotionEvent.ACTION_MOVE) {
			if (this.myPlane.getLife() > 0) {
				if (!this.isDownMove) {
					this.temp.x = x;
					this.temp.y = y;
					this.isDownMove = true;
				} else {
					int xSpan = this.temp.x - x;
					int ySpan = this.temp.y - y;
					if (this.des_myPlane.left - xSpan > 0
							&& this.des_myPlane.right - xSpan < ConstantUtil.SCREEN_WIDTH) {
						this.des_myPlane.left -= xSpan;
						this.des_myPlane.right -= xSpan;
					}
					if (this.des_myPlane.top - ySpan > 0
							&& this.des_myPlane.bottom - ySpan < ConstantUtil.SCREEN_HEIGHT) {
						this.des_myPlane.top -= ySpan;
						this.des_myPlane.bottom -= ySpan;
					}
					this.temp.x = x;
					this.temp.y = y;
				}
			}
		}
		return true;
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {

	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		this.gameThread.setFlag(true);
		this.gameThread.start();
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		boolean retry = true;
		this.gameThread.setFlag(false);
		while (retry) {
			try {
				this.gameThread.join();
				retry = false;
			} catch (InterruptedException e) {// 不断地循环，直到刷帧线程结束
			}
		}
	}

	public void setGameThread(GameViewThread gameViewThread) {
		this.gameThread = gameViewThread;
	}

	public void setFps(String string) {
		this.fps = string;
	}
}
