#include "terminal.hpp"
namespace Kernel{
    namespace Terminal{
        static const int8_t width = 80;
        static const int8_t height = 25;

        uint8_t row;
        uint8_t column;
        uint8_t currentcolor;
        uint16_t* buffer;
        enum Color{
            black,
            blue,
            green,
            cyan,
            red,
            magenta,
            brown,
            light_grey,
            dark_grey,
            light_blue,
            light_green,
            light_cyan,
            light_red,
            light_magenta,
            light_brown,
            white
        };
        size_t strlen(char* str){
            size_t len = 0;
            while (str[len])
                len++;
            return len;
        }
        inline uint8_t get_color_byte(enum Color fg, enum Color bg){
            return fg | bg << 4;
        }
        inline uint16_t get_entry(unsigned char uc, uint8_t colornum){
            return (uint16_t) uc | (uint16_t) colornum << 8;;
        }
        inline uint16_t getCharAt(uint8_t x, uint8_t y){
            const size_t index = y * width + x;
            return buffer[index];
        }
        inline void putEntryAt(char c, uint8_t color, uint8_t x, uint8_t y){
            const size_t index = y * width + x;
	        buffer[index] = get_entry(c, color);
        }
        inline void putEntryAt(uint16_t c, uint8_t x, uint8_t y){
            const size_t index = y * width + x;
	        buffer[index] = c;
        }
        void scroll(){
            for(uint8_t x = 0; x < width; x++){
                for(uint8_t y = 1; y < height; y++){
                    putEntryAt(getCharAt(x, y), x, y-1);
                }            
            }
        }
        void printc(char c){
            if(c == '\n'){
                column = 0;
                if(++row == height){
                    scroll();
                }
                return;
            }
            putEntryAt(c, currentcolor, column, row);
            if(++column == width){
                column = 0;
                if(++row == height)
                    scroll();
            }
        }
        void print(char* str){
            size_t len = strlen(str);
            for(size_t i = 0; i < len; i++){
                printc(str[i]);
            }
        }

        void print(bool b){
            if(b){
                printc('t');
            }
            else{
                printc('f');
            }
        }
        void printb(uint16_t i){
            for(int8_t z = 15; z >=0; z--){
                if((i & (uint16_t)1<<z) == 1<<z){
                    printc('1');
                }else
                {
                    printc('0');
                }
            }
        }
        void printb(uint32_t i){
            for(int8_t z = 31; z >=0; z--){
                if((i & (uint32_t)1<<z) == 1<<z){
                    printc('1');
                }else
                {
                    printc('0');
                }
            }
        }
        
        void printh(uint32_t i){
            printc('0');
            printc('x');
            bool started = false;
            for(int8_t z = 28; z >= 0; z-=4){
                uint32_t bits4 = (i>>z)&0b1111;
                if(bits4 == 0){
                    if(started){
                        printc('0');
                    }
                }
                else if(bits4 < 10){
                    printc('0'+bits4);
                }
                else if(bits4 < 17){
                    printc('A' + bits4-10);
                }
            }
        }
        void init(){
            row = 0;
            column = 0;
            currentcolor = get_color_byte(white, black);
            buffer = (uint16_t*) 0xB8000;
            for (uint8_t y = 0; y < height; y++) {
                for (uint8_t x = 0; x < width; x++) {
                    putEntryAt(' ', currentcolor, x, y);
		        }
	        }
        }
    };
}
