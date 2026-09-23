#pragma once

#include <iostream>
#include <iomanip> // Debug, might remove.
#include <cstdint>
#include <functional>
#include <array>
#include <stdexcept>
#include <bit>

#include "memory_bus.hpp"

class cpu{
private:

    // -- CPU registers --

    uint8_t a = 0x00; // Accumulator
    uint8_t b = 0x00;
    uint8_t c = 0x00;
    uint8_t d = 0x00;
    uint8_t e = 0x00;
    uint8_t f = 0x00; // Flags
    uint8_t h = 0x00;
    uint8_t l = 0x00;
    uint16_t sp = 0x0000; // Stack Pointer
    uint16_t pc = 0x0150; // Program Counter, since we have no boot ROM start PC starts at the programs entry point address $0100. ($0150 cuz I have no JP instruction yet lol)


    // -- Special variables for decoding operands --

    uint8_t decoded_byte = 0x00; 
    uint16_t decoded_word = 0x0000;

    memory_bus mem;

    // -- Opcode tables --

    struct opcode
    {
        std::string name;
        uint8_t length;
        uint8_t cycles;
        std::function<void()> execute; 
    };
    
    std::array<opcode, 256> opcode_table;
    std::array<opcode, 256> cb_opcode_table;
    uint16_t last_instruction; // 16 bit int to support 0xCB instructions later down the line.


    // -- Flag bit "presets" --

    const uint8_t ZERO_FLAG          = 0b10000000;
    const uint8_t SUBTRACTION_FLAG   = 0b01000000;
    const uint8_t HALF_CARRY_FLAG    = 0b00100000;
    const uint8_t CARRY_FLAG         = 0b00010000;

    const uint8_t flag_table[4] = {
        ZERO_FLAG, 
        SUBTRACTION_FLAG,
        HALF_CARRY_FLAG,
        CARRY_FLAG
    };

    enum flags{
        zero,
        substraction,
        half_carry,
        carry
    };

    // -- 8 bit register functions --

    uint8_t get_reg(uint8_t& reg);
    void set_reg(uint8_t& reg, uint8_t value);

    //  -- 16 bit register functions --

    uint16_t get_reg(uint8_t& high, uint8_t& low);
    void set_reg(uint8_t& high, uint8_t& low, uint16_t value);

    //  -- Flag functions --

    // Returns current state of flag. (0 or 1)
    // Use the "flags" enum for flag selection.
    bool get_flag(flags flag);
    // Sets the flag bit to argument value.
    void set_flag(flags flag, bool value);


    //  -- CPU instructions --

    // - Load Instructions -

    // Stores "value" into register "reg"
    void op_LD_reg_val(uint8_t& reg, uint8_t value);
    void op_LD_reg_val(uint8_t& reg_high, uint8_t& reg_low, uint16_t value);

    // Stores "value" into address "address"
    void op_LD_mem_val(uint16_t address, uint8_t value);  // single byte write

    // - Arithmetic Instructions -

    // Increase destination by one
    void op_INC_reg(uint8_t& reg); // 8 bit register, flips flags
    void op_INC_reg(uint8_t& reg_high, uint8_t& reg_low); // 16 bit register, doesn't flip flags
    void op_INC_mem(uint16_t address);

    // Decrease destination by one
    void op_DEC_reg(uint8_t& reg); // 8 bit register, flips flags
    void op_DEC_reg(uint8_t& reg_high, uint8_t& reg_low); // 16 bit register, doesn't flip flags
    void op_DEC_mem(uint16_t address);

    // Add value to destination
    void op_ADD_reg(uint8_t& reg, uint8_t value); // 8 bit register
    void op_ADD_reg(uint8_t& reg_high, uint8_t& reg_low, uint16_t value); // 16 bit register
    // NOTE: This function apparently doesn't have a to add to a memory address, and uses the accumulator as 8 bit reg and HL register as 16 bit/.

    // - Bit shift Instructions -

    // Rotate value to the left, copying bit 7 (leftmost) to register C
    void op_RLC_reg(uint8_t& reg, bool always_zero);
    void op_RLC_mem(uint16_t address, bool always_zero);

    // Rotate value to the right, copying bit 0 (rightmost) to register C
    void op_RRC_reg(uint8_t& reg, bool always_zero);
    void op_RRC_mem(uint16_t address, bool always_zero);

    void op_RL_reg(uint8_t& reg, bool always_zero);

    // - Program control instructions -
    void op_JR(uint8_t steps, bool condition);

    // - Misc instructions -

    void op_STOP();
    void op_HALT();

    // Error-handling imaginary instruction.
    void op_NULL(); 

    void setup_opcode_tables();
    void dump_registers();
    void dump_instruction(const opcode &instruction);
    void dump_state(const opcode &instruction);

public:

    cpu(memory_bus &memory_map); // Creates a reference to the memory map for posterior acessing

    // -- CPU cycle emulation --

    // Fetch-Decode-Execute cycle.
    uint8_t cycles_remaining = 0;  
    void step();

    // "Emulates" one CPU cycle.
    // Useful for timing instruction execution. (Hey! That rhymes!)
    // Should be called by the main program loop.
    void tick();
    
    uint16_t get_last_instruction();
};