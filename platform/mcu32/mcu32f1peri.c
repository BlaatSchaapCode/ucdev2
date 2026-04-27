/*
 * mcu32f1peri.c
 *
 *  Created on: 5 apr. 2026
 *      Author: andre
 */

#include <mcu32f1peri.h>
#include <mcu32regs.h>

int peripheral_enable(void *p) {
	uint32_t peri = (uint32_t) p;
	peri -= PERIPH_BASE;
	peri >>= 10;
	uint32_t bus = peri >> 5;
	uint32_t pid = peri & 0x1F;



	// TODO: some other place to put this
	volatile mcu32f1_rcc_t *rcc = (volatile mcu32f1_rcc_t*) RCC_BASE;


	// handle peripherals with a different enable bit then the rule suggests
	if (p == RTC_BASE) {
		rcc->bdc.rtc_en = 1;
		return 0;
	}



	switch (bus) {
	case 0:
		rcc->apb1_enable.word |= 1 << pid;
		return 0;
		break;
	case 2:
		rcc->apb2_enable.word |= 1 << pid;
		return 0;
		break;
	case 4:
		rcc->ahb1_enable.word |= 1 << pid;
		return 0;
		break;
	default:
		return -1;
	}

}
