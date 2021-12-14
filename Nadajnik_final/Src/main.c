/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void delay_us (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim21,0);
	while (__HAL_TIM_GET_COUNTER(&htim21) < us);
}

void burst(uint8_t cycle_count)
{
	for(int i = 0; i < cycle_count ; i++ ){
		HAL_GPIO_WritePin(TX_GPIO_Port, TX_Pin, GPIO_PIN_SET);
		delay_us(13);
		HAL_GPIO_WritePin(TX_GPIO_Port, TX_Pin, GPIO_PIN_RESET);
		delay_us(13);
	}
}

void send_symbol(uint8_t bit_count, uint8_t cycle_count){
	for(int i = 0; i < bit_count; i++){
		if(i%2) delay_us((26 * 6 * cycle_count) / 5);
		else burst((6 * cycle_count) / 7);
	}
}

//Global variables

volatile uint8_t button = 1;

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
  MX_ADC_Init();
  MX_TIM2_Init();
  MX_TIM21_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start(&htim21);
  
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);// red
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);// green
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);// blue

  uint16_t Red = 999;
  uint16_t Green = 500;
  uint16_t Blue = 0;

  uint32_t Red_Scaled = 0;
  uint32_t Green_Scaled = 0;
  uint32_t Blue_Scaled = 0;

  uint8_t state = 0;
  uint16_t power = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  while(button){
		  if(Red == 999) state = 1;
		  else if(Green == 999) state = 2;
		  else if(Blue == 999) state = 3;
		  else if(Green == 0) state = 4;

		  if(state == 1){
			  Red = Red - 1;
			  Green = Green + 1;
			  Blue = Blue + 1;
		  }
		  else if(state == 2){
			  Red = Red - 1;
			  Green = Green - 1;
			  Blue = Blue + 1;
		  }
		  else if(state == 3){
			  Red = Red + 1;
			  Green = Green - 1;
			  Blue = Blue - 1;
		  }
		  else if(state == 4){
			  Red = Red + 1;
			  Green = Green + 1;
			  Blue = Blue - 1;
		  }

		  HAL_ADC_Start(&hadc);
		  HAL_ADC_PollForConversion(&hadc,1000);
		  power = HAL_ADC_GetValue(&hadc);

		  if(power < 180){

			  Red_Scaled = 0;
			  Green_Scaled = 0;
			  Blue_Scaled = 0;

		  }
		  else if((power > 200)){

			  Red_Scaled = ( Red * power ) / 4095;
			  Green_Scaled = ( Green * power ) / 4095;
			  Blue_Scaled = ( Blue * power ) / 4095;
			  }

			  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Red_Scaled);
			  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Green_Scaled);
			  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, Blue_Scaled);

		  for(int i=0 ;i<20 ;i++){
			  if(!button) break;
			  HAL_Delay(1);
		  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  }

	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 100);
	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
	  send_symbol(10, 23);
	  HAL_Delay(150);
	  button = 1;

	  for(int i=0 ;i<450 ;i++){
	  			  if(!button) {
	  				  i = 0;
	  				  send_symbol(10, 23);
	  				  HAL_Delay(150);
	  				  button = 1;
	  			  }
	  			HAL_Delay(1);
	  		  }


	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Red_Scaled);
	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Green_Scaled);
	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, Blue_Scaled);


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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	if(GPIO_Pin == Button_Pin){
		button = 0;
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
