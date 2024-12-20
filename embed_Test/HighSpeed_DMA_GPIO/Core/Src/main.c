/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#undef TIM1_PERIOD
#define TIM1_PERIOD 1-1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern DMA_HandleTypeDef hdma_tim2_up;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  // uint32_t pixelclock[8] = {0x00000000, 0xffffffff, 0xffffffff, 0x0000, 0x00, 0x00, 0xffffffff, 0x00};
   uint32_t pixelclock[16];
   for(int i=0;i<16;i++)
 	  pixelclock[i]=0;
   // the pixelclock goes straight to the BSRR register
   // [31 ... 16] are reset bits corresponding to Ports [15 ... 0]
   // [15 ...  0] are   set bits corresponding to Ports [15 ... 0]
   // if a reset bit is set, the GPIO port will be set LOW
   // if a   set bit is set, the GPIO port will be set HIGH
   pixelclock[ 0] = 0x00000001;  // set SI signal on 1st clock edge
   pixelclock[ 2] = 0x00010000;  // reset SI signal on 3rd clock edge
   pixelclock[ 9] = 0x00000002;  // set the ESH signal high
   pixelclock[13] = 0x00020000;  // set the ESH signal low


   // DMA, circular memory-to-peripheral mode, full word (32 bit) transfer
   HAL_DMA_Start(&hdma_tim2_up,  (uint32_t)pixelclock, (uint32_t)&(GPIOB->BSRR), 16);
   HAL_TIM_Base_Start(&htim2);
   HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_1);
   TIM2->DIER |= (1 << 8);   // set UDE bit (update dma request enable)

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
///**
//  * @brief  DMA conversion complete callback
//  * @note   This function is executed when the transfer complete interrupt
//  *         is generated at each end of DMA buffer
//  * @retval None
//  */
//static void TransferComplete(DMA_HandleTypeDef *DmaHandle)
//{
//  uint32_t tmp_ct = 0;
//  uint32_t tmp_mar = 0;
//
//  tmp_ct = ((hdma_tim2_up.hdma[TIMx_DMA_ID]->Instance->CR) & DMA_SxCR_CT);
//  /* Update memory 0 address location */
//  if (tmp_ct != 0)
//  {
//    tmp_mar = hdma_tim2_up->Instance->CMAR;
//    HAL_DMAEx_ChangeMemory(TimHandlex.hdma[TIMx_DMA_ID], (tmp_mar + (2*DMA_MEM_BUFF_SIZE)), MEMORY0);
//  }
//  /* Update memory 1 address location */
//  else
//  {
//    tmp_mar = hdma_tim2_up.hdma[TIMx_DMA_ID]->Instance->M1AR;
//    HAL_DMAEx_ChangeMemory(TimHandlex.hdma[TIMx_DMA_ID], (tmp_mar + (2*DMA_MEM_BUFF_SIZE)), MEMORY1);
//  }
//
//}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
