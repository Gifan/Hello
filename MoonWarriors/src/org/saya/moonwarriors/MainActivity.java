package org.saya.moonwarriors;

import org.saya.moonwarriors.service.LevelService;
import org.saya.moonwarriors.service.MusicService;
import org.saya.moonwarriors.thread.FailedViewThread;
import org.saya.moonwarriors.thread.GameViewThread;
import org.saya.moonwarriors.util.ConstantUtil;
import org.saya.moonwarriors.view.AboutView;
import org.saya.moonwarriors.view.FailedView;
import org.saya.moonwarriors.view.GameView;
import org.saya.moonwarriors.view.MainView;
import org.saya.moonwarriors.view.OptionView;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.SurfaceHolder;
import android.view.Window;
import android.view.WindowManager;

/**
 * ��Activity ������Ϸ�����
 * 
 * @author Saya
 * 
 */
public class MainActivity extends Activity {

	private MainView mainView; 				// ���˵���ͼ
	private GameView gameView; 				// ��Ϸ��ͼ
	private AboutView aboutView;			// ������Ϸ��ͼ
	private OptionView optionView;			// ѡ��������ͼ
	private FailedView failedView;			// ��Ϸ������ͼ
	private int curView;                  	// ��ǰ��ͼ���
	private Handler myHandler;				// ��Ϣ���

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		this.curView = ConstantUtil.MAIN_VIEW;
		this.initHandler();
		this.initService();
		this.initMainView();

	}

	// ��ʼ����Ϣ���
	private void initHandler() {
		myHandler = new Handler() {
			public void handleMessage(Message msg) {
				switch (msg.what) {
				case ConstantUtil.HAND_NEW_GAME:
					MusicService.initMediaPlay(getApplicationContext(),
							ConstantUtil.MUSIC_AT_GAME);
					MusicService.playMusic();
					MainActivity.this.initGameView();

					MainActivity.this.curView = ConstantUtil.GAME_VIEW;
					break;
				case ConstantUtil.HAND_OPTION:
					MainActivity.this.initOptionView();
					MainActivity.this.curView = ConstantUtil.OPTION_VIEW;
					break;
				case ConstantUtil.HAND_ABOUT:
					MainActivity.this.initAboutView();
					MainActivity.this.curView = ConstantUtil.ABOUT_VIEW;
					break;
				case ConstantUtil.HAND_MAIN:
					MainActivity.this.initMainView();
					MusicService.initMediaPlay(getApplicationContext(),
							ConstantUtil.MUSIC_AT_OTHER);
					MusicService.playMusic();
					MainActivity.this.curView = ConstantUtil.MAIN_VIEW;
					break;
				case ConstantUtil.HAND_OVER:
					MainActivity.this.initFaildView(msg.arg1);
					MusicService.initMediaPlay(getApplicationContext(),
							ConstantUtil.MUSIC_AT_OTHER);
					MusicService.playMusic();
					MainActivity.this.curView = ConstantUtil.FAILED_VIEW;
				default:
					break;
				}
			}
		};
	}

	// ��ʼ��������ͼ
	
	public void initFaildView(int score) {
		this.failedView = new FailedView(MainActivity.this, score);
		this.setContentView(this.failedView);
	}

	public void initGameView() {
		this.gameView = new GameView(MainActivity.this);
		this.setContentView(this.gameView);
	}

	public void initAboutView() {
		this.aboutView = new AboutView(MainActivity.this);
		this.setContentView(this.aboutView);
	}

	public void initMainView() {
		this.mainView = new MainView(MainActivity.this);
		setContentView(this.mainView);
	}

	public void initOptionView() {
		this.optionView = new OptionView(MainActivity.this);
		setContentView(this.optionView);
	}

	// ��ʼ������ �����ȼ���������
	public void initService() {
		LevelService.setLevel(ConstantUtil.LV_EASY);
		MusicService.initMediaPlay(getApplicationContext(),
				ConstantUtil.MUSIC_AT_OTHER);
		MusicService.initRating(getApplicationContext());
		MusicService.setIsSound(ConstantUtil.SOUND_ON);
		if (MusicService.getIsSound() == ConstantUtil.SOUND_ON)
			MusicService.playMusic();
	}

	// ��ȡ���
	public Handler getMyHandler() {
		return myHandler;
	}

	@Override
	protected void onRestart() {
		if (MusicService.getIsSound() == ConstantUtil.SOUND_ON) {
			if (!MusicService.isPlaying()) {
				MusicService.playMusic();
			}
		}
		this.ReturnView();
		super.onRestart();
	}

	@Override
	protected void onStop() {
		if (MusicService.getIsSound() == ConstantUtil.SOUND_ON) {
			MusicService.Pause();
		}
		super.onStop();
	}

	/**
	 * ���ڸ��ݵ�ǰ��ͼ��ʶ�л���ǰ̨��ҵʱ����Ҫ�ָ��Ķ���
	 */
	public void ReturnView() {
		switch (this.curView) {
		case ConstantUtil.MAIN_VIEW:
			this.initMainView();
			break;
		case ConstantUtil.GAME_VIEW:
			SurfaceHolder sfh = this.gameView.getHolder();
			this.gameView.setGameThread(new GameViewThread(sfh, this.gameView));
			break;
		case ConstantUtil.ABOUT_VIEW:
			this.initAboutView();
			break;
		case ConstantUtil.OPTION_VIEW:
			this.initOptionView();
			break;
		case ConstantUtil.FAILED_VIEW:
			SurfaceHolder sfh1 = this.failedView.getHolder();
			this.failedView.setFailedThread(new FailedViewThread(sfh1,
					this.failedView));
			setContentView(this.failedView);
			break;

		default:
			break;
		}
	}

}
