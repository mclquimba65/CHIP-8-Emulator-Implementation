# CHIP-8 Emulator Implementation
This is an emulator for a Chip-8 Virtual Machine and is programmed in C. The emulator includes all opcodes for Chip-8 excluding 0NNN 
(Calling machine code which is not needed for most ROM files). ```chip8-implementation.c``` is the implementation for the Chip-8
emulator. The ```sdl-basecode-derivation.c``` contains sdl code in order to create and render and SDL window used for displaying
the Chip-8 Virtual Machine. This file is a modified version of an original basecode file worked on by Lawrence Sebald, a professor
from University of Maryland, Baltimore County. The file, ```chip8-implementation.c``` was solely worked on by
me. 
### Currently Implemented
1. chip8_init() is implemented and initializes the chip8 state, initializes the program counter, address register, stack pointer and the font.
2. chip8_reset() resets the emulator and initializes the address register, stack pointer, and all registers to 0. Stack is the program counter is cleared.
3. chip8_shutdown() is currently not being used.
4. chip8_execute_instruction()
   - Instruction fetching, decoding, and execution is performed in this function.
   - Helper functions are used for decoding.
   - Uses array of function pointers for the highest nibble of the opcode
   - Each opcode that has the same highest nibble, a static array of subfunctions differentiates each opcode.
### Compiling Instructions
- Msys2 needs to be installed in order to run the ```Makefile``` that builds and compiles the code.
- SDL, gcc, and make needs to be installed for this to run.
- In the Msys2 terminal, under the directory containing all files included in this repository, type ```make``` to build the program.
### References
- https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
- https://en.wikipedia.org/wiki/CHIP-8
- https://www.geeksforgeeks.org/c/function-pointer-in-c/ <br> <br>
```sdl-basecode-derivation.c``` is derived from an original basecode file worked on by Lawrence Sebald, a professor from University of Maryland, Baltimore County. Modifications
to this file include removing unneccessary features that is not needed for the purpose of the program.
