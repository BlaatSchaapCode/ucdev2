#include <stdio.h>
#include <string.h>

#include <system.h>
#include <cpuid.h>
#include "fpu.h"
#include "identify.h"

#include <mcu32f1peri.h>
#include <mcu32f1rcc.h>
#include <mcu32gpiov1.h>
#include <mcu32regs.h>
#include <mcu32f1flash.h>
#include <mcu32rtcv1.h>

//#include "identify.h"
int identify(void); // TODO
int identify_system_rom_vt(void); // TODO
int irq_prio_count(void);
int has_fpu(void);
int has_mpu(void);

[[gnu::alias("USART2_IRQHandler")]] void Interrupt38_Handler(void) [[gnu::unused]];
[[gnu::interrupt]] void USART2_IRQHandler(void) {
}

void gpio_pin_set(int pin) {
	unsigned port = pin >> 4;
	// All port peripherals are in order in memory, we can use them as
	// an array, provided the peripheral struct matches in size.
	// Hence the padding
	volatile mcu32gpiov1_t *ports = (volatile mcu32gpiov1_t*) GPIOA_BASE;

	// we have port A to G, thus 7 ports in total
	if (port < 7) {
		ports[port].set = 1 << (pin & 0xF);
	}
}

void gpio_pin_clear(int pin) {
	unsigned port = pin >> 4;
	// All port peripherals are in order in memory, we can use them as
	// an array, provided the peripheral struct matches in size.
	// Hence the padding
	volatile mcu32gpiov1_t *ports = (volatile mcu32gpiov1_t*) GPIOA_BASE;

	// in the largest configuration,
	// we have port A to G, thus 7 ports in total
	if (port < 7) {
		ports[port].reset = 1 << (pin & 0xF);
	}
}

void dwt_enable(void) {
	CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t dwt_get(void) {
	return DWT->CYCCNT;
}

uint32_t measure_speed() {
	dwt_enable();

	// Crystal is at 32768 Hz
	// If we divide by 4096 instead we have 1/8th of a second ticks
	// to speed up measure
	// or 1/4 of a second?
	rtc_init2(8192);
	int time = rtc_get();
	while (time == rtc_get()) {
	}
	uint32_t dwt_begin = dwt_get();
	time = rtc_get();
	while (time == rtc_get()) {
	}
	uint32_t dwt_end = dwt_get();
	uint32_t f = dwt_end - dwt_begin;

	// as we measured 1/8th sec we multiply by 8
	f *= 4;
	// Well... measuring like this seems to introduce
	// some latency, which gets amplified now we
	// multiply by 8.

	return f;
}

uint32_t system_set_sysclk(uint32_t speed, bool use_xtal) {
	volatile mcu32f1_rcc_t *rcc = (volatile mcu32f1_rcc_t*) RCC_BASE;
	// Set system to a safe config before doing anything else
	// Select HSI as System Clock
	rcc->cfg1.system_clock_select = system_clock_hsi;
	// Stop the PLL
	rcc->cr.pllon = 0;
	// Stop the xtal
	rcc->cr.hseon = 0;

	if (use_xtal) {
		if (speed == hse_speed) {
			// Not using PLL
			// Run on crystal speed
			// Reset all prescalers to 0
			rcc->cfg1.ahb1_prescaler = 0b0000; // do not prescale ahb1
			rcc->cfg1.apb2_prescaler = 0b0000; // do not prescale apb2
			rcc->cfg1.apb1_prescaler = 0b000; // do not prescale apb1
			// Start the crystal if it ain't running

			rcc->cr.hseon = 1;
			// TODO, timeout
			while (!rcc->cr.hserdy)
				;

			// Select HSE as clock source
			rcc->cfg1.system_clock_select = system_clock_hse;

			// We are done: return current speed
			return rcc_get_sysclk();
		}
	} else {
		if (speed == hsi_speed) {
			// We are already running at HSI at this point
			// Reset all prescalers to 0 and be done
			rcc->cfg1.ahb1_prescaler = 0b0000; // do not prescale ahb1
			rcc->cfg1.apb2_prescaler = 0b0000; // do not prescale apb2
			rcc->cfg1.apb1_prescaler = 0b000; // do not prescale apb1

			// We are done: return current speed
			return rcc_get_sysclk();
		}
	}

	int pll_in_speed;

	if (use_xtal)
		pll_in_speed = hse_speed;
	else
		pll_in_speed = hsi_speed / 2;

	int fraq = speed % pll_in_speed;
	if (fraq) {
		// Requested speed is not divisible
		// Might be possible on MCUs that implement RCC->CFG2
		// Note: If it is half there still is PREDIV1_0 (PLLXTPRE) in RCC->CFG1
		//       but that seems to be missing in some parts. That needs more testing.
		//       We will skip this for now, this is for later
		// Also, there is this 6.5x case on certain parts.
		return -1;
	}
	int mul = speed / pll_in_speed;
	if (mul > 16) {
		// Higher multiplications are possible on some parts
		// Gigadevice and Megahunt. For now we limit ourselves
		// to the behaviour that is available on the STM32F1
		// We will look at extra features later
		return -1;
	}
	if (mul < 2) {
		// Lowest multiplication is x2
		return -1;
	}

	// Note: These max speeds are for STM32F10{1,2,3,5,7}
	// For the STM32F100, max speeds are 24 MHz for all expect 12 MHz for the ADC
	// Other devices with extended clocking capabilities will have higher values
	// 24 MHz is the speed per flash latency increase. Does this mean F100
	// does not implement flash latency?

	// uint32_t ahb1_max = 72000000;
	// uint32_t apb1_max = 36000000;
	// uint32_t apb2_max = 72000000;
	// uint32_t adc1_max = 14000000;

	// For now, we'll clock APB1 at half the

	rcc->cfg1.ahb1_prescaler = 0b0000; // do not prescale ahb1
	rcc->cfg1.apb2_prescaler = 0b0000; // do not prescale apb2
	if (speed > 36000000)
		rcc->cfg1.apb1_prescaler = 0b100; // APB1CLK = SYSCLK / 2
	else
		rcc->cfg1.apb1_prescaler = 0b000; // do not prescale apb1

	// For every 24 MHz, we need 1 flash latency
	flash_set_latency((speed + 23999999) / 24000000);

	if (use_xtal) {
		rcc->cr.hseon = 1;
		// TODO, timeout
		while (!rcc->cr.hserdy)
			;
		rcc->cfg1.pll_source = 1; // TODO enum

	} else {
		rcc->cfg1.pll_source = 0; // TODO enum
	}

	rcc->cfg1.pll1_mul = mul - 2;

	rcc->cr.pllon = 1;
	while (!rcc->cr.pllrdy)
		;

	// Set clock source to PLL
	rcc->cfg1.system_clock_select = system_clock_pll;

	switch (speed) {
	case 48000000:
		rcc->cfg1.usbpre = 1;
		break;
	case 72000000:
		rcc->cfg1.usbpre = 0;
		break;
	default:
		// TODO extra usb speed options on some models
		break;
	}

	// TODO: ADC clock divider settings

	// We are done: return current speed
	return rcc_get_sysclk();
}

int main() {
	puts("--------------------");
	romtable_pid_t rt_pid = get_romtable_pid();

	printf("CPUID               : %08X\n", get_cpuid());

	// Processor Feature bits. Expected to be the same for the same
	// core. Note that some chips have a Cortex M4 in stead of an M3
	// When an M4 is fitted, optionally there is an FPU, in which case
	// the MVFR registers show a non-zero value.
	puts("--------------------");
	printf("PFR0                : %08X\n", *(uint32_t*) 0xE000ED40);
	printf("PFR1                : %08X\n", *(uint32_t*) 0xE000ED44);
	puts("--------------------");
	printf("DFR0                : %08X\n", *(uint32_t*) 0xE000ED48);
	puts("--------------------");
	printf("AFR0                : %08X\n", *(uint32_t*) 0xE000ED4C);
	puts("--------------------");
	printf("MMFR0               : %08X\n", *(uint32_t*) 0xE000ED50);
	printf("MMFR1               : %08X\n", *(uint32_t*) 0xE000ED54);
	printf("MMFR2               : %08X\n", *(uint32_t*) 0xE000ED58);
	printf("MMFR3               : %08X\n", *(uint32_t*) 0xE000ED5C);
	puts("--------------------");
	printf("ISAR0               : %08X\n", *(uint32_t*) 0xE000ED60);
	printf("ISAR1               : %08X\n", *(uint32_t*) 0xE000ED64);
	printf("ISAR2               : %08X\n", *(uint32_t*) 0xE000ED68);
	printf("ISAR3               : %08X\n", *(uint32_t*) 0xE000ED6C);
	printf("ISAR4               : %08X\n", *(uint32_t*) 0xE000ED70);
	printf("ISAR5               : %08X\n", *(uint32_t*) 0xE000ED74);
	puts("--------------------");
	printf("MVFR0               : %08X\n", *(uint32_t*) 0xE000EF40);
	printf("MVFR1               : %08X\n", *(uint32_t*) 0xE000EF44);
	printf("MVFR2               : %08X\n", *(uint32_t*) 0xE000EF48);
	puts("--------------------");
	printf("CLIDR               : %08X\n", *(uint32_t*) 0xE000ED78);
	printf("CTR                 : %08X\n", *(uint32_t*) 0xE000ED7C);
	printf("CCSIDR              : %08X\n", *(uint32_t*) 0xE000ED80);
	printf("CSSELR              : %08X\n", *(uint32_t*) 0xE000ED84);
	puts("--------------------");

	// Quick and Dirty way to print the ROMTABLE entries
	// 2 means absent, 3 means present,
	// We observe some differences between chips here.
	// Some chip do have an ETM fitted, according to the ROMTABLE
	// The STM32F103 has all options but the ETM fitted.
	uint32_t rt = 0;
	for (int i = 0; i < 8; i++) {
		uint32_t *romtable_entry = (uint32_t*) (0xE00FF000 + (4 * i));
		rt |= (*romtable_entry & 0xF) << (4 * i);
		if (!*romtable_entry)
			break;
	}
	printf("Romtable entries      %08X\n", rt);

	// STM32F103 has 16 priorities (4 bits)
	// MH2103 (MH32F103/Air32F103) has only 8 (3bit)
	printf("Interrupt Prio Count: %d\n", irq_prio_count());
	// Some chips have an Cortex-M4F
	printf("FPU Precision       : %d\n", has_fpu());
	// STM32F103 has no MPU. MPU has been observed in MH2103/MH2514
	printf("MPU Regions         : %d\n", has_mpu());

	printf("Romtable          ID: %08X %08X\n", rt_pid.word_h, rt_pid.word_l);
	printf("Compact Romtable  ID: %08X\n", get_pid32());

	//----------------------------------------------------------------------
	// Up to here generic ARM Cortex-M. Should run on any Cortex-M MCU.

	// ST-style DBGMCU. This points to IMPLEMENTATION DEFINED range
	// in the Private Peripheral Bus (PPB) region.
	uint32_t dbgmcu = *(uint32_t*) DBGMCU_BASE;
	printf("DBGMCU            ID: %08X\n", dbgmcu);

	printf("Identifies as ");
	// Not the most reliable detection, but Artery MCUs have
	// the most significant nibble of the MCU ID value either 5 or 7.
	// Other clones have been observed to have either a 1 or a 2,
	// but of course there is no guarantee there aren't any.
	// For Artery MCU's it seems the following pattern holds:
	// The models that have a 7 have the RTCv1 which is expected on a
	// F1-like MCU.
	// The models that have a 5 implement the RTCv2, which on real STM
	// is only seen on later introduced series
	//
	// Of course we could check for the full ids listed in the
	// Artery datasheets, but Artery is not the focus of this project.
	bool artery = false;
	bool rtcv2 = false;
	switch (dbgmcu >> 28) {
	case 5:
		// AT32F421
		// AT32F425
		artery = true;
		rtcv2 = true;
		puts("Artery AT32F42x; RTCv2");
		break;
	case 7:
		// AT32F403
		// AT32F407
		// AT32F413
		// AT32F415
		artery = true;
		rtcv2 = false;
		puts("Artery AT32F40x or AT32F41x; RTCv1");
		break;
	default:
		break;
	}

	if (!artery) {
		int die_id = dbgmcu & 0xFFF;
		switch (die_id) {
		case 0x420:
			puts("Low- and Medium-density Value Line: STM32F100x{4,6,8,B}");
			break;
		case 0x428:
			puts("High-density Value Line             STM32F100x{C,D,E}");
			break;
		case 0x412:
			puts("Low-density Mainstream Line         STM32F10{1,2,3}x{4,6}");
			break;
		case 0x410:
			puts("Medium-density Mainstream Line      STM32F10{1,2,3}x{8,B}");
			break;
		case 0x414:
			puts("High-density Mainstream Line        STM32F10{1,2,3}x{C.D.E}");
			// also GD32F303CCT6
			break;
		case 0x430:
			puts("XL-density Mainstream Line          STM32F10{1,2,3}x{F,G}");
			break;
		case 0x418:
			puts("Connectivity line                   STM32F10{5,7}x");
			// GD32F207 identifies as 0x418
			break;

		case 0x000:
			puts("Zero ID, real STM32F1 without debugger?");
			// Might be some real STM32F1 without debugger connected
			// This bug exists in the whole family.
			// Check ROMTABLE and CPUID to guess
			break;
		default:
			printf("Unsupported MCU? 0x%03X\n", die_id);

			// TODO: AT32F4xx parts seem largely MCU32F1 compatible
			// Note the F42x parts have RTCv2 in stead of RTCv1.
			// Seems Artery-parts use a generic ARM ROMTABLE
			// Can we get away with looking straight at the Artery ID codes
			// and hope no other manufacturer uses the same codes?
			// Investigate other ways to detect Artery-parts

			// TODO: GD32Fx0x parts: Seems for Gigadevice, devices in the
			// GD32F2, F3 and F4 look F1 compatible on first glance.
			// Needs more investigation.

			break;
		}
	}
	// Following code is F1x specific
	printf("Serial Number       : %08X %08X %08X\n", *(int*) (0x1FFFF7E8),
			*(int*) (0x1FFFF7EC), *(int*) (0x1FFFF7F0));
	printf("System ROM VT SUM   : %08X\n", identify_system_rom_vt());
	printf("Sticky RCC Bits     : %08lX %08lX %08lX\n", sticky_ahb1_bits(),
			sticky_apb1_bits(), sticky_apb2_bits());
	printf("Identification      : %08X\n", identify());

	if (rtcv2) {
		puts("RTCv2 current not supported; aborting further testing");
		while (1)
			;
	}
//	rtc_init();

	peripheral_enable((void*) FMC_BASE);

	for (uint32_t speed = 8000000; speed <= 64000000; speed += 4000000) {
		puts(
				"----------------------------------------------------------------------");
		printf("RC  : Setting speed to    %9ld Hz\n", speed);
		printf("RC  : System speed set to %9ld Hz\n",
				system_set_sysclk(speed, false));
		printf("RC  : Measured speed is   %9ld Hz\n", measure_speed());
	}

	for (uint32_t speed = 8000000; speed <= 128000000; speed += 8000000) {
		puts(
				"----------------------------------------------------------------------");
		printf("XTAL: Setting speed to    %9ld Hz\n", speed);
		printf("XTAL: System speed set to %9ld Hz\n",
				system_set_sysclk(speed, true));
		printf("XTAL: Measured speed is   %9ld Hz\n", measure_speed());
	}
}

void old_test(void) {
	volatile mcu32f1_rcc_t *rcc = (volatile mcu32f1_rcc_t*) RCC_BASE;
// First test on HSI

	for (int i = 0b0000; i <= 0b1111; i++) {
		// Select HSI as System Clock
		rcc->cfg1.system_clock_select = system_clock_hsi;
		// Disable PLL
		rcc->cr.pllon = 0;
		// Set new multiplier
		rcc->cfg1.pll1_mul = i;
		// TODO: Flash Wait State, one wait state for each 24 Mhz in speed
		// Note: we double it, the 8 should be 4, when using HSI
		// TODO: Flash latency setting crashes
		flash_set_latency(((i + 2) * 8) / 24);
		// Enable PLL

		// Behaviour MS32F103:
		// PLL fails to become ready when
		// clock source is HSI and pll1_mul = 0 (2x)
		// PLL becomes ready for pll1_mul = 1 (3x) or higher
		// However, measured speeds are 2/3 or expected value.
		// On HSE it seems to behave as expected
		rcc->cr.pllon = 1;
		while (!rcc->cr.pllrdy)
			;

		// Set clock source to PLL
		rcc->cfg1.system_clock_select = system_clock_pll;

		printf(
				"PLL Source: HSI/2: PLL1MUL: %X Calculated: %3d Mhz Measured: %3d MHz\n",
				i, (rcc_get_sysclk() + 500000) / 1000000,
				(measure_speed() + 500000) / 1000000);

	}

	// restore clock state
	// Select HSI as System Clock
	rcc->cfg1.system_clock_select = system_clock_hsi;
	// Disable PLL
	rcc->cr.pllon = 0;

	rcc->cr.hseon = 1;
	while (!rcc->cr.hserdy)
		;
	rcc->cfg1.pll_source = 1; // TODO enum

	for (int i = 0b0000; i <= 0b1111; i++) {
		// Select HSI as System Clock
		rcc->cfg1.system_clock_select = system_clock_hsi;
		// Disable PLL
		rcc->cr.pllon = 0;
		// Set new multiplier
		rcc->cfg1.pll1_mul = i;
		// TODO: Flash Wait State, one wait state for each 24 Mhz in speed
		// Note: we double it, the 8 should be 4, when using HSI
		// TODO: Flash latency setting crashes
		flash_set_latency(((i + 2) * 8) / 24);

		// Enable PLL
		rcc->cr.pllon = 1;
		while (!rcc->cr.pllrdy)
			;

		// Set clock source to PLL
		rcc->cfg1.system_clock_select = system_clock_pll;

		printf(
				"PLL Source: HSE/1: PLL1MUL: %X Calculated: %3d Mhz Measured: %3d MHz\n",
				i, (rcc_get_sysclk() + 500000) / 1000000,
				(measure_speed() + 500000) / 1000000);
	}

// restore clock state
// Select HSI as System Clock
	rcc->cfg1.system_clock_select = system_clock_hsi;
// Disable PLL
	rcc->cr.pllon = 0;

// Set  prediv using cfg1 (one bit)
// Check if cfg2 is supported (not on F103, but on f100, 107,
// what do other brands support ?)
	rcc->cfg1.prediv1_0 = 1; //
	printf("prediv: cfg1: %X cfg2: %X\n", rcc->cfg1.prediv1_0,
			rcc->cfg2.prediv1);

//	for (int i = 0b0000; i <= 0b1111; i++) {
//		// Select HSI as System Clock
//		rcc->cfg1.system_clock_select = system_clock_hsi;
//		// Disable PLL
//		rcc->cr.pllon = 0;
//		// Set new multiplier
//		rcc->cfg1.pll1_mul = i;
//		// TODO: Flash Wait State, one wait state for each 24 Mhz in speed
//		// Note: we double it, the 8 should be 4, when using HSI
//		// TODO: Flash latency setting crashes
//		flash_set_latency(((i + 2) * 8) / 24);
//
//		// Enable PLL
//		rcc->cr.pllon = 1;
//		while (!rcc->cr.pllrdy)
//			;
//
//		// Set clock source to PLL
//		rcc->cfg1.system_clock_select = system_clock_pll;
//
//		printf("PLL Source: HSE/2: PLL Multiplier reg val %X ", i);
//		measure_speed_mhz();
//	}

// Some MCUs appear not to even implement the cfg1 HSE division
// Such as the MH32 (AIR32)

	if (!rcc->cfg2.prediv1) {
		// Test if the mirroring bit is implemented
		// rcc->cfg1.prediv1_0 should be the same as rcc->cfg2.prediv1 bit 0
		rcc->cfg2.prediv1 = 1;
	}

	if (rcc->cfg2.prediv1) {
		puts("This MCU supports RCC Config Register 2");
		// so far only on fcm32103,

		for (int i = 0b0000; i <= 0b1111; i++) {
			// Select HSI as System Clock
			rcc->cfg1.system_clock_select = system_clock_hsi;
			// Disable PLL
			rcc->cr.pllon = 0;
			// Set new multiplier
			rcc->cfg1.pll1_mul = 0;
			rcc->cfg2.prediv1 = i;
			flash_set_latency(0);

			// Enable PLL
			rcc->cr.pllon = 1;
			while (!rcc->cr.pllrdy)
				;

			// Set clock source to PLL
			rcc->cfg1.system_clock_select = system_clock_pll;

			printf(
					"PLL Source: HSE/%2d: PLL1MUL 1, Calculated: %5d kHz Measured: %5d kHz\n",
					i, (rcc_get_sysclk() + 500) / 1000,
					(measure_speed() + 500) / 1000);

		}

	} else {
		puts("This MCU does not implement PREDIV1");
	}

// restore clock state
// Select HSI as System Clock
	rcc->cfg1.system_clock_select = system_clock_hsi;
// Disable PLL
	rcc->cr.pllon = 0;

//enable_peripheral( (void*)  TIM2_BASE);
//enable_peripheral( (void*)  TIM3_BASE);
	peripheral_enable((void*) GPIOA_BASE);
	peripheral_enable((void*) GPIOB_BASE);
	peripheral_enable((void*) GPIOC_BASE);
	volatile mcu32gpiov1_t *gpioa = (volatile mcu32gpiov1_t*) GPIOA_BASE;
	volatile mcu32gpiov1_t *gpiob = (volatile mcu32gpiov1_t*) GPIOB_BASE;
	volatile mcu32gpiov1_t *gpioc = (volatile mcu32gpiov1_t*) GPIOC_BASE;

//	gpioc->crl.pin13_mode = mcu32gpiov1_mode_output_slow;
//	gpioc->crl.pin13_conf = mcu32gpiov1_conf_output_pp;
//
//	gpioa->crl.pin4_mode = mcu32gpiov1_mode_output_slow;
//	gpioa->crl.pin4_conf = mcu32gpiov1_conf_output_pp;

	gpioa->crl.pin0_mode = mcu32gpiov1_mode_output_slow;
	gpioa->crl.pin0_conf = mcu32gpiov1_conf_output_pp;

//	gpioa->crl.pin1_mode = mcu32gpiov1_mode_output_slow;
//	gpioa->crl.pin1_conf = mcu32gpiov1_conf_output_pp;

// This appears to work, but the debugger does not show
// the correct values when inspecting the peripheral.

	while (1) {
		//gpioc->set = 1 << 13;
//		gpio_pin_set(45);
		gpio_pin_set(0);
//		for (volatile int i = 0 ; i < 80000; i++);
		//gpioc->reset = 1 << 13;
//		gpio_pin_clear(45);
		gpio_pin_clear(0);
//		for (volatile int i = 0 ; i < 80000; i++);
	}

}
