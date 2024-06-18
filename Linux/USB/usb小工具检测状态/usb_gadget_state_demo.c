#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* 检测 usb 是否连接 */
int check_usb_connected(void)
{
    /* ref: https://unix.stackexchange.com/questions/364119/detect-change-in-usb-gadget-state */
    const char *state_path = "/sys/class/udc/3550000.xudc/state";

    int fd = open(state_path, O_RDONLY);
    if (fd < 0)
    {
        fprintf(stderr, "Can't open file %s: %s\n", state_path, strerror(errno));
        return 0;
    }

    char buf[64] = "";
    read(fd, buf, sizeof(buf));
    close(fd);
    return strstr(buf, "configured") != NULL;
}

int main(int argc, char const *argv[])
{
    int fd;

    while (1)
    {

        if (check_usb_connected()) 
        {
            if (fd <= 0)
            {
                printf("usb connected, file open\n");
                fd = open(argv[1], O_RDWR);
                if (fd < 0)
                {
                    fprintf(stderr, "Can't open file %s: %s\n", argv[1], strerror(errno));
                    sleep(1);
                }
            }

            else 
            {
                char buf[64];
                int n = read(fd, buf, sizeof(buf));

                /* 数据正常 */
                if (n > 0)
                {
                    printf("get %.*s\n", n, buf);
                }
            }
        }

        else
        {
            printf("usb disconnected, file close\n");
            if (fd > 0)
                close(fd);
            fd = 0;
            sleep(1);
        }

    }

    return 0;
}
