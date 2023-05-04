#include "Enable32Bit.hpp"
#include "../kernel/terminal.hpp"

using namespace Kernel;

void Enable32Bit::enable32BitMode(){
    asm("cli");
}
bool Enable32Bit::checkA20Line(){
    uintptr_t p = 0x1007DFE;
    uint16_t value = *reinterpret_cast<uint16_t*>(p);
    p = 0x7DFE;
    uint16_t value1 = *reinterpret_cast<uint16_t*>(p);
    Terminal::print(" ");
    Terminal::print(value);
    Terminal::print(" ");
    Terminal::print(value);
    if(value1 != value){
        return true;
    }
    p = 0x1007DEE;
    value = *reinterpret_cast<uint16_t*>(p);
    p = 0x7DEE;
    value1 = *reinterpret_cast<uint16_t*>(p);
    if(value1 != value){
        return true;
    }
    Terminal::print("\n");
    return false;
}