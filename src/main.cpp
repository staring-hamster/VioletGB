#include <iostream>
// #include <print>

#include "cartridge.hpp"
#include "memory_bus.hpp"
#include "cpu.hpp"

int main(){
    cartridge cart("../roms/tetris.gb");
    memory_bus bus(cart);
    cpu main_cpu(bus);

    int rom_size = cart.get_rom_size();

    std::cout << "Hello, Gameboy!\n";
    std::cout << "ROM size: " << std::to_string(rom_size);

    /*
    incase i want to print the rom's data
    for (size_t i = 0; i < rom_size; i++){
        
        if (i % 15 == 0){
            std::cout << std::endl;
        }

        uint8_t printed_byte = cart.read(i);
        if (printed_byte == 0) {std::cout << "00 ";}
        else {std::print("{:X} ", cart.read(i));}
    }
    */
    return 0;
}