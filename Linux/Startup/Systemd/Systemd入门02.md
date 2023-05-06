
**Systemd**是 Linux 操作系统的系统和服务管理器。它旨在向后兼容 SysV 初始化脚本，并提供许多功能，例如在启动时并行启动系统服务、按需激活守护程序或基于依赖关系的服务控制逻辑。

#### 单元文件

systemd 引进了 *systemd 单元的概念*。这些单元由各种**单元配置文件**来表示, 并封装系统服务、侦听套接字和其他与 init 系统相关的对象信息。

**单元文件**通常由三个章节组成

- [Unit] - 包含不依赖于该单元类型的通用选项。这些选项提供单元描述,指定单元的行为,并将依赖项设置为其他单元。

- [单元类型] - 指明单元的类型 比如服务单元 `[Service]` 定时器单元 `[Timer]`

- [install] - 包含 `systemctl enable` 和 `disable` 命令使用的单元安装信息
  

**[Unit] 章节选项**

下面是常见的 `Unit` 章节选项

| 选项            | 描述                                                                                           |
| --------------- | ---------------------------------------------------------------------------------------------- |
| `Description`   | 对这个单元的描述, 会显示在`systemctl status`的输出中                                           |
| `Documentation` | 提供单元参考文档的URI列表                                                                      |
| `After`         | 定义启动单位的顺序。只有在 `After` 指定的单位处于活动状态后，该单位才会启动。                  |
|                 | 与 `Requires` 不同，`After` 不会显式激活指定的单位。`Before` 选项与 `After` 选项具有相反的功能 |
| `Requires`      | 配置对其他单元的依赖关系。`Requires`中列出的单元将先被激活。                                   |
|                 | 如果任何所需的单元无法启动，则该单元不会被激活                                                 |
| `Wants`         | 配置比 `Requires` 更弱的依赖项。即使列出的单元激活不成功，                                     |
|                 | 也不会影响本单元的激活。这是建立自定义设备依赖关系的推荐方式                                   |
| `Conflicts`     | 配置负依赖关系，与`Requires`相反, 如果`Requires`中列出的单元有一项被激活了, 该单元就不要激活了 |

> 在大多数情况下，仅使用`After`和`Before`设置顺序和依赖关系就足够了。如果您还使用`Wants`(推荐)或`Requires`设置依赖关系，则仍然需要对依赖项进行排序。这是因为排序和依赖关系相互独立地工作

**systemd 单元类型**的完整列表如下:

| 单位类型       | 文件扩展     | 描述                           |
| :------------- | :----------- | :----------------------------- |
| 服务单元       | `.service`   | 系统服务                       |
| 目标单元       | `.target`    | 一组 systemd 单元              |
| Automount 单元 | `.automount` | 文件系统自动挂载点             |
| 设备单元       | `.device`    | 内核可识别的设备文件           |
| 挂载单位       | `.mount`     | 文件系统挂载点                 |
| 路径单元       | `.path`      | 文件系统中的一个文件或者目录   |
| Scope 单元     | `.scope`     | 外部创建的进程                 |
| Slice 单元     | `.slice`     | 一组管理系统进程的分层组织单元 |
| 快照单元       | `.snapshot`  | systemd 管理器的保存状态       |
| 套接字单元     | `.socket`    | 进程间的通信套接字             |
| Swap 单元      | `.swap`      | 一个交换设备或者一个交换文件   |
| 计时器单元     | `.timer`     | systemd 计时器                 |

https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/system_administrators_guide/chap-managing_services_with_systemd#sect-Managing_Services_with_systemd-Services-Start_confl

**[Service] 单元的部分选项**

| 选项              | 描述                                                                                        |
| :---------------- | :------------------------------------------------------------------------------------------ |
| `Type`            | 影响 `ExecStart` 功能和相关选项的单元进程启动类型。以下选项之一:                            |
|                   | **simple** (默认) -- `ExecStart` 启动的进程是服务的主进程                                   |
|                   | **forking** -- 从 `ExecStart` 开始的进程会产生一个子进程，该子进程将成为服务的主进程。      |
|                   | 启动完成后，父进程退出                                                                      |
|                   | **oneshot** -- 类似于`simple`，但进程在启动后续单元之前退出                                 |
|                   | **dbus** -- 类似于`simple`，但后续单元仅在主进程获得D-Bus名称后启动                         |
|                   | **notify** -- 类似于`simple`，但后续单元仅在通过 `sd_notify()` 函数发送通知消息后启动       |
|                   | **idle** -- 类似于`simple`，服务二进制文件的实际执行被延迟到所有作业完成，这避免了将 status |
|                   | 输出与服务的 shell 输出混淆                                                                 |
| `ExecStart`       | 指定单元启动时要执行的命令或脚本。`ExecStartPre` 和 `ExecStartPost` 指定在 `ExecStart`      |
|                   | 之前和之后执行的自定义命令。`Type=oneshot` 时允许指定多个自定义命令，然后依次执行这些命令。 |
| `ExecStop`        | 指定在设备停止时要执行的命令或脚本。                                                        |
| `ExecReload`      | 指定重新加载单元时要执行的命令或脚本。                                                      |
| `Restart`         | `systemctl`启用此选项后，服务将在其进程退出后重新启动，但命令 完全停止除外。                |
| `RemainAfterExit` | 如果设置为 True，即使所有进程都退出，该服务也被认为是活动的。默认值为假。                   |
|                   | `Type=oneshot`如果已配置， 此选项特别有用。                                                 |

**systemd 单元文件位置**

| 目录                       | 描述                                                                       |
| :------------------------- | :------------------------------------------------------------------------- |
| `/usr/lib/systemd/system/` | 安装的软件包中的 systemd 单元文件。                                        |
| `/run/systemd/system/`     | 在运行时创建的 systemd 单元文件。该目录优先于安装了的服务单元文件的目录。  |
| `/etc/systemd/system/`     | Systemd 单元文件由 `systemctl enable` 创建，并添加用于扩展服务的单元文件。 |
|                            | 这个目录优先于带有运行时单元文件的目录。                                   |



#### 使用 system.conf 覆盖默认 systemd 配置

默认 systemd 配置是在编译过程中定义的,它可在 systemd 配置文件中找到,地址为 `/etc/systemd/system.conf`。如果您想与那些默认值分离，并全局覆盖所选的 systemd 单元默认值，请使用这个文件。

例如：要覆盖超时限制的默认值（设置为 90 秒）,请使用 `DefaultTimeoutStartSec` 参数输入所需的值, 以秒为单位。

```none
DefaultTimeoutStartSec=required value
```

## 管理系统服务


服务单元以 `.service` 文件扩展名结尾,其用途与初始化脚本类似。要查看、启动、停止、重启、启用或禁用系统服务使用`systemctl`

| service                    | systemctl                                                          | 描述               |
| :------------------------- | :----------------------------------------------------------------- | :----------------- |
| `service name start`       | `systemctl start name.service`                                     | 启动一个服务       |
| `service name stop`        | `systemctl stop name.service`                                      | 停止服务           |
| `service name restart`     | `systemctl restart name.service`                                   | 重启服务           |
| `service name condrestart` | `systemctl try-restart name.service`                               | 仅在运行时重启服务 |
| `service name reload`      | `systemctl reload name.service`                                    | 重新加载配置。     |
| `service name status`      | `systemctl status name.service` `systemctl is-active name.service` | 检查服务是否在运行 |
| `service --status-all`     | `systemctl list-units --type service --all`                        | 显示所有服务的状态 |



| chkconfig               | systemctl                                  | 描述                             |
| :---------------------- | :----------------------------------------- | :------------------------------- |
| `chkconfig name on`     | `systemctl enable name.service`            | 启用服务                         |
| `chkconfig name off`    | `systemctl disable name.service`           | 禁用服务                         |
| `chkconfig --list name` | `systemctl status name.service`            |                                  |
|                         | `systemctl is-enabled name.service`        | 检查是否启用了服务               |
| `chkconfig --list`      | `systemctl list-unit-files --type service` | 列出所有服务并检查是否启用它们   |
| `chkconfig --list`      | `systemctl list-dependencies --after`      | 列出在指定单元前排序启动的服务   |
| `chkconfig --list`      | `systemctl list-dependencies --before`     | 列出在指定单元之后排序启动的服务 |

对于服务单元而言, 后缀 `.service` 可以省略 即 `systemctl stop nfs-server` 等价于 `systemctl stop nfs-server.service` 

另外,有些单元有别名。我们可以使用别名而不是实际的单元名称。可显示可用于特定单元的所有别名, 比如

```shell
systemctl show sshd -p Names
```

#### systemctl 在 chroot 环境中的行为

在 chroot 环境中大多数 `systemctl` 命令会拒绝执行任何操作。这样做的原因是, `systemd` 进程以及使用 `chroot` 命令的用户在文件系统中并没有同样的视图。

一个例外是单元文件命令,如 `systemctl enable` 和 `systemctl disable` 命令。这些命令不需要正在运行的系统,且不会影响正在运行的进程,但它们确实会影响单元文件。

### 列出服务

列出所有目前载入的服务单元

```none
systemctl list-units --type service
```

对于每个服务单元文件,这个命令显示其全名(`UNIT`),后面有是否载入了单元文件(`LOAD`)、其高级别(`ACTIVE`)和低级别(`SUB`)单元文件激活状态,以及简短描述(`DESCRIPTION`)。

默认情况下, `systemctl list-units` 命令只显示活跃的单元。如果您想列出所有载入的单元,无论它们的状态如何,请使用 `--all` 或 `-a` 命令行选项运行这个命令

您还可以列出所有可用服务单元以查看是否启用了它们。要做到这一点，请键入

```none
systemctl list-unit-files --type service
```
