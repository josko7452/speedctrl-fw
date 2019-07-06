#include "nco.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    struct nco nco = {
        .sample_rate   = 10000,
    };

    nco_init(&nco, 50.00f);

    for (int i = 0; i < 10000 / 50; ++i) {
        printf("%d,\n", nco_run(&nco));
    }
    nco_update_freq(&nco, 100.0f);
    for (int i = 0; i < 10000 / 50; ++i) {
        printf("%d,\n", nco_run(&nco));
    }
}
