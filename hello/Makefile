TARGET  = hello
MCU = MCU32F1
LDSCRIPT=link.ld
#LDSCRIPT=nrf5x.ld

C_SOURCES   += main.c
C_SOURCES   += identify.c
CXX_SOURCES += test.cpp
ASM_SOURCES += test.s


ASM_SOURCES   += hardfault.S

C_SOURCES+=$(UCDEV_ROOT)/platform/arm/cpuid.c
C_INCLUDES+=$(UCDEV_ROOT)/platform/arm/

C_SOURCES+=$(UCDEV_ROOT)/platform/mcu32/mcu32rtcv1.c
C_SOURCES+=$(UCDEV_ROOT)/platform/mcu32/mcu32f1peri.c
C_SOURCES+=$(UCDEV_ROOT)/platform/mcu32/mcu32f1flash.c
C_SOURCES+=$(UCDEV_ROOT)/platform/mcu32/mcu32f1rcc.c
C_INCLUDES+=$(UCDEV_ROOT)/platform/mcu32/


C_INCLUDES+=$(UCDEV_ROOT)/ext/RTT/Config
C_INCLUDES+=$(UCDEV_ROOT)/ext/RTT/RTT
C_SOURCES+=$(UCDEV_ROOT)/ext/RTT/RTT/SEGGER_RTT.c
C_SOURCES+=$(UCDEV_ROOT)/ext/RTT/Syscalls/SEGGER_RTT_Syscalls_GCC.c



# C_SOURCES+=$(UCDEV_ROOT)/os/os_nosys.c
# C_INCLUDES+=$(UCDEV_ROOT)/os

UCDEV_ROOT=..
include $(UCDEV_ROOT)/make/all.mk

