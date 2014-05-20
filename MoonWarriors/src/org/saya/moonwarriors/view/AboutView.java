package org.saya.moonwarriors.view;

import org.saya.moonwarriors.MainActivity;
import org.saya.moonwarriors.thread.AboutViewThread;
import org.saya.moonwarriors.util.ConstantUtil;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.os.Message;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.Toast;

import com.example.moonwarriors.R;

/**
 * 关于游戏视图类
 * 
 * @author Saya
 * 
 */
public class AboutView extends SurfaceView implements SurfaceHolder.Callback {

	private MainActivity mainActivity; 			// 主面板
	private AboutViewThread aboutThread; 		// 线程
	private Bitmap background; 					// 背景图片
	private Bitmap logo; 						// 关于logo
	private Paint paint; 						// 画笔
	private Rect src_logo; 						// logo图片区域
	private Rect des_logo; 						// logo位置渔区
	private Rect goBack; 						// 返回按钮区域
	private Point pGoBack;						// 返回按钮区域的某个点
	private int isBack; 						// 是否点击了返回按钮

	public AboutView(MainActivity activity) {
		super(activity);
		this.mainActivity = activity;

		getHolder().addCallback(this);

		// 初始化图片资源
		this.initBitmap();

		// 初始化图片位置
		this.initPosition();

		this.aboutThread = new AboutViewThread(getHolder(), this);
	}

	private void initBitmap() {
		paint = new Paint();
		this.background = BitmapFactory.decodeResource(getResources(),
				R.drawable.loading);
		this.logo = BitmapFactory.decodeResource(getResources(),
				R.drawable.menutitle);
	}

	private void initPosition() {

		int width = this.background.getWidth();
		int height = this.background.getHeight();
		this.src_logo = new Rect(0, 34, this.logo.getWidth(), 68);
		this.des_logo = new Rect(100, 50, 100 + this.logo.getWidth(), 50 + 34);
		this.goBack = new Rect(width / 2 - 40, height - 80, width / 2 + 40,
				height - 65);

		int x = this.goBack.left + (this.goBack.right - this.goBack.left) / 2;
		int y = this.goBack.top + (this.goBack.bottom - this.goBack.top) / 4
				* 3;
		this.pGoBack = new Point(x, y);
		this.isBack = ConstantUtil.KEY_UP;
	}

	// 绘制图片
	public void draw(Canvas canvas) {
		try {
			canvas.drawBitmap(this.background, 0, 0, paint);
			canvas.drawBitmap(this.logo, this.src_logo, this.des_logo, paint);

			paint.setColor(Color.WHITE);
			paint.setTextSize(16.0f);
			paint.setAntiAlias(true);
			paint.setTypeface(Typeface.DEFAULT_BOLD);
			paint.setTextAlign(Paint.Align.CENTER);
			canvas.drawText("Test", 50, 200, paint);

			if (this.isBack == ConstantUtil.KEY_DOWN) {
				paint.setTextSize(21.0f);
			}
			canvas.drawText("GoBack", this.pGoBack.x, this.pGoBack.y, paint);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// 监听触摸事件
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int x = (int) event.getX();
		int y = (int) event.getY();

		if (event.getAction() == MotionEvent.ACTION_UP) {
			this.isBack = ConstantUtil.KEY_UP;
			if (this.goBack.contains(x, y)) {
				Message msg = this.mainActivity.getMyHandler().obtainMessage(
						ConstantUtil.HAND_MAIN);
				this.mainActivity.getMyHandler().sendMessage(msg);

			}
		} else if (event.getAction() == MotionEvent.ACTION_DOWN
				|| event.getAction() == MotionEvent.ACTION_MOVE) {
			if (this.goBack.contains(x, y)) {
				this.isBack = ConstantUtil.KEY_DOWN;
			} else {
				this.isBack = ConstantUtil.KEY_UP;
			}

		}
		return true;
	}

	// surfaceView视图改变时调用
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {

	}

	// surfaceView视图创建时调用
	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		this.aboutThread.setFlag(true);
		this.aboutThread.start();
	}

	// surfaceView视图销毁时调用
	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		boolean retry = true;
		this.aboutThread.setFlag(false);
		while (retry) {
			try {
				this.aboutThread.join();
				retry = false;
			} catch (InterruptedException e) {// 不断地循环，直到刷帧线程结束
				e.printStackTrace();
			}
		}
	}

}
