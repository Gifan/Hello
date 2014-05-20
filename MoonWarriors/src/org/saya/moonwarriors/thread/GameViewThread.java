package org.saya.moonwarriors.thread;

import org.saya.moonwarriors.view.GameView;

import android.graphics.Canvas;
import android.graphics.Path;
import android.view.SurfaceHolder;


/**
 * ��Ϸ��ͼ���߳��࣬���ڻ�����ͼ
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
			this.count = 0; // ��ռ�����
			long tempStamp = System.nanoTime();// ��ȡ��ǰʱ��
			long lspan = tempStamp - this.start; // ��ȡʱ����
			this.start = tempStamp; // Ϊstart���¸�ֵ
			double fps = Math.round(100000000000.0 / lspan * 20) / 100.0;// ����֡����
			this.gameView.setFps("FPS:" + fps);
		}
	}

	public void setFlag(boolean flag) {
		this.flag = flag;
	}

}
