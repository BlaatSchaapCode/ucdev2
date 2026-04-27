/*
 * mcu32f1rtc.h
 *
 *  Created on: 26 apr. 2026
 *      Author: andre
 */

#ifndef MCU32_MCU32F1RTC_H_
#define MCU32_MCU32F1RTC_H_

#include <stdint.h>

void rtc_init(void);
uint32_t rtc_get(void);

#endif /* MCU32_MCU32F1RTC_H_ */
