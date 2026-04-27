/*
 * mcu32gpiov1.h
 *
 *  Created on: 5 apr. 2026
 *      Author: andre
 */

#ifndef MCU32_MCU32GPIOV1_H_
#define MCU32_MCU32GPIOV1_H_

#include <assert.h>

#pragma pack(push,1)

typedef enum {
	mcu32gpiov1_mode_input = 0b00, // input
	mcu32gpiov1_mode_output_medium = 0b01, // 10 MHz
	mcu32gpiov1_mode_output_slow = 0b10, // 2 MHz
	mcu32gpiov1_mode_output_fast = 0b11, // 50 MHz
} mcu32gpiov1_mode;

typedef enum {
	mcu32gpiov1_conf_input_analog = 0b00, mcu32gpiov1_conf_output_pp = 0b00,

	mcu32gpiov1_conf_input_float = 0b01, mcu32gpiov1_conf_output_od = 0b01,

	mcu32gpiov1_conf_input_od = 0b10, mcu32gpiov1_conf_output_afpp = 0b10,

	mcu32gpiov1_conf_input_reserved = 0b11, mcu32gpiov1_conf_output_afod = 0b11,
} mcu32gpiov1_conf;

typedef struct {
	mcu32gpiov1_mode mode :2;
	mcu32gpiov1_conf conf :2;
} modeconf_t;

typedef union {
	uint32_t word[2];
	uint64_t dword;
	struct {
		mcu32gpiov1_mode pin0_mode :2;
		mcu32gpiov1_conf pin0_conf :2;
		mcu32gpiov1_mode pin1_mode :2;
		mcu32gpiov1_conf pin1_conf :2;
		mcu32gpiov1_mode pin2_mode :2;
		mcu32gpiov1_conf pin2_conf :2;
		mcu32gpiov1_mode pin3_mode :2;
		mcu32gpiov1_conf pin3_conf :2;
		mcu32gpiov1_mode pin4_mode :2;
		mcu32gpiov1_conf pin4_conf :2;
		mcu32gpiov1_mode pin5_mode :2;
		mcu32gpiov1_conf pin5_conf :2;
		mcu32gpiov1_mode pin6_mode :2;
		mcu32gpiov1_conf pin6_conf :2;
		mcu32gpiov1_mode pin7_mode :2;
		mcu32gpiov1_conf pin7_conf :2;
		mcu32gpiov1_mode pin8_mode :2;
		mcu32gpiov1_conf pin8_conf :2;
		mcu32gpiov1_mode pin9_mode :2;
		mcu32gpiov1_conf pin9_conf :2;
		mcu32gpiov1_mode pin10_mode :2;
		mcu32gpiov1_conf pin10_conf :2;
		mcu32gpiov1_mode pin11_mode :2;
		mcu32gpiov1_conf pin11_conf :2;
		mcu32gpiov1_mode pin12_mode :2;
		mcu32gpiov1_conf pin12_conf :2;
		mcu32gpiov1_mode pin13_mode :2;
		mcu32gpiov1_conf pin13_conf :2;
		mcu32gpiov1_mode pin14_mode :2;
		mcu32gpiov1_conf pin14_conf :2;
		mcu32gpiov1_mode pin15_mode :2;
		mcu32gpiov1_conf pin15_conf :2;
	};

} mcu32gpiov1_crl_t;

static_assert(sizeof(mcu32gpiov1_crl_t) == 8, "Data structure size error");


typedef union {
	uint32_t word[0x100]; // word access, padding
	struct {
		mcu32gpiov1_crl_t crl;
		uint32_t input;
		uint32_t output;
		uint32_t set;
		uint32_t reset;
		uint32_t lock;
	};
} mcu32gpiov1_t;


#pragma pack(pop)

#endif /* MCU32_MCU32GPIOV1_H_ */
