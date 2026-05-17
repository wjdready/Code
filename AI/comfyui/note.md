
```sh

git clone https://github.com/Comfy-Org/ComfyUI.git

# 安装 gpu 版本的 torch
pip install torch torchvision torchaudio --extra-index-url https://download.pytorch.org/whl/cu130

# 安装剩余依赖依赖
pip install -r requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple

# 运行服务
python main.py

```

修改模型配置, 模型可以设置到外部路径, 参考 xtra_model_paths.yaml.example

```yml
comfyui:
    base_path: D:/Home/Documents/ComfyUI
    # You can use is_default to mark that these folders should be listed first, and used as the default dirs for eg downloads
    is_default: true
    checkpoints: models/checkpoints/
    text_encoders: |
         models/text_encoders/
         models/clip/  # legacy location still supported
    clip_vision: models/clip_vision/
    configs: models/configs/
    controlnet: models/controlnet/
    diffusion_models: |
                 models/diffusion_models
                 models/unet
    embeddings: models/embeddings/
    loras: models/loras/
    upscale_models: models/upscale_models/
    vae: models/vae/
    audio_encoders: models/audio_encoders/
    model_patches: models/model_patches/
```
