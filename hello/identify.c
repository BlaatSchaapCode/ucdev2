/*
 * identify.c
 *
 *  Created on: 19 apr. 2026
 *      Author: andre
 */

#include <cpuid.h>
#include "identify.h"
#include "fpu.h"

#include "mcu32regs.h"
#include "mcu32f1peri.h"

//----------------------------------------
int try_read32(void *addr, uint32_t *val);
int try_write32(void *addr, uint32_t val);
//----------------------------------------

uint32_t sticky_ahb1_bits(void) {
	volatile mcu32f1_rcc_t *rcc = (volatile mcu32f1_rcc_t*) RCC_BASE;
	uint32_t initial_value = rcc->ahb1_enable.word;
	rcc->ahb1_enable.word = -1;
	uint32_t result = rcc->ahb1_enable.word;
	rcc->ahb1_enable.word = initial_value;
	return result;
}

uint32_t sticky_apb1_bits(void) {
	volatile mcu32f1_rcc_t *rcc = (volatile mcu32f1_rcc_t*) RCC_BASE;
	uint32_t initial_value = rcc->apb1_enable.word;
	rcc->apb1_enable.word = -1;
	uint32_t result = rcc->apb1_enable.word;
	rcc->apb1_enable.word = initial_value;
	return result;
}

uint32_t sticky_apb2_bits(void) {
	volatile mcu32f1_rcc_t *rcc = (volatile mcu32f1_rcc_t*) RCC_BASE;
	uint32_t initial_value = rcc->apb2_enable.word;
	rcc->apb2_enable.word = -1;
	uint32_t result = rcc->apb2_enable.word;
	rcc->apb2_enable.word = initial_value;
	return result;
}

int identify_system_rom_vt(void) {
	// System ROM starts at 0x1FFFF000
	// This contains some built-in uart bootloader
	// This code tried to identify it by calculating
	// a checksum of the vector table
	uint32_t value;
	uint32_t checksum = 0;

	// Some devices place the boot rom at a different location
	// Accessing these addresses that may not be defined could
	// cause a hardfault. We have the try_read() function to
	// handle these cases.

	// on STM32F10{5,7} the system rom appears to start at 0x1FFFB000
	// on MH32F103 the system rom appears to start at      0x1FFFB000
	// on the FCM32F103 the system rom start at            0x1FFFE000

	// RX32F103: reading from system memory, supposidly at 0x1FFFF000
	// returns non-constant data. Seems to apply to the whole space

	// Note the MH2514 also has a valid vector table at 0x1FFFF000
	// as well as at 0x1FFFB000. Sp, we check for the 0x1FFFB000 one first.

	if (!try_read32((void*) 0x1FFFB000, &value)) {
		// Succeeded to read from 0x1FFFB000
		if (((*(uint32_t*) 0x1FFFB000) & 0xF0000000) == 0x20000000) {
			for (uint32_t *system_rom = (uint32_t*) 0x1FFFB000;
					system_rom < (uint32_t*) 0x1FFFB020; system_rom++)
				checksum += *system_rom;
			return checksum;
		}
	}

	if (!try_read32((void*) 0x1FFFE000, &value)) {
		// Succeeded to read from 0x1FFFE000
		if (((*(uint32_t*) 0x1FFFE000) & 0xF0000000) == 0x20000000) {
			for (uint32_t *system_rom = (uint32_t*) 0x1FFFE000;
					system_rom < (uint32_t*) 0x1FFFE020; system_rom++)
				checksum += *system_rom;
			return checksum;
		}
	}

	if (((*(uint32_t*) 0x1FFFF000) & 0xF0000000) == 0x20000000) {
		// There appears to be a valid vector table at 0x1FFFF000
		// Some variants do not have the system rom at this address
		// So if there is nothing that looks like a valid top of stack value
		// We skip here

		// Seems we have a minimal vector table. Looking at STM's bootloader
		// the reset vector is at 0x1FFFF020

		for (uint32_t *system_rom = (uint32_t*) 0x1FFFF000;
				system_rom < (uint32_t*) 0x1FFFF020; system_rom++)
			checksum += *system_rom;
		return checksum;

	} else if (((*(uint32_t*) 0x1FFFF400) & 0xF0000000) == 0x20000000) {
		// There appears to be a valid vector table at 0x1FFFF400
		// This is observed on the MM32F103, BLM32F103

		for (uint32_t *system_rom = (uint32_t*) 0x1FFFF400;
				system_rom < (uint32_t*) 0x1FFFF420; system_rom++)
			checksum += *system_rom;
		return checksum;
	}

	return checksum;
}

int has_fpu(void) {
	if (FPU->MVFR0.fp64)
		return 64; // double precision
	if (FPU->MVFR0.fp32)
		return 32; // single precision
	if (FPU->MVFR1.fp16)
		return 16; // half precision
	return 0;
}

int has_mpu(void) {
	// TODO MPU register struct
	// Get a pointer to the MPU Type register
	uint32_t *mpu_type = (uint32_t*) (0xE000ED90);
	// extract the DREGION field, number of regions supported
	return ((*mpu_type) >> 8) & 0xFF;
}

int irq_prio_count(void) {
	// TODO using NVIC struct

	// Get a pointer to NVIC->IPR[0]
	uint32_t *nvic_ipr0 = (uint32_t*) (0xE000E400);
	// Store the current value
	uint32_t initial_state = *nvic_ipr0;
	// Write all bits to 1
	*nvic_ipr0 = -1;
	// Only the implemented bits stick, the rest is zero

	uint32_t prio_bits = ((*nvic_ipr0) & 0xFF);
	uint32_t prio_bit_count = __builtin_popcount(prio_bits);

	// Restore initial state
	*nvic_ipr0 = initial_state;
	// result result
	return 1 << prio_bit_count;
//	return prio_bits;
}

int identify(void) {
	int fpu = has_fpu();
	int mpu = has_mpu();
	int prio = irq_prio_count();

	romtable_pid_t pid = get_romtable_pid();
	int vendor_id = pid.identity_code | pid.continuation_code << 8
			| pid.jep106_used << 12;
	switch (vendor_id) {
	case 0:
		// An all-zero romtable has been observed on MegaHunt devices
		// Documentation specifies an identification method my matching
		// the first 28 bit of the serial number.
		// Another thing we could look at is the fact it only has 8
		// interrupt priorities in stead of 16.
		// For now identify by system rom vector table checksum.
		switch (identify_system_rom_vt()) {
		// Looking at the System ROM
		case ID_BOOTROM_VT_MH2103:
			return VENDOR_MH << 16 | 0x2103;
		case ID_BOOTROM_VT_BLM32:
			// BetterLife (https://blestech.com/en/)
			return VENDOR_BLM << 16;
			break;
		default:
			break;
		}

		break;
	case VENDOR_ARM:
		// Generic ARM Romtable
		// ROMTABLE does not specify MCU vendor,
		switch (identify_system_rom_vt()) {
		// Looking at the System ROM
		case ID_BOOTROM_VT_STM32F103:
			// A copy of ST's Bootrom

			switch ((*(uint32_t*) (0x1FFFF7d0))) {
			case 0x0CF300FF:
				// 0x0CF300FF is at this location for both ApexMic and Geehy
				// branded APM32F103. This seems an identifying factor.
				// ApexMic branded APM32
				//return VENDOR_APM << 16;
				return VENDOR_GH << 16;
			case 0xFFFFFFFF:
				// Possibly CSK32F103? or MS32F103?
				// Can we tell them apart?
				// These features are too generic to tell.
				// TODO: other things to detect?
				return VENDOR_CETC << 16;
			default:
				break;
			}

			break;
		case ID_BOOTROM_VT_CH32F1:
			// CH32F1
			return VENDOR_CH << 16;
			break;
		case ID_BOOTROM_VT_APM32F1:
			// Geehy branded APM32
			return VENDOR_GH << 16;
			break;
		case ID_BOOTROM_VT_MM32F1:
			// MindMotion MM32F103
			return VENDOR_MM << 16;
		case ID_BOOTROM_VT_AT32F4: {
			uint32_t dbgmcu = *(uint32_t*) DBGMCU_BASE;
			uint8_t artery1 = (dbgmcu >> 28) & 0xF;
			uint8_t artery2 = (dbgmcu >> 16) & 0xF;
			switch (artery1) {
			case 5:
			case 7:
				return VENDOR_AT << 16 | artery1 << 12 | artery2 << 8;
			default:
				break;
			}

			break;
		}
		default:
			break;
		}

		break;
	case VENDOR_HK:
		// ROMTABLE entry invalid, observed on HK32F103CB
		// TODO: Investigate HK32F103ACB
		return vendor_id << 16 | pid.partno;
	case VENDOR_GD:
		return vendor_id << 16 | pid.partno;
	case VENDOR_ST:
		switch (get_cpuid().cpuid)
		case CM3R1P1:
			// Original STM32F1 uses Cortex M3 r1p1
			// All known clones use later revisions of
			// the CM3 core.
			// TODO: tell the dies apart.
			return vendor_id << 16 | pid.partno;
	case CM3R2P0:
		// Potentially MH2514
		switch (identify_system_rom_vt()) {
		case ID_BOOTROM_VT_MH2514:
			return VENDOR_MH << 16 | 0x2514;
			break;
		default:
			break;
		}

		break;

	default:
		return -1;
	}

	return -1;
}

