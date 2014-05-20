package org.saya.moonwarriors.view;

import org.saya.moonwarriors.MainActivity;
import org.saya.moonwarriors.thread.FailedViewThread;
import org.saya.moonwarriors.util.ConstantUtil;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.os.Message;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.example.moonwarriors.R;

/**
 * 游戏结束视图类
 * @author Saya
 *
 */
public class FailedView extends SurfaceView implements SurfaceHolder.Callback {
	private MainActivity mainActivity; 					// 主面板
	private FailedViewThread failedThread;				// 线程类
	private Bitmap background;							// 背景图片
	private Bitmap logo;								// 选项logo
	private Bitmap menu;								// playAgain菜单
	private Rect[] playAgain;							// 菜单按键帧图片区域
	private Rect des_again;								// 菜单按键目标位置
	private int f_again;								// 按键当前帧
	private Paint paint;								// 画笔
	private int score;									// 游戏分数

	public FailedView(MainActivity activity, int score) {
		super(activity);
		this.score = score;
		this.mainActivity = activity;
		getHolder().addCallback(this);

		this.f_again = ConstantUtil.KEY_UP;
		
		// 初始化图片
		this.initBitmap();
		
		//初始化位置
		this.initPosition();
		this.failedThread = new FailedViewThread(getHolder(), this);
	}

	private void initBitmap() {
		paint = new Paint();
		this.background = BitmapFactory.decodeResource(getResources(),
				R.drawable.loading);
		this.logo = BitmapFactory.decodeResource(getResources(),
				R.drawable.gameover);
		this.menu = BitmapFactory.decodeResource(getResources(),
				R.drawable.menu);
	}

	public void initPosition() {
		int width = this.menu.getWidth() / 4;
		int height = this.menu.getHeight();
		this.playAgain = new Rect[2];
		this.playAgain[0] = new Rect(width * 3, 0, width * 4, 33);
		this.playAgain[1] = new Rect(width * 3, 33, width * 4, 66);

		width = this.background.getWidth();
		height = this.background.getHeight();
		this.des_again = new Rect(width - 220, height - 180, width - 220 + 126,
				height - 180 + 34);

	}

	// 绘制图片
	public void draw(Canvas canvas) {
		try {
			canvas.drawBitmap(this.background, 0, 0, paint);
			canvas.drawBitmap(this.logo, 0, this.background.getHeight() - 380,
					paint);

			paint.setTextSize(24.0f);
			paint.setAntiAlias(true);
			paint.setTextAlign(Paint.Align.CENTER);
			paint.setColor(Color.YELLOW);
			canvas.drawText("Your Score:" + this.score,
					this.background.getWidth() / 2,
					this.background.getHeight() - 250, paint);
			canvas.drawBitmap(this.menu, this.playAgain[this.f_again],
					this.des_again, paint);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	
	// 触摸消息监听
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int x = (int) event.getX();
		int y = (int) event.getY();

		if (event.getAction() == MotionEvent.ACTION_UP) {
			if (!des_again.contains(x, y)) {
				this.f_again = ConstantUtil.KEY_UP;
			} else {
				Message msg = this.mainActivity.getMyHandler().obtainMessage(
						ConstantUtil.HAND_NEW_GAME);
				this.mainActivity.getMyHandler().sendMessage(msg);
			}
		} else if (event.getAction() == MotionEvent.ACTION_DOWN
				|| event.getAction() == MotionEvent.ACTION_MOVE) {
			if (des_again.contains(x, y)) {
				this.f_again = ConstantUtil.KEY_DOWN;
			} else {
				this.f_again = ConstantUtil.KEY_UP;
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
		this.failedThread.setFlag(true);
		this.failedThread.start();
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		boolean retry = true;
		this.failedThread.setFlag(false);
		while (retry) {
			try {
				this.failedThread.join();
				retry = false;
			} catch (InterruptedException e) {// 不断地循环，直到刷帧线程结束
				e.printStackTrace();
			}
		}
	}

	public void setFailedThread(FailedViewThread failedThread) {
		this.failedThread = failedThread;
		this.failedThread.setFlag(true);
		this.failedThread.start();
	}
}
