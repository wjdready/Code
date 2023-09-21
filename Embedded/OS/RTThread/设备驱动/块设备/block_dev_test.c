#include <dfs_fs.h>
#include <rtdevice.h>
#include <rtthread.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    struct rt_device dev;
    struct dfs_partition part;
    struct rt_device_blk_geometry geometry;
    FILE *fp;
    char *mem;
} myblkdev_t;

#define SIM_BLOCK_SIZE 512
#define SIM_BLOCK_COUNT 200

// static char mem[SIM_BLOCK_SIZE * SIM_BLOCK_COUNT];

static rt_err_t dev_control(rt_device_t dev, int cmd, void *args)
{
    myblkdev_t *blk_dev = (myblkdev_t *)dev->user_data;
    switch (cmd)
    {
    case RT_DEVICE_CTRL_BLK_GETGEOME:
        rt_memcpy(args, &blk_dev->geometry, sizeof(struct rt_device_blk_geometry));
        break;
    default:
        break;
    }
    return RT_EOK;
}

static rt_err_t dev_init(rt_device_t dev)
{
    return RT_EOK;
}

static rt_err_t dev_open(rt_device_t dev, rt_uint16_t oflag)
{
    // myblkdev_t *blk_dev = (myblkdev_t *)dev->user_data;
    // blk_dev->fp = fopen("sd1.img", "r+b");
    // if(blk_dev->fp == NULL)
    //     return RT_ERROR;

    return RT_EOK;
}

static rt_err_t dev_close(rt_device_t dev)
{
    // myblkdev_t *blk_dev = (myblkdev_t *)dev->user_data;
    // fclose(blk_dev->fp);
    return RT_EOK;
}

static rt_size_t dev_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    myblkdev_t *blk_dev = (myblkdev_t *)dev->user_data;
    int blk_size = blk_dev->geometry.block_size;

    // fseek(blk_dev->fp, pos * blk_size, SEEK_SET);
    // fread(buffer, 1, size * blk_size, blk_dev->fp);

    memcpy(buffer, blk_dev->mem + pos * blk_size, blk_size * size);
    return size;
}

static rt_size_t dev_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    myblkdev_t *blk_dev = (myblkdev_t *)dev->user_data;
    int blk_size = blk_dev->geometry.block_size;

    // fseek(blk_dev->fp, pos * blk_size, SEEK_SET);
    // fwrite(buffer, 1, size * blk_size, blk_dev->fp);

    memcpy(blk_dev->mem + pos * blk_size, buffer, blk_size * size);

    return size;
}

static myblkdev_t *mydev_sim_register(const char *dname)
{
    myblkdev_t *mydev = rt_malloc(sizeof(myblkdev_t));
    if (mydev == NULL)
        return NULL;
    memset(mydev, 0, sizeof(myblkdev_t));

    mydev->mem = rt_malloc(SIM_BLOCK_SIZE * SIM_BLOCK_COUNT);
    if (mydev->mem == NULL)
    {
        rt_free(mydev);
        return NULL;
    }

    mydev->dev.user_data = mydev;
    // mydev->mem = mem;

    mydev->geometry.block_size = SIM_BLOCK_SIZE;
    mydev->geometry.sector_count = SIM_BLOCK_COUNT;
    mydev->geometry.bytes_per_sector = SIM_BLOCK_SIZE;

    mydev->dev.init = dev_init;
    mydev->dev.open = dev_open;
    mydev->dev.close = dev_close;
    mydev->dev.read = dev_read;
    mydev->dev.write = dev_write;
    mydev->dev.control = dev_control;

    /* register as block device type */
    mydev->dev.type = RT_Device_Class_Block;
    rt_device_register(&mydev->dev, dname, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_REMOVABLE | RT_DEVICE_FLAG_STANDALONE);

    return NULL;
}

/* ================== 使用测试 ================ */

static myblkdev_t *mydev;

static int my_dev_init(void)
{
    mydev = mydev_sim_register("sd1");
    return 0;
}
INIT_BOARD_EXPORT(my_dev_init);


// static int create_sd1img(void)
// {

//     FILE *fp = fopen("sd1.img", "wb+");

//     char dat[512];

//     for (int i = 0; i < 1024; i++)
//         fwrite(dat, 1, sizeof(dat), fp);
//     fclose(fp);

//     return 0;
// }
// MSH_CMD_EXPORT(create_sd1img, create_sd1img);
