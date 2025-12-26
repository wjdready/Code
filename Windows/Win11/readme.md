

### 1. 右键菜单总是展开

```sh
reg.exe add "HKCU\Software\Classes\CLSID\{86ca1aa0-34aa-4e8b-a509-50c905bae2a2}\InprocServer32" /f /ve
taskkill /F /IM explorer.exe
explorer.exe
```

### 2. 显示(0)/隐藏(1)「了解此图片」桌面图标（1）

```sh
reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\HideDesktopIcons\NewStartPanel" /f /v "{2cc5ca98-6485-489a-920e-b3e88a6ccce3}" /t REG_DWORD /d 0
然后刷新桌面即可
```
