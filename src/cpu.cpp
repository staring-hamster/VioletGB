#include "cpu.hpp"

cpu::cpu()
{
    // TODO: The constructor should have a way to take a reference to the memory array, then, be able to modify it.
}

// -- 8 bit register functions --

uint8_t cpu::get_reg(uint8_t& reg){
    return reg;
}

void cpu::set_reg(uint8_t& reg, uint8_t value){
    reg = value;
}

//  -- 16 bit register functions --

uint16_t cpu::get_reg(uint8_t& high, uint8_t& low){
    return (high << 8) | low;
}

void cpu::set_reg(uint8_t& high, uint8_t& low, uint16_t value){
    high = (value & 0xFF00) >> 8;
    low  = value & 0x00FF;
}

//  -- Flag functions --

// Returns current state of flag. (0 or 1)
// Use the "flags" enum for flag selection.
bool cpu::get_flag(flags flag){
    uint8_t flag_bit = flag_table[flag];
    bool flag_value;

    flag_value = (f & flag_bit) == flag_bit;
    return flag_value;
}

// Sets the flag bit to argument value.
void cpu::set_flag(flags flag, bool value){
    uint8_t flag_bit = flag_table[flag];
    if (value) {
        f = f | flag_bit; // Turns on chosen bit by using bit preset and OR bitwise operator.
    }
    else {
        f = f & (~flag_bit); // Turns off chosen bit by flipping bit preset and using AND bitwise operator.
    }
}

// -- CPU instructions --

// - Load Instructions -

// Stores "value" into register "reg"
void cpu::op_LD_reg_val(uint8_t& reg, uint8_t value){
    set_reg(reg, value);
}

void cpu::op_LD_reg_val(uint8_t& reg_high, uint8_t& reg_low, uint16_t value){
    set_reg(reg_high, reg_low, value);
}

// Stores "value" into address "address"
void cpu::op_LD_mem_val(uint16_t address, uint8_t value){ // 8bit
    return; // Should be replaced to reference to memory array when implemented.
}

void cpu::op_LD_mem_val(uint16_t address, uint16_t value){ // 16bit
    return; // Should be replaced to reference to memory array when implemented.
}

// - Misc instructions -

// Increases Program Counter by one.
void cpu::op_NOP(){
    pc++; // Instruction takes 4 Cycles
}

void cpu::setup_opcode_tables()
{
    for (int i = 0; i < 256; i++){
        opcode_table[i] = {"NULL", 0, 0, [](){}}; // Stop emulation somehow...
        cb_opcode_table[i] = {"NULL", 0, 0, [](){}}; // Same.
    }

    // opcode_table[0x00] = {"", 1, 1, [this](){}}; Template...
    opcode_table[0x00] = {"NOP", 1, 1, [this](){op_NOP();}};
    opcode_table[0x02] = {"LD [BC],A", 1, 1, [this](){op_LD_reg_val(b, c, a);}}; 
}

// -- CPU cycle emulation --

// "Emulates" one CPU cycle.
// Useful for timing instruction execution. (Hey! That rhymes!)
// Should be called by the main program loop.
void cpu::tick(){
    return;
}

// Executes an instruction when the CPU is done with all previous.
void cpu::execute(){
    return;
}