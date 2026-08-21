#include <cstdint>

namespace cpu{
    const uint8_t ZERO_FLAG = 0b00010000;
    const uint8_t SUBSTRACTION_FLAG = 0b00001000;
    const uint8_t HALF_CARRY_FLAG = 0b00000100;
    const uint8_t CARRY_FLAG = 0b00000010;

    enum flags{
        zero,
        substraction,
        half_carry,
        carry
    };

    struct registers
    {
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
    };

    struct registers cpu_reg;

    //  -- 16 bit register functions --

    uint16_t get_register_BC(){
        uint16_t bc = (cpu_reg.a << 8) | cpu_reg.f;
        return bc; 
    }
    void set_register_BC(uint16_t value){
        cpu_reg.b = (value & 0xFF00);
        cpu_reg.c = (value & 0x00FF) >> 8;
    }

    uint16_t get_register_DE(){
        uint16_t de = (cpu_reg.d << 8) | cpu_reg.e;
        return de; 
    }
    void set_register_DE(uint16_t value){
        cpu_reg.d = (value & 0xFF00);
        cpu_reg.e = (value & 0x00FF) >> 8;
    }

    uint16_t get_register_HL(){
        uint16_t hl = (cpu_reg.h << 8) | cpu_reg.l;
        return hl; 
    }
    void set_register_HL(uint16_t value){
        cpu_reg.h = (value & 0xFF00);
        cpu_reg.l = (value & 0x00FF) >> 8;
    }

    //  -- Flag functions --

    // Decodes flag requested through enum.
    uint8_t _decode_flag(flags flag){
        uint8_t selected_flag;
        switch (flag){
            case zero:
                selected_flag = ZERO_FLAG;
                break;
            case substraction:
                selected_flag = SUBSTRACTION_FLAG;
                break;
            case half_carry:
                selected_flag = HALF_CARRY_FLAG;
                break;
            case carry:
                selected_flag = CARRY_FLAG;
                break;
        }
    }

    // Returns current state of flag. (0 or 1)
    // Use the "flags" enum for flag selection.
    bool get_flag(flags flag){
        uint8_t flag_bit = _decode_flag(flag);
        bool flag_value;

        flag_value = (cpu_reg.f & flag_bit) == flag_bit;
        return flag_value;
    }

    // Sets the flag bit to argument value.
    void set_flag(flags flag, bool value){
        uint8_t flag_bit = _decode_flag(flag);
        if (value) {
            cpu_reg.f = cpu_reg.f | flag_bit; // Turns on chosen bit by using bit preset and OR bitwise operator.
        }
        else {
            cpu_reg.f = cpu_reg.f & (~flag_bit); // Turns off chosen bit by flipping bit preset and using AND bitwise operator.
        }
    }

    // -- CPU instructions --

    // - Misc instructions -

    // Increases Program Counter by one.
    void op_NOP(){
        cpu_reg.pc++; 
    }


    // -- CPU cycle emulation --

    // "Emulates" one CPU cycle.
    // Useful for timing instruction execution. (Hey! That rhymes!)
    // Should be called by the main program loop.
    void tick(){
        return;
    }

    // Executes an instruction when the CPU is done with all previous.
    void execute(){
        return;
    }

}
