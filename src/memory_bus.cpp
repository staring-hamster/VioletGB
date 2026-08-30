#include "memory_bus.hpp"

void memory_bus::init_memory(){
    memory_map.fill(0x0000);
}

memory_bus::memory_bus(){
    init_memory();
}

uint8_t memory_bus::read(uint16_t address){
    return memory_map[address];
}

void memory_bus::write(uint16_t address, uint8_t value){
    memory_map[address] = value;
}
