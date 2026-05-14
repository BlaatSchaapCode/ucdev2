#include <stdint.h>

#pragma pack(push,1)
typedef union {
	uint32_t word;
	struct {
		unsigned simd :4;
		unsigned fp32 :4;
		unsigned fp64 :4;
		unsigned trap :4;
		unsigned fdiv :4;
		unsigned fsqr :4;
		unsigned svec :4;
		unsigned frnd :4;
	};
} cm_fpu_mvfr0_t;

#pragma pack(push,1)
typedef union {
	uint32_t word;
	struct {
		// TODO
		unsigned flush_to_zero :4;
		unsigned nan_propagation :4;
		unsigned simd_load_store :4;
		unsigned simd_integer :4;
		unsigned simd_fp32 :4;
		unsigned fp16 :4;
		unsigned fp16_conversion :4;
		unsigned fused_multiply_accumulate :4;
	};
} cm_fpu_mvfr1_t;

#pragma pack(push,1)
typedef union {
	uint32_t word;
	struct {
		unsigned :4;
		unsigned fp_misc :4;
		// TODO
	};
} cm_fpu_mvfr2_t;

typedef struct {
	volatile uint32_t RESERVED0[1U];
	volatile uint32_t FPCCR; /*!< Offset: 0x004 (R/W)  Floating-Point Context Control Register */
	volatile uint32_t FPCAR; /*!< Offset: 0x008 (R/W)  Floating-Point Context Address Register */
	volatile uint32_t FPDSCR; /*!< Offset: 0x00C (R/W)  Floating-Point Default Status Control Register */
	volatile cm_fpu_mvfr0_t MVFR0; /*!< Offset: 0x010 (R/ )  Media and VFP Feature Register 0 */
	volatile cm_fpu_mvfr1_t MVFR1; /*!< Offset: 0x014 (R/ )  Media and VFP Feature Register 1 */
	volatile cm_fpu_mvfr2_t MVFR2; /*!< Offset: 0x018 (R/ )  Media and VFP Feature Register 2 */
} FPU_Type;
#define FPU_BASE            (0xE000EF30)                   /*!< Floating Point Unit */
#define FPU                 ((FPU_Type       *)     FPU_BASE      )   /*!< Floating Point Unit */

#pragma pack(pop)
