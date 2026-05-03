/*
 * identify.h
 *
 *  Created on: 1 mei 2026
 *      Author: andre
 */

#ifndef IDENTIFY_H_
#define IDENTIFY_H_

#include <stdint.h>

#define ID_BOOTROM_VT_STM32F1 0x6bb012c4 // STMicroelectronics
#define ID_BOOTROM_VT_CH32F1  0xDFFFDF9A // WinChipHead
#define ID_BOOTROM_VT_APM32F1 0xB2CA0C8B // Geehy
#define ID_BOOTROM_VT_MM32F1  0x96187e19 // MindMotion


uint32_t sticky_apb2_bits(void);
uint32_t sticky_apb1_bits(void);
uint32_t sticky_ahb1_bits(void);

#endif /* IDENTIFY_H_ */
