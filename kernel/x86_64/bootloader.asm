[org 0x7c00]
bits 16
KERNEL_LOCATION equ 0x7e00

mov [BOOT_DISK], dl  

xor ax, ax                          
mov es, ax
mov ds, ax
mov bp, 0x8000
mov sp, bp

mov ah, 0x0
mov al, 0x3
int 0x10

mov bx, KERNEL_LOCATION
mov dh, 63

mov ah, 2
mov al, dh
mov ch, 0x00
mov dh, 0x00
mov cl, 0x02
mov dl, [BOOT_DISK]
int 0x13

cbw
mov bx, ax
push ax
call print_dec
call print_newline

mov bx, KERNEL_LOCATION+(63*512)
mov dh, 2

mov ah, 2
mov al, 63
mov ch, 0x00
mov dh, 0x00
mov cl, 65
mov dl, [BOOT_DISK]
int 0x13

cbw
mov bx, ax
push ax
call print_dec
call print_newline

jc diskfail

pop cx
pop ax
add cx, ax
cmp cx, 126
jne diskfailnotenough

CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start

cli
lgdt [GDT_descriptor]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp CODE_SEG:start_protected_mode

jmp $



diskfail:
    mov bx, FATAL_ERR
    call print_string
    jmp $
diskfailnotenough:
    mov bx, DISK_ERR_NOT_ENOUGH
    call print_string
    cbw
    mov bx, ax
    call print_dec
    call print_newline
    mov bx, INFO
    call print_string
    call print_newline
    mov bx, BOOT_DISK_STR
    call print_string
    mov bx, BOOT_DISK
    call print_digit
    jmp $

print_string:
	mov ah, 0x0E
	.loop:
		mov al, [bx]
		cmp al, 0x00
		je .end
		int 0x10
		inc bx
		jmp .loop
	.end:
		ret
    
FATAL_ERR: db "Fatal disk error ocurred! Error code: ", 0
DISK_ERR_NOT_ENOUGH: db "Not enough sectors red, only ", 0
INFO: db "Info ", 0
BOOT_DISK_STR: db "Boot disk number: ", 0
BOOT_DISK: db 0

print_dec:
	mov dx, 0x00
	mov ax, bx
	mov bx, 10000
	div bx

	mov ah, 0x00
	call print_digit
	mov bx, dx
	
	mov dx, 0x00
	mov ax, bx
	mov bx, 1000
	div bx
	
	mov ah, 0x00
	call print_digit
	mov bx, dx
	
	mov dx, 0x00
	mov ax, bx
	mov bx, 100
	div bx

	mov ah, 0x00
	call print_digit
	mov bx, dx

	mov dx, 0x00
	mov ax, bx
	mov bx, 10
	div bx
	mov ah, 0x00
	call print_digit
	
	mov ax, dx
	mov ah, 0x00
	call print_digit
	ret
	
print_digit:
	mov ah, 0x0E
	add al, 48
	int 0x10
	ret

print_newline:
    mov ah, 0x0e
    mov al, 0xa
    int 0x10
    mov al, 0xd
    int 0x10
    ret
GDT_start:
    GDT_null:
        dd 0x0
        dd 0x0

    GDT_code:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10011010
        db 0b11001111
        db 0x0

    GDT_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

GDT_end:
GDT_descriptor:
    dw GDT_end - GDT_start - 1
    dd GDT_start


[bits 32]
start_protected_mode:
    mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000		; 32 bit stack base pointer
	mov esp, ebp

    jmp KERNEL_LOCATION
times 510-($-$$) db 0
db 0x55, 0xaa

