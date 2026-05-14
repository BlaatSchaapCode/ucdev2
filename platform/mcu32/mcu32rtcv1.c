/*
 * mcu32f1rtc.c
 *
 *  Created on: 26 apr. 2026
 *      Author: andre
 */

#ifndef MCU32_MCU32F1RTC_C_
#define MCU32_MCU32F1RTC_C_

#include <mcu32regs.h>
#include <mcu32rtcv1.h>
#include <mcu32f1peri.h> // TODO

static mcu32f1_rtc_t *rtc = (mcu32f1_rtc_t*) RTC_BASE;
static mcu32f1_rcc_t *rcc = (mcu32f1_rcc_t*) RCC_BASE;
void rtc_init(bool use_xtal) {
	peripheral_enable((void*) PWR_BASE);



	// Quick and dirty, set DBP bit in PWR_CR
	// As I don't want to implement yet another peripheral
	// at this time. Just to get the RTC going.
	(*(uint32_t*) PWR_BASE) |= 1 << 8;

	peripheral_enable((void*) RTC_BASE);

	if (use_xtal) {
		rcc->bdc.lse_on = 1;
		while (!rcc->bdc.lse_rdy)
			;
		rcc->bdc.rtc_sel = 0b01; // LSE, TODO enum
	} else {
		rcc->cs.lsi_on = 1;
		while (!rcc->cs.lsi_rdy)
			;
		rcc->bdc.rtc_sel = 0b10; // LSI, TODO enum
	}
	rcc->bdc.rtc_en = 1;

	// Wait for RTC to be ready
	while (!rtc->flag.ready);
}

void rtc_init2(uint32_t divider) {
	peripheral_enable((void*) PWR_BASE);

	// Quick and dirty, set DBP bit in PWR_CR
	// As I don't want to implement yet another peripheral
	// at this time. Just to get the RTC going.
	(*(uint32_t*) PWR_BASE) |= 1 << 8;
	peripheral_enable((void*) RTC_BASE);

	rcc->bdc.lse_on = 1;
	while (!rcc->bdc.lse_rdy)
		;
	rcc->bdc.rtc_sel = 0b01; // LSE, TODO enum

	rcc->bdc.rtc_en = 1;
	// Enter config
	rtc->flag.configuration = 1;
	// TODO: MH32 works different
	rtc->preload.lo = divider;
	rtc->preload.hi = divider >> 16;
	// Exit config
	rtc->flag.configuration = 0;

	// Wait for RTC to be ready
	while (!rtc->flag.ready);


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
