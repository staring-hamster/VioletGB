#include "cpu.hpp"

cpu::cpu(memory &memory_map){
    this->mem = memory_map; // Creates a reference to the memory map for posterior acessing
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

//  -- special register functions --

uint8_t cpu::get_reg(uint16_t& special_reg){
    return special_reg;
}

void cpu::set_reg(uint16_t& special_reg, uint16_t value){
    special_reg = value;
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
    mem.write(address, value);
}


// Stores "value" into address "address"


void cpu::op_INC_reg(uint8_t &reg){
    uint8_t reg_value = get_reg(reg);
    uint8_t result = reg_value + 1;
    bool half_carry = ((reg_value & 0x0F) + 1) > 0x0F;
    
    set_reg(reg, result);
    set_flag(flags::zero, result == 0);
    set_flag(flags::substraction, false);
    set_flag(flags::half_carry, half_carry);
}

void cpu::op_INC_reg(uint8_t &reg_high, uint8_t &reg_low){
    uint8_t reg_value = get_reg(reg_high, reg_low);
    uint8_t result = reg_value + 1;
    
    set_reg(reg_high, reg_low, result);
}

void cpu::op_INC_reg(uint16_t &special_register){
    uint8_t reg_value = get_reg(special_register);
    uint8_t result = reg_value + 1;
    
    set_reg(special_register, result);
}

void cpu::op_INC_mem(uint16_t address){
    uint8_t mem_value = mem.read(address);
    uint8_t result = mem_value + 1;
    bool half_carry = ((mem_value & 0x0F) + 1) > 0x0F;
    
    mem.write(address, result);
    set_flag(flags::zero, result == 0);
    set_flag(flags::substraction, false);
    set_flag(flags::half_carry, half_carry);
}

void cpu::op_DEC_reg(uint8_t &reg){
    uint8_t reg_value = get_reg(reg);
    uint8_t result = reg_value - 1;
    bool half_carry = (reg_value & 0x0F) == 0;

    set_reg(reg, result);
    set_flag(flags::zero, result == 0);
    set_flag(flags::substraction, true);
    set_flag(flags::half_carry, half_carry);
}

void cpu::op_DEC_reg(uint8_t &reg_high, uint8_t &reg_low){
    uint8_t reg_value = get_reg(reg_high, reg_low);
    uint8_t result = reg_value - 1;

    set_reg(reg_high, reg_low, result);
}

void cpu::op_DEC_reg(uint16_t &special_register){
    uint8_t reg_value = get_reg(special_register);
    uint8_t result = reg_value - 1;

    set_reg(special_register, result);
}

void cpu::op_DEC_mem(uint16_t address){
    uint8_t mem_value = mem.read(address);
    uint8_t result = address - 1;
    bool half_carry = (mem_value & 0x0F) == 0;

    mem.write(address, result);
    set_flag(flags::zero, result == 0);
    set_flag(flags::substraction, true);
    set_flag(flags::half_carry, half_carry);
}

void cpu::op_ADD_reg(uint8_t &reg, uint8_t value){
    uint8_t reg_value = get_reg(reg);
    uint16_t sum = reg_value + value;
    uint8_t result = static_cast<uint8_t>(sum); // Clamps to be uint_8, the right type.

    bool half_carry = ((reg_value & 0x0F) + (value & 0x0F)) > 0x0F;
    bool carry = sum > 0xFF;

    set_reg(reg, result);

    set_flag(flags::zero, result == 0);
    set_flag(flags::substraction, false);
    set_flag(flags::half_carry, half_carry);
    set_flag(flags::carry, carry);
}

void cpu::op_ADD_reg(uint8_t &reg_high, uint8_t &reg_low, uint16_t value){
    uint16_t reg_value = get_reg(reg_high, reg_low);
    uint32_t sum = reg_value + value;
    uint16_t result = static_cast<uint16_t>(sum); // Clamps to be uint_16, the right type.

    bool half_carry = (reg_value & 0x00FF) + (value & 0x00FF) > 0x00FF;
    bool carry = sum > 0xFFFF;

    set_reg(reg_high, reg_low, result);

    set_flag(flags::substraction, false);
    set_flag(flags::half_carry, half_carry);
    set_flag(flags::carry, carry);
}

void cpu::op_RLC_reg(uint8_t &reg, bool always_zero = false)
{
    uint8_t reg_value = get_reg(reg);
    bool carry_bit = (reg_value & 0b10000000) == 0b10000000; // Isolates bit of interest and compares to its ON state.
    uint8_t result = (reg_value << 1) | carry_bit;
    
    set_reg(reg, result);
    set_flag(flags::zero, (result == 0) || always_zero); // always_zero for RLCA instruction!
    set_flag(flags::substraction, false);
    set_flag(flags::half_carry, false);
    set_flag(flags::carry, carry_bit);
}

void cpu::op_RLC_mem(uint16_t address)
{
    uint8_t mem_value = mem.read(address);
    bool carry_bit = (mem_value & 0b10000000) == 0b10000000; // Isolates bit of interest and compares to its ON state.
    uint8_t result = (mem_value << 1) | carry_bit;

    mem.write(address, result);
    set_flag(flags::zero, result == 0);
    set_flag(flags::substraction, false);
    set_flag(flags::half_carry, false);
    set_flag(flags::carry, carry_bit);
}

void cpu::op_RRC_reg(uint8_t &reg){
    uint8_t reg_value = get_reg(reg);
    bool carry_bit = (reg_value & 0b00000001) == 0b00000001; // Isolates bit of interest and compares to its ON state.
    uint8_t result = (reg_value >> 1) | (carry_bit << 7);
    
    set_reg(reg, result);
    set_flag(flags::zero, result == 0);
    set_flag(flags::substraction, false);
    set_flag(flags::half_carry, false);
    set_flag(flags::carry, carry_bit);
}

void cpu::op_RRC_mem(uint16_t address){
    uint8_t mem_value = mem.read(address);
    bool carry_bit = (mem_value & 0b00000001) == 0b00000001; // Isolates bit of interest and compares to its ON state.
    uint8_t result = (mem_value >> 1) | (carry_bit << 7);
    
    mem.write(address, result);
    set_flag(flags::zero, result == 0);
    set_flag(flags::substraction, false);
    set_flag(flags::half_carry, false);
    set_flag(flags::carry, carry_bit);
}

void cpu::op_RL_reg(uint8_t& reg, bool always_zero = false){
    uint8_t reg_value = get_reg(reg);
    bool carry_bit = get_flag(flags::carry);

    uint8_t result = (reg_value << 1) | carry_bit; // Rotate reg_value one bit left and add C flag status at bit 0
    bool result_carry = (reg_value >> 7) == 0x01;
    
    set_reg(reg, result);
    set_flag(flags::zero, (result == 0) || always_zero); // always_zero for RLA instruction!
    set_flag(flags::substraction, false);
    set_flag(flags::half_carry, false);
    set_flag(flags::carry, result_carry);
}

// - Misc instructions -

// Increases Program Counter by one.
void cpu::op_NOP(){
    pc++;
}

void cpu::op_STOP(){
    return; // I'm not implementing this shit right now.
}

void cpu::op_NULL(){
    throw std::runtime_error("Attempted to run non-existent instruction!");
}

void cpu::setup_opcode_tables()
{
    for (int i = 0; i < 256; i++){
        opcode_table[i] = {"NULL", 0, 0, [this](){op_NULL();}}; 
        cb_opcode_table[i] = {"NULL", 0, 0, [this](){op_NULL();}}; 
    }

    // opcode_table[0x00] = {"", 1, 1, [this](){}}; Template...
    // decoded_byte is an 8bit value used as an operand for certain instructions
    // decoded_word is the same for 16bit.
    // Cycles are stored in M-Cycles which equals T-states / 4

    opcode_table[0x00] = {"NOP", 1, 1, [this](){op_NOP();}};
    opcode_table[0x01] = {"LD BC,n16", 3, 3, [this](){op_LD_reg_val(b, c, decoded_word);}};  
    opcode_table[0x02] = {"LD [BC],A", 1, 2, [this](){op_LD_mem_val(get_reg(b, c), a);}};
    opcode_table[0x03] = {"INC BC", 1, 2, [this](){op_INC_reg(b, c);}};
    opcode_table[0x04] = {"INC B", 1, 1, [this](){op_INC_reg(b);}};
    opcode_table[0x05] = {"DEC B", 1, 1, [this](){op_DEC_reg(b);}};
    opcode_table[0x06] = {"LD B,n8", 2, 2, [this](){op_LD_reg_val(b, decoded_byte);}};
    opcode_table[0x07] = {"RLCA", 1, 1, [this](){op_RLC_reg(a, true);}};
    opcode_table[0x08] = {"LD [a16],SP", 3, 5, [this](){op_LD_mem_val(decoded_word, sp);}};
    opcode_table[0x09] = {"ADD HL,BC", 1, 2, [this](){op_ADD_reg(h, l, get_reg(b, c));}};
    opcode_table[0x0A] = {"LD A,[BC]", 1, 2, [this](){op_LD_reg_val(a, get_reg(b, c));}};
    opcode_table[0x0B] = {"DEC BC", 1, 2, [this](){op_DEC_reg(b, c);}};
    opcode_table[0x0C] = {"INC C", 1, 1, [this](){op_INC_reg(c);}};
    opcode_table[0x0D] = {"DEC C", 1, 1, [this](){op_DEC_reg(c);}};
    opcode_table[0x0E] = {"LD C,n8", 2, 2, [this](){op_LD_reg_val(c, decoded_byte);}};
    opcode_table[0x0F] = {"RRCA", 1, 1, [this](){op_RRC_reg(a);}};

    opcode_table[0x1000] = {"STOP", 2, 1, [this](){op_STOP();}};
    opcode_table[0x11] = {"LD DE,n16", 3, 3, [this](){op_LD_reg_val(d, e, decoded_word);}}; 
    opcode_table[0x12] = {"LD [DE],A", 1, 2, [this](){op_LD_mem_val(get_reg(d, e), a);}};
    opcode_table[0x13] = {"INC DE", 1, 2, [this](){op_INC_reg(d, e);}};
    opcode_table[0x14] = {"INC D", 1, 1, [this](){op_INC_reg(d);}};
    opcode_table[0x15] = {"DEC D", 1, 1, [this](){op_DEC_reg(d);}};
    opcode_table[0x16] = {"LD D,n8", 2, 2, [this](){op_LD_reg_val(d, decoded_byte);}};
    opcode_table[0x17] = {"RLA", 1, 1, [this](){op_RL_reg(a, true);}};
    opcode_table[0x18] = {"JR s8", 2, 3, [this](){}}; // NEEDS IMPLEMENTATION!
    opcode_table[0x19] = {"ADD HL,DE", 1, 2, [this](){op_ADD_reg(h, l, get_reg(d, e));}};
    opcode_table[0x1A] = {"LD A,[DE]", 1, 2, [this](){op_LD_reg_val(a, get_reg(d, e));}};
    opcode_table[0x1B] = {"DEC DE", 1, 2, [this](){op_DEC_reg(d, e);}};
    opcode_table[0x1C] = {"INC E", 1, 1, [this](){op_INC_reg(e);}};
    opcode_table[0x1D] = {"DEC E", 1, 1, [this](){op_DEC_reg(e);}};
    opcode_table[0x1E] = {"LD E,n8", 2, 2, [this](){op_LD_reg_val(e, decoded_byte);}};
    opcode_table[0x1F] = {"RRA", 1, 1, [this](){}}; // NEEDS IMPLEMENTATION!!!

    opcode_table[0x20] = {"JR NZ,s8", 2, 3, [this](){}}; // PLEASE IMPLEMENT ME, BOY!!
    opcode_table[0x21] = {"LD HL,n16", 3, 3, [this](){op_LD_reg_val(h, l, decoded_word);}}; 
    opcode_table[0x22] = {"LD [HL+],A", 1, 2, [this](){uint8_t help = get_reg(h,l);op_LD_mem_val(help, a);set_reg(h,l,help+1);}}; // HOLY FUCKING SHIT FIX THIS IMPLEMENTATION FOR THE LOVE OF GOD
    opcode_table[0x23] = {"INC HL", 1, 2, [this](){op_INC_reg(h, l);}};
    opcode_table[0x24] = {"INC H", 1, 1, [this](){op_INC_reg(h);}};
    opcode_table[0x25] = {"DEC H", 1, 1, [this](){op_DEC_reg(h);}};
    opcode_table[0x26] = {"LD H,n8", 2, 2, [this](){op_LD_reg_val(h, decoded_byte);}};
    opcode_table[0x27] = {"DAA", 1, 1, [this](){}}; // i want someone to inflate me like a balloon (: PLEASE IMPLEMENT THIS!!
    opcode_table[0x28] = {"JR Z,s8", 2, 3, [this](){}}; // NEEDS IMPLEMENTATION!
    opcode_table[0x29] = {"ADD HL,HL", 1, 2, [this](){op_ADD_reg(h, l, get_reg(h, l));}};
    opcode_table[0x2A] = {"LD A,[HL+]", 1, 2, [this](){uint16_t reg_val = get_reg(h,l);uint8_t help = mem.read(reg_val);op_LD_mem_val(a, help);set_reg(h,l,reg_val+1);}}; // heyyy please fix me but only if you want to tho
    opcode_table[0x2B] = {"DEC HL", 1, 2, [this](){op_DEC_reg(h, l);}};
    opcode_table[0x2C] = {"INC L", 1, 1, [this](){op_INC_reg(l);}};
    opcode_table[0x2D] = {"DEC L", 1, 1, [this](){op_DEC_reg(l);}};
    opcode_table[0x2E] = {"LD L,n8", 2, 2, [this](){op_LD_reg_val(l, decoded_byte);}};
    opcode_table[0x2F] = {"CPL", 1, 1, [this](){}}; // do i even have to say it?

    opcode_table[0x30] = {"JR NC,s8", 2, 3, [this](){}}; // PLEASE IMPLEMENT
    opcode_table[0x31] = {"LD HL,n16", 3, 3, [this](){op_LD_reg_val(h, l, decoded_word);}}; 
    opcode_table[0x32] = {"LD [HL-],A", 1, 2, [this](){uint8_t help = get_reg(h,l);op_LD_mem_val(help, a);set_reg(h,l,help-1);}}; // HOLY FUCKING SHIT FIX THIS IMPLEMENTATION FOR THE LOVE OF GOD
    opcode_table[0x33] = {"INC SP", 1, 2, [this](){op_INC_reg(sp);}};
    opcode_table[0x34] = {"INC [HL]", 1, 3, [this](){op_INC_mem(get_reg(h,l));}};
    opcode_table[0x35] = {"DEC [HL]", 1, 3, [this](){op_DEC_mem(get_reg(h,l));}};
    opcode_table[0x36] = {"LD [HL],n8", 2, 3, [this](){op_LD_mem_val(get_reg(h,l), decoded_byte);}};
    opcode_table[0x37] = {"SCF", 1, 1, [this](){}}; // did i say balloon? more like, blimp
    opcode_table[0x38] = {"JR C,s8", 2, 3, [this](){}}; // NEEDS IMPLEMENTATION!
    opcode_table[0x39] = {"ADD HL,SP", 1, 2, [this](){op_ADD_reg(h, l, get_reg(sp));}};
    opcode_table[0x3A] = {"LD A,[HL-]", 1, 2, [this](){uint16_t reg_val = get_reg(h,l);uint8_t help = mem.read(reg_val);op_LD_mem_val(a, help);set_reg(h,l,reg_val-1);}}; // heyyy please fix me but only if you want to tho
    opcode_table[0x3B] = {"DEC SP", 1, 2, [this](){op_DEC_reg(sp);}};
    opcode_table[0x3C] = {"INC A", 1, 1, [this](){op_INC_reg(a);}};
    opcode_table[0x3D] = {"DEC A", 1, 1, [this](){op_DEC_reg(a);}};
    opcode_table[0x3E] = {"LD A,n8", 2, 2, [this](){op_LD_reg_val(a, decoded_byte);}};
    opcode_table[0x3F] = {"CCF", 1, 1, [this](){}}; // yup, this too

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