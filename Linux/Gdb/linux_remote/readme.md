

adb push hello /root && adb shell chmod +x /root/hello && adb shell "cd /root && gdbserver :1234 ./hello"

gdb-multiarch.exe -ex "target remote 192.168.123.100:1234" 
