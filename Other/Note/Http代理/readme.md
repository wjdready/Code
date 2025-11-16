
https://www.cnblogs.com/migrator/p/18837223#linux%E8%AE%BE%E7%BD%AE%E6%B0%B8%E4%B9%85httphttps%E4%BB%A3%E7%90%86



```sh

export http_proxy=http://127.0.0.1:7890
export https_proxy=http://127.0.0.1:7890

# 验证
curl -I https://www.google.com
```