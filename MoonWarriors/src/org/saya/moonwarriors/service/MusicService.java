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
 * ����������
 * @author Saya
 *
 */
public class MusicService {
	private static int isSound;						// �Ƿ񲥷�����
	private static MediaPlayer mediaPlayer = null;	// ������
	private static SoundPool spool;					// ��Ч���ų�
	private static HashMap<Integer, Integer> sp_map;// ��Ч���

	public static int getIsSound() {
		return isSound;
	}

	// ��ʼ����Ч
	public static void initRating(Context context) {
		spool = new SoundPool(4, AudioManager.STREAM_MUSIC, 0);
		sp_map = new HashMap<Integer, Integer>();
		sp_map.put(1, spool.load(context, R.raw.explodeeffect, 1));
		sp_map.put(2, spool.load(context, R.raw.shipdestroyeffect, 1));
	}

	// ������Ч
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

	// ��ʼ��������������
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

	// ���ű�������
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
