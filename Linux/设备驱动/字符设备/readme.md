

## 字符设备

设备号 32 位, 次设备号 20 位, 主设备号 12 位,  最大 4096 

```c
#define MINORBITS 20
#define MINORMASK       ((1U << MINORBITS) - 1)
#define MAJOR(dev)      ((unsigned int) ((dev) >> MINORBITS))
#define MINOR(dev)      ((unsigned int) ((dev) & MINORMASK))
#define MKDEV(ma,mi)    (((ma) << MINORBITS) | (mi))

dev_t dev = MKDEV(245, 15);
printk("devnum: %d, major %d minor %d\n", dev, MAJOR(dev), MINOR(dev));
// devnum: 256901135, major 245 minor 15
```

* register_chrdev_region
    ```c
    /**
     * @brief 分配一个或多个设备号
     * 
     * @param first 要分配的起始设备编号, 包括次设备号, 比如 MKDEV(5, 3) 作为起始, MKDEV(5, 0) 就不算在内
     * @param count 请求的连续设备编号的总数, 注意, 如果 count 太大, 你要求的范围可能溢出到下一个主设备号
     *              但是只要你要求的编号范围可用, 一切都仍然会正确工作.
     * @param name  应当连接到这个编号范围的设备的名字, 它会出现在 /proc/devices 和 sysfs 中
     * @return 如果分配成功进行, register_chrdev_region 的返回值是 0， 出错的情况下, 返回一个负的错误码
     */
    int register_chrdev_region(dev_t first, unsigned int count, char *name);
    ```
    分配设备号时不能有交集, 同一个主设备号可以有不同的名字, 只需注册两次, 两次主设备号相同, 此设备号不同和名称不同即可, 比如
    ```c
    register_chrdev_region(MKDEV(257, 15), 3, "hello");
    register_chrdev_region(MKDEV(257, 18), 5, "fuck");
    ```
    > 设备号 257:15 ~ 257:17 被分配, 名称为 hello, 设备号 257:18 ~ 257:22 被分配, 名称为 fuck, 如果第二次分配时, 和第一次分配的有交集, 比如 `register_chrdev_region(MKDEV(257, 17), 5, "fuck");` 则第二次分配将出错, 返回非0值.

    一个设备可以有多个设备号(包括不同的主设备号), 一个主设备号(包含多个次设备号)也可以分配到不同的设备中(只要次设备号不同), 比如
    ```c
    分配到设备号 257:15 ~ 257:17, 设备号 666:0 ~ 666:12  可以对同一个设备同时添加这两组设备号.
    分配到设备号 257:15 ~ 257:17 共 3 个, 可以分别添加到不同的设备
    ```
    
    也就是说分配到的每一个设备号(主+次组合)都可以独立地被标识并添加到设备中. 设备号的分配和实际设备如何添加设备号没有关系, 实际设备可以添加多个设备号作为自己的标识索引, 而且每个主设备号分配时名称不一致也没有关系. 
    
    设备添加设备号时如果设备号已经被添加到其它设备, 将会发生覆盖, 即设备号将和旧的设备断联, 而和新加的设备关联.

* alloc_chrdev_region
    ```c
    /**
     * @brief 分配一系列字符设备编号。主编号将被动态选择，并在dev中返回(与第一个次要编号一起)
     * 
     * @param dev 输出第一个分配的设备号
     * @param baseminor 请求的次设备号范围的第一个
     * @param count 所需的设备号数量
     * @param name 关联设备或驱动程序的名称
     * @return 返回零错误代码或负错误代码
     */
    int alloc_chrdev_region (dev_t * dev, unsigned baseminor, unsigned count, const char * name);
    该函数总是分配一个新的主设备号, 即使老的设备号还有很多区域没有注册。因此不像 `register_chrdev_region` 那样灵活, 能够一个主设备号注册不同设备。比如
    ```c
    alloc_chrdev_region(&dev, 0, 15, "hello");
    alloc_chrdev_region(&dev, 18, 15, "hello");
    ```
    虽然两次分配, 次设备号没有交集, 但函数返回两个不同的主设备号


* unregister_chrdev_region
    ```c
    /**
     * @brief 销毁分配的设备号
     * 
     * @param from 取消登记的数字范围中的第一个
     * @param count 要注销的设备编号的数量
     */
    void unregister_chrdev_region (	dev_t from, unsigned count);
    ```
    若 count 数量不对, 还有次设备号没没被释放, 则主设备号还会存在, 直到所有次设备号被释放

* cdev_init
    ```c
    /**
     * @brief 初始化一个 cdev 结构，并绑定 fops，使其准备好使用 cdev_add 添加到系统
     * 
     * @param cdev 要初始化的结构
     * @param fops 常量结构 fops
     */
    void cdev_init (struct cdev * cdev, const struct file_operations * fops);
    ```

* cdev_add
    ```c
    /**
     * @brief cdev_add 将由 p 表示的设备添加到系统中，使其立即处于活动状态。
     * 
     * @param p 设备的 cdev 结构
     * @param dev 此设备负责的设备号的第一个
     * @param count 与此设备对应的连续次要号码的数量
     * @return 如果失败，则返回负错误代码 
     */
    int cdev_add (struct cdev * p, dev_t dev, unsigned count);
    ```

* cdev_del
    ```c
    /**
     * @brief 从系统中删除设备 p，可能释放结构本身
     * 
     * @param p 要删除的 cdev 结构
     */
    void cdev_del (struct cdev * p);
    ```