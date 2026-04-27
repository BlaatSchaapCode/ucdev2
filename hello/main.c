#include <stdio.h>
#include <string.h>

#include <system.h>
#include <cpuid.h>

#include <mcu32f1peri.h>
#include <mcu32f1rtc.h>
#include <mcu32gpiov1.h>

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
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t dwt_get(void) {
	return DWT->CYCCNT;
}

int main() {

	cpuid_t cpuid = get_cpuid();
	romtable_pid_t rt_pid = get_romtable_pid();
	pid32_t pid32 = get_pid32();

	rtc_init();
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
