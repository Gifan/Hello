package org.saya.moonwarriors.thread;

import org.saya.moonwarriors.view.MainView;

import android.annotation.SuppressLint;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.view.SurfaceHolder;

/**
 * 主菜单视图的线程类，用于绘制视图
 * @author Saya
 *
 */
public class MainViewThread extends Thread {

	private MainView mainView;
	private SurfaceHolder holder;
	private boolean flag = false;
	private int span = 16;

	public MainViewThread(SurfaceHolder holder, MainView mainView) {
		this.mainView = mainView;
		this.holder = holder;
	}

	@Override
	public void run() {
		Canvas c;

		while (this.flag) {
			c = null;
			try {
				c = this.holder.lockCanvas(null);
				synchronized (this.holder) {
					this.mainView.updatePlanePos();
					this.mainView.draw(c);
				}
			} finally {
				if (c != null)
					this.holder.unlockCanvasAndPost(c);
			}

			try {
				Thread.sleep(span);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	public void setFlag(boolean flag) {
		this.flag = flag;
	}

}
