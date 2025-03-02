#include <stdio.h>
#include <string.h>

#include <system.h>
[[gnu::alias("EXTI0_IRQHandler")]] void Interrupt6_Handler(void) [[gnu::unused]];


[[gnu::interrupt]] void EXTI0_IRQHandler(void)  {
	while(1);
}



int main() {

	NVIC_SetPendingIRQ(5);
	NVIC_EnableIRQ(5);
	while (1);

}
