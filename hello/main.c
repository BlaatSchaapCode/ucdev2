#include <stdio.h>
#include <string.h>

#include <system.h>
#include <cpuid.h>

#include <mcu32f1peri.h>
#include <mcu32f1rtc.h>
#include <mcu32gpiov1.h>
#include <mcu32regs.h>
#include <mcu32f1flash.h>

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

void measure_speed_mhz() {
	dwt_enable();
	int time = rtc_get();
	while (time == rtc_get()) {
	}
	uint32_t dwt_begin = dwt_get();
	time = rtc_get();
	while (time == rtc_get()) {
	}
	uint32_t dwt_end = dwt_get();
	uint32_t f = dwt_end - dwt_begin;

//	printf("%9d  Hz\n", f);
//	printf("%9d kHz\n", (f + 500) / 1000);
	printf("%9d MHz\n", (f + 500000) / 1000000);
}

void measure_speed_khz() {
	dwt_enable();
	int time = rtc_get();
	while (time == rtc_get()) {
	}
	uint32_t dwt_begin = dwt_get();
	time = rtc_get();
	while (time == rtc_get()) {
	}
	uint32_t dwt_end = dwt_get();
	uint32_t f = dwt_end - dwt_begin;

//	printf("%9d  Hz\n", f);
	printf("%9d kHz\n", (f + 500) / 1000);
//	printf("%9d MHz\n", (f + 500000) / 1000000);
}

uint32_t rcc_get_sysclk(void) {
	volatile mcu32f1_rcc_t *rcc = (volatile mcu32f1_rcc_t*) RCC_BASE;
	int hsi_speed = 8000000; // TODO
	int hse_speed = 8000000; // TODO
	switch (rcc->cfg1.system_clock_status) {
	case system_clock_hsi: //0b00
		return hsi_speed;
	case system_clock_hse: //0b01
		return hse_speed;
	case system_clock_pll: // 0b10
	}

	return 0;
}

int main() {
	puts("--------------------");
	romtable_pid_t rt_pid = get_romtable_pid();

	printf("Core CPU          ID: %08X\n", get_cpuid());
	printf("Romtable          ID: %08X %08X\n" , rt_pid.word_h, rt_pid.word_l );
	printf("Compact Romtable  ID: %08X\n", get_pid32());


	rtc_init();

	volatile mcu32f1_rcc_t *rcc = (volatile mcu32f1_rcc_t*) RCC_BASE;

	peripheral_enable((void*) FMC_BASE);
	// First test on HSI

//	for (int i = 0b0000; i <= 0b1111; i++) {
//		puts("--------------------");
//		printf("PLL Source: HSI/2: PLL Multiplier reg val %d\n", i);
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
//		// Enable PLL
//		rcc->cr.pllon = 1;
//		// Set clock source to PLL
//		rcc->cfg1.system_clock_select = system_clock_pll;
//
//
//		measure_speed_mhz();
//	}

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

		printf("PLL Source: HSE/1: PLL Multiplier reg val %X ", i);
		measure_speed_mhz();
	}

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

			printf("PLL Source: HSE/%d: PLL Multiplier reg val 1 ", i);
			measure_speed_khz();
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
