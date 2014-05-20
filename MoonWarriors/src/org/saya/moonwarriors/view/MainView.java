package org.saya.moonwarriors.view;

import org.saya.moonwarriors.MainActivity;
import org.saya.moonwarriors.thread.MainViewThread;
import org.saya.moonwarriors.util.ConstantUtil;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.Rect;
import android.os.Message;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.Toast;

import com.example.moonwarriors.R;

/**
 * 主菜单类
 * @author Saya
 *
 */
public class MainView extends SurfaceView implements SurfaceHolder.Callback {

	private MainActivity mainActivity;				// 主面板
	private MainViewThread mainThread;				// 线程
	private Paint paint;							
	private Bitmap logo;							// 游戏logo
	private Bitmap background;						// 背景图片
	private Bitmap menu;							// 菜单项图片
	private Bitmap plane;							// 飞机图片
	private Rect[] newGame;							// 选项newGame区域
	private Rect[] option;							// 选项option区域
	private Rect[] about;							// 选项about区域
	private Rect src_plane;							// 飞机源区域
	private Rect des_newGame;						// newGame目标区域
	private Rect des_option;						// option目标区域
	private Rect des_about;							// about 目标区域
	private Rect des_plane;							// 飞机目标区域
	private int f_newGame;							// 是否按下newGame
	private int f_option;							// 是否按下option		
	private int f_about;							// 是否按下about
	private boolean bnew_plane;						// 是否新建飞机
	private Point plane_pos;						// 飞机坐标
	private int planeSpeedX;						// 飞机X轴速度
	private int planeSpeedY;						// 飞机Y轴速度

	public MainView(MainActivity activity) {
		super(activity);
		this.mainActivity = activity;

		getHolder().addCallback(this);
		this.f_newGame = ConstantUtil.KEY_UP;
		this.f_option = ConstantUtil.KEY_UP;
		this.f_about = ConstantUtil.KEY_UP;
		this.initBitmap();
		this.initPosition();
		this.mainThread = new MainViewThread(getHolder(), this);
	}

	
	// 初始化图片
	private void initBitmap() {
		this.paint = new Paint();
		this.logo = BitmapFactory.decodeResource(getResources(),
				R.drawable.logo);
		this.background = BitmapFactory.decodeResource(getResources(),
				R.drawable.loading);
		this.menu = BitmapFactory.decodeResource(getResources(),
				R.drawable.menu);
		this.plane = BitmapFactory.decodeResource(getResources(),
				R.drawable.texturetransparentpack);
	}

	// 绘制视图
	public void draw(Canvas canvas) {
		try {
			canvas.drawBitmap(this.background, 0, 0, paint);
			canvas.drawBitmap(this.plane, this.src_plane, this.des_plane, paint);
			canvas.drawBitmap(this.logo, 0, 100, paint);
			canvas.drawBitmap(this.menu, newGame[this.f_newGame], des_newGame,
					paint);
			canvas.drawBitmap(this.menu, option[this.f_option], des_option,
					paint);
			canvas.drawBitmap(this.menu, about[this.f_about], des_about, paint);

			paint.setColor(Color.WHITE);
			paint.setTextSize(20.0f);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// 初始化各个图片位置
	public void initPosition() {
		this.newGame = new Rect[2];
		this.option = new Rect[2];
		this.about = new Rect[2];
		for (int i = 0; i < 2; i++) {
			this.newGame[i] = new Rect(0, i * 33, 125, (i + 1) * 33);
			this.option[i] = new Rect(126, i * 33, 125 + 126, (i + 1) * 33);
			this.about[i] = new Rect(126 + 126, i * 33, 125 + 126 + 126,
					(i + 1) * 33);
		}

		int top = 250;
		int left = 110;
		this.des_newGame = new Rect(left, top, left + 126, top + 34);
		this.des_option = new Rect(left, top + 50, left + 126, top + 50 + 34);
		this.des_about = new Rect(left, top + 100, left + 126, top + 100 + 34);

		this.plane_pos = new Point(100, ConstantUtil.SCREEN_HEIGHT);
		this.des_plane = new Rect(this.plane_pos.x, this.plane_pos.y,
				this.plane_pos.x + 64, this.plane_pos.y + 48);
		this.src_plane = new Rect(448, 114, 448 + 64, 114 + 48);
		this.bnew_plane = true;
		this.planeSpeedX = this.planeSpeedY = 0;
	}

	// 更新飞机坐标，到达边境后随机生成坐标继续飞
	public void updatePlanePos() {
		if (this.bnew_plane) {
			this.plane_pos.x = (int) (Math.random() * 200) + 1;
			this.plane_pos.y = ConstantUtil.SCREEN_HEIGHT;
			this.planeSpeedX = (int) (Math.random() * 5) + 2;
			this.planeSpeedY = (int) (Math.random() * 10) + 4;
			this.bnew_plane = false;
		} else {
			this.plane_pos.x += this.planeSpeedX;
			this.plane_pos.y -= this.planeSpeedY;
			if (this.plane_pos.x > ConstantUtil.SCREEN_WIDTH + 30
					|| this.plane_pos.y < -30) {
				this.bnew_plane = true;
			}
		}
		this.des_plane.left = this.plane_pos.x;
		this.des_plane.top = this.plane_pos.y;
		this.des_plane.right = this.plane_pos.x + 64;
		this.des_plane.bottom = this.plane_pos.y + 48;
	}

	// 触摸事件监听 确定点击之后向主面板发送消息切换对应视图
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int x = (int) event.getX();
		int y = (int) event.getY();

		if (event.getAction() == MotionEvent.ACTION_UP) {
			if (!des_newGame.contains(x, y)) {
				this.f_newGame = ConstantUtil.KEY_UP;

			} else {
				Message msg = this.mainActivity.getMyHandler().obtainMessage(
						ConstantUtil.HAND_NEW_GAME);
				this.mainActivity.getMyHandler().sendMessage(msg);
			}

			if (!des_option.contains(x, y)) {
				this.f_option = ConstantUtil.KEY_UP;
			} else {
				Message msg = this.mainActivity.getMyHandler().obtainMessage(
						ConstantUtil.HAND_OPTION);
				this.mainActivity.getMyHandler().sendMessage(msg);
			}

			if (!des_about.contains(x, y)) {
				this.f_about = ConstantUtil.KEY_UP;
			} else {
				Message msg = this.mainActivity.getMyHandler().obtainMessage(
						ConstantUtil.HAND_ABOUT);
				this.mainActivity.getMyHandler().sendMessage(msg);
			}
		} else if (event.getAction() == MotionEvent.ACTION_DOWN
				|| event.getAction() == MotionEvent.ACTION_MOVE) {
			if (des_newGame.contains(x, y)) {
				this.f_newGame = ConstantUtil.KEY_DOWN;
			} else {
				this.f_newGame = ConstantUtil.KEY_UP;
			}
			if (des_option.contains(x, y)) {
				this.f_option = ConstantUtil.KEY_DOWN;

			} else {
				this.f_option = ConstantUtil.KEY_UP;
			}
			if (des_about.contains(x, y)) {
				this.f_about = ConstantUtil.KEY_DOWN;

			} else {
				this.f_about = ConstantUtil.KEY_UP;
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
		this.mainThread.setFlag(true);
		this.mainThread.start();
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		boolean retry = true;
		this.mainThread.setFlag(false);
		while (retry) {
			try {
				this.mainThread.join();
				retry = false;
			} catch (InterruptedException e) {// 不断地循环，直到刷帧线程结束
				e.printStackTrace();
			}
		}
	}

	// get and set

	public Point getPlane_pos() {
		return plane_pos;
	}

	public void setPlane_pos(Point plane_pos) {
		this.plane_pos = plane_pos;
	}

}
