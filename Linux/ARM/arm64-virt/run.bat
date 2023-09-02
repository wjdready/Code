set QEMU=C:\ProgramFiles\qemu\qemu-system-aarch64.exe
set LFS=.

%QEMU%                          ^
    -M  virt                    ^
    -cpu cortex-a53             ^
    -smp 8,                     ^
    -m 8192M                    ^
    -kernel Image     ^
    -serial stdio               ^
    -device virtio-gpu-pci      ^
    -net nic -net user,hostfwd=tcp::10021-:22   ^
    -append "root=/dev/vda rw rootfstype=ext4 console=ttyAMA0 ignore_loglevel" ^
    -drive if=none,file=rootfs.img,id=hd0 ^
    -device virtio-blk-device,drive=hd0 ^
    -device usb-ehci -device usb-kbd -device usb-mouse

    @REM -nographic                  ^
