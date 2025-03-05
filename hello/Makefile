TARGET  = hello
MCU = STM32F103xB
LDSCRIPT=link.ld

# TODO Automate this
LDFLAGS +=  -L$(UCDEV_ROOT)/lib/arm_m3/size -l hal_stm32f103xb

C_SOURCES+=main.c
CXX_SOURCES += test.cpp

C_SOURCES+=$(UCDEV_ROOT)/startup/startup_cortexm.c
C_INCLUDES+=$(UCDEV_ROOT)/startup

C_SOURCES+=$(UCDEV_ROOT)/os/os_nosys.c
C_INCLUDES+=$(UCDEV_ROOT)/os

UCDEV_ROOT=..
include $(UCDEV_ROOT)/make/all.mk

