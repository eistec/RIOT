/*
 * Copyright (C) 2017 Eistec AB
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @defgroup        cpu_k64f Freescale K64F MCU
 * @ingroup         cpu
 * @brief           CPU specific implementations for the Freescale K64F
 *                  Kinetis Cortex-M4 MCU.
 * @{
 *
 * @file
 * @brief           Implementation specific CPU configuration options
 *
 * @author          Hauke Petersen <hauke.petersen@fu-berlin.de>
 * @author          Johann Fischer <j.fischer@phytec.de>
 */

#ifndef CPU_CONF_H
#define CPU_CONF_H

#include "cpu_conf_common.h"

#ifdef CPU_MODEL_MK22FN512VLH12
#include "vendor/MK22F51212.h"
#else
#error "undefined CPU_MODEL"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief   ARM Cortex-M specific CPU configuration
 * @{
 */
#define CPU_DEFAULT_IRQ_PRIO            (1U)
#define CPU_IRQ_NUMOF                   (89U)
#define CPU_FLASH_BASE                  (0x00000000)
/** @} */

/**
 * @name GPIO pin mux function numbers
 */
/** @{ */
#define PIN_MUX_FUNCTION_ANALOG 0
#define PIN_MUX_FUNCTION_GPIO 1
/** @} */
/**
 * @name GPIO interrupt flank settings
 */
/** @{ */
#define PIN_INTERRUPT_RISING 0b1001
#define PIN_INTERRUPT_FALLING 0b1010
#define PIN_INTERRUPT_EDGE 0b1011
/** @} */

/**
 * @name Timer hardware information
 */
/** @{ */
#define LPTMR_CLKEN()  (bit_set32(&SIM->SCGC5, SIM_SCGC5_LPTMR_SHIFT)) /**< Enable LPTMR0 clock gate */
#define PIT_CLKEN()    (bit_set32(&SIM->SCGC6, SIM_SCGC6_PIT_SHIFT)) /**< Enable PIT clock gate */
/** @} */

/**
 * @name DMA channels
 */
/** @{ */
#define DMA_NUMOF 16
/** @} */


/**
 * @name DMA channel source
 */
/** @{ */
typedef enum {
    DMA_SOURCE_DISABLED        =  0,
    DMA_SOURCE_RESERVED1       =  1,
    DMA_SOURCE_UART0_RX        =  2,
    DMA_SOURCE_UART0_TX        =  3,
    DMA_SOURCE_UART1_RX        =  4,
    DMA_SOURCE_UART1_TX        =  5,
    DMA_SOURCE_UART2_RX        =  6,
    DMA_SOURCE_UART2_TX        =  7,
    DMA_SOURCE_RESERVED8       =  8,
    DMA_SOURCE_RESERVED9       =  9,
    DMA_SOURCE_RESERVED10      = 10,
    DMA_SOURCE_RESERVED11      = 11,
    DMA_SOURCE_I2S0RX          = 12,
    DMA_SOURCE_I2S0TX          = 13,
    DMA_SOURCE_SPI0RX          = 14,
    DMA_SOURCE_SPI0TX          = 15,
    DMA_SOURCE_SPI1RXTX        = 16,
    DMA_SOURCE_RESERVED17      = 17,
    DMA_SOURCE_I2C0            = 18,
    DMA_SOURCE_I2C1            = 19,
    DMA_SOURCE_FTM0CH0         = 20,
    DMA_SOURCE_FTM0CH1         = 21,
    DMA_SOURCE_FTM0CH2         = 22,
    DMA_SOURCE_FTM0CH3         = 23,
    DMA_SOURCE_FTM0CH4         = 24,
    DMA_SOURCE_FTM0CH5         = 25,
    DMA_SOURCE_FTM0CH6         = 26,
    DMA_SOURCE_FTM0CH7         = 27,
    DMA_SOURCE_FTM1CH0         = 28,
    DMA_SOURCE_FTM1CH1         = 29,
    DMA_SOURCE_FTM2CH0         = 30,
    DMA_SOURCE_FTM2CH1         = 31,
    DMA_SOURCE_FTM3CH0         = 32,
    DMA_SOURCE_FTM3CH1         = 33,
    DMA_SOURCE_FTM3CH2         = 34,
    DMA_SOURCE_FTM3CH3         = 35,
    DMA_SOURCE_FTM3CH4		   = 36,
    DMA_SOURCE_FTM3CH5		   = 37,
    DMA_SOURCE_FTM3CH6	  	   = 38,
    DMA_SOURCE_FTM3CH7		   = 39,
    DMA_SOURCE_ADC0            = 40,
    DMA_SOURCE_ADC1            = 41,
    DMA_SOURCE_CMP0            = 42,
    DMA_SOURCE_CMP1            = 43,
    DMA_SOURCE_RESERVED44      = 44,
    DMA_SOURCE_DAC0            = 45,
    DMA_SOURCE_DAC1		       = 46,
    DMA_SOURCE_RESERVED47      = 47,
    DMA_SOURCE_PDB             = 48,
    DMA_SOURCE_PORT_A          = 49,
    DMA_SOURCE_PORT_B          = 50,
    DMA_SOURCE_PORT_C          = 51,
    DMA_SOURCE_PORT_D          = 52,
    DMA_SOURCE_PORT_E          = 53,
    DMA_SOURCE_RESERVED54	   = 54,
    DMA_SOURCE_RESERVED55	   = 55,
    DMA_SOURCE_RESERVED56 	   = 56,
    DMA_SOURCE_RESERVED57	   = 57,
    DMA_SOURCE_LPUART0_RX	   = 58,
    DMA_SOURCE_LPUART0_TX	   = 59,
    DMA_SOURCE_DMAMUX_ALWAYS0  = 60,
    DMA_SOURCE_DMAMUX_ALWAYS1  = 61,
    DMA_SOURCE_DMAMUX_ALWAYS2  = 62,
    DMA_SOURCE_DMAMUX_ALWAYS3  = 63,
} dma_source_t;
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* CPU_CONF_H */
/** @} */
