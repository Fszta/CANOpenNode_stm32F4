
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "can.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "CANopen.h"
#include "task.h"
#include "CO_SDO.h"

/* Private variables ---------------------------------------------------------*/
CO_SDOclient_t *SDO_C ;
CO_NMT_reset_cmd_t reset = CO_RESET_NOT;
uint32_t SDOabortCode = 0;	
uint32_t timer1ms;
uint32_t ID_SERVER_TO_CLIENT = 0;
uint32_t ID_CLIENT_TO_SERVER = 0;
uint32_t timer1msDiff;

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
 {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM6_Init();
  MX_SPI3_Init();
  MX_I2C1_Init();
  MX_CAN1_Init();
  MX_USART1_UART_Init();

  programStart();
  
  while (reset != CO_RESET_APP)
  {
    static uint32_t timer1msPrevious;
    communicationReset();
        
    /* Switch state */
    reset = CO_RESET_NOT;
    timer1msPrevious = timer1ms;
    
    while (reset == CO_RESET_NOT) 
    {
      SDO_C = CO->SDOclient;
      
      CO_SDOclient_setup(SDO_C, ID_CLIENT_TO_SERVER, ID_SERVER_TO_CLIENT, 0x03);

      /* Add SDO upload and download here */
      
      timer1msDiff = timer1ms - timer1msPrevious;
      timer1msPrevious = timer1ms;
			
      /* Program Async for SDO and NMT messages */
      reset = CO_process(CO, (uint16_t)timer1msDiff, NULL);
    }
  }  
   //CO_LOCK_CAN_SEND();
   //CO_delete((int32_t)(&hcan1));
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
