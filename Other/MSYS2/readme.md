
### [2023-03-01 20:50:38] 

打开 MSYS2 时卡顿一会才进入, 参考解决方案:[Disable db in /etc/nsswitch.conf](https://bjg.io/guide/cygwin-ad/)

```shell
mkpasswd -l -c > /etc/passwd
mkgroup -l -c > /etc/group

# 将 files db 中的 db 去掉
vim /etc/nsswitch.conf
```

以及 bash 补全很慢, 参考解决方案:[Disable ~* in bash_completion](https://gist.github.com/k-takata/9b8d143f0f3fef5abdab)

```patch
--- /usr/share/bash-completion/bash_completion.org
+++ /usr/share/bash-completion/bash_completion
@@ -542,9 +542,9 @@
     elif [[ $1 == \'* ]]; then
         # Leave out first character
         printf -v $2 %s "${1:1}"
-    elif [[ $1 == ~* ]]; then
-        # avoid escaping first ~
-        printf -v $2 ~%q "${1:1}"
+    #elif [[ $1 == ~* ]]; then
+    #    # avoid escaping first ~
+    #    printf -v $2 ~%q "${1:1}"
     else
         printf -v $2 %q "$1"
     fi
```