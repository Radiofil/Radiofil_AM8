// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ---- //
// carr //
// ---- //

#define carr_wrap_target 0
#define carr_wrap 1

static const uint16_t carr_program_instructions[] = {
            //     .wrap_target
    0xe001, //  0: set    pins, 1                    
    0xe000, //  1: set    pins, 0                    
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program carr_program = {
    .instructions = carr_program_instructions,
    .length = 2,
    .origin = -1,
};

static inline pio_sm_config carr_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + carr_wrap_target, offset + carr_wrap);
    return c;
}

// Helper function (for use in C program) to initialize this PIO program
void carr_program_init(PIO pio, uint sm, uint offset, uint pin, uint16_t divi, uint8_t frac) {
    // Sets up state machine and wrap target. This function is automatically
    // generated in carr.pio.h.
    pio_sm_config c = carr_program_get_default_config(offset);
    // Allow PIO to control GPIO pin (as output)
    pio_gpio_init(pio, pin);
    // Connect pin to SET pin (control with 'set' instruction)
    sm_config_set_set_pins(&c, pin, 1);
    // Set the pin direction to output (in PIO)
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    // Set the clock divider for the state machine
    sm_config_set_clkdiv_int_frac(&c, divi, frac);
    // Load configuration and jump to start of the program
    pio_sm_init(pio, sm, offset, &c);
}

#endif

