


从 huggingface 的 开源模型入手 https://huggingface.co/docs 

LLM 基础教程: https://huggingface.co/learn/llm-course

## 命令行工具

https://hugging-face.cn/docs/huggingface_hub/index


```sh

# 模型下载网站参考: https://zhuanlan.zhihu.com/p/1954293183883215174

# 设置镜像
# $env:HF_ENDPOINT = "https://hf-mirror.com"

pip install huggingface_hub

# 安装模型到本地
hf download black-forest-labs/FLUX.2-klein-4B --local-dir D:/B/Model/black-forest-labs/FLUX.2-klein-4B

# 只下载单个文件
hf download jayn7/Z-Image-Turbo-GGUF z_image_turbo-Q8_0.gguf --local-dir D:/B/Model/jayn7/Z-Image-Turbo-GGUF

```

## 1. 依赖

```sh
# transformers：加载模型 / 分词器、生成文本
# accelerate：自动分配 GPU/CPU、显存优化
# safetensors：读取权重文件
# sentencepiece：Qwen 分词器依赖
pip install torch transformers accelerate safetensors sentencepiece --upgrade

mise use uv

uv venv myenv

myenv\Scripts\activate.ps1

# 安装 cuda 版本的 torch torchvision
uv pip install torch --index-url https://download.pytorch.org/whl/cu124
# 安装 diffusers 和 transformers
uv pip install diffusers["torch"] transformers

```

https://huggingface.co/learn/diffusion-course/unit0/1



## 直接使用 huggingface 运行模型的方式似乎对显存要求较高

使用压缩模型 GGUF 

国内开源模型

TODO  Z-Image-Turbo 在 4090 16G 显存 diffusers 下运行很慢, 如何使用压缩版 Z-Image-Turbo GGUF 

LLaMA (Large Language Model Meta AI) Meta 开源的大语言模型框架, 拉丁语 llama 是羊驼

llama.cpp 让 LLaMA 等大模型在消费级硬件上高效、低内存本地运行

vLLM (Virtual Large Language Model) 是伯克利 LMSYS 开源的企业级、高吞吐、GPU 优先的 LLM 推理 / 服务引擎

ollama 是基于 llama.cpp 的本地部署工具, 定位是大模型版 Docker, 一条命令拉取、运行、管理 LLaMA/Qwen/Mistral 等 GGUF 模型

LMStudio 似乎也是基于 llama.cpp 的本地部署工具, GUI 更友好

.safetensors 是 Hugging Face 推出的安全、高效、纯张量权重存储格式（后缀 .safetensors），专门替代 PyTorch 传统的 .pt/.bin/pth（基于 pickle），核心是只存权重张量 + 元数据，不存任何可执行代码。safetensors 只存权重，必须搭配 config.json 才能完整定义模型

VAE (Variational Auto-Encoders) 变分自编码器, 本身也是一个网络模型, 用于将图片编码或解码

#### llama.cpp 编译

运行 llama-cli --model 指定的 gguf 模型即可进入对话

