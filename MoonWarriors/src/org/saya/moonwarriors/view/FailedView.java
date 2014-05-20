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
 * ��Ϸ������ͼ��
 * @author Saya
 *
 */
public class FailedView extends SurfaceView implements SurfaceHolder.Callback {
	private MainActivity mainActivity; 					// �����
	private FailedViewThread failedThread;				// �߳���
	private Bitmap background;							// ����ͼƬ
	private Bitmap logo;								// ѡ��logo
	private Bitmap menu;								// playAgain�˵�
	private Rect[] playAgain;							// �˵�����֡ͼƬ����
	private Rect des_again;								// �˵�����Ŀ��λ��
	private int f_again;								// ������ǰ֡
	private Paint paint;								// ����
	private int score;									// ��Ϸ����

	public FailedView(MainActivity activity, int score) {
		super(activity);
		this.score = score;
		this.mainActivity = activity;
		getHolder().addCallback(this);

		this.f_again = ConstantUtil.KEY_UP;
		
		// ��ʼ��ͼƬ
		this.initBitmap();
		
		//��ʼ��λ��
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

	// ����ͼƬ
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

	
	// ������Ϣ����
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
			} catch (InterruptedException e) {// ���ϵ�ѭ����ֱ��ˢ֡�߳̽���
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
