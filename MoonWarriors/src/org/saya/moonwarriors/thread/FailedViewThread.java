package org.saya.moonwarriors.thread;

import org.saya.moonwarriors.view.FailedView;

import android.graphics.Canvas;
import android.view.SurfaceHolder;

/**
 * 游戏结束视图的线程类，用于绘制视图
 * @author Saya
 *
 */
public class FailedViewThread extends Thread {
	private FailedView failedView;
	private SurfaceHolder holder;
	private boolean flag = false;
	private int span = 16;

	public FailedViewThread(SurfaceHolder holder, FailedView failedView) {
		this.holder = holder;
		this.failedView = failedView;

	}

	@Override
	public void run() {
		Canvas c;
		while (this.flag) {
			c = null;
			try {
				c = this.holder.lockCanvas(null);
				synchronized (this.holder) {
					this.failedView.draw(c);
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

	public void setFlag(boolean b) {
		this.flag = b;
	}

}
