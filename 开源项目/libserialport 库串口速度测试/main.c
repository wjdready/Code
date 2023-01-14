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

    unsigned int timeout = 500;
    int buf_len = 40960000;
    char *buf = malloc(buf_len);

    while (1)
    {
        struct timeval start, end;

        gettimeofday(&start, NULL);
        int n = sp_blocking_read(port, buf, buf_len, timeout);
        gettimeofday(&end, NULL);

        if(n > 0)
        {
            long timeUsedUs = (end.tv_sec - start.tv_sec) * 1000 * 1000 + end.tv_usec - start.tv_usec;
            double speed = n * 1000.0 * 1000.0 / timeUsedUs / 1024;

            printf("recv n: %d  time timeUsedUs %d us: speed %.2f KB/s\n", n, timeUsedUs, speed);
        }
    }
    
    free(buf);
    sp_close(port);

    return 0;
}
