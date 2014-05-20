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
 * 主Activity 承载游戏的面板
 * 
 * @author Saya
 * 
 */
public class MainActivity extends Activity {

	private MainView mainView; 				// 主菜单视图
	private GameView gameView; 				// 游戏视图
	private AboutView aboutView;			// 关于游戏视图
	private OptionView optionView;			// 选项设置视图
	private FailedView failedView;			// 游戏结束视图
	private int curView;                  	// 当前视图标记
	private Handler myHandler;				// 消息句柄

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

	// 初始化消息句柄
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

	// 初始化个个视图
	
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

	// 初始化服务 包括等级和声音等
	public void initService() {
		LevelService.setLevel(ConstantUtil.LV_EASY);
		MusicService.initMediaPlay(getApplicationContext(),
				ConstantUtil.MUSIC_AT_OTHER);
		MusicService.initRating(getApplicationContext());
		MusicService.setIsSound(ConstantUtil.SOUND_ON);
		if (MusicService.getIsSound() == ConstantUtil.SOUND_ON)
			MusicService.playMusic();
	}

	// 获取句柄
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
	 * 用于根据当前视图标识切换到前台作业时候需要恢复的东西
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
