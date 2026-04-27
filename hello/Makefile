TARGET  = hello
MCU = MCU32F1
LDSCRIPT=link.ld

C_SOURCES   += main.c
CXX_SOURCES += test.cpp


C_SOURCES+=$(UCDEV_ROOT)/platform/arm/cpuid.c
C_INCLUDES+=$(UCDEV_ROOT)/platform/arm/

C_SOURCES+=$(UCDEV_ROOT)/platform/mcu32/mcu32f1rtc.c
C_SOURCES+=$(UCDEV_ROOT)/platform/mcu32/mcu32f1peri.c
C_SOURCES+=$(UCDEV_ROOT)/platform/mcu32/mcu32f1flash.c
C_INCLUDES+=$(UCDEV_ROOT)/platform/mcu32/


C_SOURCES+=$(UCDEV_ROOT)/os/os_nosys.c
C_INCLUDES+=$(UCDEV_ROOT)/os

UCDEV_ROOT=..
include $(UCDEV_ROOT)/make/all.mk

