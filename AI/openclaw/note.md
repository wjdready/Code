
配置文件: 

cmd /c "code %USERPROFILE%/.openclaw/openclaw.json"


```sh
npm install -g openclaw@latest

# 最小配置
openclaw setup

# 启动服务器, token 会自动生成到 openclaw.json
openclaw gateway

# 在另一终端
openclaw onboard

# 交互式初始化配置, 最终输出 openclaw.json
# 推荐直接编辑 openclaw.json 文件, 该命令可用来输出参考配置
openclaw onboard --install-daemon
```

我的配置文件:

```json
{
    "agents": {
        "defaults": {
            "workspace": "C:\\Users\\shino\\.openclaw\\workspace"
        }
    },
    "gateway": {
        "mode": "local",
        "auth": {
            "mode": "token",
            "token": "a84e08eaef20d8377bb0753ca776e821c255ae60f94bcc51"
        }
    },
    "meta": {
        "lastTouchedVersion": "2026.4.15",
        "lastTouchedAt": "2026-04-17T13:38:22.266Z"
    },
    "channels": {
        // 接入 telegram 
        "telegram": {
            "enabled": true,
            "botToken": "8726131811:AAFcUsjMqOe3_ALd2K-IWbLIuNCFshPi0zU",
            "dmPolicy": "pairing"
        }
    },
    "models": {
        "mode": "merge",
        "providers": {
            // 模型 API, 支持中转站
            "mycodeapi": {
                "baseUrl": "https://yunyi.rdzhvip.com/claude",
                "apiKey": "NXKSZAD1-EZNR-DCU5-EXAP-SC9RXY8X7RH8-SK",
                "api": "anthropic-messages",
                "models": [
                    {
                        "id": "claude-opus-4-6",
                        "name": "Claude Opus 4.6"
                    }
                ]
            }
        }
    }
}
```
