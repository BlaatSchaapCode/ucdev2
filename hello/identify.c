/*
 * identify.c
 *
 *  Created on: 19 apr. 2026
 *      Author: andre
 */

#define CM3P1P1  0x411fc231

#define STM32F1 1

int identify(void) {
	if (SCB->CPUID == CM3P1P1) {
		// Original STM32F1 uses Cortex M3 r1p1
		// All known clones use later revisions of
		// the CM3 core.
		return STM32F1;
	}
	return -1;
}

