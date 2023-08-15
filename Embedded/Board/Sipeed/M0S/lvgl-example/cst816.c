#include "cst816.h"
#include "bflb_gpio.h"
#include "bflb_i2c.h"
#include "bflb_mtimer.h"
#include "lv_conf.h"
#include "lv_port_indev.h"

static struct bflb_device_s *touch_cst816_i2c = NULL;

static void cst816_i2c_gpio_init(void)
{
    struct bflb_device_s *gpio = bflb_device_get_by_name("gpio");

    /* I2C0_SCL */
    bflb_gpio_init(gpio, GPIO_PIN_6, GPIO_FUNC_I2C0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    /* I2C0_SDA */
    bflb_gpio_init(gpio, GPIO_PIN_7, GPIO_FUNC_I2C0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
}

int cst816_init(void)
{
    touch_cst816_i2c = bflb_device_get_by_name("i2c0");

    if (touch_cst816_i2c)
    {
        printf("cst816 i2c gpio init\r\n");
        /* init i2c gpio */
        cst816_i2c_gpio_init();
        /* init i2c 200k */
        bflb_i2c_init(touch_cst816_i2c, 200000);
    }
    else
    {
        printf("i2c device get fail\r\n");
        return -1;
    }

    return 0;
}

static int cst816_i2c_read_byte(uint16_t register_addr, void *data_buf, uint16_t len)
{
    static struct bflb_i2c_msg_s msg[2];

    msg[0].addr = CST816_DEFAULT_ADDR;
    msg[0].flags = I2C_M_NOSTOP;
    msg[0].buffer = (uint8_t *)(&register_addr);
    msg[0].length = 2;

    msg[1].addr = CST816_DEFAULT_ADDR;
    msg[1].flags = I2C_M_READ;
    msg[1].buffer = (uint8_t *)data_buf;
    msg[1].length = len;
    bflb_i2c_transfer(touch_cst816_i2c, msg, 2);
    return 0;
}


void cst826_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    uint8_t touchBuffer[8];
    cst816_i2c_read_byte(0x01, touchBuffer, 8);

    data->state = touchBuffer[2];
    
    data->point.x = touchBuffer[4];
    data->point.y = (touchBuffer[5] << 8) | touchBuffer[6];

    if(data->point.y > 260)
    {
        data->point.y -= 20;
    }

    printf("read data state %d x %d y %d\n", data->state, data->point.x, data->point.y);
}
