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
 * ��Ϸ����ͼ��
 * @author Saya
 *
 */
public class GameView extends SurfaceView implements SurfaceHolder.Callback {

	private MainActivity mainActivity;				// ��Ϸ�����
	private GameViewThread gameThread;				// ��Ϸ�߳�
	private MyPlane myPlane;						// ��ҷɻ�
	private EnemyPlane[] enemyPlanes;				// �л�
	private Floater[] floater;						// Ư����
	private ArrayList<Explosion> exs;				// ��ըЧ������
	private ArrayList<Rect> hints;					// ����Ч������
	private Bitmap bitmapExplosion;					// ��ըͼƬ
	private Bitmap gameRes;							// ��Ϸ��Դ
	private Bitmap hint;							// ����Ч��ͼƬ
	private Bitmap bitmapFloater;					// Ư����ͼƬ
	private Bitmap bitmapPlane;						// �ɻ�ͼƬ
	private Bitmap bitmapBullet;					// �ӵ�ͼƬ
	private Paint paint;							// ����
	private Rect[] src_bg;							// ����Դ����
	private Rect[] des_bg;							// ����Ŀ������
	private Rect[] des_floater;						// Ư����Ŀ������
	private Rect[] des_enemy;						// �л�Ŀ������
	private Rect des_myPlane;						// ��ҷɻ�Ŀ������
	private Rect src_hint;							// ����Ч��Դ����
	private static int cycle = 0;					// ��������ѭ����������
	private int cycle_start;						//
	private int pict_split;							// ͼƬ�ָ�� ��С 
	private boolean isDownMove;						// �Ƿ�����Ļ�ƶ��ɻ�
	private Point temp;								// �ƶ��ɻ���������
	private int score;								// ��¼����
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
		
		// ��ʼ��ͼƬ��Դ
		this.initBitmap();
		
		// ��ʼ����ҷɻ�
		this.initMyPlane();
		
		// ��ʼ���л�
		this.initEnemyPlane();
		
		// ��ʼ������
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
		
		// ��ʼ����ҷɻ��ӵ�
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
			
			// �����л�
			this.createEnemyPlane(i);
		}
	}

	// �����л����������λ��
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

	// �ѱ���ͼ�ָ�����飬������������Ч��
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

		// ����Ư����
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

	// ������Ϸ���� �Ȼ��Ƶ�һ��Bitmap�ϣ�Ȼ���������Ƶ������Ϸ�ֹ������˸
	public void draw(Canvas canvas) {

		try {

			Bitmap bak = Bitmap.createBitmap(ConstantUtil.SCREEN_WIDTH,
					ConstantUtil.SCREEN_HEIGHT, Bitmap.Config.ARGB_8888);
			Canvas bakCanvas = new Canvas(bak);

			this.drawBG(bakCanvas);					// ���Ʊ���
			this.drawExplosion(bakCanvas, paint);	// ���Ʊ�ըЧ��
			this.drawPlane(bakCanvas);				// ���Ʒɻ�
			this.drawBullets(bakCanvas);			// �����ӵ�
			this.drawHUD(bakCanvas);				// ����HUD
			canvas.drawBitmap(bak, 0, 0, paint);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// ���Ʊ�ըЧ���������ײ��ʱ��л��ٻ������һ����ըЧ��
	// ����������Ƴ���ըЧ��������Ч������
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

	// ���Ʒɻ� ��ҷɻ���������ʱ����ƣ��л�һֱ����
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

	// �����ӵ� ���÷ɻ��ڲ����ƺ���
	private void drawBullets(Canvas bakCanvas) {
		this.myPlane.drawBullet(bakCanvas, paint, this.des_myPlane);
		for (int i = 0; i < LevelService.getLevel(); i++) {
			this.enemyPlanes[i].drawBullet(bakCanvas, paint);
		}
	}

	// ���ƹ�������
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

	// ����fps����ҷɻ����������������˵�
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

	// ������Ϸ
	public void update() {
		this.updateBg();
		this.updatePlane();
		this.collision();
	}

	/**
	 * �򵥾�����ײ��⣺
	 * ѭ����ҷɻ��ӵ�������л�������ȫ�����ӵ����߲��ְ���������ײ
	 * ���ְ��������ص���������Ƿ��Ƿ�����ײ
	 */
	private void collision() {
		if (this.myPlane.getLife() > 0) {
			for (int i = 0; i < LevelService.getLevel(); i++) {
				if (this.isIntersect(this.des_enemy[i], this.des_myPlane, 150)
						|| this.des_myPlane.contains(this.enemyPlanes[i]
								.getDes_bullet())) {
					
					// ������ײ ������ҷɻ���ըЧ���������
					Explosion e1 = new Explosion(this.bitmapExplosion,
							this.des_myPlane);
					this.exs.add(e1);
					
					// �ɻ�����-1  ����Ϸ������ֻ��1
					this.myPlane.setLife(this.myPlane.getLife() - 1);

					// �л�������ҷɻ����ӵ���ʧ
					if (this.des_myPlane.contains(this.enemyPlanes[i]
							.getDes_bullet())) {
						this.enemyPlanes[i].bullet.setLive(false);
					}
					
					// �������û���˾�������巢����Ϣ�л�����Ϸ��������
					if (this.myPlane.getLife() <= 0) {
						MusicService.playRating(ConstantUtil.SHIP_EXPLODE);
						Message msg = this.mainActivity.getMyHandler()
								.obtainMessage(ConstantUtil.HAND_OVER);
						msg.arg1 = this.score;
						this.mainActivity.getMyHandler().sendMessageDelayed(
								msg, 3000);   // �ӳ�3�뷢����Ϣ
					}
				}
			}
			
			// ����ӵ���л���ײ��⣬��ҷɻ��ӵ������ڵĲ����м�� 
			for (int i = 0; i < ConstantUtil.MY_BULLET_COUNT; i++) {
				if (!this.myPlane.getBullet(i).isLive())
					continue;
				Rect r = this.myPlane.getBulletByindex(i);
				
				// ѭ���л����������������Ч��������У������л���զЧ�����뱬ը����
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
	 * ���±�������
	 * �ѱ���ͼƬ�ָ��ConstantUtil.PIC_COUNT�ݣ�0-ConstantUtil.PIC_COUNT-1
	 * ������������1�ݺ�pict_split �ͻ���һ�š���Ļ��ʾ�������ڱ���ͼƬ�ָ�����
	 * Ư����������ɡ����Զ��Ч��
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
	 * ��ʼ���л������Ѷ�ѡ���в�ͬ�����ĵл���Ѫ����
	 * ���ﳡ������������������Χ�ƶ���������һ���ӵ���
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
	 * @param a1 ��ײ������1
	 * @param a2 ��ײ������2
	 * @param inter ����ص�ֵ
	 * @return true ��������ײ  falseû�з�����ײ
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

	//�����¼� ʵ����ҷɻ��ƶ�
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
			} catch (InterruptedException e) {// ���ϵ�ѭ����ֱ��ˢ֡�߳̽���
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
