

# 2025-11-19 20:50:58
C# 低功耗蓝牙读写测试, 坑点在于一开始 `InTheHand.BluetoothLE` 库一直报错，一度以为只支持类Unix, 
后来发现 TargetFramework 必须还要包含指定 windows 版本，也就是说必须是指定 windows + .NET 版本才行,
现在测试 net8.0-windows10.0.19041 是可以的

