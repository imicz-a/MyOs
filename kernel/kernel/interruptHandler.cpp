#include "interruptHandler.hpp"
#include "terminal.hpp"

using namespace Kernel;

void InterruptManager::SetIDT(
    uint8_t interruptNumber,
    uint16_t codeSegmentSelectorOffset,
    void (*handler)(),
    uint8_t descriptorPrivilegeLevel,
    uint8_t descriptorType
){
    const uint8_t IDT_DESC_PRESENT = 0x80;

    IDT[interruptNumber].handlerAdressLowBits = ((uint32_t)handler & 0xFFFF);
    IDT[interruptNumber].handlerAdressHighBits = (((uint32_t)handler >> 16) & 0xFFFF);
    IDT[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
    IDT[interruptNumber].access = IDT_DESC_PRESENT | descriptorType | ((descriptorPrivilegeLevel&3)<<5);
    IDT[interruptNumber].reserved = 0;
}

InterruptManager::GateDescriptor InterruptManager::IDT[256];

void InterruptManager::enableNMI(){
    nmiport.Write(nmiport.Read() & 0x7F);
}
void InterruptManager::disableNMI(){
    nmiport.Write(nmiport.Read() | 0x80);
}

Port8Bit InterruptManager::nmiport(0x70);
InterruptManager::InterruptManager(GDT* gdt)
: picMasterCommand(0x20),
    picMasterData(0x21),
    picSlaveCommand(0xA0),
    picSlaveData(0xA1)
{
    uint32_t CodeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for(uint16_t i = 0; i < 256; i++)
        SetIDT(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);

    SetIDT(0x20, CodeSegment, &HandleInterrupt0x00, 0, IDT_INTERRUPT_GATE);
    SetIDT(0x21, CodeSegment, &HandleInterrupt0x01, 0, IDT_INTERRUPT_GATE);

    picMasterCommand.Write(0x11);
    picSlaveCommand.Write(0x11);

    picMasterData.Write(0x20);
    picSlaveData.Write(0x28);

    picMasterData.Write(0x04);
    picSlaveData.Write(0x02);

    picMasterData.Write(0x01);
    picSlaveData.Write(0x01);

    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);

    IDTpointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)IDT;
    asm volatile("lidt %0" : : "m" (idt));
}
InterruptManager::~InterruptManager(){}
uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp){
    Terminal::print("Interrupt\n");
    return esp;
}
void InterruptManager::HandleInterrupt0x00(){
    
}
void InterruptManager::HandleInterrupt0x01(){

}
void InterruptManager::Activate(){
    asm("sti");
}
void InterruptManager::IgnoreInterruptRequest(){}

void InterruptManager::exceptionEnd(){
    Terminal::print("Exception ocurred. Abort operation.\n");
    asm("cli");
    asm("hlt");
    while(true);
}
void InterruptManager::HandleException0x00(){
    Terminal::print("Division Error\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x01(){
    Terminal::print("Debug error\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x02(){
    Terminal::print("Non maskable interrupt\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x03(){
    Terminal::print("Breakpoint\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x04(){
    Terminal::print("Overflow\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x05(){
    Terminal::print("Bound range exceeded\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x06(){
    Terminal::print("Invalid opcode\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x07(){
    Terminal::print("Device not available\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x08(){
    Terminal::print("Double fault.\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x09(){
    Terminal::print("Overrun\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x0A(){
    Terminal::print("Invalid TSS\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x0B(){
    Terminal::print("Segment Not Present\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x0C(){
    Terminal::print("Stack Segment Fault\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x0D(){
    Terminal::print("General Protection Fault\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x0E(){
    Terminal::print("Page Fault\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x0F(){
    Terminal::print("Reserved, impossible to ocurr\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x10(){
    Terminal::print("x87 floating point exception\n");
    exceptionEnd();
}

void InterruptManager::HandleException0x11(){
    Terminal::print("Alignment check\n");
    exceptionEnd();
}

void InterruptManager::HandleException0x12(){
    Terminal::print("Machine check\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x13(){
    Terminal::print("SIMD floating point exception\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x14(){
    Terminal::print("Virtualisation Exception\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x15(){
    Terminal::print("Control Projection Exception\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x1C(){
    Terminal::print("Hypervisor Injection Exception\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x1D(){
    Terminal::print("VMM Communication Exception\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x1E(){
    Terminal::print("Security Exception\n");
    exceptionEnd();
}
void InterruptManager::HandleException0x1F(){
    Terminal::print("Impossible, reserved\n");
    exceptionEnd();
}
