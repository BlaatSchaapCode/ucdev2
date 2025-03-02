
################################################################################
# Architectures:	Set the compiler and flags for the architecture
################################################################################
ifeq ($(ARCH), arm)
	COMPILER_TYPE?=gcc
	PREFIX?=arm-none-eabi-
	SPECS ?=  --specs=nosys.specs  --specs=nano.specs 
	C_INCLUDES +=$(CMSIS_INC_CORE)

	ifeq ($(SUBARCH), m0)
			CPU?=	-mcpu=cortex-m0
			FPU?=
			ABI?=	-mfloat-abi=soft
	endif

	ifeq ($(SUBARCH), m0plus)
			CPU?=	-mcpu=cortex-m0plus
			FPU?=
			ABI?=	-mfloat-abi=soft
	endif

	ifeq ($(SUBARCH), m1)
			CPU?=	-mcpu=cortex-m1
			FPU?=
			ABI?=	-mfloat-abi=soft
	endif

	ifeq ($(SUBARCH), m23)
			CPU?=	-mcpu=cortex-m23
			FPU?=	
			ABI?=	-mfloat-abi=soft
	endif

	ifeq ($(SUBARCH), m3)
			CPU?=	-mcpu=cortex-m3
			FPU?=
			ABI?=	-mfloat-abi=soft
	endif

	ifeq ($(SUBARCH), m33)
			CPU?=	-mcpu=cortex-m33+nodsp+nofp
			FPU?=	
			ABI?=	-mfloat-abi=soft
	endif

	ifeq ($(SUBARCH), m33f)
			CPU?=	-mcpu=cortex-m33
			FPU?=	-mfpu=fpv5-sp-d16
			ABI?=	-mfloat-abi=hard
	endif

	ifeq ($(SUBARCH), m35p)
			CPU?=	-mcpu=cortex-m35p
			FPU?=	
			ABI?=	-mfloat-abi=soft
	endif

	ifeq ($(SUBARCH), m35pf)
			CPU?=	-mcpu=cortex-m35p
			FPU?=	-mfpu=fpv5-sp-d16
			ABI?=	-mfloat-abi=hard
	endif

	ifeq ($(SUBARCH), m4)
			CPU?=	-mcpu=cortex-m4
			FPU?=
			ABI?=	-mfloat-abi=soft
	endif

	ifeq ($(SUBARCH), m4f)
			CPU?=	-mcpu=cortex-m4
			FPU?=	-mfpu=fpv4-sp-d16
			ABI?=	-mfloat-abi=hard 
	endif

	ifeq ($(SUBARCH), m55)
			CPU?=	-mcpu=cortex-m55
			FPU?=
			ABI?=	-mfloat-abi=soft
	endif

	ifeq ($(SUBARCH), m55f)
			CPU?=	-mcpu=cortex-m55
			FPU?= 	-mfpu=fpv5-sp-d16
			ABI?=	-mfloat-abi=hard
	endif

	ifeq ($(SUBARCH), m55d)
			CPU?=	-mcpu=cortex-m55
			FPU?= 	-mfpu=fpv5-d16
			ABI?=	-mfloat-abi=hard
	endif

	ifeq ($(SUBARCH), m7)
			CPU?=	-mcpu=cortex-m7
			FPU?=
			ABI?=	-mfloat-abi=soft
	endif

	ifeq ($(SUBARCH), m7f)
			CPU?=	-mcpu=cortex-m7
			FPU?=	-mfpu=fpv5-sp-d16
			ABI?=	-mfloat-abi=hard
	endif

	ifeq ($(SUBARCH), m7d)
			CPU?=	-mcpu=cortex-m7
			FPU?=	-mfpu=fpv5-d16
			ABI?=	-mfloat-abi=hard
	endif

	ifeq ($(SUBARCH), m85)
			CPU?=	-mcpu=cortex-m85
			FPU?=
			ABI?=	-mfloat-abi=soft
	endif

	ifeq ($(SUBARCH), m85f)
			CPU?=	-mcpu=cortex-m85
			FPU?=	-mfpu=fpv5-sp-d16
			ABI?=	-mfloat-abi=hard
	endif

	ifeq ($(SUBARCH), m85d)
			CPU?=	-mcpu=cortex-m85
			FPU?=	-mfpu=fpv5-d16
			ABI?=	-mfloat-abi=hard
	endif

	ifeq ($(SUBARCH), mc1)
			CPU?=	-mcpu=star-mc1
			FPU?=	
			ABI?=	-mfloat-abi=soft
	endif

	ifeq ($(SUBARCH), mc1f)
			CPU?=	-mcpu=star-mc1
			FPU?=	-mfpu=fpv5-sp-d16
			ABI?=	-mfloat-abi=hard
	endif


endif

################################################################################
# Compiler types: Configure the executables for the compiler
################################################################################

ifeq ($(COMPILER_TYPE),gcc)

#$(info ARCH: Configuring GCC)

	AR  = $(PREFIX)ar
#	LD  = $(PREFIX)ld
	LD  = $(PREFIX)gcc
	CC  = $(PREFIX)gcc
	CXX = $(PREFIX)g++
	AS  = $(PREFIX)gcc -x assembler-with-cpp
	CP  = $(PREFIX)objcopy
	SZ  = $(PREFIX)size
	HEX = $(CP) -O ihex
	BIN = $(CP) -O binary -S

	# libraries
#	TODO
#	LIBDIR = -L$(SLIB_DIR) -L$(LD_DIR)
	LIBDIR = -L$(LD_DIR)

#$(info ARCH: OPT IN MCU 	   $(OPT))

	ASFLAGS  += $(foreach d, $(AS_INCLUDES),  -I$d)
	CFLAGS   += $(foreach d, $(C_INCLUDES),   -I$d)
	CXXFLAGS += $(foreach d, $(CXX_INCLUDES), -I$d)

    COMPILEFLAGS += -Wall -fdata-sections -ffunction-sections -fwrapv
	LINKERFLAGS  += -Wl,--gc-sections -Wl,--print-memory-usage


	CSTD ?= gnu23
	CXXSTD ?= gnu++26

	# Flags for assembler, C compiler and linker
	ASFLAGS  += $(CPU) $(FPU) $(ABI) $(AS_DEFS)  $(OPT) 
	CFLAGS   += $(CPU) $(FPU) $(ABI) $(C_DEFS)   $(OPT) $(COMPILEFLAGS) -std=$(CSTD)
	CXXFLAGS += $(CPU) $(FPU) $(ABI) $(CXX_DEFS) $(OPT) $(COMPILEFLAGS) -std=$(CXXSTD)
	LDFLAGS  += $(CPU) $(FPU) $(ABI) $(SPECS) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) $(LINKERFLAGS)

	# Generate dependency information
	CFLAGS +=  -MMD -MP -MF"$(@:%.o=%.d)"
	ASFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
endif

$(info MCU           $(MCU))
$(info Family        $(FAMILY))

$(info Architecture  $(ARCH))
$(info Subarch       $(SUBARCH))
$(info Compile type  $(COMPILER_TYPE))

$(info CC           $(CC))

$(info CPU           $(CPU))
$(info FPU           $(FPU))
$(info ABI           $(ABI))

#$(info ARCH: LIBDIR 	   $(LIBDIR))
#$(info ARCH: ASFLAGS  	   $(ASFLAGS))
#$(info ARCH: CFLAGS 	   $(CFLAGS))
#$(info ARCH: LDFLAGS  	   $(LDFLAGS))
$(info ------------------------------------------------------------------------)
