#include <stdint.h>
#include "terminal.hpp"
#include "../x86_64/gdt.hpp"
#include "interruptHandler.hpp"
#include "../x86_64/Enable32Bit.hpp"

namespace Kernel{
    namespace Core{
        void Main();
    };
}