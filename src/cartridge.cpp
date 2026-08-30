#include "cartridge.hpp"

cartridge::cartridge(std::string path){
    std::ifstream rom(path, std::ios::binary);

    if (!rom.is_open()){
        throw std::runtime_error("Error accessing ROM file!");
    }

    rom.seekg(0, rom.end); // Set file pointer to the end
    size_t rom_file_size = rom.tellg(); // Get the current file pointer position 
    rom.seekg(0, std::ios::beg); // Set file pointer to beginning

    cart_buffer.resize(rom_file_size);

    rom.read(cart_buffer.data(), rom_file_size);
}

uint8_t cartridge::read(uint16_t address){
    return cart_buffer.at(address);
}

size_t cartridge::get_rom_size(){
    return cart_buffer.size();
}
