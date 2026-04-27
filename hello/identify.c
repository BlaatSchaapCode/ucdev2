/*
 * identify.c
 *
 *  Created on: 19 apr. 2026
 *      Author: andre
 */


#include <cpuid.h>


#define STM32F1 1




int identify(void) {
//	if (get_cpuid() == CM3P1P1) {
//		// Original STM32F1 uses Cortex M3 r1p1
//		// All known clones use later revisions of
//		// the CM3 core.
//		return STM32F1;
//	}

	romtable_pid_t pid = get_romtable_pid();
	int vendor_id = pid.identity_code |
			pid.continuation_code << 8|
	            pid.jep106_used << 12 ;
	switch(vendor_id) {
	case 0:
		// An all-zero romtable has been observed on MegaHunt devices
		// Documentation specifies an identification method my matching
		// the first 28 bit of the serial number.
		break;
	case VENDOR_ARM:
		// Generic ARM Romtable
		// ROMTABLE does not specify MCU vendor,
		break;
	case VENDOR_HK:
		// ROMTABLE entry invalid, observed on HK32F103CB
		// TODO: Investigate HK32F103ACB
		return vendor_id << 16 | pid.partno;
	case VENDOR_GD:
		return vendor_id << 16 | pid.partno;
	case VENDOR_ST:
			if (get_cpuid().cpuid == CM3P1P1) {
				// Original STM32F1 uses Cortex M3 r1p1
				// All known clones use later revisions of
				// the CM3 core.
				return vendor_id << 16 | pid.partno;
			}
			break;

	default : return -1;
	}

	return -1;
}

