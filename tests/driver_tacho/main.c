/*
 * Copyright (C) 2018 SKF AB
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     tests
 * @{
 *
 * @file
 * @brief       Test application for the tacho driver
 *
 * @author      Joakim Nohlgård <joakim.nohlgard@eistec.se>
 *
 * @}
 */

#include <stdio.h>

#include "tacho.h"
#include "xtimer.h"

#define SLEEP       US_PER_SEC

#define MIN_DURATION US_PER_SEC
#define NUM_BUFS 16
static tacho_interval_t bufs[NUM_BUFS];
static tacho_t tacho = {
    .bufs = bufs,
    .min_duration = {XTIMER_HZ},
    .num_bufs = sizeof(bufs) / sizeof(bufs[0]),
};

static const tacho_params_t params = {
    .gpio = BTN0_PIN,
    .gpio_flank = GPIO_RISING,
    .gpio_mode = GPIO_IN_PU,
    .debounce_usec = 0,
};

int main(void)
{
    printf("Tacho driver test application\n");

    puts("Initializing tacho pin");
    int res = tacho_init(&tacho, &params);
    if (res < 0) {
        printf("tacho_init failed: %d\n", res);
        return 1;
    }

    while (1) {
        printf("Tacho: ");
        unsigned count = 0;
        uint32_t duration = 3000000ul;
        uint32_t start_time = 0;
        tacho_read(&tacho, &count, &duration, &start_time);
        unsigned long rpm = 0;
        if (duration > 0) {
            rpm = ((uint64_t)count * 60000000ul) / duration;
        }
        printf("ts: %8" PRIu32 " [us], %8lu BPM (%8u [count] / %8" PRIu32 " [us])\n", start_time, rpm, count, duration);

        xtimer_usleep(SLEEP);
    }

    return 0;
}
