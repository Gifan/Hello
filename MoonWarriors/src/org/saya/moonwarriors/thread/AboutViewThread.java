package org.saya.moonwarriors.thread;

import org.saya.moonwarriors.view.AboutView;

import android.graphics.Canvas;
import android.util.Log;
import android.view.SurfaceHolder;
/**
 * 关于视图的线程类，用于绘制视图
 * @author Saya
 *
 */
public class AboutViewThread extends Thread {
	private AboutView aboutView;
	private SurfaceHolder holder;
	private boolean flag = false;
	private int span = 16;

	public AboutViewThread(SurfaceHolder holder, AboutView aboutView) {
		this.holder = holder;
		this.aboutView = aboutView;
	}

	@Override
	public void run() {
		Canvas c;
		while (this.flag) {
			c = null;
			try {
				c = this.holder.lockCanvas(null);
				synchronized (this.holder) {
					this.aboutView.draw(c);
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
