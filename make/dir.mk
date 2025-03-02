LD_DIR 						?= $(UCDEV_ROOT)/ld
SLIB_ROOT 					?= $(UCDEV_ROOT)/slib

CMSIS_ROOT 					?= $(UCDEV_ROOT)/ext/CMSIS_6/
CMSIS_INC_CORE 				?= $(CMSIS_ROOT)/CMSIS/Core/Include/

NMSIS_ROOT 					?= $(UCDEV_ROOT)/ext/NMSIS
NMSIS_INC_CORE 				?= $(NMSIS_ROOT)/NMSIS/Core/Include/

CUBEF0_HAL_SRC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32f0xx_hal_driver/Src
CUBEF0_HAL_INC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32f0xx_hal_driver/Inc
CUBEF0_CMSIS_DEV      =    $(UCDEV_ROOT)/ext/stm32/cmsis_device_f0
CUBEF0_CMSIS_INC_DEV  =    $(CUBEF0_CMSIS_DEV)/Include/

CUBEF1_HAL_SRC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32f1xx_hal_driver/Src
CUBEF1_HAL_INC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32f1xx_hal_driver/Inc
CUBEF1_CMSIS_DEV      =    $(UCDEV_ROOT)/ext/stm32/cmsis_device_f1
CUBEF1_CMSIS_INC_DEV  =    $(CUBEF1_CMSIS_DEV)/Include/

CUBEF2_HAL_SRC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32f2xx_hal_driver/Src
CUBEF2_HAL_INC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32f2xx_hal_driver/Inc
CUBEF2_CMSIS_DEV      =    $(UCDEV_ROOT)/ext/stm32/cmsis_device_f2
CUBEF2_CMSIS_INC_DEV  =    $(CUBEF2_CMSIS_DEV)/Include/

CUBEF3_HAL_SRC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32f3xx_hal_driver/Src
CUBEF3_HAL_INC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32f3xx_hal_driver/Inc
CUBEF3_CMSIS_DEV      =    $(UCDEV_ROOT)/ext/stm32/cmsis_device_f3
CUBEF3_CMSIS_INC_DEV  =    $(CUBEF3_CMSIS_DEV)/Include/

CUBEF4_HAL_SRC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32f4xx_hal_driver/Src
CUBEF4_HAL_INC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32f4xx_hal_driver/Inc
CUBEF4_CMSIS_DEV      =    $(UCDEV_ROOT)/ext/stm32/cmsis_device_f4
CUBEF4_CMSIS_INC_DEV  =    $(CUBEF4_CMSIS_DEV)/Include/

CUBEF7_HAL_SRC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32f7xx_hal_driver/Src
CUBEF7_HAL_INC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32f7xx_hal_driver/Inc
CUBEF7_CMSIS_DEV      =    $(UCDEV_ROOT)/ext/stm32/cmsis_device_f7
CUBEF7_CMSIS_INC_DEV  =    $(CUBEF7_CMSIS_DEV)/Include/

CUBEL0_HAL_SRC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32l0xx_hal_driver/Src
CUBEL0_HAL_INC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32l0xx_hal_driver/Inc
CUBEL0_CMSIS_DEV      =    $(UCDEV_ROOT)/ext/stm32/cmsis_device_l0
CUBEL0_CMSIS_INC_DEV  =    $(CUBEL0_CMSIS_DEV)/Include/

CUBEL1_HAL_SRC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32l1xx_hal_driver/Src
CUBEL1_HAL_INC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32l1xx_hal_driver/Inc
CUBEL1_CMSIS_DEV      =    $(UCDEV_ROOT)/ext/stm32/cmsis_device_l1
CUBEL1_CMSIS_INC_DEV  =    $(CUBEL1_CMSIS_DEV)/Include/

CUBEL4_HAL_SRC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32l4xx_hal_driver/Src
CUBEL4_HAL_INC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32l4xx_hal_driver/Inc
CUBEL4_CMSIS_DEV      =    $(UCDEV_ROOT)/ext/stm32/cmsis_device_l4
CUBEL4_CMSIS_INC_DEV  =    $(CUBEL4_CMSIS_DEV)/Include/

CUBEL5_HAL_SRC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32l5xx_hal_driver/Src
CUBEL5_HAL_INC_ROOT   =    $(UCDEV_ROOT)/ext/stm32/stm32l5xx_hal_driver/Inc
CUBEL5_CMSIS_DEV      =    $(UCDEV_ROOT)/ext/stm32/cmsis_device_l5
CUBEL5_CMSIS_INC_DEV  =    $(CUBEL5_CMSIS_DEV)/Include/


