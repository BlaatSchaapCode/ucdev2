/*
 * mcu32f1rcc.h
 *
 *  Created on: 9 mei 2026
 *      Author: andre
 */

#ifndef MCU32_MCU32F1RCC_H_
#define MCU32_MCU32F1RCC_H_

#include <stdint.h>

extern const int hsi_speed;
extern const int hse_speed;

uint32_t rcc_get_sysclk(void);
uint32_t rcc_get_ahb1clk(void);
uint32_t rcc_get_apb1clk(void);
uint32_t rcc_get_apb2clk(void);
uint32_t rcc_get_adcclk(void);




#endif /* MCU32_MCU32F1RCC_H_ */
