#include <stdbool.h>
#include <stdint.h>

namespace Kernel{
    namespace Enable32Bit{
        void enable32BitMode();
        void enableA20Line();
        bool checkA20Line();
    }
}