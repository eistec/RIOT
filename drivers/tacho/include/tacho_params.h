/*
 * Copyright (C) 2018 SKF AB
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_tacho
 *
 * @{
 * @file
 * @brief       Default configuration for tacho driver
 *
 * @author      Joakim Nohlgård <joakim.nohlgard@eistec.se>
 */

#ifndef TACHO_PARAMS_H
#define TACHO_PARAMS_H

#include "board.h"
#include "tacho.h"
#include "saul_reg.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TACHO_GPIO
#define TACHO_GPIO GPIO_PIN(0,18)
#endif

#ifndef TACHO_GPIO_FLANK
#define TACHO_GPIO_FLANK GPIO_FALLING
#endif

#ifndef TACHO_PARAMS
#define TACHO_PARAMS      { .gpio = TACHO_GPIO, \
                            .gpio_flank = TACHO_GPIO_FLANK }
#endif

#ifndef TACHO_SAUL_INFO
#define TACHO_SAUL_INFO   { .name = "pulse counter" }
#endif

/**
 * @brief   TACHO configuration
 */
static const tacho_params_t tacho_params[] =
{
    TACHO_PARAMS,
};

/**
 * @brief   Additional meta information to keep in the SAUL registry
 */
static const saul_reg_info_t tacho_saul_info[] =
{
    TACHO_SAUL_INFO
};

#ifdef __cplusplus
}
#endif

#endif /* TACHO_PARAMS_H */
/** @} */
