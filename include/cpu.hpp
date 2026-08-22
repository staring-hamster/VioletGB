#pragma once

#include <iostream>
#include <cstdint>
#include <functional>

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
    uint16_t pc = 0x0000; // Program Counter

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

    public:

    cpu();

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

    // -- CPU instructions --

    // - Load Instructions -

    // Stores "value" into register "reg"
    void op_LD_reg_val(uint8_t& reg, uint8_t value);
    void op_LD_reg_val(uint8_t& reg_high, uint8_t& reg_low, uint16_t value);

    // Stores "value" into address "address"
    void op_LD_mem_val(uint16_t address, uint8_t value);  // 8bit
    void op_LD_mem_val(uint16_t address, uint16_t value); // 16bit

    // - Misc instructions -

    // Increases Program Counter by one.
    void op_NOP();

    void setup_opcode_tables();

    // -- CPU cycle emulation --

    uint8_t instruction_cycles = 0;

    // "Emulates" one CPU cycle.
    // Useful for timing instruction execution. (Hey! That rhymes!)
    // Should be called by the main program loop.
    void tick();

    // Executes an instruction when the CPU is done with all previous.
    void execute();

};