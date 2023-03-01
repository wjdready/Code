

# 本文将在 01Base 的基础上添加 USB 功能

[官方文档](https://qemu-project.gitlab.io/qemu/system/devices/usb.html)

添加U USB 鼠标和 USB 键盘

```shell
-device usb-ehci -device usb-kbd -device usb-mouse -usb
```

lsusb -t
-device usb-host,hostbus=3,hostport=10 \
-device usb-host,hostbus=3,hostport=11 \

对鼠标进行读取

```c
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fd, i;
    int x=0,y=0,z=0;
    if(argc < 2) {
        printf("usage: %s /dev/input/eventX \n", argv[0]);
        return 1;
    }
    fd = open(argv[1], O_RDONLY);
    struct input_event ev[64];

    while (1)
    {
        int bytes = read(fd, &ev, sizeof(struct input_event)*64);

        if(bytes > 0){
            for (i = 0;  i < (int)(bytes / sizeof(struct input_event)); i++) {
                if(ev[i].type == 1)
                    //printf("key %i state %i\n", ev.code, ev.value);
                    if(ev[i].value == 0)
                        printf(" : [key %i]\n ", ev[i].code);
                if(ev[i].type == EV_REL) {
                    //printf("Ev.type %d, Ev.code %d, Ev.value %d\n", ev[i].type, ev[i].code, ev[i].value);
                    if(ev[i].code == REL_X){
                         x+=ev[i].value;
                    }
                    if(ev[i].code == REL_Y){
                        y+=ev[i].value;
                    }
                    if(ev[i].code == REL_WHEEL)
                        z+=ev[i].value;
                    printf("X = %d, Y = %d, Z = %d\n", x,y,z);
                }
            }
        }
    }
}
```

#### 到底如何让 Geust 成为 USB 设备啊

https://source.android.com/docs/devices/automotive/start/passthrough