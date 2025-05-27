import wave
import numpy as np
from scipy.signal import stft

# 读取 WAV 文件
def read_wav_file(file_path):
    with wave.open(file_path, 'rb') as wf:
        # 获取音频参数
        num_channels = wf.getnchannels()
        sample_width = wf.getsampwidth()
        frame_rate = wf.getframerate()
        num_frames = wf.getnframes()

        # 读取所有帧的数据
        raw_data = wf.readframes(num_frames)

        # 根据采样宽度转换数据类型
        if sample_width == 1:
            dtype = np.uint8
        elif sample_width == 2:
            dtype = np.int16
        elif sample_width == 4:
            dtype = np.int32
        else:
            raise ValueError("Unsupported sample width")

        # 将原始数据转换为 numpy 数组
        audio_data = np.frombuffer(raw_data, dtype=dtype)

        # 如果是多声道，只取第一个通道（单通道处理）
        if num_channels > 1:
            audio_data = audio_data[::num_channels]

    return audio_data, frame_rate

# 进行短时傅里叶变换并检测波动
def detect_spectrum_fluctuations(audio_data, frame_rate):
    # 进行短时傅里叶变换
    f, t, Zxx = stft(audio_data, fs=frame_rate, nperseg=1024)
    # 计算频谱幅度
    magnitude = np.abs(Zxx)

    # 计算每帧频谱幅度的均值
    frame_means = np.mean(magnitude, axis=0)

    # 计算相邻帧幅度均值的变化率
    changes = np.abs(np.diff(frame_means) / frame_means[:-1])

    # 设定波动阈值
    threshold = 0.01  # 可根据实际情况调整

    # 找到变化率超过阈值的帧索引
    fluctuation_indices = np.where(changes > threshold)[0]

    # 将帧索引转换为时间
    fluctuation_times = t[fluctuation_indices]

    return fluctuation_times, t, f, magnitude

# 将秒转换为分钟和秒的格式
def seconds_to_min_sec(seconds):
    minutes = int(seconds // 60)
    sec = seconds % 60
    return f"{minutes:02d}:{sec:06.3f}"

# 主函数
def main():
    file_path = '../audio/未命名 1.wav'  # 替换为实际的 WAV 文件路径
    audio_data, frame_rate = read_wav_file(file_path)
    fluctuation_times, t, f, magnitude = detect_spectrum_fluctuations(audio_data, frame_rate)

    print("检测到频谱波动的时间点:")
    for time in fluctuation_times:
        print(seconds_to_min_sec(time))

if __name__ == "__main__":
    main()
