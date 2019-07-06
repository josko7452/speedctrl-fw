//
// nco.h
//
// Note: Numerically controlled oscillator
//
// Copyright (c) 2019 Bruno KREMEL
#ifndef SRC_NCO_H
#define SRC_NCO_H

#include <stdint.h>

struct nco {
    const uint16_t sample_rate;
    uint32_t phase_delta;
    uint32_t phase_acc;
};

void    nco_update_freq(struct nco *self, float freq);
void    nco_init(struct nco *self, float freq);
int32_t nco_run(struct nco *self);

#endif // SRC_NCO_H
