#pragma pack(push,1)

#include <stdint.h>
typedef union {
	volatile uint32_t word;
	// layout if the same across the f012347 series
	struct {
		unsigned hsion :1;   // 0
		unsigned hsirdy :1;   // 1
		unsigned :1;   // 2
		unsigned hsitrum :5;   // 3..7
		unsigned hsical :8;   // 8..15
		unsigned hseon :1;   // 16
		unsigned hserdy :1;   // 17
		unsigned hsebyp :1;   // 18
		unsigned csson :1;   // 19
		unsigned :4;   // 20..23
		unsigned pllon :1;   // 24
		unsigned pllrdy :1;   // 25
		unsigned :6;
	};

} mcu32f_rcc_cr_t;

typedef union {
	volatile uint32_t word;
	// layout is the same between c0, (not c5) g0 and g4 series
	struct {
	};

} mcu32c0g_rcc_cr_t;

typedef union {
	volatile uint32_t word;
	// layout is the same between c0, (not c5) g0 and g4 series
	struct {
	};

} mcu32c5_rcc_cr_t;

typedef union {
	volatile uint32_t word;
	// layout is the same between h5 and h7
	struct {
	};

} mcu32h_rcc_cr_t;

typedef enum {
	system_clock_hsi = 0b00, system_clock_hse = 0b01, system_clock_pll = 0b10,
} mcu32f1_rcc_system_clock_t;

typedef enum {
	clock_out_none   = 0b000, //
	clock_out_sysclk = 0b0100, //
	clock_out_hsi    = 0b0101, //
	clock_out_hse    = 0b0110, //
	clock_out_pll1_div2 = 0b0111, //
	clock_out_pll2 = 0b1000, //
	clock_out_pll3_div2 = 0b1001, //
	clock_out_xt1 = 0b1010, //
	clock_out_pll3 = 0b1011, //
} mcu32f1_rcc_clock_out_t;;

typedef union {
	uint32_t word;
	struct {
		mcu32f1_rcc_system_clock_t system_clock_select :2;   // 0..1
		mcu32f1_rcc_system_clock_t system_clock_status :2;   // 2..3
		unsigned ahb1_prescaler :4;   // 4..7
		unsigned apb1_prescaler :3;   // 8..10
		unsigned apb2_prescaler :3;   // 11..13
		unsigned adc_prescaler :2;   // 14..15
		unsigned pll_source :1;   // 16
		unsigned prediv1_0 :1; // 17: cfg2 prediv1 bit 0 where it is implemented
		unsigned pll1_mul :4;   // 18..21 // differs between {1,2,3} and {5,7}?
		unsigned usbpre :2;   // 22..23 GD32F1
		mcu32f1_rcc_clock_out_t clock_output :4;   // 24..27

		// GD32 speed extensions: todo check other brands
		unsigned gd32_adc_prescaler_2 :1; // 28 gd32f1: adc_prescaler[2]; // mh32:  pllmul[4]
		unsigned gd32_pll_mul_4 :1; // 29 gd32f1: pllmul[4];
	};
} mcu32f1_rcc_cfg1_t;

typedef union {
	uint32_t word;
	struct {
		unsigned prediv1 :4; // 0..3
		unsigned prediv2 :4; // 4..7
		unsigned pll2_mul :4; // 8..11
		unsigned pll3_mul :4; // 12..15
		unsigned prediv1_src :1; // 16
		unsigned i2s2_src :1; // 17
		unsigned i2s3_src :1; // 18
	}
} mcu32f1_rcc_cfg2_t;

typedef union {
	uint32_t word;
	struct {
		struct {
			unsigned lsi :1; // 0
			unsigned lse :1;   // 1
			unsigned hsi :1;   // 2
			unsigned hse :1;   // 3
			unsigned pll1 :1;   // 4
			unsigned pll2 :1;   // 5
			unsigned pll3 :1;   // 6
			unsigned css :1;   // 7

		} flag;
		struct {
			unsigned lsi :1;   // 8
			unsigned lse :1;   // 9
			unsigned hsi :1;   // 10
			unsigned hse :1;   // 11
			unsigned pll1 :1;   // 12
			unsigned pll2 :1;   // 13
			unsigned pll3 :1;   // 14
			unsigned :1;   // 15
		} enable;
		struct {
			unsigned lsi :1;   // 16
			unsigned lse :1;   // 17
			unsigned hsi :1;   // 18
			unsigned hse :1;   // 19
			unsigned pll1 :1;   // 20
			unsigned pll2 :1;   // 21
			unsigned pll3 :1;   // 22
			unsigned css :1;   // 23
		} clear;
		unsigned :1;   // 24
		unsigned :1;   // 25
		unsigned :1;   // 26
		unsigned :1;   // 27
		unsigned :1;   // 28
		unsigned :1;   // 29
		unsigned :1;   // 30
		unsigned :1;   // 31
	};
} mcu32f1_rcc_ci_t;

typedef union {
	uint32_t word;
	struct {
		unsigned afio :1; // 0
		unsigned und1 :1;   // 1
		unsigned iopa :1;   // 2
		unsigned iopb :1;   // 3
		unsigned iopc :1;   // 4
		unsigned iopd :1;   // 5
		unsigned iope :1;   // 6
		unsigned iopf :1;   // 7
		unsigned iopg :1;   // 8
		unsigned adc1 :1;   // 9
		unsigned adc2 :1;   // 10
		unsigned tim1 :1;   // 11
		unsigned spi1 :1;   // 12
		unsigned tim8 :1;   // 13
		unsigned usart1 :1;   // 14
		unsigned adc3 :1;   // 15
		unsigned und16 :1;   // 16
		unsigned und17 :1;   // 17
		unsigned und18 :1;   // 18
		unsigned tim9 :1;   // 19
		unsigned tim10 :1;   // 20
		unsigned tim11 :1;   // 21
		unsigned und22 :1;   // 22
		unsigned und23 :1;   // 23
		unsigned und24 :1;   // 24
		unsigned und25 :1;   // 25
		unsigned und26 :1;   // 26
		unsigned und27 :1;   // 27
		unsigned und28 :1;   // 28
		unsigned und29 :1;   // 29
		unsigned und30 :1;   // 30
		unsigned und31 :1;   // 31
	}
} mcu32f1_rcc_apb2_t;

typedef union {
	uint32_t word;
	struct {
		unsigned tim2 :1; // 0
		unsigned tim3 :1;   // 1
		unsigned tim4 :1;   // 2
		unsigned tim5 :1;   // 3
		unsigned tim6 :1;   // 4
		unsigned tim7 :1;   // 5
		unsigned tim12 :1;   // 6
		unsigned tim13 :1;   // 7
		unsigned tim14 :1;   // 8
		unsigned und9 :1;   // 9
		unsigned und10 :1;   // 10
		unsigned wwdg :1;   // 11
		unsigned und12 :1;   // 12
		unsigned und13 :1;   // 13
		unsigned spi2 :1;   // 14
		unsigned spi3 :1;   // 15
		unsigned und16 :1;   // 16
		unsigned usart2 :1;   // 17
		unsigned usart3 :1;   // 18
		unsigned usart4 :1;   // 19
		unsigned usart5 :1;   // 20
		unsigned i2c1 :1;   // 21
		unsigned i2c2 :1;   // 22
		unsigned usb :1;   // 23
		unsigned und24 :1;   // 24
		unsigned can1 :1;   // 25
		unsigned can2 :1;   // 26
		unsigned bkp :1;   // 27
		unsigned pwm :1;   // 28
		unsigned dac1 :1;   // 29
		unsigned und30 :1;   // 30
		unsigned und31 :1;   // 31
	}
} mcu32f1_rcc_apb1_t;

typedef union {
	uint32_t word;
	struct {
		unsigned dma1 :1; // 0
		unsigned dma2 :1;   // 1
		unsigned sram :1;   // 2
		unsigned und3 :1;   // 3
		unsigned flitf :1;   // 4
		unsigned und5 :1;   // 5
		unsigned crc :1;   // 6
		unsigned und7 :1;   // 7
		unsigned fsmc :1;   // 8
		unsigned und9 :1;   // 9
		unsigned sdio :1;   // 10
		unsigned und11 :1;   // 11
		unsigned otgfs :1;   // 12
		unsigned und13 :1;   // 13
		unsigned ethmac :1;   // 14
		unsigned ethmactx :1;   // 15
		unsigned ethmacrx :1;   // 16
		unsigned und17 :1;   // 17
		unsigned und18 :1;   // 18
		unsigned und19 :1;   // 19
		unsigned und20 :1;   // 20
		unsigned und21 :1;   // 21
		unsigned und22 :1;   // 22
		unsigned und23 :1;   // 23
		unsigned und24 :1;   // 24
		unsigned und25 :1;   // 25
		unsigned und26 :1;   // 26
		unsigned und27 :1;   // 27
		unsigned und28 :1;   // 28
		unsigned und29 :1;   // 29
		unsigned und30 :1;   // 30
		unsigned und31 :1;   // 31
	}
} mcu32f1_rcc_ahb1_t;

typedef union {
	uint32_t word;
	struct {
		unsigned lse_on :1;
		unsigned lse_rdy :1;
		unsigned lse_byp :1;
		unsigned :5;
		unsigned rtc_sel :2;
		unsigned :5;
		unsigned rtc_en :1;
		unsigned bd_rst :1;
	}
} mcu32f1_rcc_bdc_t;

typedef union {
	uint32_t word;
	struct {
		unsigned lsi_on :1;
		unsigned lsi_rdy :1;
		unsigned :22;
		unsigned rmvf :1;
		unsigned :1;
		unsigned pin_rst :1;
		unsigned por_rst :1;
		unsigned sft_rst :1;
		unsigned iwdg_rst :1;
		unsigned wwdg_rst :1;
		unsigned lpwrrst :1;
	}
} mcu32f1_rcc_cs_t;

typedef struct {
	volatile mcu32f_rcc_cr_t cr;	// 0x00
	volatile mcu32f1_rcc_cfg1_t cfg1;	// 0x04
	volatile mcu32f1_rcc_ci_t ci;	// 0x08
	volatile mcu32f1_rcc_apb2_t apb2_reset;	// 0x0C
	volatile mcu32f1_rcc_apb1_t apb1_reset;	// 0x10
	volatile mcu32f1_rcc_ahb1_t ahb1_enable;	// 0x14
	volatile mcu32f1_rcc_apb2_t apb2_enable;	// 0x18
	volatile mcu32f1_rcc_apb1_t apb1_enable;	// 0x1C
	volatile mcu32f1_rcc_bdc_t bdc;	// 0x20
	volatile mcu32f1_rcc_cs_t cs;	// 0x24
	volatile mcu32f1_rcc_ahb1_t ahb1_reset;	// 0x28
	volatile mcu32f1_rcc_cfg2_t cfg2;	// 0x2C
	// todo  GD32: RCU_DSV
} mcu32f1_rcc_t;
//---------------------------------------------------------------------------
// Flash Memory Controller
// Using the GigaDevice designation FMC rather then FLASH
// as a nicer way to avoid the name clash STM uses FLASH_R
// Note: STM does not document the programming related registers
// in the TRM but refers to a different document,
// the Flash Programming Manual PM0068 and PM0075
//

typedef union {
	volatile uint32_t word;
	struct {
		volatile unsigned latency :3;
		volatile unsigned half_cycle_access :1;
		volatile unsigned prefetch_buffer_enable :1;
		volatile unsigned prefetch_buffer_status :1;
	};
} mcu32f1_fmc_ac_t;

// At this point, we only consider the Access Control Register
// to control the flash latency, as we need this to implement
// clocking the MCU at all supported speeds.

typedef union {
	volatile mcu32f1_fmc_ac_t ac;
} mcu32f1_fmc_t;

//---------------------------------------------------------------------------
typedef union {
	volatile uint32_t word;
	struct {
		unsigned second : 1;
		unsigned alarm : 1;
		unsigned overflow : 1;
		unsigned synchronised : 1;
		unsigned configuration : 1;
		unsigned ready : 1;
	};
} mcu32f1_rtc_if;

typedef union {
	volatile uint32_t word_hi;
	volatile uint32_t word_lo;
	struct {
		volatile unsigned hi : 16;
		volatile unsigned : 16; // padding
		volatile unsigned lo : 16;
		volatile unsigned : 16; // padding
	};
} mcu32f1_rtc_val;


typedef struct {
	volatile mcu32f1_rtc_if interrupt;
	volatile mcu32f1_rtc_if flag;
	volatile mcu32f1_rtc_val preload;
	volatile mcu32f1_rtc_val prediv;
	volatile mcu32f1_rtc_val count;
	volatile mcu32f1_rtc_val alarm;
} mcu32f1_rtc_t;

//---------------------------------------------------------------------------


#pragma pack(pop)

