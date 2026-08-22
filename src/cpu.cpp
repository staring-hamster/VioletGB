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

void cpu::op_LD_mem_val(uint16_t address, uint8_t value){
    return;
}

void cpu::op_LD_mem_val(uint16_t address, uint16_t value)
{
    return;
}

// Stores "value" into address "address"


void cpu::op_INC_reg(uint8_t &reg){
    uint8_t reg_value = get_reg(reg);
    uint8_t new_reg_value = reg_value + 1;
    bool half_carry = ((reg_value & 0b00001111) + 1) > 0b00001111;
    
    set_reg(reg, new_reg_value);
    set_flag(flags::zero, new_reg_value == 0);
    set_flag(flags::substraction, false);
    set_flag(flags::half_carry, half_carry);
}

void cpu::op_INC_reg(uint8_t &reg_high, uint8_t &reg_low){
    uint8_t reg_value = get_reg(reg_high, reg_low);
    uint8_t new_reg_value = reg_value + 1;
    
    set_reg(reg_high, reg_low, new_reg_value);
}

void cpu::op_INC_mem(uint8_t &address_high, uint8_t &address_low){
    return; // TODO: Replace with memory implementation!
}

void cpu::op_DEC_reg(uint8_t &reg){
    uint8_t reg_value = get_reg(reg);
    uint8_t new_reg_value = reg_value - 1;
    bool half_carry = (reg_value & 0b00001111) == 0;

    set_reg(reg, new_reg_value);
    set_flag(flags::zero, new_reg_value == 0);
    set_flag(flags::substraction, true);
    set_flag(flags::half_carry, half_carry);
}

void cpu::op_DEC_reg(uint8_t &reg_high, uint8_t &reg_low){
    uint8_t reg_value = get_reg(reg_high, reg_low);
    uint8_t new_reg_value = reg_value - 1;

    set_reg(reg_high, reg_low, new_reg_value);
}

void cpu::op_DEC_mem(uint8_t &address_high, uint8_t &address_low){
    return; // TODO: Replace with memory implementation!
}

void cpu::op_RLC_reg(uint8_t &reg)
{
    uint8_t reg_value = get_reg(reg);
    bool carry_bit = (reg_value & 0b10000000) == 0b10000000; // Isolates bit of interest and compares to its ON state.

    uint8_t new_reg_value = (reg_value << 1) | carry_bit;
    set_reg(reg, new_reg_value);
    set_flag(flags::zero, new_reg_value == 0);
    set_flag(flags::substraction, false);
    set_flag(flags::half_carry, false);
    set_flag(flags::carry, carry_bit);
}

void cpu::op_RLC_mem(uint16_t &address)
{
    return; // TODO: memory implementation...
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
    opcode_table[0x01] = {"LD BC,n16", 3, 3, [this](){op_LD_reg_val(b, c, decoded_word);}}; 
    opcode_table[0x02] = {"LD [BC],A", 1, 2, [this](){op_LD_mem_val(get_reg(b, c), a);}};
    opcode_table[0x03] = {"INC BC", 1, 2, [this](){op_INC_reg(b, c);}};
    opcode_table[0x04] = {"INC B", 1, 1, [this](){op_INC_reg(b);}};
    opcode_table[0x05] = {"DEC B", 1, 1, [this](){op_DEC_reg(b);}};
    opcode_table[0x06] = {"LD B,n8", 2, 2, [this](){op_LD_reg_val(b, decoded_byte);}};
    opcode_table[0x07] = {"RLCA", 1, 1, [this](){op_RLC_reg(a);}};
    opcode_table[0x08] = {"LD [a16],SP", 3, 5, [this](){op_LD_mem_val(decoded_word, sp);}}; // decoded_word would be [a16] in this case; Also need to improve implementation.

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