#pragma once

#include <common.h>

typedef struct {
    bool isPaused;
    bool isRunning;
    u64 ticks;
} emulator_context;

int emulator_run(int argc, char **argv);

emulator_context *emu_get_context();
