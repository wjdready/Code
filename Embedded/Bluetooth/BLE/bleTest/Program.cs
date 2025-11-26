using InTheHand.Bluetooth;

class Program
{
    static async Task Main(string[] args)
    {
        try
        {
            // 1. 扫描蓝牙设备
            Console.WriteLine("扫描蓝牙设备...");
            IReadOnlyCollection<BluetoothDevice> devices = await Bluetooth.ScanForDevicesAsync();
            
            if (devices.Count == 0)
            {
                Console.WriteLine("未发现任何蓝牙设备。");
                return;
            }
            
            // 2. 列出设备供用户选择
            Console.WriteLine("请选择要连接的设备：");
            List<BluetoothDevice> deviceList = devices.ToList();
            for (int i = 0; i < deviceList.Count; i++)
            {
                Console.WriteLine($"{i + 1}. {deviceList[i].Name} ({deviceList[i].Id})");
            }
            
            Console.Write("请输入设备编号: ");
            string input = Console.ReadLine();
            if (!int.TryParse(input, out int selectedIndex) || selectedIndex < 1 || selectedIndex > deviceList.Count)
            {
                Console.WriteLine("无效的选择。");
                return;
            }
            
            BluetoothDevice selectedDevice = deviceList[selectedIndex - 1];
            Console.WriteLine($"正在连接到: {selectedDevice.Name}...");
            
            // 3. 连接到选择的设备
            await selectedDevice.Gatt.ConnectAsync();
            Console.WriteLine("连接成功！");
            
            // 4. 获取并列出所有服务
            Console.WriteLine("正在获取服务...");
            IReadOnlyCollection<GattService> services = await selectedDevice.Gatt.GetPrimaryServicesAsync();
            
            if (services.Count == 0)
            {
                Console.WriteLine("未发现任何GATT服务。");
                return;
            }
            
            // 创建列表存储可通知和可写入的特征
            List<(GattService service, GattCharacteristic characteristic)> notifyCharacteristics = new List<(GattService, GattCharacteristic)>();
            List<(GattService service, GattCharacteristic characteristic)> writeCharacteristics = new List<(GattService, GattCharacteristic)>();
            
            Console.WriteLine("\n设备提供的服务和特征：");
            int serviceIndex = 1;
            
            foreach (GattService service in services)
            {
                Console.WriteLine($"\n服务 {serviceIndex}: {service.Uuid}");
                
                // 获取服务的所有特征
                IReadOnlyCollection<GattCharacteristic> characteristics = await service.GetCharacteristicsAsync();
                int charIndex = 1;
                
                foreach (GattCharacteristic characteristic in characteristics)
                {
                    // 检查特征属性
                    bool supportsNotify = characteristic.Properties.HasFlag(GattCharacteristicProperties.Notify) || 
                                          characteristic.Properties.HasFlag(GattCharacteristicProperties.Indicate);
                    bool supportsWrite = characteristic.Properties.HasFlag(GattCharacteristicProperties.Write) || 
                                         characteristic.Properties.HasFlag(GattCharacteristicProperties.WriteWithoutResponse);
                    
                    Console.WriteLine($"  特征 {charIndex}: {characteristic.Uuid}");
                    Console.WriteLine($"     特性: {characteristic.Properties}");
                    Console.WriteLine($"     支持通知: {supportsNotify}");
                    Console.WriteLine($"     支持写入: {supportsWrite}");
                    
                    // 根据属性添加到相应列表
                    if (supportsNotify)
                    {
                        notifyCharacteristics.Add((service, characteristic));
                    }
                    if (supportsWrite)
                    {
                        writeCharacteristics.Add((service, characteristic));
                    }
                    
                    charIndex++;
                }
                
                serviceIndex++;
            }
            
            // 主功能菜单
            while (true)
            {
                Console.WriteLine("\n=== 功能菜单 ===");
                Console.WriteLine("1. 启用通知监听");
                Console.WriteLine("2. 写入特征值");
                Console.WriteLine("3. 断开连接并退出");
                Console.Write("请选择功能: ");
                
                input = Console.ReadLine();
                
                switch (input)
                {
                    case "1":
                        await HandleNotificationFeature(notifyCharacteristics);
                        break;
                    case "2":
                        await HandleWriteFeature(writeCharacteristics);
                        break;
                    case "3":
                        Console.WriteLine("程序已结束。");
                        return;
                    default:
                        Console.WriteLine("无效的选择，请重试。");
                        break;
                }
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"发生错误: {ex.Message}");
            Console.WriteLine($"错误详情: {ex.StackTrace}");
        }
    }
    
    // 处理通知功能
    private static async Task HandleNotificationFeature(List<(GattService service, GattCharacteristic characteristic)> notifyCharacteristics)
    {
        if (notifyCharacteristics.Count == 0)
        {
            Console.WriteLine("没有找到支持通知的特征。");
            return;
        }
        
        Console.WriteLine("\n支持通知的特征列表：");
        for (int i = 0; i < notifyCharacteristics.Count; i++)
        {
            var (service, characteristic) = notifyCharacteristics[i];
            Console.WriteLine($"{i + 1}. 服务: {service.Uuid}, 特征: {characteristic.Uuid}");
        }
        
        Console.Write("请选择要启用通知的特征编号: ");
        string input = Console.ReadLine();
        if (!int.TryParse(input, out int charSelection) || charSelection < 1 || charSelection > notifyCharacteristics.Count)
        {
            Console.WriteLine("无效的选择。");
            return;
        }
        
        var (selectedService, selectedCharacteristic) = notifyCharacteristics[charSelection - 1];
        
        Console.WriteLine($"\n正在启用 {selectedCharacteristic.Uuid} 的通知...");
        
        // 查找CCCD描述符
        GattDescriptor cccdDescriptor = null;
        try
        {
            var descriptors = await selectedCharacteristic.GetDescriptorsAsync();
            cccdDescriptor = descriptors.FirstOrDefault(d => d.Uuid.ToString().Equals("00002902-0000-1000-8000-00805f9b34fb", StringComparison.OrdinalIgnoreCase));
        }
        catch (Exception ex)
        {
            Console.WriteLine($"获取描述符时出错: {ex.Message}");
        }
        
        // 启用通知
        bool notificationStarted = false;
        if (cccdDescriptor != null)
        {
            try
            {
                // 写入CCCD以启用通知 (0x0001 表示启用通知)
                byte[] notificationValue = { 0x01, 0x00 };
                await cccdDescriptor.WriteValueAsync(notificationValue);
                notificationStarted = true;
                Console.WriteLine("通过写入描述符成功启用通知。");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"写入描述符时出错: {ex.Message}");
            }
        }
        
        // 尝试另一种方式启用通知
        if (!notificationStarted)
        {
            try
            {
                await selectedCharacteristic.StartNotificationsAsync();
                notificationStarted = true;
                Console.WriteLine("通过StartNotificationsAsync成功启用通知。");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"调用StartNotificationsAsync时出错: {ex.Message}");
            }
        }
        
        if (!notificationStarted)
        {
            Console.WriteLine("无法启用通知。");
            return;
        }
        
        // 使用轮询方式检查值变化
        Console.WriteLine("通知已启用。正在轮询数据...");
        Console.WriteLine("按任意键停止监听并返回主菜单。");
        
        // 创建一个任务来监听键盘输入
        var keyTask = Task.Run(() => Console.ReadKey(true));
        
        // 轮询检查特征值变化
        byte[] lastValue = null;
        bool running = true;
        
        while (running)
        {
            if (keyTask.IsCompleted)
            {
                running = false;
                break;
            }
            
            try
            {
                // 读取当前值
                byte[] currentValue = await selectedCharacteristic.ReadValueAsync();
                
                // 检查值是否变化
                if (currentValue != null && !currentValue.SequenceEqual(lastValue ?? Array.Empty<byte>()))
                {
                    string hexValue = BitConverter.ToString(currentValue).Replace("-", " ");
                    Console.WriteLine($"接收到数据: {hexValue}");
                    
                    // 尝试将字节数据转换为字符串
                    try
                    {
                        string asciiValue = System.Text.Encoding.ASCII.GetString(currentValue);
                        Console.WriteLine($"ASCII 格式: {asciiValue}");
                    }
                    catch { }
                    
                    lastValue = currentValue;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"读取特征值时出错: {ex.Message}");
            }
            
            await Task.Delay(100);
        }
        
        // 停止通知
        try
        {
            if (cccdDescriptor != null)
            {
                // 写入0x0000来禁用通知
                await cccdDescriptor.WriteValueAsync(new byte[] { 0x00, 0x00 });
            }
            
            // 尝试调用StopNotificationsAsync
            try
            {
                await selectedCharacteristic.StopNotificationsAsync();
            }
            catch { }
            
            Console.WriteLine("通知已停止。");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"停止通知时出错: {ex.Message}");
        }
    }
    
    // 处理写入特征功能
    private static async Task HandleWriteFeature(List<(GattService service, GattCharacteristic characteristic)> writeCharacteristics)
    {
        if (writeCharacteristics.Count == 0)
        {
            Console.WriteLine("没有找到支持写入的特征。");
            return;
        }
        
        Console.WriteLine("\n支持写入的特征列表：");
        for (int i = 0; i < writeCharacteristics.Count; i++)
        {
            var (service, characteristic) = writeCharacteristics[i];
            Console.WriteLine($"{i + 1}. 服务: {service.Uuid}, 特征: {characteristic.Uuid}");
            Console.WriteLine($"   写入属性: {GetWriteProperties(characteristic.Properties)}");
        }
        
        Console.Write("请选择要写入的特征编号: ");
        string input = Console.ReadLine();
        if (!int.TryParse(input, out int charSelection) || charSelection < 1 || charSelection > writeCharacteristics.Count)
        {
            Console.WriteLine("无效的选择。");
            return;
        }
        
        var (selectedService, selectedCharacteristic) = writeCharacteristics[charSelection - 1];
        
        // 询问用户输入方式
        Console.WriteLine("\n请选择输入数据的格式：");
        Console.WriteLine("1. 十六进制字符串 (例如: 01 02 03 0A)");
        Console.WriteLine("2. ASCII字符串");
        Console.Write("请选择: ");
        
        string formatChoice = Console.ReadLine();
        byte[] dataToWrite = null;
        
        switch (formatChoice)
        {
            case "1":
                Console.Write("请输入十六进制数据 (空格分隔): ");
                string hexInput = Console.ReadLine();
                dataToWrite = HexStringToByteArray(hexInput);
                break;
            case "2":
                Console.Write("请输入ASCII字符串: ");
                string asciiInput = Console.ReadLine();
                dataToWrite = System.Text.Encoding.ASCII.GetBytes(asciiInput);
                break;
            default:
                Console.WriteLine("无效的选择。");
                return;
        }
        
        if (dataToWrite == null || dataToWrite.Length == 0)
        {
            Console.WriteLine("无效的数据。");
            return;
        }
        
        // 显示将要写入的数据
        Console.WriteLine($"\n将要写入的数据 (十六进制): {BitConverter.ToString(dataToWrite).Replace("-", " ")}");
        
        // 执行写入操作
        try
        {
            // 检查是否支持WriteWithoutResponse
            if (selectedCharacteristic.Properties.HasFlag(GattCharacteristicProperties.WriteWithoutResponse))
            {
                // 使用无响应写入（更快速）
                await selectedCharacteristic.WriteValueWithoutResponseAsync(dataToWrite);
                Console.WriteLine("使用无响应模式写入数据成功。");
            }
            else
            {
                // 使用普通写入（有确认）
                await selectedCharacteristic.WriteValueWithResponseAsync(dataToWrite);
                Console.WriteLine("写入数据成功。");
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"写入数据时出错: {ex.Message}");
        }
    }
    
    // 辅助方法：获取写入属性的描述
    private static string GetWriteProperties(GattCharacteristicProperties properties)
    {
        List<string> writeProps = new List<string>();
        
        if (properties.HasFlag(GattCharacteristicProperties.Write))
            writeProps.Add("Write");
        if (properties.HasFlag(GattCharacteristicProperties.WriteWithoutResponse))
            writeProps.Add("WriteWithoutResponse");
        
        return writeProps.Count > 0 ? string.Join(", ", writeProps) : "None";
    }
    
    // 辅助方法：将十六进制字符串转换为字节数组
    private static byte[] HexStringToByteArray(string hex)
    {
        try
        {
            // 移除空格并检查格式
            string cleanedHex = hex.Replace(" ", "");
            
            if (cleanedHex.Length % 2 != 0)
            {
                Console.WriteLine("十六进制字符串长度必须是偶数。");
                return null;
            }
            
            byte[] bytes = new byte[cleanedHex.Length / 2];
            for (int i = 0; i < bytes.Length; i++)
            {
                bytes[i] = Convert.ToByte(cleanedHex.Substring(i * 2, 2), 16);
            }
            return bytes;
        }
        catch (Exception ex)
        {
            Console.WriteLine($"解析十六进制字符串时出错: {ex.Message}");
            return null;
        }
    }
}