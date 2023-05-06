#### systemd 

Systemd 是几乎所有现代 Linux 发行版的 init (初始化) 系统。它是 Linux 启动时启动的第一个进程，它控制计算机上运行的所有其他内容。

在我们深入之前，要做的第一件事是检查你是否正在使用 systemd。你可以使用以下命令执行此操作：

```
pidof systemd
```

如果这返回一个数字，说明你正在运行使用 systemd 的 Linux 发行版，如果没有输出，说明你正在使用其他的 init 系统。

我提到过，systemd 控制着你的计算机上运行的所有东西。它通过 services (服务) 来做到这一点，这些服务是 systemd 在预定点自动启动的普通软件。

> 例如，如果你使用的是桌面环境，则在图形系统启动后，systemd 将启动你的登录管理器。此登录管理器启动你的桌面，你的桌面启动你的图形软件。

你可以使用以下命令查看 Linux 上的所有进程是如何从 systemd 衍生出来的。

```
pstree -p
```

#### 服务文件

systemd 通过查看服务文件来决定要启动的内容。如果您熟悉旧式的 SysVinit 系统，你会发现这些服务文件正是 SysVinit 中 init.d 脚本的替代品，但它们的语法非常不同。

让我们通过创建自己的服务来了解这一点。一般来说，服务是无限期运行的软件，我们的服务也一样。这是一个 Hello world 服务，每 3 秒输出一次 `Hello world`。使用以下内容在您的主目录中创建一个名为 hws.sh 的文本文件：

```shell
#!/bin/bash
while $(sleep 3); do
    echo "hello world"
done
```

为确保文件可执行, 输入 `chmod a+x hws.sh`

现在，您可以使用 `./hws.sh` 从命令行运行它，但这不是将其作为服务运行。让我们看看如何让 systemd 在每次启动计算机时自动启动它。服务文件通常分为两个部分：

* unit (单元): 包含有关此文件的用途以及何时运行它的基本信息
* service (服务): 包含有关我们实际要运行的内容的详细信息

而我们的 Hello World 服务的基本服务文件是：

```shell
[Unit]
Description=Hello World service
After=systemd-user-sessions.service

[Service]
Type=simple
ExecStart=/home/shino/hws.sh
```

> * Description: 描述该服务的任何内容
>
> * After: 位于什么服务之后, 即告诉 systemd 何时启动该服务, 在这里表示用户会话服务启动后开始运行，如果需要的话，还可以更早地开始
>
> * Type: 服务类型, simple 表示这是一个将在启动的会话中继续运行的命令, 此外还有比如 Type=forking，它用于那些需要处理自己的守护进程的命令
>
>   也就是说, 如果您执行一个命令时，然后他需要在后台继续运行，那么它是一个 simple 类型，而如果命令触发了一些后台进程并返回命令提示符，那么它就是一个 forking 类型
>
> * ExecStart: 要运行的命令

将其保存在一个名为 hws.service 的文件中(在本例中，文件名很重要)，然后将该文件复制到 `/etc/systemd/system` (您可能需要使用sudo)。一旦准备就绪，您就可以通过以下方式启动该服务：

```shell
systemctl start hws.service
```

一旦完成，你的 Hello World 服务就会在后台运行，不过, 你可能无法直接在终端中看到输出, 因为这样启动的进程的所有输出都被重定向到了 systemd 的另一个部分 `journald`。您可以使用以下命令查看:

```
systemctl status hws
journalctl -u hws -e
```

第一条命令将向您显示最后几个日志行以及有关正在运行的进程的信息。第一条命令可查看整个日志。`-u` 标志告诉 jouralctl 显示哪个单元(即服务文件)的输出，而 `-e` 标志表示从文件末尾开始并向后工作。

默认情况下，这将在可滚动的环境中显示日志，但如果您通过管道将命令发送到其他内容(例如grep)，则它会将文本发送到stdout。



大多数发行版还将来自系统服务的输出发送到系统日志。这是 systemd 中的一个选项，而不是一个固定的特性，我们可能会发现，随着时间的推移，这在越来越少的发行版中发生，因为人们开始习惯于使用 jouralctl 而不是 logfiles (日志文件)。



让我们来看看这些 journald 行

```
9月 24 15:21:06 ubuntu hws.sh[41128]: hello world
9月 24 15:21:09 ubuntu hws.sh[41128]: hello world
```

显然，第一部分是日期。紧接着是发生此操作的机器。尽管最初看起来很明显它在哪台机器上运行，但情况并不总是如此，因为日志通常合并到一个集中的日志记录系统中。下一个是正在运行的软件的名称和进程标识符。最后一个是实际输出信息。

大部分输出看起来都很好，但 `hws.sh` 部分似乎不是很清楚。只有一个文件名, 让人比较困扰, 虽然我们可以重命名运行的文件，但我们不需要这样做。我们可以使用 systemd 的 SyslogIndentifier 选项。

如果在 `hws.service` 文件的 `service` 部分包含以下行，它将用更有意义的内容替换 `hws.sh`：

```
SyslogIndentifier=HelloWorldService
```

我们在这里只看了最基本的选项，还有许多更高级的功能。它们中的大多数都非常简单，只需在服务文件中设置选项即可，这在 syd.exec 手册页中有很好的说明。

