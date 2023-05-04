#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

namespace Kernel{
    namespace Terminal{ 
        void print(char* s);
        void printc(char c);
        void print(bool b);
        void printb(uint16_t i);
        void printb(uint32_t i);
        void init();
        void printh(uint32_t i);
        inline uint8_t get_color_byte();
        inline uint16_t get_entry();
        inline void putEntryAt();
        void scroll();
    };
}