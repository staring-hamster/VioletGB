#include "cpu.hpp"

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
bool cpu::get_flag(Flags flag){
    return (f & flag) == flag;
}

// Sets the flag bit to argument value.
void cpu::set_flag(Flags flag, bool value){
    if (value) {
        f = f | flag; // Turns on chosen bit by using bit preset and OR bitwise operator.
    }
    else {
        f = f & (~flag); // Turns off chosen bit by flipping bit preset and using AND bitwise operator.
    }
}

// -- CPU instructions --

// - Load Instructions -

// Stores "value" into register "reg"
void cpu::op_LD_reg(uint8_t& reg, uint8_t value){
    set_reg(reg, value);
}

void cpu::op_LD_reg(uint8_t& reg_high, uint8_t& reg_low, uint16_t value){
    set_reg(reg_high, reg_low, value);
}

void cpu::op_LD_mem(uint16_t address, uint8_t value){
    mem.write(address, value);
}


// Stores "value" into address "address"
void cpu::op_INC_reg(uint8_t &reg){
    uint8_t reg_value = get_reg(reg);
    uint8_t result = reg_value + 1;
    bool half_carry = ((reg_value & 0x0F) + 1) > 0x0F;
    
    set_reg(reg, result);
    set_flag(Flags::ZERO, result == 0);
    set_flag(Flags::SUBSTRACTION, false);
    set_flag(Flags::HALF_CARRY, half_carry);
}

void cpu::op_INC_reg(uint8_t &reg_high, uint8_t &reg_low){
    uint16_t reg_value = get_reg(reg_high, reg_low);
    uint16_t result = reg_value + 1;
    
    set_reg(reg_high, reg_low, result);
}

void cpu::op_INC_mem(uint16_t address){
    uint8_t mem_value = mem.read(address);
    uint8_t result = mem_value + 1;
    bool half_carry = ((mem_value & 0x0F) + 1) > 0x0F;
    
    mem.write(address, result);
    set_flag(Flags::ZERO, result == 0);
    set_flag(Flags::SUBSTRACTION, false);
    set_flag(Flags::HALF_CARRY, half_carry);
}

void cpu::op_DEC_reg(uint8_t &reg){
    uint8_t reg_value = get_reg(reg);
    uint8_t result = reg_value - 1;
    bool half_carry = (reg_value & 0x0F) == 0;

    set_reg(reg, result);
    set_flag(Flags::ZERO, result == 0);
    set_flag(Flags::SUBSTRACTION, true);
    set_flag(Flags::HALF_CARRY, half_carry);
}

void cpu::op_DEC_reg(uint8_t &reg_high, uint8_t &reg_low){
    uint16_t reg_value = get_reg(reg_high, reg_low);
    uint16_t result = reg_value - 1;

    set_reg(reg_high, reg_low, result);
}

void cpu::op_DEC_mem(uint16_t address){
    uint8_t mem_value = mem.read(address);
    uint8_t result = mem_value - 1;
    bool half_carry = (mem_value & 0x0F) == 0;

    mem.write(address, result);
    set_flag(Flags::ZERO, result == 0);
    set_flag(Flags::SUBSTRACTION, true);
    set_flag(Flags::HALF_CARRY, half_carry);
}

void cpu::op_ADD_reg(uint8_t &reg, uint8_t value){
    uint8_t reg_value = get_reg(reg);
    uint16_t sum = reg_value + value;
    uint8_t result = static_cast<uint8_t>(sum); // Clamps to be uint_8, the right type.

    bool half_carry = ((reg_value & 0x0F) + (value & 0x0F)) > 0x0F;
    bool carry = sum > 0xFF;

    set_reg(reg, result);

    set_flag(Flags::ZERO, result == 0);
    set_flag(Flags::SUBSTRACTION, false);
    set_flag(Flags::HALF_CARRY, half_carry);
    set_flag(Flags::CARRY, carry);
}

void cpu::op_ADD_reg(uint8_t &reg_high, uint8_t &reg_low, uint16_t value){
    uint16_t reg_value = get_reg(reg_high, reg_low);
    uint32_t sum = reg_value + value;
    uint16_t result = static_cast<uint16_t>(sum); // Clamps to be uint_16, the right type.

    bool half_carry = ((reg_value & 0x0FFF) + (value & 0x0FFF)) > 0x0FFF;
    bool carry = sum > 0xFFFF;

    set_reg(reg_high, reg_low, result);

    set_flag(Flags::SUBSTRACTION, false);
    set_flag(Flags::HALF_CARRY, half_carry);
    set_flag(Flags::CARRY, carry);
}

void cpu::op_RLC_reg(uint8_t &reg, bool always_zero = false)
{
    uint8_t reg_value = get_reg(reg);
    bool carry_bit = (reg_value & 0b10000000) == 0b10000000; // Isolates bit of interest and compares to its ON state.
    uint8_t result = (reg_value << 1) | carry_bit;
    
    set_reg(reg, result);
    if (always_zero){
        set_flag(Flags::ZERO, false); // For RLCA instruction, ignore if result is zero or not and just turn flag off
    }
    else{
        set_flag(Flags::ZERO, (result == 0));
    }
    set_flag(Flags::SUBSTRACTION, false);
    set_flag(Flags::HALF_CARRY, false);
    set_flag(Flags::CARRY, carry_bit);
}

void cpu::op_RLC_mem(uint16_t address, bool always_zero = false)
{
    uint8_t mem_value = mem.read(address);
    bool carry_bit = (mem_value & 0b10000000) == 0b10000000; // Isolates bit of interest and compares to its ON state.
    uint8_t result = (mem_value << 1) | carry_bit;

    mem.write(address, result);
    if (always_zero) {
        set_flag(Flags::ZERO, false);
    }
    else {
        set_flag(Flags::ZERO, result == 0);
    }
    set_flag(Flags::SUBSTRACTION, false);
    set_flag(Flags::HALF_CARRY, false);
    set_flag(Flags::CARRY, carry_bit);
}

void cpu::op_RRC_reg(uint8_t &reg, bool always_zero = false){
    uint8_t reg_value = get_reg(reg);
    bool carry_bit = (reg_value & 0b00000001) == 0b00000001; // Isolates bit of interest and compares to its ON state.
    uint8_t result = (reg_value >> 1) | (carry_bit << 7);
    
    set_reg(reg, result);
    if (always_zero) {
        set_flag(Flags::ZERO, false);
    }
    else {
        set_flag(Flags::ZERO, result == 0);
    }
    set_flag(Flags::SUBSTRACTION, false);
    set_flag(Flags::HALF_CARRY, false);
    set_flag(Flags::CARRY, carry_bit);
}

void cpu::op_RRC_mem(uint16_t address, bool always_zero){
    uint8_t mem_value = mem.read(address);
    bool carry_bit = (mem_value & 0b00000001) == 0b00000001; // Isolates bit of interest and compares to its ON state.
    uint8_t result = (mem_value >> 1) | (carry_bit << 7);
    
    mem.write(address, result);
    if (always_zero) {
        set_flag(Flags::ZERO, false);
    }
    else {
        set_flag(Flags::ZERO, result == 0);
    }
    set_flag(Flags::SUBSTRACTION, false);
    set_flag(Flags::HALF_CARRY, false);
    set_flag(Flags::CARRY, carry_bit);
}

void cpu::op_RL_reg(uint8_t& reg, bool always_zero = false){
    uint8_t reg_value = get_reg(reg);
    bool carry_bit = get_flag(Flags::CARRY);

    uint8_t result = (reg_value << 1) | carry_bit; // Rotate reg_value one bit left and add C flag status at bit 0
    bool result_carry = (reg_value >> 7) == 0x01;
    
    set_reg(reg, result);

    if (always_zero) set_flag(Flags::ZERO, false);
    else set_flag(Flags::ZERO, (result == 0));

    set_flag(Flags::SUBSTRACTION, false);
    set_flag(Flags::HALF_CARRY, false);
    set_flag(Flags::CARRY, result_carry);
}

void cpu::op_RR_reg(uint8_t &reg, bool always_zero){
    uint8_t reg_value = get_reg(reg);
    bool carry_bit = get_flag(Flags::CARRY);

    uint8_t result = (reg_value >> 1) | (carry_bit << 7); // Rotate reg_value one bit right and add C flag status at bit 7
    bool result_carry = reg_value & 0x01;
    
    set_reg(reg, result);

    if (always_zero) set_flag(Flags::ZERO, false);
    else set_flag(Flags::ZERO, (result == 0));

    set_flag(Flags::SUBSTRACTION, false);
    set_flag(Flags::HALF_CARRY, false);
    set_flag(Flags::CARRY, result_carry);
}

void cpu::op_JR(uint8_t steps, bool condition){
    if (condition){
        int8_t reinterpreted_steps = std::bit_cast<int8_t>(steps);
        pc += reinterpreted_steps;
    }
}

void cpu::op_DAA() {
    uint8_t a_value = get_reg(a);
    uint8_t adjustment = 0;

    const bool n_flag = get_flag(Flags::SUBSTRACTION);
    const bool h_flag = get_flag(Flags::HALF_CARRY);
    bool c_flag = get_flag(Flags::CARRY);

    if (n_flag) {
        // After subtraction (SUB, SBC)
        if (h_flag) adjustment += 6;
        if (c_flag) adjustment += 60;
        a_value -= adjustment;
    } else {
        // After addition (ADD, ADC)
        if (h_flag || (a_value & 0x0F) > 0x09) adjustment += 6;
        if (c_flag || a_value > 0x99) {
            adjustment += 60;
            c_flag = true; // DAA sets Carry if addition overflowed BCD range
        }
        a_value += adjustment;
    }

    set_reg(a, a_value);
    set_flag(Flags::HALF_CARRY, false);
    set_flag(Flags::CARRY, c_flag);
    set_flag(Flags::ZERO, a_value == 0);
}

void cpu::op_CPL(){
    uint8_t a_value = get_reg(a);
    set_reg(a, ~a_value);
}

void cpu::op_STOP(){
    return; // I'm not implementing this shit right now.
}

void cpu::op_HALT(){
    return; // Nope.
}

void cpu::op_NULL(){
    dump_registers();
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

    opcode_table[0x00] = {"NOP", 1, 1, [](){}}; // No OPeration
    opcode_table[0x01] = {"LD BC,n16", 3, 3, [this](){op_LD_reg(b, c, decoded_word);}};  
    opcode_table[0x02] = {"LD [BC],A", 1, 2, [this](){op_LD_mem(get_reg(b, c), a);}};
    opcode_table[0x03] = {"INC BC", 1, 2, [this](){op_INC_reg(b, c);}};
    opcode_table[0x04] = {"INC B", 1, 1, [this](){op_INC_reg(b);}};
    opcode_table[0x05] = {"DEC B", 1, 1, [this](){op_DEC_reg(b);}};
    opcode_table[0x06] = {"LD B,n8", 2, 2, [this](){op_LD_reg(b, decoded_byte);}};
    opcode_table[0x07] = {"RLCA", 1, 1, [this](){op_RLC_reg(a, true);}};
    opcode_table[0x08] = {"LD [a16],SP", 3, 5, [this](){
        uint8_t low_byte = static_cast<uint8_t>(sp);
        uint8_t high_byte = static_cast<uint8_t>(sp >> 8);        
        op_LD_mem(decoded_word, low_byte);
        op_LD_mem(decoded_word + 1, high_byte);
    }};
    opcode_table[0x09] = {"ADD HL,BC", 1, 2, [this](){op_ADD_reg(h, l, get_reg(b, c));}};
    opcode_table[0x0A] = {"LD A,[BC]", 1, 2, [this](){op_LD_reg(a, mem.read(get_reg(b, c)));}};
    opcode_table[0x0B] = {"DEC BC", 1, 2, [this](){op_DEC_reg(b, c);}};
    opcode_table[0x0C] = {"INC C", 1, 1, [this](){op_INC_reg(c);}};
    opcode_table[0x0D] = {"DEC C", 1, 1, [this](){op_DEC_reg(c);}};
    opcode_table[0x0E] = {"LD C,n8", 2, 2, [this](){op_LD_reg(c, decoded_byte);}};
    opcode_table[0x0F] = {"RRCA", 1, 1, [this](){op_RRC_reg(a);}};

    opcode_table[0x10] = {"STOP", 2, 1, [this](){op_STOP();}};
    opcode_table[0x11] = {"LD DE,n16", 3, 3, [this](){op_LD_reg(d, e, decoded_word);}}; 
    opcode_table[0x12] = {"LD [DE],A", 1, 2, [this](){op_LD_mem(get_reg(d, e), a);}};
    opcode_table[0x13] = {"INC DE", 1, 2, [this](){op_INC_reg(d, e);}};
    opcode_table[0x14] = {"INC D", 1, 1, [this](){op_INC_reg(d);}};
    opcode_table[0x15] = {"DEC D", 1, 1, [this](){op_DEC_reg(d);}};
    opcode_table[0x16] = {"LD D,n8", 2, 2, [this](){op_LD_reg(d, decoded_byte);}};
    opcode_table[0x17] = {"RLA", 1, 1, [this](){op_RL_reg(a, true);}};
    opcode_table[0x18] = {"JR s8", 2, 3, [this](){op_JR(decoded_word, true);}};
    opcode_table[0x19] = {"ADD HL,DE", 1, 2, [this](){op_ADD_reg(h, l, get_reg(d, e));}};
    opcode_table[0x1A] = {"LD A,[DE]", 1, 2, [this](){op_LD_reg(a, mem.read(get_reg(d, e)));}};
    opcode_table[0x1B] = {"DEC DE", 1, 2, [this](){op_DEC_reg(d, e);}};
    opcode_table[0x1C] = {"INC E", 1, 1, [this](){op_INC_reg(e);}};
    opcode_table[0x1D] = {"DEC E", 1, 1, [this](){op_DEC_reg(e);}};
    opcode_table[0x1E] = {"LD E,n8", 2, 2, [this](){op_LD_reg(e, decoded_byte);}};
    opcode_table[0x1F] = {"RRA", 1, 1, [this](){op_RR_reg(a, true);}};

    opcode_table[0x20] = {"JR NZ,s8", 2, 3, [this](){op_JR(decoded_byte, !get_flag(Flags::ZERO));}};
    opcode_table[0x21] = {"LD HL,n16", 3, 3, [this](){op_LD_reg(h, l, decoded_word);}}; 
    opcode_table[0x22] = {"LD [HL+],A", 1, 2, [this](){
        uint16_t reg_value = get_reg(h,l); 
        op_LD_mem(reg_value, get_reg(a));
        set_reg(h,l,reg_value+1);
    }};
    opcode_table[0x23] = {"INC HL", 1, 2, [this](){op_INC_reg(h, l);}};
    opcode_table[0x24] = {"INC H", 1, 1, [this](){op_INC_reg(h);}};
    opcode_table[0x25] = {"DEC H", 1, 1, [this](){op_DEC_reg(h);}};
    opcode_table[0x26] = {"LD H,n8", 2, 2, [this](){op_LD_reg(h, decoded_byte);}};
    opcode_table[0x27] = {"DAA", 1, 1, [this](){op_DAA();}}; 
    opcode_table[0x28] = {"JR Z,s8", 2, 3, [this](){op_JR(decoded_byte, get_flag(Flags::ZERO));}};
    opcode_table[0x29] = {"ADD HL,HL", 1, 2, [this](){op_ADD_reg(h, l, get_reg(h, l));}};
    opcode_table[0x2A] = {"LD A,[HL+]", 1, 2, [this](){
        uint16_t reg_val = get_reg(h,l); 
        uint8_t mem_val = mem.read(reg_val); // Whatever is on HL's address
        set_reg(a, mem_val); 
        set_reg(h,l, reg_val + 1);}
    };    
    opcode_table[0x2B] = {"DEC HL", 1, 2, [this](){op_DEC_reg(h, l);}};
    opcode_table[0x2C] = {"INC L", 1, 1, [this](){op_INC_reg(l);}};
    opcode_table[0x2D] = {"DEC L", 1, 1, [this](){op_DEC_reg(l);}};
    opcode_table[0x2E] = {"LD L,n8", 2, 2, [this](){op_LD_reg(l, decoded_byte);}};
    opcode_table[0x2F] = {"CPL", 1, 1, [this](){op_CPL();}};

    opcode_table[0x30] = {"JR NC,s8", 2, 3, [this](){op_JR(decoded_byte, !get_flag(Flags::CARRY));}};
    opcode_table[0x31] = {"LD HL,n16", 3, 3, [this](){op_LD_reg(h, l, decoded_word);}}; 
    opcode_table[0x32] = {"LD [HL-],A", 1, 2, [this](){
        uint16_t help = get_reg(h,l);
        op_LD_mem(help, get_reg(a));
        set_reg(h,l,help-1);
    }};
    opcode_table[0x33] = {"INC SP", 1, 2, [this](){sp++;}};
    opcode_table[0x34] = {"INC [HL]", 1, 3, [this](){op_INC_mem(get_reg(h,l));}};
    opcode_table[0x35] = {"DEC [HL]", 1, 3, [this](){op_DEC_mem(get_reg(h,l));}};
    opcode_table[0x36] = {"LD [HL],n8", 2, 3, [this](){op_LD_mem(get_reg(h,l), decoded_byte);}};
    opcode_table[0x37] = {"SCF", 1, 1, [this](){set_flag(Flags::CARRY, true);}};
    opcode_table[0x38] = {"JR C,s8", 2, 3, [this](){op_JR(decoded_byte, get_flag(Flags::CARRY));}};
    opcode_table[0x39] = {"ADD HL,SP", 1, 2, [this](){op_ADD_reg(h, l, sp);}};
    opcode_table[0x3A] = {"LD A,[HL-]", 1, 2, [this](){
        uint16_t reg_val = get_reg(h,l);
        uint8_t mem_val = mem.read(reg_val);
        set_reg(a, mem_val);
        set_reg(h,l, reg_val-1);
    }}; 
    opcode_table[0x3B] = {"DEC SP", 1, 2, [this](){sp--;}};
    opcode_table[0x3C] = {"INC A", 1, 1, [this](){op_INC_reg(a);}};
    opcode_table[0x3D] = {"DEC A", 1, 1, [this](){op_DEC_reg(a);}};
    opcode_table[0x3E] = {"LD A,n8", 2, 2, [this](){op_LD_reg(a, decoded_byte);}};
    opcode_table[0x3F] = {"CCF", 1, 1, [this](){set_flag(Flags::CARRY, !get_flag(Flags::CARRY));}};

    opcode_table[0x40] = {"LD B,B", 1, 1, [this](){op_LD_reg(b, b);}};
    opcode_table[0x41] = {"LD B,C", 1, 1, [this](){op_LD_reg(b, c);}};
    opcode_table[0x42] = {"LD B,D", 1, 1, [this](){op_LD_reg(b, d);}};
    opcode_table[0x43] = {"LD B,E", 1, 1, [this](){op_LD_reg(b, e);}};
    opcode_table[0x44] = {"LD B,H", 1, 1, [this](){op_LD_reg(b, h);}};
    opcode_table[0x45] = {"LD B,L", 1, 1, [this](){op_LD_reg(b, l);}};
    opcode_table[0x46] = {"LD B,[HL]", 1, 2, [this](){op_LD_reg(b, mem.read(get_reg(h, l)));}};
    opcode_table[0x47] = {"LD B,A", 1, 1, [this](){op_LD_reg(b, a);}};
    opcode_table[0x48] = {"LD C,B", 1, 1, [this](){op_LD_reg(c, b);}};
    opcode_table[0x49] = {"LD C,C", 1, 1, [this](){op_LD_reg(c, c);}};
    opcode_table[0x4A] = {"LD C,D", 1, 1, [this](){op_LD_reg(c, d);}};
    opcode_table[0x4B] = {"LD C,E", 1, 1, [this](){op_LD_reg(c, e);}};
    opcode_table[0x4C] = {"LD C,H", 1, 1, [this](){op_LD_reg(c, h);}};
    opcode_table[0x4D] = {"LD C,L", 1, 1, [this](){op_LD_reg(c, l);}};
    opcode_table[0x4E] = {"LD C,[HL]", 1, 2, [this](){op_LD_reg(c, mem.read(get_reg(h,l)));}};
    opcode_table[0x4F] = {"LD C,A", 1, 1, [this](){op_LD_reg(c, a);}};

    opcode_table[0x50] = {"LD D,B", 1, 1, [this](){op_LD_reg(d, b);}};
    opcode_table[0x51] = {"LD D,C", 1, 1, [this](){op_LD_reg(d, c);}};
    opcode_table[0x52] = {"LD D,D", 1, 1, [this](){op_LD_reg(d, d);}};
    opcode_table[0x53] = {"LD D,E", 1, 1, [this](){op_LD_reg(d, e);}};
    opcode_table[0x54] = {"LD D,H", 1, 1, [this](){op_LD_reg(d, h);}};
    opcode_table[0x55] = {"LD D,L", 1, 1, [this](){op_LD_reg(d, l);}};
    opcode_table[0x56] = {"LD D,[HL]", 1, 2, [this](){op_LD_reg(d, mem.read(get_reg(h, l)));}};
    opcode_table[0x57] = {"LD D,A", 1, 1, [this](){op_LD_reg(d, a);}};
    opcode_table[0x58] = {"LD E,B", 1, 1, [this](){op_LD_reg(e, b);}};
    opcode_table[0x59] = {"LD E,C", 1, 1, [this](){op_LD_reg(e, c);}};
    opcode_table[0x5A] = {"LD E,D", 1, 1, [this](){op_LD_reg(e, d);}};
    opcode_table[0x5B] = {"LD E,E", 1, 1, [this](){op_LD_reg(e, e);}};
    opcode_table[0x5C] = {"LD E,H", 1, 1, [this](){op_LD_reg(e, h);}};
    opcode_table[0x5D] = {"LD E,L", 1, 1, [this](){op_LD_reg(e, l);}};
    opcode_table[0x5E] = {"LD E,[HL]", 1, 2, [this](){op_LD_reg(e, mem.read(get_reg(h,l)));}};
    opcode_table[0x5F] = {"LD E,A", 1, 1, [this](){op_LD_reg(e, a);}};

    opcode_table[0x60] = {"LD H,B", 1, 1, [this](){op_LD_reg(h, b);}};
    opcode_table[0x61] = {"LD H,C", 1, 1, [this](){op_LD_reg(h, c);}};
    opcode_table[0x62] = {"LD H,D", 1, 1, [this](){op_LD_reg(h, d);}};
    opcode_table[0x63] = {"LD H,E", 1, 1, [this](){op_LD_reg(h, e);}};
    opcode_table[0x64] = {"LD H,H", 1, 1, [this](){op_LD_reg(h, h);}};
    opcode_table[0x65] = {"LD H,L", 1, 1, [this](){op_LD_reg(h, l);}};
    opcode_table[0x66] = {"LD H,[HL]", 1, 2, [this](){op_LD_reg(h, mem.read(get_reg(h, l)));}};
    opcode_table[0x67] = {"LD H,A", 1, 1, [this](){op_LD_reg(h, a);}}; // 67
    opcode_table[0x68] = {"LD L,B", 1, 1, [this](){op_LD_reg(l, b);}};
    opcode_table[0x69] = {"LD L,C", 1, 1, [this](){op_LD_reg(l, c);}};
    opcode_table[0x6A] = {"LD L,D", 1, 1, [this](){op_LD_reg(l, d);}};
    opcode_table[0x6B] = {"LD L,E", 1, 1, [this](){op_LD_reg(l, e);}};
    opcode_table[0x6C] = {"LD L,H", 1, 1, [this](){op_LD_reg(l, h);}};
    opcode_table[0x6D] = {"LD L,L", 1, 1, [this](){op_LD_reg(l, l);}};
    opcode_table[0x6E] = {"LD L,[HL]", 1, 2, [this](){op_LD_reg(l, mem.read(get_reg(h,l)));}};
    opcode_table[0x6F] = {"LD L,A", 1, 1, [this](){op_LD_reg(l, a);}};

    opcode_table[0x70] = {"LD [HL],B", 1, 1, [this](){op_LD_mem(get_reg(h, l), b);}};
    opcode_table[0x71] = {"LD [HL],C", 1, 1, [this](){op_LD_mem(get_reg(h, l), c);}};
    opcode_table[0x72] = {"LD [HL],D", 1, 1, [this](){op_LD_mem(get_reg(h, l), d);}};
    opcode_table[0x73] = {"LD [HL],E", 1, 1, [this](){op_LD_mem(get_reg(h, l), e);}};
    opcode_table[0x74] = {"LD [HL],H", 1, 1, [this](){op_LD_mem(get_reg(h, l), h);}};
    opcode_table[0x75] = {"LD [HL],L", 1, 1, [this](){op_LD_mem(get_reg(h, l), l);}};
    opcode_table[0x76] = {"HALT", 1, 1, [this](){op_HALT();}};
    opcode_table[0x77] = {"LD [HL],A", 1, 1, [this](){op_LD_mem(get_reg(h, l), a);}};
    opcode_table[0x78] = {"LD A,B", 1, 1, [this](){op_LD_reg(a, b);}};
    opcode_table[0x79] = {"LD A,C", 1, 1, [this](){op_LD_reg(a, c);}};
    opcode_table[0x7A] = {"LD A,D", 1, 1, [this](){op_LD_reg(a, d);}};
    opcode_table[0x7B] = {"LD A,E", 1, 1, [this](){op_LD_reg(a, e);}};
    opcode_table[0x7C] = {"LD A,H", 1, 1, [this](){op_LD_reg(a, h);}};
    opcode_table[0x7D] = {"LD A,L", 1, 1, [this](){op_LD_reg(a, l);}};
    opcode_table[0x7E] = {"LD A,[HL]", 1, 2, [this](){op_LD_reg(a, mem.read(get_reg(h,l)));}};
    opcode_table[0x7F] = {"LD A,A", 1, 1, [this](){op_LD_reg(a, a);}};
}

void cpu::dump_registers(){
    std::cout << std::hex << std::uppercase << std::setfill('0')
              << "A: "  << std::setw(2) << +a  << " "
              << "B: "  << std::setw(2) << +b  << " "
              << "C: "  << std::setw(2) << +c  << " "
              << "D: "  << std::setw(2) << +d  << " "
              << "E: "  << std::setw(2) << +e  << " "
              << "F: "  << std::setw(2) << +f  << " "
              << "H: "  << std::setw(2) << +h  << " "
              << "L: "  << std::setw(2) << +l  << " "
              << "SP: " << std::setw(4) << sp << " "
              << "PC: " << std::setw(4) << pc
              << std::dec << std::setfill(' ') << std::endl; // reset sticky hex
}

void cpu::dump_instruction(const opcode& instruction){
    std::cout << "Instr: " << instruction.name;

    if (instruction.length == 2) {
        std::cout << std::hex << std::uppercase << std::setfill('0') << " OP: " << std::setw(2) << +decoded_byte;
    } else if (instruction.length == 3) {
        std::cout << std::hex << std::uppercase << std::setfill('0') << " OP: " << std::setw(4) << decoded_word;
    }
    std::cout << '\n';
}

void cpu::dump_state(const opcode& instruction) {
    dump_instruction(instruction);
    dump_registers();
}


cpu::cpu(memory_bus &memory_map) : mem(memory_map){
    setup_opcode_tables(); 
}

void cpu::step(){
    // Fetch
    uint8_t opcode_hex = mem.read(pc); 
    opcode instruction = opcode_table[opcode_hex];

    last_instruction = opcode_hex; // Debug, might remove.

    // Decode
    if (instruction.length == 2) {
        decoded_byte = mem.read(pc + 1);
    }
    else if (instruction.length == 3) {
        uint16_t new_word = mem.read(pc + 2); // Little endian, most significant byte is stored after, so store backwards
        new_word = new_word << 8;
        new_word = new_word | static_cast<uint16_t>(mem.read(pc + 1));
        decoded_word = new_word;
    }
    // Execute
    instruction.execute();
    dump_state(instruction);

    cycles_remaining = instruction.cycles - 1;
    pc += instruction.length;
}

// -- CPU cycle emulation --

// "Emulates" one CPU M-cycle.
// Useful for timing instruction execution. (Hey! That rhymes!)
// Should be called by the main program loop.
void cpu::tick(){
    if (cycles_remaining == 0){
        step();
    }
    else{
        cycles_remaining--;
    }
}

uint16_t cpu::get_last_instruction()
{
    return last_instruction;
}