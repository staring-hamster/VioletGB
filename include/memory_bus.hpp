#pragma once

#include <iostream>
#include <cstdint>
#include <array>

#include "cartridge.hpp"

/*
    THIS IS A PLACEHOLDER IMPLEMENTATION MADE FOR CPU TESTING,
    NOT A FINISHED, FULLY WORKING MEMORY MAP!
*/

class memory_bus{
private:
    // 8kb arrays
    std::array<uint8_t, 0x2000> ram; 
    std::array<uint8_t, 0x2000> vram;
    cartridge cart;
public:
    memory_bus(cartridge &cart);
    
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
};


