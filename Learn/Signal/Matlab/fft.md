
```m
% 如果 x 是向量，则 fft(x) 返回该向量的傅里叶变换
% 如果 x 是矩阵，则 fft(x) 将 x 的各列视为向量，并返回每列的傅里叶变换。
% 如果 x 是一个多维数组，则 fft(X) 将沿大小不等于 1 的第一个数组维度的值视为向量，并返回每个向量的傅里叶变换。
Y = fft(x)

% 如果 X 是向量且 X 的长度小于 n，则为 X 补上尾零以达到长度 n。
% 如果 X 是向量且 X 的长度大于 n，则对 X 进行截断以达到长度 n。
% 如果 X 是矩阵，则每列的处理与在向量情况下相同。
% 如果 X 为多维数组，则大小不等于 1 的第一个数组维度的处理与在向量情况下相同。
Y = fft(X,n)

% 当 x 为矩阵时，在指定的 dim 维上进行傅里叶变换
% dim=1 按列
% dim=2 按行
Y = fft(X,n,dim)
```

因为实信号以 fs 为采样速率的信号在 fs/2 处混叠，所以实信号 fft 的结果中前半部分对应 [0, fs/2], 后半部分对应 [ -fs/2, 0]；

1）实信号 fft 的结果前半部分对应 [0, fs/2] 是正频率的结果，后半部分对应 [ -fs/2, 0] 是负频率的结果。大于 fs/2 的部分的频谱实际上是实信号的负频率加 fs 的结果。故要得到正确的结果，只需将视在频率减去 fs 即可得到频谱对应的真实负频率；

2）如果要让实信号 fft 的结果与 [-fs/2, fs/2] 对应，则要 fft 后 fftshift 一下即可，fftshift 的操作是将 fft 结果以 fs/2 为中心左右互换；

3）如果实信号 fft 的绘图频率 f 从 [-fs/2, fs/2]，并且没有 fftshift，则 fft 正频谱对应 f 在 [0, fs/2] 的结果将混叠到 (f - fs/2) 的位置；
fft 负频谱对应 f 在 [-fs/2, 0] 的结果混叠到 f + fs - fs/2 的位置，注意这里 f 为负值，也就是说此种情况下 fft 负频谱对应的视在频率减去 fs/2 即可得到频谱对应的真实负频率。

二 复信号情况
1）复信号没有负频率，以 fs 为采样速率的信号，fft 的频谱结果是从 [0,fs] 的。

2）在 f> fs/2 时，对复信号的 fft 结果进行 fftshift 会产生频率混叠（将下面的示例 2 中的频率从 f=15 改为 f=85 可以验证 f=85 的谱线在 fftshift 后跑到 f= -15 = 85 - fs = 85 - 100 的位置了），所以复信号也一般要求 f <= fs/2

3）在对雷达的慢时间维（复信号）进行 fft 后，由于要用 doppler= ((0:LFFT-1)/LFFT - 0.5)*PRF; 计算多普勒频率，所以对该慢时间信号 fft 后要 fftshift 下，以便和正确的频率单元相对应。注意多普勒频率 fd < = PRF/2 时才测的准！

实信号

```matlab
clf;
 
fs=100;N=256;   
n=0:N-1;t=n/fs;  
x=0.5*sin(2*pi*15*t)+2*sin(2*pi*40*t); 
 
y1=fft(x,N);    
y2=fftshift(y1);
 
mag1=abs(y1);     
mag2=abs(y2);   
 
f1=n*fs/N;    
f2=n*fs/N-fs/2;
 
subplot(3,1,1),plot(f1,mag1,'r');
xlabel('频率/Hz');
ylabel('振幅');title('图1FFT','color','r');grid on;
 
subplot(3,1,2),plot(f2,mag1,'b');  
xlabel('频率/Hz');
ylabel('振幅');title('FFT变换后无FFTSHIFT','color','b');grid on;
 
subplot(3,1,3),plot(f2, mag2,'c');
xlabel('频率/Hz');
ylabel('振幅');title('FFT后进行FFTSHIFT','color','c');grid on;
```


```
clf;
 
fs=100;N=256;   
n=0:N-1;t=n/fs;  
x=0.5*sin(2*pi*15*t)+2*sin(2*pi*40*t); 

RandStream('dsfmt19937');
white_noise = randn(N, 1);

x = white_noise;

y1=fft(x,N);    
y2=fftshift(y1);
 
mag1=abs(y1);     
mag2=abs(y2);   
 
f1=n*fs/N;    
f2=n*fs/N-fs/2;

subplot(4,1,1),plot(f1,x,'r');
xlabel('频率/Hz');
ylabel('振幅');title('图1FFT','color','r');grid on;

subplot(4,1,2),plot(f1,mag1,'r');
xlabel('频率/Hz');
ylabel('振幅');title('图1FFT','color','r');grid on;
 
subplot(4,1,3),plot(f2,mag1,'b');  
xlabel('频率/Hz');
ylabel('振幅');title('FFT变换后无FFTSHIFT','color','b');grid on;
 
subplot(4,1,4),plot(f2, mag2,'b');
xlabel('频率/Hz');
ylabel('振幅');title('FFT后进行FFTSHIFT','color','b');grid on;
```