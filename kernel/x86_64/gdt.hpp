#ifndef __GDT_H
#define __GDT_H
#include <stdint.h>
    class GDT{
        public: 
            class SegmentDescriptor{
                private:
                    uint16_t limit_lo;
                    uint16_t base_lo;
                    uint8_t base_hi;
                    uint8_t type;
                    uint8_t flags_limit_li;
                    uint8_t base_vhi;
                public:
                    SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
                    uint32_t Base();
                    uint32_t Limit();
            } __attribute__((packed));
            class AccessRights{
                bool isvalid;
                bool privilegelow;
                bool privilegehigh;
                bool issystem;
                bool isexec;
                bool conformingbit;
                bool readwrite;
                
                public:
                    class DataSeg
                    {
                        public:
                            static AccessRights system;
                            static AccessRights user;
                    }; // namespace DataSeg
                    class ExecSeg
                    {
                        public:
                            static AccessRights system;
                            static AccessRights user;
                    }; // namespace ExecSeg
                    AccessRights(bool valid, uint8_t privilege, bool system, bool exec, bool rw);
                    uint8_t byteRepresentation;
                    static void init();
            }__attribute__((packed));
            SegmentDescriptor nullsegment;
            SegmentDescriptor unusedsegment;
            SegmentDescriptor codesegment;
            SegmentDescriptor datasegment;
            GDT();
            ~GDT();

            uint16_t CodeSegmentSelector();
            uint16_t DataSegmentSelector();
    };
#endif