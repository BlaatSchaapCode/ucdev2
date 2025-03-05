TARGET  = hello
MCU = STM32F103xB
LDSCRIPT=link.ld

# TODO Automate this
LDFLAGS +=  -L$(UCDEV_ROOT)/lib/arm_m3/size -l hal_stm32f103xb


C_SOURCES += $(UCDEV_ROOT)/ext/stm32/cmsis-device-f1/Source/Templates/system_stm32f1xx.c

C_SOURCES+=main.c
CXX_SOURCES += test.cpp



C_SOURCES+=$(UCDEV_ROOT)/os/os_nosys.c
C_INCLUDES+=$(UCDEV_ROOT)/os

UCDEV_ROOT=..
include $(UCDEV_ROOT)/make/all.mk

