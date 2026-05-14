/*
 * mcu32f1rcc.c
 *
 *  Created on: 9 mei 2026
 *      Author: andre
 */

#include <mcu32f1rcc.h>

#include <mcu32regs.h>
#include <mcu32f1peri.h>

[[gnu::weak]] const int hsi_speed = 8000000;
[[gnu::weak]] const int hse_speed = 8000000;

uint32_t rcc_get_sysclk(void) {
	volatile mcu32f1_rcc_t *rcc = (volatile mcu32f1_rcc_t*) RCC_BASE;

	int pll_in, prediv;
	switch (rcc->cfg1.system_clock_status) {
	case system_clock_hsi: //0b00
		return hsi_speed;
	case system_clock_hse: //0b01
		return hse_speed;
	case system_clock_pll: // 0b10
		switch (rcc->cfg1.pll_source) {
		case 0:
			// PLL Source is hsi_speed / 2
			pll_in = hsi_speed / 2;
			prediv = 1;
			break;
		case 1:
			// PLL Source is hse_speed / prediv1
			if (rcc->cfg2.prediv1) {
				prediv = 1 + rcc->cfg2.prediv1;
			} else {
				// If cfg2->prediv1 reads zero,
				// it is either not implemented or really zero
				// In either case, we use the value from cfg1
				prediv = 1 + rcc->cfg1.prediv1_0;
			}

			int pll2mul, pll2div;
			switch (rcc->cfg2.prediv1_src) {
			case 0: // todo ENUM
				pll_in = hse_speed / prediv;
				break;
			case 1:
				pll2mul = 2 + rcc->cfg2.prediv2;
				if (pll2mul == 17)
					pll2mul = 20;
				pll2div = rcc->cfg2.prediv2;
				pll_in = ((hse_speed / pll2div) * pll2mul) / prediv;
				break;
			}
		}
		int pllmul = rcc->cfg1.pll1_mul + 2;
		// TODO: extra pll1 bits for various variants
		// TODO: connectivity line exception: 13 -> 6.5 (also applies to GD32E1)
		if (pllmul == 17)
			pllmul = 16;

		return pll_in * pllmul;

		break;
	default:
		return 0;
	}

	return 0;
}

uint32_t rcc_get_ahb1clk(void) {
	volatile mcu32f1_rcc_t *rcc = (volatile mcu32f1_rcc_t*) RCC_BASE;
	int shift;
	switch ((rcc->cfg1.ahb1_prescaler & 0b1100)) {
	case 0b0000:
		shift = 0;
		break;
	case 0b1000:
		shift = rcc->cfg1.ahb1_prescaler - 7;
		break;
	case 0b1100:
		shift = rcc->cfg1.ahb1_prescaler - 6;
		break;
	}
	return rcc_get_sysclk() >> shift;
}

uint32_t rcc_get_apb1clk(void) {
	volatile mcu32f1_rcc_t *rcc = (volatile mcu32f1_rcc_t*) RCC_BASE;
	int shift;
	switch ((rcc->cfg1.apb1_prescaler & 0b100)) {
	case 0b000:
		shift = 0;
		break;
	case 0b100:
		shift = rcc->cfg1.apb1_prescaler - 3;
		break;
	}
	return rcc_get_ahb1clk() >> shift;

}
uint32_t rcc_get_apb2clk(void) {
	volatile mcu32f1_rcc_t *rcc = (volatile mcu32f1_rcc_t*) RCC_BASE;
	int shift;
	switch ((rcc->cfg1.apb2_prescaler & 0b100)) {
	case 0b000:
		shift = 0;
		break;
	case 0b100:
		shift = rcc->cfg1.apb2_prescaler - 3;
		break;
	}
	return rcc_get_ahb1clk() >> shift;
}
uint32_t rcc_get_adcclk(void) {
	volatile mcu32f1_rcc_t *rcc = (volatile mcu32f1_rcc_t*) RCC_BASE;
	int shift = rcc->cfg1.adc_prescaler + 1;
	return rcc_get_apb2clk() >> shift;
}

