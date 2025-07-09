#include <libserialport.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: %s <port> <baudrate> \n", argv[0]);
        return -1;
    }

    char *port_names = argv[1];
    struct sp_port *port;
    int baudrate = argc == 3 ? atoi(argv[2]) : 115200;

    sp_get_port_by_name(port_names, &port);
    int res = sp_open(port, SP_MODE_READ_WRITE);
    if(res != SP_OK)
    {
        printf("Open port: %s ERROR %d\n", port_names, res);
        abort();
    }

    printf("Open port: %s baudrate: %d\n", port_names, baudrate);
    sp_set_baudrate(port, baudrate);
    sp_set_bits(port, 8);
    sp_set_parity(port, SP_PARITY_NONE);
    sp_set_stopbits(port, 1);
    sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE);

    const char *get_file_cmd = "<N>{\"Class\":\"135\",\"Flag\":\"20170516010101001\",\"DeviceAWAID\":\"310012000025\",\"CMD\":1,\"Path\":\"/data/2023-01-01/002831/audio_base_00.wav\"}</N>";
    const char *rep_ok = "<N>OK</N>";

    sp_blocking_write(port, get_file_cmd, strlen(get_file_cmd), 100);
    sp_blocking_write(port, rep_ok, strlen(rep_ok), 100);
    unsigned int timeout = 0;  // 设置为0表示非阻塞模式
    int buf_len = 40960000;
    char *buf = malloc(buf_len);
    long total_bytes = 0;
    struct timeval start, end;

    gettimeofday(&start, NULL);  // 开始计时

    while (1)
    {
        int n = sp_nonblocking_read(port, buf, buf_len);
        if(n > 0)
        {
            total_bytes += n;
            gettimeofday(&end, NULL);
            long timeUsedUs = (end.tv_sec - start.tv_sec) * 1000 * 1000 + end.tv_usec - start.tv_usec;
            double speed = total_bytes * 1000.0 * 1000.0 / timeUsedUs / 1024;
            
            printf("Received: %d bytes, Total: %ld bytes, Speed: %.2f KB/s\n", 
                   n, total_bytes, speed);
            
            // 可选：发送确认
            sp_nonblocking_write(port, rep_ok, strlen(rep_ok));
        }
        else if(n == 0)  // 无数据可读
        {
            // usleep(1000);  // 短暂休眠避免CPU占用过高
        }
        else  // 错误
        {
            printf("Read error: %d\n", n);
            break;
        }
    }

    free(buf);
    sp_close(port);

    return 0;
}


/* 
<N>{"Class":"135","Flag":"20170516010101001","DeviceAWAID":"310012000025","CMD":0,"Path":"/"}</N>

<N>{"Class":"135","Flag":"20170516010101001","DeviceAWAID":"310012000025","CMD":1,"Path":"/data/2023-01-01/001021/audio_base_00.wav"}</N>
<N>OK</N>

<N>{"Class":"135","Flag":"20170516010101001","DeviceAWAID":"310012000025","CMD":5,"Option":1,"Disk":"0","Sector":1,"Count":5}</N>
*/
