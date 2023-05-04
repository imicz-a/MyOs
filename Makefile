all: default qemu
objects = boot.o core.o gdt.o kernel.o terminal.o port.o interruptHandler.o interruptstubs.o Enable32Bit.o
default:
	nasm -f bin -o bootloader.bin kernel/x86_64/bootloader.asm
	nasm -f bin -o emptyspace.bin kernel/x86_64/emptyspace.asm
	nasm -f elf32 -o boot.o kernel/x86_64/boot.asm
	i686-elf-as kernel/x86_64/interruptstubs.s -o interruptstubs.o
	i686-elf-g++ -c kernel/kernel/*.cpp -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-use-cxa-atexit
	i686-elf-g++ -c kernel/x86_64/*.cpp -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-use-cxa-atexit
	i686-elf-gcc -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib $(objects) -lgcc
	mv *.o build
	cat bootloader.bin kernel.bin emptyspace.bin > myos.bin
qemu:
	qemu-system-x86_64 -drive format=raw,file="myos.bin",index=0,if=floppy -m 128M

qemukernel:
	qemu-system-x86_64 -kernel kernel.bin

.Phony: clean
clean:
	rm build/*.o