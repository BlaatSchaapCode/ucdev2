#include <stdio.h>
#include <string.h>

#include <system.h>

#include <stm32f1xx.h>
#include <stm32f1xx_hal.h>


//  TODO: this must move to some system support file for stm32f1
const uint8_t APBPrescTable[8U] =  {0, 0, 0, 0, 1, 2, 3, 4};
uint32_t SystemCoreClock = 8000000;



static UART_HandleTypeDef m_uart;
unsigned char recvbuffer[2048] = { }; 

[[gnu::alias("USART2_IRQHandler")]] void Interrupt38_Handler(void) [[gnu::unused]];
[[gnu::interrupt]] void USART2_IRQHandler(void) {
	HAL_UART_IRQHandler(&m_uart);
}

void RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	HAL_UARTEx_ReceiveToIdle_IT(&m_uart, recvbuffer, sizeof(recvbuffer));
}

void uart_init() {
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	UART_HandleTypeDef *handle = &m_uart;
	handle->Instance = USART2;

	handle->Init.BaudRate = 115200;
	handle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	handle->Init.Mode = UART_MODE_TX_RX;
	handle->Init.OverSampling = UART_OVERSAMPLING_16;
	handle->Init.Parity = UART_PARITY_NONE;
	handle->Init.WordLength = UART_WORDLENGTH_8B;


	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	HAL_UART_Init(handle);
	NVIC_EnableIRQ(USART2_IRQn);
	HAL_UART_RegisterRxEventCallback(handle,RxEventCallback);
	HAL_UARTEx_ReceiveToIdle_IT(handle, recvbuffer, sizeof(recvbuffer));
}

void cpptest(void);

int main() {

	uart_init();
	cpptest();
	while (1);

}
