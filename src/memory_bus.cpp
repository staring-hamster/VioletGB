#include "memory_bus.hpp"

memory_bus::memory_bus(cartridge &cart) : cart(cart){
    ram.fill(0x00);
    vram.fill(0x00);
}

// Entering address should be offset to zero depending what zone is being accessed
uint8_t memory_bus::read(uint16_t address){
    if (address >= 0x0000 && address <= 0x7FFF)      { return cart.read(address); }
    else if (address >= 0x8000 && address <= 0x9FFF) { return vram.at(address - 0x9FFF); }
    else if (address >= 0xC000 && address <= 0xDFFF) { return ram.at(address - 0xC000); }
    else                                             { throw std::runtime_error("Attempted to read non-mapped memory!"); }
}

void memory_bus::write(uint16_t address, uint8_t value){
    if (address >= 0x0000 && address <= 0x7FFF)      { throw std::runtime_error("Attempted to write to ROM!"); }
    else if (address >= 0x8000 && address <= 0x9FFF) { vram.at(address - 0x9FFF) = value; }
    else if (address >= 0xC000 && address <= 0xDFFF) { ram.at(address - 0xC000) = value; }
    else                                             { throw std::runtime_error("Attempted to write non-mapped memory!"); }
}
