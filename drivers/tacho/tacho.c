/*
 * Copyright (C) 2018 SKF AB
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 */

/**
 * @ingroup     drivers_tacho
 * @{
 *
 * @file
 * @brief       GPIO tachometer driver implementation
 *
 * @author      Joakim Nohlgård <joakim.nohlgard@eistec.se>
 *
 * @}
 */

#include <stdint.h>
#include <string.h>
#include "tacho.h"
#include "xtimer.h"
#include "llwu.h"

#define ENABLE_DEBUG    (0)
#include "debug.h"

static void tacho_rotate_buffers(tacho_t *dev)
{
    unsigned next = (dev->idx + 1) % dev->num_bufs;
    tacho_interval_t *ival = &dev->bufs[next];
    ival->count = 0;
    ival->time_start = ival->time_end = dev->bufs[dev->idx].time_end;
    dev->idx = next;
}

/* Accumulate pulse count */
static void tacho_trigger(tacho_t *dev, xtimer_ticks32_t now)
{
    tacho_interval_t *ival = &dev->bufs[dev->idx];
    if (xtimer_less(dev->min_duration, xtimer_diff(now, ival->time_end))) {
        /* The last pulse came a long time ago, place an empty buffer between */
        /* This will make a pulse buffer of length (now - time_end) with a
         * single pulse */
        if (ival->count > 0) {
            tacho_rotate_buffers(dev);
        }
        ival = &dev->bufs[dev->idx];
    }
    ++ival->count;
    ival->time_end = now;
    if (xtimer_less(dev->min_duration, xtimer_diff(ival->time_end, ival->time_start))) {
        /* Rotate buffers when enough time has passed */
        tacho_rotate_buffers(dev);
    }
}

/* debouncing xtimer callback */
static void tacho_debounce_cb(void *arg)
{
    tacho_t *dev = (tacho_t*)arg;
    gpio_irq_enable(dev->gpio);
}

/* GPIO callback wrapper */
static void tacho_cb(void *arg)
{
    tacho_t *dev = (tacho_t *)arg;
    xtimer_ticks32_t now = xtimer_now();
    if (dev->gpio != GPIO_UNDEF) {
        gpio_irq_disable(dev->gpio);
        _xtimer_set(&dev->debounce_xt, dev->debounce_timeout.ticks32);
    }
    tacho_trigger(dev, now);
}

int tacho_init(tacho_t *dev, const tacho_params_t *params)
{
    int res = gpio_init_int(params->gpio, params->gpio_mode, params->gpio_flank, tacho_cb, dev);
    if (res != 0) {
        return res;
    }
    llwu_wakeup_pin_set(LLWU_WAKEUP_PIN_PTA18, LLWU_WAKEUP_EDGE_RISING, tacho_cb, dev);
    if (params->debounce_usec > 0) {
        dev->gpio = params->gpio;
        dev->debounce_timeout = xtimer_ticks_from_usec(params->debounce_usec);
        dev->debounce_xt = (const xtimer_t) {
            .target = 0,
            .long_target = 0,
            .callback = tacho_debounce_cb,
            .arg = dev,
        };
    }
    else {
        dev->gpio = GPIO_UNDEF;
    }

    dev->idx = 0;
    assert(dev->bufs);
    assert(dev->num_bufs);
    memset(dev->bufs, 0, dev->num_bufs * sizeof(dev->bufs[0]));
    return 0;
}

void tacho_read(const tacho_t *dev, unsigned *count, uint32_t *duration, uint32_t *start_time)
{
    assert(count);
    assert(duration);
    unsigned sum_count = 0;
    uint32_t sum_duration = 0;
    xtimer_ticks32_t start_tick = {0};
    do {
        unsigned idx = dev->idx;
        xtimer_ticks32_t now = xtimer_now();
        xtimer_ticks32_t diff = xtimer_diff(now, dev->bufs[idx].time_end);

        if ((*duration) < xtimer_usec_from_ticks(diff)) {
            /* no pulses detected within the duration */
            start_tick = dev->bufs[idx].time_end;
            sum_duration = xtimer_usec_from_ticks(diff);
            break;
        }
        unsigned n = dev->num_bufs;
        if (xtimer_less(dev->min_duration, diff)) {
            /* A long time since the last tick */
            sum_duration += xtimer_usec_from_ticks(diff);
        }
        while ((n > 0) && (sum_duration < (*duration))) {
            tacho_interval_t *ival = &dev->bufs[idx];
            sum_count += ival->count;
            sum_duration += xtimer_usec_from_ticks(xtimer_diff(ival->time_end, ival->time_start));
            start_tick = ival->time_start;
            --n;
            idx = (dev->num_bufs + idx - 1) % dev->num_bufs;
        }
    } while(0);
    *count = sum_count;
    *duration = sum_duration;
    if (start_time) {
        *start_time = xtimer_usec_from_ticks(start_tick);
    }
}

void tacho_debug_print(const tacho_t *dev)
{
    printf("tacho %p, nbufs=%u, idx=%u\n", (void*)dev, dev->num_bufs, dev->idx);
    for (unsigned k = 0; k < dev->num_bufs; ++k) {
        printf("  %8" PRIu32 "-%8" PRIu32 ": %3u",
            dev->bufs[k].time_start.ticks32,
            dev->bufs[k].time_end.ticks32,
            dev->bufs[k].count);
        if (k == dev->idx) {
            printf(" <---");
        }
        printf("\n");
    }
}
