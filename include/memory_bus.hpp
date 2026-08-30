#pragma once

#include <iostream>
#include <cstdint>
#include <array>

/*
    THIS IS A PLACEHOLDER IMPLEMENTATION MADE FOR CPU TESTING,
    NOT A FINISHED, FULLY WORKING MEMORY MAP!
*/

class memory_bus{
private:
    std::array<uint8_t, 0xFFFF> memory_map;
    void init_memory();

public:
    memory_bus();
    
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
};


