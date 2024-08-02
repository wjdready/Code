

function main()

    fs = 48000;
    t = 0 : 1/fs : 1;

    y = 20 * sin(2 * pi * 1000 * t) + 5 * sin(2 * pi *  2000 * t);

    % 将噪声添加到信号中  
    noise_level = 10*max(abs(y)) * 0.1;  
    noise = noise_level * randn(size(t));  
    y = y + noise; 

    Spectrum_Calc(y, fs);
end

function [Yf,f] = Spectrum_Calc(yt,Fs)
    %功能：实现快速fourier变换
    %输入参数：yt为时域信号序列，Fs为采样频率
    %返回值：Yf为经过fft的频域序列，f为相应频率

    L = length(yt);
    subplot(211)
    plot(yt)
    % 加Hanning窗
    
    w = hann(L);
    
    yt = yt(:).*w(:);
    
    NFFT = 2^nextpow2(L);
    Yf = fft(yt,NFFT)/L;
    
    Yf = 2*abs(Yf(1:NFFT/2+1));
    f = Fs/2 * linspace(0,1,NFFT/2+1);

    subplot(212)
    plot(f, Yf)


end
