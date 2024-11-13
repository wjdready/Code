


音频驱动

sound/core/sound.c

```c
// 全局变量
static int major = CONFIG_SND_MAJOR;
int snd_major;
EXPORT_SYMBOL(snd_major);

// 直接是使用 subsys_initcall 来调用初始化音频子系统
subsys_initcall(alsa_sound_init);

// 调用顺序
alsa_sound_init
    // 先是向内核注册一个字符类设备 alsa
    // 其中 major 是全局变量， 也就说固定为 CONFIG_SND_MAJOR, 默认 116
    // 注册后通过 cat /proc/devices 可以看到
    register_chrdev(major, "alsa", &snd_fops)

    // 然后调用初始化声卡信息列表, snd_info_init 函数在 sound/core/info.c
    snd_info_init()

        // 首先声卡信息列表入口, 存到全局变量 snd_proc_root
        snd_proc_root = snd_info_create_entry("asound", NULL);

        // 然后在 /proc 中创建一个文件夹, 可以通过 ls /proc/asound 来得到验证
        snd_proc_root->p = proc_mkdir("asound", NULL);

        // 之后就是一系列信息初始化
        snd_info_version_init()     // 创建了文件 /proc/asound/version
	    snd_minor_info_init()       // 创建了文件夹 /proc/asound/devices
	    snd_minor_info_oss_init() 
	    snd_card_info_init()        // 创建了文件 /proc/asound/cards
	    snd_info_minor_register() 
```

然后就结束了, 之后介绍默认自带的一个节点 timer 

sound/core/timer.c

```c
// 全局变量
static struct device timer_dev;

// 模块入口
module_init(alsa_timer_init)

// 执行流程
alsa_timer_init()
    // snd_device_initialize 函数在 sound/core/init.c
    // 主要就是初始化设备，并设置设备的 class
    
    snd_device_initialize(&timer_dev, NULL);
        device_initialize(dev);
        dev->class = sound_class;
            // sound_class 是在 sound/sound_core.c 中定义和初始化的, 入口是 subsys_initcall(init_soundcore);
            // init_soundcore 执行流程:
            // 创建一个设备类 sound 并保存到全局变量， 这样在 /sys/class/ 下就有了sound类了
            sound_class = class_create(THIS_MODULE, "sound");
            // sound_devnode 函数返回前缀字符串, 也决定了相应的设备节点也将会出现在/dev/snd/下面
            sound_class->devnode = sound_devnode;
        dev->release = default_release;
    
    // 设置设备名称
    dev_set_name(&timer_dev, "timer")

    // 注册系统定时器
    err = snd_timer_register_system()

    // 注册该设备
    err = snd_register_device(SNDRV_DEVICE_TYPE_TIMER, NULL, 0, &snd_timer_f_ops, NULL, &timer_dev);

    // 初始化定时器
    snd_timer_proc_init()
```

剩下的函数基本都是 IOCTL 了. 
有了基本的声卡设备框架，之后就是根据设备树动态创建声卡节点了

现在来看 simple-audio-card, 位置是 kernel/sound/soc/generic/simple-card.c

```c

// 主要数据结构
struct simple_card_data {
	struct snd_soc_card snd_card;
	struct simple_dai_props {
		struct asoc_simple_dai cpu_dai;
		struct asoc_simple_dai codec_dai;
		unsigned int mclk_fs;
	} *dai_props;
	unsigned int mclk_fs;
	int gpio_hp_det;
	int gpio_hp_det_invert;
	int gpio_mic_det;
	int gpio_mic_det_invert;
	bool codec_hp_det;
	struct snd_soc_dai_link dai_link[];	/* dynamically allocated */
};

// 入口
static int asoc_simple_card_probe(struct platform_device *pdev)

    struct simple_card_data *priv;

    // 首先进来就是分配一个 simple_card_data 数据
    priv = devm_kzalloc(dev, sizeof(*priv) + sizeof(*dai_link) * num_links, GFP_KERNEL);

    // 然后设置数据
    snd_soc_card_set_drvdata(&priv->snd_card, priv);
    // 注册声卡
	ret = devm_snd_soc_register_card(&pdev->dev, &priv->snd_card);
        ret = snd_soc_register_card(card);
            ret = snd_soc_init_multicodec(card, link);
```
