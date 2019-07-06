#include "ac_synth.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    struct ac_synth ac_synth = {
        .nco = {
            .sample_rate   = 10000,
        },
    };

    ac_synth_init(&ac_synth, 50.00f);

    for (int i = 0; i < 10000 / 50; ++i) {
        ac_synth_run(&ac_synth);
        printf("%d, %d\n", ac_synth.duty_pos, ac_synth.duty_neg);
    }
    ac_synth_update_freq(&ac_synth, 100.0f);
    for (int i = 0; i < 10000 / 50; ++i) {
        ac_synth_run(&ac_synth);
        printf("%d, %d\n", ac_synth.duty_pos, ac_synth.duty_neg);
    }
}
