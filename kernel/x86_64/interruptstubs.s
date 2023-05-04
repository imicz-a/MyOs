.section text
.extern _ZN6Kernel16InterruptManager15handleInterruptEhm

.set IRQ_BASE, 0x20

.macro HandleExceptionRequest num
    .global _ZN6Kernel16InterruptManager26HandleException\num\()Ev
    _ZN6Kernel16InterruptManager26HandleException\num\()Ev:
        movb $\num, (interruptnumber)
        jmp int_bottom

.endm

.macro HandleInterruptRequest num
    .global _ZN6Kernel16InterruptManager26HandleInterrupt\num\()Ev
    _ZN6Kernel16InterruptManager26HandleInterrupt\num\()Ev:
        movb $\num + IRQ_BASE, (interruptnumber)
        jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

HandleExceptionRequest 0x00
HandleExceptionRequest 0x01
HandleExceptionRequest 0x02
HandleExceptionRequest 0x03
HandleExceptionRequest 0x04
HandleExceptionRequest 0x05
HandleExceptionRequest 0x06
HandleExceptionRequest 0x07
HandleExceptionRequest 0x08
HandleExceptionRequest 0x09
HandleExceptionRequest 0x0A
HandleExceptionRequest 0x0B
HandleExceptionRequest 0x0C
HandleExceptionRequest 0x0D
HandleExceptionRequest 0x0E
HandleExceptionRequest 0x0F
HandleExceptionRequest 0x10
HandleExceptionRequest 0x11
HandleExceptionRequest 0x12
HandleExceptionRequest 0x13
HandleExceptionRequest 0x14
HandleExceptionRequest 0x15
HandleExceptionRequest 0x1C
HandleExceptionRequest 0x1D
HandleExceptionRequest 0x1E
HandleExceptionRequest 0x1F

int_bottom:
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    call print_noterminal
    push (interruptnumber)
    call _ZN6Kernel16InterruptManager15handleInterruptEhm
    # addl $5, %esp
    movl %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

    
    .global _ZN16InterruptManager15InterruptIgnoreEv
    _ZN16InterruptManager15InterruptIgnoreEv:

    iret

.data
    interruptnumber: .byte 0
