#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <sound/soc.h>
#include <sound/pcm.h>
#include <sound/initval.h>
#include <linux/clk.h>

struct snd_soc_dai_driver dummy_dai = {
	.name = "dummy_codec",
	.playback = {
		.stream_name = "Dummy Playback",
		.channels_min = 2,
		.channels_max = 8,
		.rates = SNDRV_PCM_RATE_8000_192000,
		.formats = (SNDRV_PCM_FMTBIT_S16_LE |
			    SNDRV_PCM_FMTBIT_S20_3LE |
			    SNDRV_PCM_FMTBIT_S24_LE |
			    SNDRV_PCM_FMTBIT_S32_LE),
	},
	.capture = {
		.stream_name = "Dummy Capture",
		.channels_min = 2,
		.channels_max = 8,
		.rates = SNDRV_PCM_RATE_8000_192000,
		.formats = (SNDRV_PCM_FMTBIT_S16_LE |
			    SNDRV_PCM_FMTBIT_S20_3LE |
			    SNDRV_PCM_FMTBIT_S24_LE |
			    SNDRV_PCM_FMTBIT_S32_LE),
	},
};

static struct snd_soc_codec_driver soc_dummy_codec;

static int rockchip_dummy_codec_probe(struct platform_device *pdev)
{
    struct clk *mclk;
    int ret;

    mclk = devm_clk_get(&pdev->dev, "mclk");
	if (IS_ERR(mclk)) {
		dev_err(&pdev->dev, "Failed getting the mclk. The current implementation does not support the usage of this codec without mclk\n");
		return PTR_ERR(mclk);
	}

    ret = clk_set_rate(mclk, 24576000);
    if (ret)
        dev_err(&pdev->dev, "Fail to set mclk %d\n", ret);
    
    ret = clk_prepare_enable(mclk);
    if (ret) {
    	dev_err(&pdev->dev, "Failed to enable master clock mclk\n");
    }

	return snd_soc_register_codec(&pdev->dev, &soc_dummy_codec,
				      &dummy_dai, 1);
}

static int rockchip_dummy_codec_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);

	return 0;
}

static const struct of_device_id rockchip_dummy_codec_of_match[] = {
	{ .compatible = "mysimplecard", },
	{},
};
MODULE_DEVICE_TABLE(of, rockchip_dummy_codec_of_match);

static struct platform_driver rockchip_dummy_codec_driver = {
	.driver = {
		.name = "mysimplecard",
		.of_match_table = of_match_ptr(rockchip_dummy_codec_of_match),
	},
	.probe = rockchip_dummy_codec_probe,
	.remove = rockchip_dummy_codec_remove,
};

module_platform_driver(rockchip_dummy_codec_driver);

MODULE_LICENSE("GPL");
