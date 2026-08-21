#include <cstdint>

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

    cpu(){
        // TODO: The constructor should have a way to take a reference to the memory array, then, be able to modify it.
    }

    enum flags{
        zero,
        substraction,
        half_carry,
        carry
    };

    // -- 8 bit register functions --

    uint8_t get_reg(uint8_t& reg){ return reg; }
    void set_reg(uint8_t& reg, uint8_t value){ reg = value; }

    //  -- 16 bit register functions --

    uint16_t get_reg(uint8_t& high, uint8_t& low){
        return (high << 8) | low;
    }
    void set_reg(uint8_t& high, uint8_t& low, uint16_t value){
        high = (value & 0xFF00) >> 8;
        low  = value & 0x00FF;
    }

    //  -- Flag functions --

    // Returns current state of flag. (0 or 1)
    // Use the "flags" enum for flag selection.
    bool get_flag(flags flag){
        uint8_t flag_bit = flag_table[flag];
        bool flag_value;

        flag_value = (f & flag_bit) == flag_bit;
        return flag_value;
    }

    // Sets the flag bit to argument value.
    void set_flag(flags flag, bool value){
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
    void op_LD_reg_val(uint8_t& reg, uint8_t value){ 
        set_reg(reg, value);
    }
    void op_LD_reg_val(uint8_t& reg_high, uint8_t reg_low, uint16_t value){ 
        set_reg(reg_high, reg_low, value);
    }

    // Stores "value" into address "address"
    void op_LD_mem_val(uint8_t address, uint16_t value){ // 8bit
        return; // Should be replaced to reference to memory array when implemented.
    }
    void op_LD_mem_val(uint16_t address, uint16_t value){ // 16bit
        return; // Should be replaced to reference to memory array when implemented.
    }

    // - Misc instructions -

    // Increases Program Counter by one.
    void op_NOP(){
        pc++; // Instruction takes 4 Cycles
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

};
