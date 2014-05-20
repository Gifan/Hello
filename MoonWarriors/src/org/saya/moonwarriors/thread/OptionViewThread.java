package org.saya.moonwarriors.thread;

import org.saya.moonwarriors.view.OptionView;

import android.graphics.Canvas;
import android.view.SurfaceHolder;

/**
 * 选项设置视图的线程类，用于绘制视图
 * @author Saya
 *
 */
public class OptionViewThread extends Thread {
	private OptionView optionView;
	private SurfaceHolder holder;
	private boolean flag = false;
	private int span = 16;

	public OptionViewThread(SurfaceHolder holder, OptionView optionView) {
		this.holder = holder;
		this.optionView = optionView;
	}

	@Override
	public void run() {
		Canvas c;
		while (this.flag) {
			c = null;
			try {
				c = this.holder.lockCanvas(null);
				synchronized (this.holder) {
					this.optionView.draw(c);
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
