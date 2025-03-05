## Will be re-created entirely




################################################################################
# MCUs: Determine family of MCU
################################################################################

ifneq (,$(findstring STM32,$(MCU)))
	FAMILY?=STM32
	C_INCLUDES += $(UCDEV_ROOT)/platform/stm32
endif

ifeq ($(FAMILY), STM32)
	ARCH?=arm
	CFLAGS += -DUSBD_LPM_ENABLED -DUSE_HAL_DRIVER

	ifneq (,$(findstring F1,$(MCU)))
		SUBARCH?=m3
		SERIES?=STM32F1
		C_INCLUDES += $(CUBEF1_HAL_INC_ROOT)
		C_INCLUDES += $(CUBEF1_CMSIS_INC_DEV)
		C_INCLUDES += $(UCDEV_ROOT)/platform/stm32/f1
		C_SOURCES  += $(UCDEV_ROOT)/platform/stm32/f1/clocksetup.c
		C_DEFS     += -DSYSTEMINCLUDE=\<stm32f1xx.h\>
		C_DEFS     += -DIRQ_COUNT=64
	endif
endif


ifdef MCU
C_DEFS += -D$(MCU)
endif

ifdef SERIES
C_DEFS += -D$(SERIES)
endif

ifdef SLIB_BLD
C_DEFS += -I$(SLIB_BLD)
endif

ifdef FAMILY
C_DEFS += -D$(FAMILY)
endif
