
按 Win + R 打开运行窗口，输入 regedit 打开注册表编辑器

HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Command Processor

在右侧空白处右键点击，选择"新建" > "字符串值"，命名为 Autorun

双击 Autorun，将数值数据设置为

chcp 65001
