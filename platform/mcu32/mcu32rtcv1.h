/*
 * mcu32f1rtc.h
 *
 *  Created on: 26 apr. 2026
 *      Author: andre
 */

#ifndef MCU32_MCU32RTCV1_H_
#define MCU32_MCU32RTCV1_H_

#include <stdint.h>
#include <stdbool.h>
void rtc_init(bool use_xtal);

void rtc_init2(uint32_t divider);
uint32_t rtc_get(void);

#endif /* MCU32_MCU32RTCV1_H_ */
