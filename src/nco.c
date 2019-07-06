#include "nco.h"
#include "nco_lut.h"
#include <math.h>

const static int32_t lut[] = LUT_INIT;

void nco_update_freq(struct nco *self, float freq)
{
    self->phase_delta =
        (uint32_t)(freq * 2.0 * (1u << (sizeof(uint32_t) * 8 - 1)) / self->sample_rate);
}

void nco_init(struct nco *self, float freq)
{
    nco_update_freq(self, freq);
    self->phase_acc = 0;
}

int32_t nco_run(struct nco *self)
{
    uint16_t index;

    self->phase_acc += self->phase_delta;

    index = self->phase_acc >> ((sizeof(uint32_t) * 8) - LUT_SIZE_BASE);

    index &= (LUT_SIZE - 1);

    return lut[index];
}
