#include "core.hpp"
using namespace Kernel;

extern "C" void* start_rodata;
void Core::Main(){
    Terminal::init();
    char array[23] = "OS booted succesfully\n";
    Terminal::print(array);
    GDT gdt;
    // Terminal::print("GDT created successfully\n");
    char* pnt = "This is a pointer";
    Terminal::print(pnt);
    Terminal::printc('\n');
    Terminal::printh(0X79F);
    Terminal::printc('\n');
    Terminal::printh((uint32_t)pnt);
    Terminal::printc('\n');
    char array1[] = "Read only data starts at ";
    Terminal::print(array1);
    Terminal::printh((uint32_t)start_rodata);
    //Terminal::print("Testing A20 line\n");
    //Terminal::print(Enable32Bit::checkA20Line());
    // Terminal::print("\n");
    // InterruptManager interruptManager(&gdt);
    // Terminal::print("IDT created successfully\n");
    // Terminal::print("testing software interrupt");

    // interruptManager.Activate();
    // Terminal::print("Interrupts now active!\n");
    while(true);
}
