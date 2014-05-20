package org.saya.moonwarriors.thread;

import org.saya.moonwarriors.view.GameView;

import android.graphics.Canvas;
import android.graphics.Path;
import android.view.SurfaceHolder;


/**
 * 游戏视图的线程类，用于绘制视图
 * @author Saya
 *
 */
public class GameViewThread extends Thread {
	private GameView gameView;
	private SurfaceHolder holder;
	private boolean flag = false;
	private int span = 16;
	private int count;
	private long start = System.nanoTime();

	public GameViewThread(SurfaceHolder holder, GameView gameView) {
		this.holder = holder;
		this.gameView = gameView;
	}

	@Override
	public void run() {
		Canvas c;
		while (this.flag) {
			c = null;
			try {
				c = this.holder.lockCanvas(null);
				synchronized (this.holder) {
					this.gameView.update();
					this.gameView.draw(c);
				}
			} finally {
				if (c != null)
					this.holder.unlockCanvasAndPost(c);
			}
			this.countFPS();
		}
		try {
			Thread.sleep(span);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void countFPS() {
		this.count++;
		if (this.count >= 20) {
			this.count = 0; // 清空计数器
			long tempStamp = System.nanoTime();// 获取当前时间
			long lspan = tempStamp - this.start; // 获取时间间隔
			this.start = tempStamp; // 为start重新赋值
			double fps = Math.round(100000000000.0 / lspan * 20) / 100.0;// 计算帧速率
			this.gameView.setFps("FPS:" + fps);
		}
	}

	public void setFlag(boolean flag) {
		this.flag = flag;
	}

}
