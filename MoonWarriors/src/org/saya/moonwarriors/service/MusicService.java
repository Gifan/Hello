package org.saya.moonwarriors.service;

import java.util.HashMap;

import org.saya.moonwarriors.util.ConstantUtil;

import android.content.Context;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.media.MediaPlayer.OnErrorListener;

import com.example.moonwarriors.R;

/**
 * 声音服务类
 * @author Saya
 *
 */
public class MusicService {
	private static int isSound;						// 是否播放声音
	private static MediaPlayer mediaPlayer = null;	// 播放器
	private static SoundPool spool;					// 声效播放池
	private static HashMap<Integer, Integer> sp_map;// 声效存放

	public static int getIsSound() {
		return isSound;
	}

	// 初始化声效
	public static void initRating(Context context) {
		spool = new SoundPool(4, AudioManager.STREAM_MUSIC, 0);
		sp_map = new HashMap<Integer, Integer>();
		sp_map.put(1, spool.load(context, R.raw.explodeeffect, 1));
		sp_map.put(2, spool.load(context, R.raw.shipdestroyeffect, 1));
	}

	// 播放声效
	public static void playRating(int type) {
		if (spool != null && MusicService.getIsSound() == ConstantUtil.SOUND_ON) {
			try {
				spool.play(sp_map.get(type), 1.5f, 1.5f, 1, 0, 1f);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	public static void setIsSound(int isSound) {
		MusicService.isSound = isSound;
	}

	// 初始化背景播放音乐
	public static void initMediaPlay(Context context, int musicSign) {
		if (mediaPlayer != null)
			MusicService.ReleaseMedia();
		if (musicSign == ConstantUtil.MUSIC_AT_OTHER)
			mediaPlayer = MediaPlayer.create(context, R.raw.mainmusic);
		else if (musicSign == ConstantUtil.MUSIC_AT_GAME)
			mediaPlayer = MediaPlayer.create(context, R.raw.bgmusic);
		mediaPlayer.setLooping(true);
		mediaPlayer.setOnErrorListener(new OnErrorListener() {
			@Override
			public boolean onError(MediaPlayer mp, int what, int extra) {
				try {
					mp.release();
				} catch (Exception e) {
				}
				return false;
			}
		});
	}

	// 播放背景音乐
	public static void playMusic() {
		if (mediaPlayer != null
				&& MusicService.getIsSound() == ConstantUtil.SOUND_ON) {
			mediaPlayer.start();
		}
	}

	public static MediaPlayer getPlayer() {
		return mediaPlayer;
	}

	public static boolean isPlaying() {
		return mediaPlayer.isPlaying();
	}

	public static void Pause() {
		mediaPlayer.pause();
	}

	public static void Stop() {
		mediaPlayer.stop();
	}

	public static void ReleaseMedia() {
		mediaPlayer.release();
		mediaPlayer = null;
	}

}
