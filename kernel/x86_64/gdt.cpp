#include "gdt.hpp"
GDT::GDT():
    nullsegment(0, 0, 0),
    unusedsegment(0, 0, 0),
    codesegment(0, 64*1024*1024, AccessRights::ExecSeg::system.byteRepresentation),
    datasegment(0, 64*1024*1024, AccessRights::DataSeg::system.byteRepresentation)
    {
        uint32_t i[2];
        i[0] = (uint32_t)this;
        i[1] = sizeof(GDT) << 16;

        asm volatile("lgdt (%0)": :"r" (((uint8_t *) i)+2));
    }
GDT::~GDT(){

}
uint16_t GDT::DataSegmentSelector(){
    return (uint8_t*)&datasegment - (uint8_t*)this;
}
uint16_t GDT::CodeSegmentSelector(){
    return (uint8_t*)&codesegment - (uint8_t*)this;
}
GDT::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags){
    uint8_t* target = (uint8_t*)this;
    if(limit <= 65536){
        target[6] = 0x40;
    }
    else{
        if((limit & 0xFFF)!= 0xFFF)
            limit = (limit>>12)-1;
        else
            limit = limit>>12;
        target[6] = 0xC0;
    }
    target[0] = limit & 0xFF;
    target[1] = (limit>>8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;

    target[2] = (base) & 0xFF;
    target[3] = (base>>8) & 0xFF;
    target[4] = (base>>16) & 0xFF;
    target[7] = (base>>24) & 0xFF;

    target[5] = flags;
}
uint32_t GDT::SegmentDescriptor::Base(){
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];
    return result;
}
uint32_t GDT::SegmentDescriptor::Limit(){
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[6] & 0xF;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];
    if((target[6] & 0xC0) == 0xC0)
        result = (result<<12) | 0xFFF;
    return result;
}
GDT::AccessRights::AccessRights(bool valid, uint8_t privilege, bool system, bool exec, bool rw){
    isvalid = valid;
    privilegelow = (privilege >> 0) & 1;
    privilegehigh = (privilege >> 1) & 1;
    issystem = system;
    isexec = exec;
    readwrite = rw;
    byteRepresentation = (uint8_t)isvalid << 7 &
    (uint8_t)privilegelow << 6 &
    (uint8_t)privilegehigh << 5 &
    (uint8_t)issystem<<4 &
    (uint8_t)isexec<<3 &
    (uint8_t)readwrite<<1;
}
GDT::AccessRights GDT::AccessRights::DataSeg::system(true, 0, true, false, true);
GDT::AccessRights GDT::AccessRights::ExecSeg::system(true, 0, true, false, true);