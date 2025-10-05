#include <SDL/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/* You must implement these functions in your code! */
void chip8_init(void);
void chip8_reset(void);
void chip8_shutdown(void);
void chip8_execute_instruction(void);

/* These functions are available to you to use in your code. */
uint8_t chip8_mem_read(uint16_t addr);
void chip8_mem_write(uint16_t addr, uint8_t val);
uint8_t chip8_register_read(uint8_t reg);
void chip8_register_write(uint8_t reg, uint8_t val);
void chip8_clear_frame(void);
void chip8_mem_clear(void);
int chip8_draw_sprite(uint16_t addr, uint8_t x, uint8_t y, uint8_t height);
void chip8_mem_reset(void);

// Struct containing variables relating to the opcode. Used to make extracting the opcode easier.
typedef struct {
    uint16_t unmodified;
    uint8_t high_nibble;
    uint16_t NNN;
    uint8_t NN;
    uint8_t N;
    uint8_t X;
    uint8_t Y;
} CHIP8FULLOPCODE;

void decode_helper(CHIP8FULLOPCODE *opcode);
// Main opcode nibble (highest nibble)
void nibble_0(void);
void nibble_1(void);
void nibble_2(void);
void nibble_3(void);
void nibble_4(void);
void nibble_5(void);
void nibble_6(void);
void nibble_7(void);
void nibble_8(void);
void nibble_9(void);
void nibble_A(void);
void nibble_B(void);
void nibble_C(void);
void nibble_D(void);
void nibble_E(void);
void nibble_F(void);
// Subfunctions for the 0 nibble
void opcode0_E0(void);
void opcode0_EE(void);
// Subfunctions for 8 nibble
void opcode8_0(void);
void opcode8_1(void);
void opcode8_2(void);
void opcode8_3(void);
void opcode8_4(void);
void opcode8_5(void);
void opcode8_6(void);
void opcode8_7(void);
void opcode8_E(void);
// Subfunctions for the E nibble
void opcodeE_9E(void);
void opcodeE_A1(void);
// Subfunctions for the F nibble
void opcodeF_07(void);
void opcodeF_0A(void);
void opcodeF_15(void);
void opcodeF_18(void);
void opcodeF_1E(void);
void opcodeF_29(void);
void opcodeF_33(void);
void opcodeF_55(void);
void opcodeF_65(void);
void set_BCD(void);
// Currently using an array of function pointers to make the program more efficient. Need a pointer type.
typedef void (*OpcodeFunctionPtr)();

// Struct containing the stack, Vx Registers, Memory address I, a stack pointer, a program counter, and
// the opcode struct.
// Added: Array of function pointers with OpcodeFunctionPtr type. 
typedef struct {
    uint16_t stack[16];
    uint8_t V[16];
    uint16_t I;
    uint16_t *stack_pointer;
    uint16_t program_counter;
    OpcodeFunctionPtr opcode_ptr[16];
    CHIP8FULLOPCODE opcode;
} CHIP8OBJECT;
CHIP8OBJECT chip8;

void chip8_init(void) {
    chip8.program_counter = 0x200;
    chip8.I = 0x200;
    chip8.stack_pointer = chip8.stack;

    // Initializes the elements in the array to each function.
    chip8.opcode_ptr[0x0] = nibble_0;
    chip8.opcode_ptr[0x1] = nibble_1;
    chip8.opcode_ptr[0x2] = nibble_2;
    chip8.opcode_ptr[0x3] = nibble_3;
    chip8.opcode_ptr[0x4] = nibble_4;
    chip8.opcode_ptr[0x5] = nibble_5;
    chip8.opcode_ptr[0x6] = nibble_6;
    chip8.opcode_ptr[0x7] = nibble_7;
    chip8.opcode_ptr[0x8] = nibble_8;
    chip8.opcode_ptr[0x9] = nibble_9;
    chip8.opcode_ptr[0xA] = nibble_A;
    chip8.opcode_ptr[0xB] = nibble_B;
    chip8.opcode_ptr[0xC] = nibble_C;
    chip8.opcode_ptr[0xD] = nibble_D;
    chip8.opcode_ptr[0xE] = nibble_E;
    chip8.opcode_ptr[0xF] = nibble_F;
}

void chip8_reset(void) {
    chip8_mem_reset();
    chip8.program_counter = 0x200;
    chip8.I = 0x200;
    chip8.stack_pointer = chip8.stack;
    memset(chip8.V, 0, sizeof(chip8.V));
    memset(chip8.stack, 0, sizeof(chip8.stack));
}

// Currently not being used.
void chip8_shutdown(void) {
}

/**
 * This function first fetches an instruction from the memory indicated by the program counter and shifts
 * bits to address big-endian and immediately updates the program counter. The function calls the decode_helper
 * function to parse the opcode. Then, checks if the highest nibble of the opcode is between 0x0 and 0xF, if so,
 * then call a function through the array of function pointers based on the highest nibble. If there are multiple opcodes
 * with the same high nibble, it uses a static array of subfunction pointers to further differentiate which opcode
 * is desired.
 */
void chip8_execute_instruction(void) {
    // First fetch the instruction from the program counter
    chip8.opcode.unmodified = (chip8_mem_read(chip8.program_counter) << 8) | (chip8_mem_read(chip8.program_counter + 1));
    chip8.program_counter += 2;
    decode_helper(&chip8.opcode);
    if (chip8.opcode.high_nibble >= 0x0 && chip8.opcode.high_nibble <= 0xF) {
        chip8.opcode_ptr[chip8.opcode.high_nibble]();
    }
    else {
        printf("Out of range");
    }
}

// Highest Nibble: 0
void nibble_0(void) {
    static OpcodeFunctionPtr opcode0[16] = {[0x0] = opcode0_E0, [0xE] = opcode0_EE};
    if (chip8.opcode.N >= 0x0 && chip8.opcode.N <= 0xF) {
        opcode0[chip8.opcode.N]();
    }
    else {
        printf("Out of range");
    }
}
// 00E0
void opcode0_E0(void) {
    chip8_clear_frame();
}
// 00EE
void opcode0_EE(void) {
    chip8.stack_pointer--;
    chip8.program_counter = *chip8.stack_pointer;
}

// Highest Nibble: 1
void nibble_1(void) {
    chip8.program_counter = chip8.opcode.NNN;
}

// Highest Nibble: 2
void nibble_2(void) {
    *chip8.stack_pointer = chip8.program_counter;
    chip8.program_counter = chip8.opcode.NNN;
    chip8.stack_pointer++;
}

// Highest Nibble: 3
void nibble_3(void) {
    if (chip8.V[chip8.opcode.X] == chip8.opcode.NN) {
        chip8.program_counter += 2;
    }
}

// Highest Nibble: 4
void nibble_4(void) {
    if (chip8.V[chip8.opcode.X] != chip8.opcode.NN) {
        chip8.program_counter += 2;
    }
}

// Highest Nibble: 5
void nibble_5(void) {
    if (chip8.V[chip8.opcode.X] == chip8.V[chip8.opcode.Y]) {
        chip8.program_counter += 2;
    }
}

// Highest Nibble: 6
void nibble_6(void) {
    chip8.V[chip8.opcode.X] = chip8.opcode.NN;
}

// Highest Nibble: 7
void nibble_7(void) {
    chip8.V[chip8.opcode.X] += chip8.opcode.NN;
}

// Highest Nibble: 8
void nibble_8(void) {
    static OpcodeFunctionPtr opcode8[16] = {[0x0] = opcode8_0, [0x1] = opcode8_1, [0x2] = opcode8_2,
    [0x3] = opcode8_3, [0x4] = opcode8_4, [0x5] = opcode8_5, [0x6] = opcode8_6, [0x7] = opcode8_7,
    [0xE] = opcode8_E};
    if (chip8.opcode.N >= 0x0 && chip8.opcode.N <= 0xF) {
        opcode8[chip8.opcode.N]();
    }
    else {
        printf("Out of range");
    }
}
// 8XY0
void opcode8_0(void) {
    chip8.V[chip8.opcode.X] = chip8.V[chip8.opcode.Y];
}
// 8XY1
void opcode8_1(void) {
    chip8.V[chip8.opcode.X] |= chip8.V[chip8.opcode.Y];
}
// 8XY2
void opcode8_2(void) {
    chip8.V[chip8.opcode.X] &= chip8.V[chip8.opcode.Y];
}
// 8XY3
void opcode8_3(void) {
    chip8.V[chip8.opcode.X] ^= chip8.V[chip8.opcode.Y];
}
// 8XY4
void opcode8_4(void) {
    if (chip8.V[chip8.opcode.X] + chip8.V[chip8.opcode.Y] > 0xFF) {
        chip8.V[0xF] = 1;
    }
    else {
        chip8.V[0xF] = 0;
    }
    chip8.V[chip8.opcode.X] += chip8.V[chip8.opcode.Y];
}
// 8XY5
void opcode8_5(void) {
    if (chip8.V[chip8.opcode.X] >= chip8.V[chip8.opcode.Y]) {
        chip8.V[0xF] = 1;
    }
    else {
        chip8.V[0xF] = 0;
    }
    chip8.V[chip8.opcode.X] -= chip8.V[chip8.opcode.Y];
}
// 8XY6
void opcode8_6(void) {
    chip8.V[0xF] = chip8.V[chip8.opcode.X] & 0x1;
    chip8.V[chip8.opcode.X] >>= 1;
}
// 8XY7
void opcode8_7(void) {
    if (chip8.V[chip8.opcode.Y] >= chip8.V[chip8.opcode.X]) {
        chip8.V[0xF] = 1;
    }
    else {
        chip8.V[0xF] = 0;
    }
    chip8.V[chip8.opcode.X] = chip8.V[chip8.opcode.Y] - chip8.V[chip8.opcode.X];
}
// 8XYE
void opcode8_E(void) {
    if ((chip8.V[chip8.opcode.X] & 0x80) == 0x80) {
        chip8.V[0xF] = 1;
    }
    else {
        chip8.V[0xF] = 0;
    }
    chip8.V[chip8.opcode.X] <<= 1;
}

// Highest Nibble: 9
void nibble_9(void) {
    if (chip8.V[chip8.opcode.X] != chip8.V[chip8.opcode.Y]) {
        chip8.program_counter += 2;
    }
}

// Highest Nibble: A
void nibble_A(void) {
    chip8.I = chip8.opcode.NNN;
}

// Highest Nibble: B
void nibble_B(void) {
    chip8.program_counter = chip8.opcode.NNN + chip8.V[0x0];
}

// Highest Nibble: C
void nibble_C(void) {
    chip8.V[chip8.opcode.X] = (rand() % 256) & chip8.opcode.NN;
}

// Highest Nibble: D
void nibble_D(void) {
    int collision = chip8_draw_sprite(chip8.I, chip8.V[chip8.opcode.X], chip8.V[chip8.opcode.Y], chip8.opcode.N);
    chip8.V[0xF] = collision;
}

// Highest Nibble: E
void nibble_E(void) {
    static OpcodeFunctionPtr opcodeE[16] = {[0x1] = opcodeE_A1, [0xE] = opcodeE_9E};
    if (chip8.opcode.N >= 0x0 && chip8.opcode.N <= 0xF) {
        opcodeE[chip8.opcode.N]();
    }
    else {
        printf("Out of range");
    }
}
// EXA1
void opcodeE_A1(void) {
    if (!chip8_register_read(chip8.V[chip8.opcode.X] & 0xF)) {
        chip8.program_counter += 2;
    }
}
// EX9E
void opcodeE_9E(void) {
    if (chip8_register_read(chip8.V[chip8.opcode.X] & 0xF)) {
        chip8.program_counter += 2;
    }
}

// Highest Nibble: F
void nibble_F(void) {
    static OpcodeFunctionPtr opcodeF[256] = {[0x7] = opcodeF_07, [0xA] = opcodeF_0A, [0x15] = opcodeF_15,
    [0x18] = opcodeF_18, [0x1E] = opcodeF_1E, [0x29] = opcodeF_29, [0x33] = opcodeF_33, [0x55] = opcodeF_55,
    [0x65] = opcodeF_65};
    if (chip8.opcode.NN >= 0x0 && chip8.opcode.NN <= 0xFF) {
        opcodeF[chip8.opcode.NN]();
    }
    else {
        printf("Out of range");
    }
}
// FX07
void opcodeF_07(void) {
    chip8.V[chip8.opcode.X] = chip8_register_read(0x10);
}
// FX0A
void opcodeF_0A(void) {
    chip8.program_counter -= 2;
    for (uint8_t i = 0x0; i < 0x10; i++) {
        if (chip8_register_read(i)) {
            chip8.V[chip8.opcode.X] = chip8_register_read(i);
            chip8.program_counter += 2;
            return;
        }
    }
}
// FX15
void opcodeF_15(void) {
    chip8_register_write(0x10, chip8.V[chip8.opcode.X]);
}
// FX18
void opcodeF_18(void) {
    chip8_register_write(0x11, chip8.V[chip8.opcode.X]);
}
// FX1E
void opcodeF_1E(void) {
    chip8.I += (uint16_t)chip8.V[chip8.opcode.X];
}
// FX29
void opcodeF_29(void) {
    chip8.I = 5 * (chip8.V[chip8.opcode.X] & 0xF);
}
// FX33
void opcodeF_33(void) {
    set_BCD();
}
// This helper function parses the data in V[x] register and writes it to memory based on the hundreds, tens, and ones digits for opcode FX33.
void set_BCD(void) {
    chip8_mem_write(chip8.I, chip8.V[chip8.opcode.X] / 100);
    chip8_mem_write(chip8.I + 1, (chip8.V[chip8.opcode.X] / 10) % 10);
    chip8_mem_write(chip8.I + 2, chip8.V[chip8.opcode.X] % 10);
}
// FX55
void opcodeF_55(void) {
    for (uint8_t i = 0x0; i <= chip8.opcode.X; i++) {
        chip8_mem_write(chip8.I + i, chip8.V[i]);
    }
}
// FX65
void opcodeF_65(void) {
    for (uint8_t i = 0x0; i <= chip8.opcode.X; i++) {
        chip8.V[i] = chip8_mem_read(chip8.I + i);
    }
}


// This function parses the opcode and bit masks them for simplicity.
void decode_helper(CHIP8FULLOPCODE *opcode) {
    opcode->high_nibble = (opcode->unmodified >> 12) & 0xF;
    opcode->NNN = opcode->unmodified & 0xFFF;
    opcode->NN = opcode->unmodified & 0xFF;
    opcode->N = opcode->unmodified & 0xF;
    opcode->X = (opcode->unmodified >> 8) & 0xF;
    opcode->Y = (opcode->unmodified >> 4) & 0xF;
}