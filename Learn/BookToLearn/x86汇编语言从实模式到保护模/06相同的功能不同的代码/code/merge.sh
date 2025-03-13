dd if=/dev/zero of=build/os.img count=1 bs=2M
dd if=build/boot.bin of=build/os.img conv=notrunc
dd if=build/os.bin of=build/os.img seek=512 bs=1 conv=notrunc
