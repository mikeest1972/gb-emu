#include <stdio.h>
#include <emu.h>
#include <cart.h>
#include <cpu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/* 
  Emu components:

  |Cart|
  |CPU|
  |Address Bus|
  |PPU|
  |Timer|

*/

static emulator_context ctx;

emulator_context *emu_get_context() {
    return &ctx;
}

void delay(u32 ms) {
    SDL_Delay(ms);
}

int emulator_run(int argc, char **argv) {
    
    printf("Hello world from GB emulator\n");
    
    if(argc < 2)
    {
        // no rom file was passed
        printf("Usage: gbemulator <romFile>\n");
        return -1; // throws error
    }
    
    if (!cart_loader(argv[1]))
    {
        printf("Failed to load ROM title: %s\n",argv[1]);
        return -2;
    }
    return 0;
}
