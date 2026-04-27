/*
 * mcu32f1flash.c
 *
 *  Created on: 26 apr. 2026
 *      Author: andre
 */

#include <mcu32regs.h>
#include <mcu32f1peri.h>

void flash_set_latency(int latency) {
	mcu32f1_fmc_t *fmc = (mcu32f1_fmc_t*) FMC_BASE;
	fmc->ac.latency = latency;
}

int flash_get_latency(void) {
	mcu32f1_fmc_t *fmc = (mcu32f1_fmc_t*) FMC_BASE;
	return fmc->ac.latency;
}
