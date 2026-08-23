#include "memory.hpp"

void memory::init_memory(){
    memory_map.fill(0x0000);
}

memory::memory(){
    init_memory();
}

uint8_t memory::read(uint16_t address){
    return memory_map[address];
}

void memory::write(uint16_t address, uint8_t value){
    memory_map[address] = value;
}
