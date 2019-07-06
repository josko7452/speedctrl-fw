//
// ac_synth.h
//
// Note: Antiphase AC synthesiser for synchronous motor
//
// Copytright (c) 2019 Bruno Kremel
#ifndef SRC_AC_SYNTH_H
#define SRC_AC_SYNTH_H
#include "nco.h"

#include <stdint.h>

struct ac_synth {
    struct nco nco;
    uint16_t duty_pos; // Will be set by ac_synth_run
    uint16_t duty_neg; // Will be set by ac_synth_run
};

void ac_synth_init(struct ac_synth *self, float freq);
void ac_synth_run(struct ac_synth *self);
void ac_synth_update_freq(struct ac_synth *self, float freq);

#endif // SRC_AC_SYNTH_H
