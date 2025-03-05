#pragma once

#ifdef SYSTEMINCLUDE
#include SYSTEMINCLUDE
#else

#ifndef __NVIC_PRIO_BITS
#define __NVIC_PRIO_BITS 4U
#endif 

#ifndef IRQ_COUNT
#define IRQ_COUNT 64
#endif 

#define IRQn_Type int
#define SysTick_IRQn -1

#if defined(__ARM_ARCH_6M__)
#include "core_cm0.h"
#elif defined(__ARM_ARCH_7M__)
#include "core_cm3.h"
#elif defined(__ARM_ARCH_7EM__)
#include "core_cm4.h"
#elif defined(__ARM_ARCH_8M_BASE__)
#include "core_cm23.h"
#elif defined(__ARM_ARCH_8M_MAIN__)
#include "core_cm33.h"
#elif defined(__ARM_ARCH_8_1M_MAIN__)
#include "core_cm55.h"
#else
#error "Unsupported Cortex-M variant!"
#endif

#undef IRQn_Type
#undef SysTick_IRQn

#endif

//int mcu_irq_count() { return 32 * (1 + SCnSCB->ICTR); }

typedef void (*interrupt_handler_func)(void);
