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
 * ������Ϸ��ͼ��
 * 
 * @author Saya
 * 
 */
public class AboutView extends SurfaceView implements SurfaceHolder.Callback {

	private MainActivity mainActivity; 			// �����
	private AboutViewThread aboutThread; 		// �߳�
	private Bitmap background; 					// ����ͼƬ
	private Bitmap logo; 						// ����logo
	private Paint paint; 						// ����
	private Rect src_logo; 						// logoͼƬ����
	private Rect des_logo; 						// logoλ������
	private Rect goBack; 						// ���ذ�ť����
	private Point pGoBack;						// ���ذ�ť�����ĳ����
	private int isBack; 						// �Ƿ����˷��ذ�ť

	public AboutView(MainActivity activity) {
		super(activity);
		this.mainActivity = activity;

		getHolder().addCallback(this);

		// ��ʼ��ͼƬ��Դ
		this.initBitmap();

		// ��ʼ��ͼƬλ��
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

	// ����ͼƬ
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

	// ���������¼�
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

	// surfaceView��ͼ�ı�ʱ����
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {

	}

	// surfaceView��ͼ����ʱ����
	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		this.aboutThread.setFlag(true);
		this.aboutThread.start();
	}

	// surfaceView��ͼ����ʱ����
	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		boolean retry = true;
		this.aboutThread.setFlag(false);
		while (retry) {
			try {
				this.aboutThread.join();
				retry = false;
			} catch (InterruptedException e) {// ���ϵ�ѭ����ֱ��ˢ֡�߳̽���
				e.printStackTrace();
			}
		}
	}

}
