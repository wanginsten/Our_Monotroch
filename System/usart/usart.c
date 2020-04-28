#include "System.h"
#include "usart.h"
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart6;

uint8_t JY901_Rx[22];
uint8_t cmd_Rx[1];

/* USART1 init function */
void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);
	HAL_UART_Receive_IT(&huart1,cmd_Rx,sizeof(cmd_Rx));
}

/* USART6 init function */
void MX_USART6_UART_Init(void)
{

  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart6);
	HAL_UART_Receive_IT(&huart6,JY901_Rx,sizeof(JY901_Rx));//////////////////////////////

}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART1)
  {
    __HAL_RCC_USART1_CLK_ENABLE();
		
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 2);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
		
  }
  else if(huart->Instance==USART6)
  {
 
    __HAL_RCC_USART6_CLK_ENABLE();
  
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(USART6_IRQn, 2, 1);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
  }

}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

  if(huart->Instance==USART1)
  {
    __HAL_RCC_USART1_CLK_DISABLE();
  
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    HAL_NVIC_DisableIRQ(USART1_IRQn);

  }
  else if(huart->Instance==USART6)
  {
    __HAL_RCC_USART6_CLK_DISABLE();
  
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7);

    HAL_NVIC_DisableIRQ(USART6_IRQn);
  }
}

void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart1);
}

void USART6_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart6);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		HAL_UART_Receive_IT(&huart1,cmd_Rx,sizeof(cmd_Rx));
		cmd_control(cmd_Rx[0]);
		
	}
	if(huart->Instance==USART6)
	{
		HAL_UART_Receive_IT(&huart6,JY901_Rx,sizeof(JY901_Rx));	//用来防止第二次的时候不中断了
		In_Rx_Callback();
	}
}
