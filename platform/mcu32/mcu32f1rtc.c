/*
 * mcu32f1rtc.c
 *
 *  Created on: 26 apr. 2026
 *      Author: andre
 */

#ifndef MCU32_MCU32F1RTC_C_
#define MCU32_MCU32F1RTC_C_

#include <mcu32regs.h>
#include <mcu32f1peri.h>

static mcu32f1_rtc_t *rtc = (mcu32f1_rtc_t*) RTC_BASE;
static mcu32f1_rcc_t *rcc = (mcu32f1_rcc_t*) RCC_BASE;
void rtc_init(void) {

	peripheral_enable((void*)PWR_BASE);
	// Quick and dirty, set DBP bit in PWR_CR
	// As I don't want to implement yet another peripheral
	// at this time. Just to get the RTC going.
	(*(uint32_t*)PWR_BASE) |= 1 << 8;

	//rcc->cs.lsi_on = 1;
	//while (!rcc->cs.lsi_rdy);
	rcc->bdc.lse_on = 1;
	while (!rcc->bdc.lse_rdy);
	//rcc->bdc.rtc_sel = 0b10; // LSI, TODO enum
	rcc->bdc.rtc_sel = 0b01; // LSE, TODO enum
	rcc->bdc.rtc_en = 1;
}

uint32_t rtc_get(void) {
	int hi = rtc->count.hi;
	int lo = rtc->count.lo;

	// overflow check
	if (hi == rtc->count.hi) {
		// if the local copy of hi is the same as in the rtc,
		// then the result is valid
		return hi << 16 | lo;
	} else {
		// Otherwise it has overflow when we were busy reading
		// the low part. use the hew hi value
		return rtc->count.hi << 16 | lo;
	}
}

#endif /* MCU32_MCU32F1RTC_C_ */
