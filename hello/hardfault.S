.syntax unified
.arch armv6-m

// Currently buildring for armv7-m
// For armv6-m we need a different code for the MSP/PSP checking
// .arch armv7-m

// Note: Setting a register to a value
// eg. setting r0 to 1
// mov r0, #1 works on ARMv7m up
// ldr r0, =1 works on ARMv6m up




// Following goes into flash
.text
.align  4

//------------------------------------------------------------------
.thumb_func
.global HardFault_Handler
HardFault_Handler:


check_stack:
    // Verify the stack is valid
    // Note: We should probably care about PSP vs MSP
    //       but at this point, we have not implemented
    //       any context switching yet so for now, SP is SP.
    mov   r0, sp

    /*
    // From the following blog (referenced in the Gecko SDK)
    // https://blog.frankvh.com/2011/12/07/cortex-m3-m4-hard-fault-handler/
    // The following snippet of code is found
	tst   lr, #4      // test link register for #4
	ite   eq          // if then else (check for equal)
	mrseq r0, msp     // then (if equal)     r0 = msp
	mrsne r0, psp     // else (if not equal) r0 = psp
    //       ARMv6m and ARMv7 TRM specifcy values
    //       0xFFFFFFF1 Handler mode, Main Stack
    //       0xFFFFFFF9 Thread mode, Main Stack
    //       0xFFFFFFFD Thread mode, Process Stack
    //       ARMv8m specifies bits, one of them is
    //       SPSEL, bit [2] 0 = Main , 1 = Process
    //       So this explains testing for the value 4
    */

    // Load the stack values to check against
    ldr   r1, =__StackTop
    ldr   r2, =__StackLimit
    // TODO: Implement a safe stack region
    //       This is also needed when putting that stack
    //       that needs to be initialised in the Reset Handler
    //       such that the Vector Table contains the safe value
    //       and after memory initialisation we set the new
    //       stack top.

   // Perform the range check: r1>r0>r2 (sl>sp>st)
    cmp   r0, r1
    bhi   set_safe_sp // B_HI(unsigned higher) SP > ST
    cmp   r0, r2
    bhi   check_special_hardling // SP > SL

set_safe_sp:
    mov   sp, r1
    ldr   r0, =0 // mov   r0, #0
    b     continue_handfault_handling

check_special_hardling:
    // We check special handling. Some functionality such
    // as try_read32, try_write32 expect a hardfault in
    // their operation, and in such care, the hardfault
    // handler should return to the specified place
    ldr r0, .try_cleanup_ptr
    ldr r0, [r0]

    // If no custom fault handler is installed
    // continue handling the fault
    cmp r0, #0
    beq continue_handfault_handling

    // Patch the stack so we can return from the exception
    // to the custom handling
    str r0, [sp, #0x18]
    bx lr

continue_handfault_handling:
	// for now, loop
	// todo: we can do more analysis into the fault
	//       probably print an error log, or maybe
	//       there are other recovery possibilies
	b continue_handfault_handling
//------------------------------------------------------------------



//------------------------------------------------------------------
.thumb_func
.global try_write32
// C signature
// int try_write32(void* addr, uint32_t val);
// thus r0 = addr, r1 = val
try_write32:
    // Place a pointer to the try_cleanup function into the
    // try_cleanup pointer
    ldr r6, .try_cleanup //
    ldr r5, .try_cleanup_ptr
    str r6, [r5]
    mov r4, r0 // r4 = r0. load address into r4
    ldr r0, =1  // mov r0, #1 // set r0 to 1 (return value for faillure)
    str r1, [r4] // store r1 (value) to address stored in r4

	// Load a value in stead to determine latency
	ldr r0, =2 // dsb
    ldr r0, =3 // dsb
    ldr r0, =4 // dsb
    ldr r0, =5 // dsb

    // If we cannot read the address, a hardfault will
    // occur here. Note the dsb instructions above,
    // hardfauls can be late, especially when writing
    // If no HardFault occurs, the next
    // instruction, setting the return value to 0 is executed,
    // otherwise, code in the HardFault handler will take over.
    // The HardFault handler will check if the try_cleanup_ptr
    // has a non-zero value, and if to, jump there.
    // Hmmm... we don't even need to make it this complicated,
    // can optimise it later.

    ldr r0, =0 // mov r0, #0 // set r0 to 0 (return value for success)

    b try_cleanup
//------------------------------------------------------------------


//------------------------------------------------------------------
.thumb_func
.global try_read32
// C signature
// int try_read32(void* addr, uint32_t *val);
// thus r0 = addr, r1 = val
try_read32:
    // Place a pointer to the try_cleanup function into the
    // try_cleanup pointer
    ldr r6, .try_cleanup
    ldr r5, .try_cleanup_ptr
    str r6, [r5]


    mov r4, r0

    ldr r0, =1 //mov r0, #1 // Set r0 to 1 (return value for faillure)

    ldr r4, [r4] // Load from address r4 points to into r4

    // Load a value in stead to determine latency
    ldr r0, =2 // dsb
    ldr r0, =3 // dsb
    ldr r0, =4 // dsb
    ldr r0, =5 // dsb

    // If we cannot read the address, a hardfault will
    // occur here. Note the dsb instructions above,
    // hardfauls can be late.
    // If no HardFault occurs, the next
    // instruction, setting the return value to 0 is executed,
    // otherwise, code in the HardFault handler will take over.
    // The HardFault handler will check if the try_cleanup_ptr
    // has a non-zero value, and if to, jump there.
    // Hmmm... we don't even need to make it this complicated,
    // can optimise it later.

    ldr r0, =0  //mov r0, #0 // Set r0 to 0 (return value)

    str r4, [r1]
    b try_cleanup
//------------------------------------------------------------------


//------------------------------------------------------------------
.global try_cleanup
try_cleanup:
	// Set the try_cleanup_ptr to 0
    ldr r6, =0 // mov r6, #0
    ldr r5, .try_cleanup_ptr
    str r6, [r5]
    bx lr
//------------------------------------------------------------------



.align 4
.try_cleanup_ptr:
    .word try_cleanup_ptr

.try_cleanup:
    .word try_cleanup

// Following is in RAM
.data
try_cleanup_ptr:
.word 0
