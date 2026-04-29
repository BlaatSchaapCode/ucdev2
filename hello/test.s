/*
 * test.s
 *
 *  Created on: 28 apr. 2026
 *      Author: andre
 */


.global System_UnlockAir
.thumb_func
System_UnlockAir:
	ldr r0,=0x400210F0
	mov r1,#1
	str r1,[r0]
	ldr r2,=0x40016C00
	ldr r3,=0xa7d93a86
	str r3,[r2]
	ldr r3,=0xab12dfcd
	str r3,[r2]
	ldr r3,=0xcded3526
	str r3,[r2]
	ldr r3,=0x200183FF
	str r3,[r2,#0x18] 		@ 32k -> 97k(!)
	ldr r4,=0x4002228c
	ldr r5,=0xa5a5a5a5
	str r5,[r4] 			@//QSPI
	bx lr

.global System_Unlock2514
.thumb_func
System_Unlock2514:
	ldr r0,=0x400210F0
	mov r1,#1
	str r1,[r0]
	ldr r2,=0x40016C00
	ldr r3,=0xA8561D89	@ Enables region of 0x40016C00..0x40016C4F
	str r3,[r2]
	ldr r3,=0x308684D1 	@ Enables region of 0x40016C50..0x40016C8F ?? (Supossedly 64B of ROM)
	str r3,[r2]
	ldr r3,=0xD9A25E3A 	@ Enables region of 0x40016C80..0x40016CFF Enables TRNG and Crypto RCC setting at 0x40016CCC.
	str r3,[r2]
	ldr r3,=0x200183FF
	str r3,[r2,#0x18] 		@ 32k -> 97k(!) but only 64kB available for linker + 1kB at 0x20018000 from code.
	ldr r4,=0x4002228c
	ldr r5,=0xa5a5a5a5
	str r5,[r4] 			@//QSPI
	ldr r4,=0x4002237c
	ldr r5,=0x5a5a5a5a
	str r5,[r4] 			@//More QSPI
	bx lr
