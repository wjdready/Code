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

    if (argc < 2)
    {
        fprintf(stderr, "Example: %s /dev/ttyGS0\n", argv[0]);
        return -1;
    }

    fd = open(argv[1], O_RDWR);
    if (fd < 0)
    {
        fprintf(stderr, "Can't open file %s: %s\n", argv[1], strerror(errno));
        return -1;
    }

    while (1)
    {
        char buf[64];
        int n = read(fd, buf, sizeof(buf));

        /* 数据正常 */
        if (n > 0)
        {
            printf("get %.*s\n", n, buf);
            continue;
        }

        if (check_usb_connected())
            continue;

        /* usb 已经断开, 等待 usb 连接 */
        while (!check_usb_connected())
        {
            printf("wait usb connected\n");
            sleep(1);
        }

        /* 重新打开 */
        printf("usb reopen\n");
        close(fd);
        fd = open(argv[1], O_RDWR);
        if (fd < 0)
        {
            fprintf(stderr, "Can't open file %s: %s\n", argv[1], strerror(errno));
            sleep(1);
        }
    }

    return 0;
}
