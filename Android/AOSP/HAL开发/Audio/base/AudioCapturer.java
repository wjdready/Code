package com.example.audiorec;


import android.annotation.SuppressLint;
import android.content.pm.PackageManager;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Environment;
import android.os.SystemClock;
import android.util.Log;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public class AudioCapturer {
    private static final String TAG = "AudioCapturer";

    private String fileName = null;

    private static final int DEFAULT_SOURCE = MediaRecorder.AudioSource.MIC;
    private static final int DEFAULT_SAMPLE_RATE = 48000;
    private static final int DEFAULT_CHANNEL_CONFIG = AudioFormat.CHANNEL_IN_STEREO;
    private static final int DEFAULT_AUDIO_FORMAT = AudioFormat.ENCODING_PCM_16BIT;

    private AudioRecord mAudioRecord;
    private int mMinBufferSize = 0;
    private Thread mCaptureThread;
    private boolean mIsCaptureStarted = false;
    private volatile boolean mIsLoopExit = false;
    private OnAudioFrameCapturedListener mAudioFrameCapturedListener;


    public interface OnAudioFrameCapturedListener {
        public void onAudioFrameCaptured(byte[] audioData);
    }

    public boolean isCaptureStarted() {
        return mIsCaptureStarted;
    }

    public void setOnAudioFrameCapturedListener(OnAudioFrameCapturedListener listener) {
        mAudioFrameCapturedListener = listener;
    }

    final int REQUEST_CODE_RECORD_AUDIO = 0x11;

    public boolean startCapture() {

        // 设置录音文件路径
        // fileName = Environment.getExternalStorageDirectory().getAbsolutePath() + "/recorded_audio.wav";

        fileName = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_MUSIC).getAbsolutePath() + "/recorded_audio.pcm";

        return startCapture(DEFAULT_SOURCE, DEFAULT_SAMPLE_RATE, DEFAULT_CHANNEL_CONFIG,
                DEFAULT_AUDIO_FORMAT);
    }

    @SuppressLint("MissingPermission")
    public boolean startCapture(int audioSource, int sampleRateInHz, int channelConfig, int audioFormat) {
        if (mIsCaptureStarted) {
            Log.e(TAG, "Capture already started !");
            return false;
        }

        Log.e(TAG, "============== audioFormat " + audioFormat + "!");

        mMinBufferSize = AudioRecord.getMinBufferSize(sampleRateInHz,channelConfig,audioFormat);
        if (mMinBufferSize == AudioRecord.ERROR_BAD_VALUE) {
            Log.e(TAG, "Invalid parameter !");
            return false;
        }
        Log.d(TAG , "getMinBufferSize = "+mMinBufferSize+" bytes !");
        mAudioRecord = new AudioRecord(audioSource, sampleRateInHz, channelConfig, audioFormat, mMinBufferSize);
        if (mAudioRecord.getState() == AudioRecord.STATE_UNINITIALIZED) {
            Log.e(TAG, "AudioRecord initialize fail !");
            return false;
        }


        mAudioRecord.startRecording();
        mIsLoopExit = false;
        mCaptureThread = new Thread(new AudioCaptureRunnable());
        mCaptureThread.start();
        mIsCaptureStarted = true;
        Log.d(TAG, "Start audio capture success !");
        return true;
    }


    public void stopCapture() {
        if (!mIsCaptureStarted) {
            return;
        }
        mIsLoopExit = true;
        try {
            mCaptureThread.interrupt();
            mCaptureThread.join(1000);
        }
        catch (InterruptedException e) {
            e.printStackTrace();
        }
        if (mAudioRecord.getRecordingState() == AudioRecord.RECORDSTATE_RECORDING) {
            mAudioRecord.stop();
        }
        mAudioRecord.release();
        mIsCaptureStarted = false;
        mAudioFrameCapturedListener = null;
        Log.d(TAG, "Stop audio capture success !");
    }


    private class AudioCaptureRunnable implements Runnable {


        @Override
        public void run() {
            FileOutputStream fileOutputStream;
            try {
                fileOutputStream = new FileOutputStream(fileName);
            } catch (FileNotFoundException e) {
                throw new RuntimeException(e);
            }

            while (!mIsLoopExit) {
                byte[] buffer = new byte[mMinBufferSize];
                int ret = mAudioRecord.read(buffer, 0, mMinBufferSize);
                if (ret == AudioRecord.ERROR_INVALID_OPERATION) {
                    Log.e(TAG , "Error ERROR_INVALID_OPERATION");
                }
                else if (ret == AudioRecord.ERROR_BAD_VALUE) {
                    Log.e(TAG , "Error ERROR_BAD_VALUE");
                }
                else {
                    if (mAudioFrameCapturedListener != null) {
                        mAudioFrameCapturedListener.onAudioFrameCaptured(buffer);
                    }
                    try {
                        fileOutputStream.write(buffer, 0, buffer.length);
                    } catch (IOException e) {
                        throw new RuntimeException(e);
                    }
                    Log.d(TAG , "OK, Captured "+ret+" bytes !");
                }
                SystemClock.sleep(10);
            }
            try {
                fileOutputStream.close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }
}