/*
 File:		arm_cpuid.c
 License: 	MIT

 Copyright (c) 2020 André van Schoubroeck

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifdef __ARM_EABI__
#include <stdint.h>
#include <cpuid.h>
#include <system.h>

cpuid_t get_cpuid(void) {
    return *(cpuid_t*) (&SCB->CPUID);
}


cortex_m_romtable_t* get_romtable(void) {
	return (cortex_m_romtable_t*)(0xE00FF000);
}

romtable_pid_t get_romtable_pid(void) {
	romtable_id_t *rid = (romtable_id_t*) 0xE00FFFD0;
	return (romtable_pid_t ) { .pid = rid->pid0 << 0 | rid->pid1 << 8
					| rid->pid2 << 16 | rid->pid3 << 24
					| (uint64_t) rid->pid4 << 32 | (uint64_t) rid->pid5 << 40
					| (uint64_t) rid->pid6 << 48 | (uint64_t) rid->pid7 << 56 } ;
		}

pid32_t get_pid32(void){
	romtable_pid_t pid64 = get_romtable_pid();
	pid32_t pid32 = {
			.partno = pid64.partno,
			.identity_code = pid64.identity_code,
			.jep106_used = pid64.jep106_used,
			.continuation_code = pid64.continuation_code,
	};
	return pid32;
}


#endif
