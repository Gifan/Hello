package org.saya.moonwarriors.view;

import org.saya.moonwarriors.MainActivity;
import org.saya.moonwarriors.service.LevelService;
import org.saya.moonwarriors.service.MusicService;
import org.saya.moonwarriors.thread.OptionViewThread;
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

import com.example.moonwarriors.R;

public class OptionView extends SurfaceView implements SurfaceHolder.Callback {

	private MainActivity mainActivity;					// �����
	private OptionViewThread optionThread;				// �߳�
	private Bitmap background;							// ����ͼƬ
	private Bitmap logo;								// ѡ��logo
	private Paint paint;								// ����
	private Rect src_logo;								// logoԴ����
	private Rect des_logo;								// logoĿ������
	private Rect des_sound;								// ����ѡ��Ŀ������
	private Rect des_mode;								// ��Ϸ�Ѷ�Ŀ������
	private Rect goBack;								// �������˵�Ŀ������
	private Point pGoBack;								// ����goback�����ĳ��
	private Point pSound;								// ����Sound�����ĳ��
	private Point pMode;								// ����Mode�����ĳ��
	private int isBack;									// �Ƿ��·���
	private int changeSound;							// �ı�����ѡ���ʶ
	private int changeLevel;							// �ı��Ѷ�ѡ���ʶ
	private String[] sLevel = { "", "", "", "Easy", "Normal", "Hard" };
	private String[] sSound = { "On", "Off" };

	public OptionView(MainActivity activity) {
		super(activity);
		this.mainActivity = activity;

		getHolder().addCallback(this);

		this.initBitmap();
		this.initPosition();

		this.optionThread = new OptionViewThread(getHolder(), this);
	}

	// ��ʼ��ͼƬλ��
	private void initPosition() {
		int width = this.background.getWidth();
		int height = this.background.getHeight();
		this.src_logo = new Rect(0, 0, this.logo.getWidth(), 34);
		this.des_logo = new Rect(100, 50, 100 + this.logo.getWidth(), 50 + 34);
		this.goBack = new Rect(width / 2 - 40, height - 80, width / 2 + 40,
				height - 65);

		this.des_sound = new Rect(80, 210, 120, 230);
		this.des_mode = new Rect(200, 210, 260, 230);

		int x = this.goBack.left + (this.goBack.right - this.goBack.left) / 2;
		int y = this.goBack.top + (this.goBack.bottom - this.goBack.top) / 4
				* 3;
		this.pGoBack = new Point(x, y);

		x = this.des_sound.left + (this.des_sound.right - this.des_sound.left)
				/ 2;
		y = this.des_sound.top + (this.des_sound.bottom - this.des_sound.top)
				/ 4 * 3;
		this.pSound = new Point(x, y);

		x = this.des_mode.left + (this.des_mode.right - this.des_mode.left) / 2;
		y = this.des_mode.top + (this.des_mode.bottom - this.des_mode.top) / 4
				* 3;
		this.pMode = new Point(x, y);

		this.isBack = ConstantUtil.KEY_UP;
		this.changeLevel = ConstantUtil.KEY_UP;
		this.changeSound = ConstantUtil.KEY_UP;
	}

	// ��ʼ��ͼƬ��Դ
	private void initBitmap() {
		paint = new Paint();
		this.background = BitmapFactory.decodeResource(getResources(),
				R.drawable.loading);
		this.logo = BitmapFactory.decodeResource(getResources(),
				R.drawable.menutitle);
	}

	// ������ͼ
	public void draw(Canvas canvas) {
		try {
			canvas.drawBitmap(this.background, 0, 0, paint);
			canvas.drawBitmap(this.logo, this.src_logo, this.des_logo, paint);

			paint.setTextSize(16.0f);
			paint.setAntiAlias(true);
			paint.setColor(Color.GRAY);
			paint.setTypeface(Typeface.DEFAULT_BOLD);
			paint.setTextAlign(Paint.Align.CENTER);

			canvas.drawText("Sound", 100, 200, paint);
			canvas.drawText("Mode", 230, 200, paint);

			paint.setColor(Color.WHITE);

			if (this.isBack == ConstantUtil.KEY_DOWN) {
				paint.setTextSize(21.0f);
			}

			canvas.drawText("GoBack", this.pGoBack.x, this.pGoBack.y, paint);

			paint.setTextSize(20.0f);
			if (this.changeSound == ConstantUtil.KEY_DOWN) {
				paint.setTextSize(23.0f);
			}
			canvas.drawText(this.sSound[MusicService.getIsSound()],
					this.pSound.x, this.pSound.y, paint);

			paint.setTextSize(20.0f);
			if (this.changeLevel == ConstantUtil.KEY_DOWN) {
				paint.setTextSize(23.0f);
			}
			canvas.drawText(this.sLevel[LevelService.getLevel()], this.pMode.x,
					this.pMode.y, paint);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// �����¼�����
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int x = (int) event.getX();
		int y = (int) event.getY();

		if (event.getAction() == MotionEvent.ACTION_UP) {
			if (this.goBack.contains(x, y)) {
				this.isBack = ConstantUtil.KEY_DOWN;
				Message msg = this.mainActivity.getMyHandler().obtainMessage(
						ConstantUtil.HAND_MAIN);
				this.mainActivity.getMyHandler().sendMessage(msg);

			} else {
				this.isBack = ConstantUtil.KEY_UP;
			}
			if (this.des_sound.contains(x, y)) {
				MusicService.setIsSound(MusicService.getIsSound() ^ 1);
				if (MusicService.getIsSound() == ConstantUtil.SOUND_ON) {
					if (!MusicService.isPlaying()) {
						MusicService.playMusic();
					}
				} else {
					MusicService.Pause();
				}
				this.changeSound = ConstantUtil.KEY_UP;

			}
			if (this.des_mode.contains(x, y)) {
				LevelService.setLevel((LevelService.getLevel() + 1)
						% ConstantUtil.LV_KIND + ConstantUtil.LV_KIND);
				this.changeLevel = ConstantUtil.KEY_UP;
			}
		} else if (event.getAction() == MotionEvent.ACTION_DOWN
				|| event.getAction() == MotionEvent.ACTION_MOVE) {
			if (this.goBack.contains(x, y)) {
				this.isBack = ConstantUtil.KEY_DOWN;
			} else {
				this.isBack = ConstantUtil.KEY_UP;
			}
			if (this.des_sound.contains(x, y)) {
				this.changeSound = ConstantUtil.KEY_DOWN;
			} else {
				this.changeSound = ConstantUtil.KEY_UP;
			}
			if (this.des_mode.contains(x, y)) {
				this.changeLevel = ConstantUtil.KEY_DOWN;
			} else {
				this.changeLevel = ConstantUtil.KEY_UP;
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
		this.optionThread.setFlag(true);
		this.optionThread.start();
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		boolean retry = true;
		this.optionThread.setFlag(false);
		while (retry) {
			try {
				this.optionThread.join();
				retry = false;
			} catch (InterruptedException e) {// ���ϵ�ѭ����ֱ��ˢ֡�߳̽���
				e.printStackTrace();
			}
		}
	}

}
