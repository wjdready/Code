#include <stdio.h>
#include <libserialport.h>
#include <string.h>
#include <windows.h>
#include <pthread.h>

struct sp_port *port = NULL;
int connected = 0;

char port_to_connect[64];

int serialport_connect(const char *port_name)
{
    if(port != NULL)
    {
        sp_close(port);
        sp_free_port(port);
        port = NULL;
    }

    sp_get_port_by_name(port_name, &port);

    int res = sp_open(port, SP_MODE_READ_WRITE);
    if(res != SP_OK)
    {
        sp_free_port(port);
        port = NULL;
        return -1;
    }
    
    sp_set_baudrate(port, 115200);
    sp_set_bits(port, 8);
    sp_set_parity(port, SP_PARITY_NONE);
    sp_set_stopbits(port, 1);
    sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE);

    return 0;
}

/* 连接到第一个扫描到的串口 */
const int serialport_connect_to_available(void)
{
    struct sp_port **port_list = NULL;
    enum sp_return result = sp_list_ports(&port_list);
    if (result != SP_OK) {
        return -1;
    }

    int wish_port_len = strlen(port_to_connect);

    for (int i = 0; port_list[i] != NULL; i++) 
    {
        struct sp_port *port = port_list[i];
        char *port_name = sp_get_port_name(port);
        if (wish_port_len > 0 && strcmp(port_name, port_to_connect) != 0)
            continue;
        int r = serialport_connect(port_name);
        if(r == 0)
        {
            printf("\n连接到串口 %s\n\n", port_name);
            
            sp_free_port_list(port_list);
            return 0;
        }
    }

    sp_free_port_list(port_list);

    return -1;
}

void *serial_write_task(void * arg)
{
    char buf[8192];

    while (1)
    {
        fgets(buf, sizeof(buf), stdin);
        int n = strlen(buf);
        // buf[n++] = '\r';
        buf[n++] = '\n';
        if(connected)
        {
            sp_blocking_write(port, buf, n, 1000);
        }
    }
}

int main(int argc, char const *argv[])
{
    SetConsoleOutputCP(65001);
    // HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    // SetConsoleMode(hInput, ENABLE_VIRTUAL_TERMINAL_INPUT);
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleMode(hOutput, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    if (argc > 1) {
        strcpy(port_to_connect, argv[1]);
        printf("Will connect to %s\n", port_to_connect);
    }

    int res = serialport_connect_to_available();
    if(res != 0)
        printf("\n未找到串口, 请将设备插入USB口\n");
    else 
        connected = 1;
    
    char buf[4096];
    int buf_len = sizeof(buf);

    fgets(buf, sizeof(buf), stdin);

    pthread_t t;
    pthread_create(&t, NULL, serial_write_task, NULL);

    while (1)
    {
        if(connected == 1)
        {
            int n = sp_blocking_read(port, buf, buf_len, 1);
            if(n < 0)
            {
                printf("\n串口连接断开了!\n");
                connected = 0;
                continue;
            }
            if(n > 0)
            {
                printf("%.*s", n, buf);
                fflush(stdout);
            }
        }

        else if(serialport_connect_to_available() == 0)
        {
            connected = 1;
        }

        Sleep(10);
    }
    
    getchar();

    return 0;
}
