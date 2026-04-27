/*
 * mcu32f1peri.h
 *
 *  Created on: 5 apr. 2026
 *      Author: andre
 */

#ifndef MCU32_MCU32F1PERI_H_
#define MCU32_MCU32F1PERI_H_

#define FLASH_BASE            0x08000000UL /*!< FLASH base address in the alias region */
#define SRAM_BASE             0x20000000UL /*!< SRAM base address in the alias region */
#define PERIPH_BASE           0x40000000UL /*!< Peripheral base address in the alias region */

/*!< Peripheral memory map */
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)

#define TIM2_BASE             (APB1PERIPH_BASE + 0x00000000UL)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x00000400UL)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x00000800UL)
#define TIM5_BASE             (APB1PERIPH_BASE + 0x00000C00UL)
#define TIM6_BASE             (APB1PERIPH_BASE + 0x00001000UL)
#define TIM7_BASE             (APB1PERIPH_BASE + 0x00001400UL)
#define TIM12_BASE            (APB1PERIPH_BASE + 0x00001800UL)
#define TIM13_BASE            (APB1PERIPH_BASE + 0x00001C00UL)
#define TIM14_BASE            (APB1PERIPH_BASE + 0x00002000UL)

#define RTC_BASE              (APB1PERIPH_BASE + 0x00002800UL)
#define WWDG_BASE             (APB1PERIPH_BASE + 0x00002C00UL)
#define IWDG_BASE             (APB1PERIPH_BASE + 0x00003000UL)

#define SPI2_BASE             (APB1PERIPH_BASE + 0x00003800UL)
#define SPI3_BASE             (APB1PERIPH_BASE + 0x00003C00UL)

#define USART2_BASE           (APB1PERIPH_BASE + 0x00004400UL)
#define USART3_BASE           (APB1PERIPH_BASE + 0x00004800UL)
#define UART4_BASE            (APB1PERIPH_BASE + 0x00004C00UL)
#define UART5_BASE            (APB1PERIPH_BASE + 0x00005000UL)
#define I2C1_BASE             (APB1PERIPH_BASE + 0x00005400UL)
#define I2C2_BASE             (APB1PERIPH_BASE + 0x00005800UL)
#define USB_BASE              (APB1PERIPH_BASE + 0x00005C00UL) /*!< USB_IP Peripheral Registers base address */
#define USB_PMAADDR           (APB1PERIPH_BASE + 0x00006000UL) /*!< USB_IP Packet Memory Area base address */
#define CAN1_BASE             (APB1PERIPH_BASE + 0x00006400UL)
#define CAN2_BASE             (APB1PERIPH_BASE + 0x00006800UL)
#define BKP_BASE              (APB1PERIPH_BASE + 0x00006C00UL)
#define PWR_BASE              (APB1PERIPH_BASE + 0x00007000UL)
#define DAC_BASE              (APB1PERIPH_BASE + 0x00007400UL)


#define AFIO_BASE             (APB2PERIPH_BASE + 0x00000000UL)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x00000400UL)
#define GPIOA_BASE            (APB2PERIPH_BASE + 0x00000800UL)
#define GPIOB_BASE            (APB2PERIPH_BASE + 0x00000C00UL)
#define GPIOC_BASE            (APB2PERIPH_BASE + 0x00001000UL)
#define GPIOD_BASE            (APB2PERIPH_BASE + 0x00001400UL)
#define GPIOE_BASE            (APB2PERIPH_BASE + 0x00001800UL)
#define GPIOF_BASE            (APB2PERIPH_BASE + 0x00001C00UL)
#define GPIOG_BASE            (APB2PERIPH_BASE + 0x00002000UL)
#define ADC1_BASE             (APB2PERIPH_BASE + 0x00002400UL)
#define ADC2_BASE             (APB2PERIPH_BASE + 0x00002800UL)
#define TIM1_BASE             (APB2PERIPH_BASE + 0x00002C00UL)
#define SPI1_BASE             (APB2PERIPH_BASE + 0x00003000UL)
#define TIM8_BASE             (APB2PERIPH_BASE + 0x00003400UL)
#define USART1_BASE           (APB2PERIPH_BASE + 0x00003800UL)
#define ADC3_BASE             (APB2PERIPH_BASE + 0x00003C00UL)

#define TIM9_BASE             (APB2PERIPH_BASE + 0x00004C00UL)
#define TIM10_BASE            (APB2PERIPH_BASE + 0x00005000UL)
#define TIM11_BASE            (APB2PERIPH_BASE + 0x00005400UL)

#define SDIO_BASE             (PERIPH_BASE + 0x00018000UL)



#define DMA1_BASE             (AHB1PERIPH_BASE + 0x00000000UL)
#define DMA1_Channel1_BASE    (AHB1PERIPH_BASE + 0x00000008UL)
#define DMA1_Channel2_BASE    (AHB1PERIPH_BASE + 0x0000001CUL)
#define DMA1_Channel3_BASE    (AHB1PERIPH_BASE + 0x00000030UL)
#define DMA1_Channel4_BASE    (AHB1PERIPH_BASE + 0x00000044UL)
#define DMA1_Channel5_BASE    (AHB1PERIPH_BASE + 0x00000058UL)
#define DMA1_Channel6_BASE    (AHB1PERIPH_BASE + 0x0000006CUL)
#define DMA1_Channel7_BASE    (AHB1PERIPH_BASE + 0x00000080UL)
#define DMA2_BASE             (AHB1PERIPH_BASE + 0x00000400UL)
#define DMA2_Channel1_BASE    (AHB1PERIPH_BASE + 0x00000408UL)
#define DMA2_Channel2_BASE    (AHB1PERIPH_BASE + 0x0000041CUL)
#define DMA2_Channel3_BASE    (AHB1PERIPH_BASE + 0x00000430UL)
#define DMA2_Channel4_BASE    (AHB1PERIPH_BASE + 0x00000444UL)
#define DMA2_Channel5_BASE    (AHB1PERIPH_BASE + 0x00000458UL)

#define RCC_BASE              (AHB1PERIPH_BASE + 0x00001000UL)
#define FMC_BASE          (AHB1PERIPH_BASE + 0x00002000UL) /*!< Flash registers base address */

#define CRC_BASE              (AHB1PERIPH_BASE + 0x00003000UL)

//#define FLASHSIZE_BASE        0x1FFFF7E0UL    /*!< FLASH Size register base address */
//#define UID_BASE              0x1FFFF7E8UL    /*!< Unique device ID register base address */
//#define OB_BASE               0x1FFFF800UL    /*!< Flash Option Bytes base address */
//
//
//#define FSMC_BANK1            (FSMC_BASE)               /*!< FSMC Bank1 base address */
//#define FSMC_BANK1_1          (FSMC_BANK1)              /*!< FSMC Bank1_1 base address */
//#define FSMC_BANK1_2          (FSMC_BANK1 + 0x04000000UL) /*!< FSMC Bank1_2 base address */
//#define FSMC_BANK1_3          (FSMC_BANK1 + 0x08000000UL) /*!< FSMC Bank1_3 base address */
//#define FSMC_BANK1_4          (FSMC_BANK1 + 0x0C000000UL) /*!< FSMC Bank1_4 base address */
//
//#define FSMC_BANK2            (FSMC_BASE + 0x10000000UL)  /*!< FSMC Bank2 base address */
//#define FSMC_BANK3            (FSMC_BASE + 0x20000000UL)  /*!< FSMC Bank3 base address */
//#define FSMC_BANK4            (FSMC_BASE + 0x30000000UL)  /*!< FSMC Bank4 base address */
//
//#define FSMC_BANK1_R_BASE     (FSMC_R_BASE + 0x00000000UL)    /*!< FSMC Bank1 registers base address */
//#define FSMC_BANK1E_R_BASE    (FSMC_R_BASE + 0x00000104UL)    /*!< FSMC Bank1E registers base address */
//#define FSMC_BANK2_3_R_BASE   (FSMC_R_BASE + 0x00000060UL)    /*!< FSMC Bank2/Bank3 registers base address */
//#define FSMC_BANK4_R_BASE     (FSMC_R_BASE + 0x000000A0UL)    /*!< FSMC Bank4 registers base address */
//
#define DBGMCU_BASE          0xE0042000UL /*!< Debug MCU registers base address */

/* USB device FS */


int peripheral_enable(void *p);


#endif /* MCU32_MCU32F1PERI_H_ */
