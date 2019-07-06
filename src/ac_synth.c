#include "ac_synth.h"


void ac_synth_update_freq(struct ac_synth *self, float freq)
{
    nco_update_freq(&self->nco, freq);
}

void ac_synth_init(struct ac_synth *self, float freq)
{
    nco_init(&self->nco, freq);
}

void ac_synth_run(struct ac_synth *self)
{
    int32_t nco_value;
    nco_value = nco_run(&self->nco);
    if (nco_value > 0) {
        self->duty_pos = ((uint16_t)1600 - nco_value) /4;
        self->duty_neg = 1600;
    } else {
        self->duty_pos = 1600;
        self->duty_neg = ((uint16_t)1600 + nco_value) / 4;
    }
}
