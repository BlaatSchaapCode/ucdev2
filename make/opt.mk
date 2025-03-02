BUILD_MODE?=debug

ifeq ($(COMPILER_TYPE), gcc)
	ifeq ($(BUILD_MODE), debug)
		OPT ?= -O0 -g3 -gdwarf-2
		CFLAGS += -DDEBUG
	endif

	ifeq ($(BUILD_MODE), size)
		OPT ?= -Os
		CFLAGS += -DRELEASE -DSIZE
	endif

	ifeq ($(BUILD_MODE), speed)
		OPT ?= -O2
		CFLAGS += -DRELEASE -DSPEED
	endif
endif 

$(info BUILD_MODE    $(BUILD_MODE))
#$(info DEBUG: OPT IN OPT 	   		   $(OPT))
