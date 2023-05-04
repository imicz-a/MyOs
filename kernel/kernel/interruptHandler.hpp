#include <stdint.h>
#include "../x86_64/port.hpp"
#include "../x86_64/gdt.hpp"

namespace Kernel{
    class InterruptManager{
        public:
            InterruptManager(GDT* gdt);
            ~InterruptManager();
            struct GateDescriptor
            {
                uint16_t handlerAdressLowBits;
                uint16_t gdt_codeSegmentSelector;
                uint8_t reserved;
                uint8_t access;
                uint16_t handlerAdressHighBits;
            } __attribute__((packed));

            static GateDescriptor IDT[256];
            
            struct IDTpointer{
                uint16_t size;
                uint32_t base;
            }__attribute__((packed));
            static void SetIDT(
                uint8_t interruptNumber,
                uint16_t codeSegmentSelectorOffset,
                void (*handler)(),
                uint8_t descriptorPrivilegeLevel,
                uint8_t descriptorType
            );

            Port8BitSlow picMasterCommand;
            Port8BitSlow picMasterData;
            Port8BitSlow picSlaveCommand;
            Port8BitSlow picSlaveData;
            static Port8Bit nmiport;

            static void disableNMI();
            static void enableNMI();

            void Activate();
                static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);
                static void IgnoreInterruptRequest();

                static void HandleInterrupt0x00();
                static void HandleInterrupt0x01();

                static void HandleException0x00();
                static void HandleException0x01();
                static void HandleException0x02();
                static void HandleException0x03();
                static void HandleException0x04();
                static void HandleException0x05();
                static void HandleException0x06();
                static void HandleException0x07();
                static void HandleException0x08();
                static void HandleException0x09();
                static void HandleException0x0A();
                static void HandleException0x0B();
                static void HandleException0x0C();
                static void HandleException0x0D();
                static void HandleException0x0E();
                static void HandleException0x0F();
                static void HandleException0x10();
                static void HandleException0x11();
                static void HandleException0x12();
                static void HandleException0x13();
                static void HandleException0x14();
                static void HandleException0x15();
                static void HandleException0x1C();
                static void HandleException0x1D();
                static void HandleException0x1E();
                static void HandleException0x1F();
            static void exceptionEnd();
            
    };
}