TARGET  = hello
ARCH    = arm
SUBARCH = m3
LDSCRIPT=link.ld

C_SOURCES+=main.c

C_SOURCES+=$(UCDEV_ROOT)/startup/startup_cortexm.c
C_INCLUDES+=$(UCDEV_ROOT)/startup

UCDEV_ROOT=..
include $(UCDEV_ROOT)/make/all.mk

