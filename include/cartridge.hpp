#pragma once

#include <iostream>
#include <cstdint>
#include <fstream>
#include <vector>

class cartridge{
private:
    std::vector<char> cart_buffer;
public:
    cartridge(std::string path);
    uint8_t read(uint16_t address);
    size_t get_rom_size();
};