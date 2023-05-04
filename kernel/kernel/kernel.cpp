#include "core.hpp"
#include "terminal.hpp"
typedef void (*constructor)();

extern "C" void kernel_startup(){
    Kernel::Core::Main();
}

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}
extern "C" void print_noterminal(){
    Kernel::Terminal::print("print noterminal\n");
}

