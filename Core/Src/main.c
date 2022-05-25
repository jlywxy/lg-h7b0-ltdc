/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ltdc.h"
#include "spi.h"
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

/* GPIO PINOUT
 * LCD VCC    -----> PB1
 * LCD VCC2    -----> PB2
 * LCD RESET  -----> PA6 HIGH
 * BOARD LED2 -----> PB4
 * BOARD LED1 -----> PA15
 */

void LCD_SPI_Transmit(uint8_t *data,int length){
    HAL_SPI_Transmit(&hspi4,data,length,1000);
}

void LCD_WriteReg(uint8_t reg){
    uint8_t regStartByte=0b01110100;  //t
    uint8_t dataBuffer[2];
    dataBuffer[0]=regStartByte;
    dataBuffer[1]=reg;
    LCD_SPI_Transmit(dataBuffer,2);
}
void LCD_WriteDat(uint8_t *data,int len){
    uint8_t datStartByte=0b01110110;  //v
    uint8_t dataBuffer[len+1];
    dataBuffer[0]=datStartByte;
    for(int index=0;index<len;index++){
        dataBuffer[index+1]=data[index];
    }
    LCD_SPI_Transmit(dataBuffer,len+1);
}

void LCD_Power_On_Set(){


    // power on set 1
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);  //VCC ON
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);  //VCC ON

    // power on set 2
    HAL_Delay(20);

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);  //RESET start
    HAL_Delay(200);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);  //RESET end

    // power on set 3
    HAL_Delay(20);
}

void LCD_Display_Initial_Set(){

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);  //LED Indicator

    uint8_t data[9]={};
    // display initial set 1-2
    LCD_WriteReg(0xc0);
    data[0]=0x01; data[1]=0x18;
    LCD_WriteDat(data,2);
    // display initial set 3
    HAL_Delay(20);
    // display initial set 4
    LCD_WriteReg(0x20);
    data[0]=0x00;
    LCD_WriteDat(data,1);
    // display initial set 5
    LCD_WriteReg(0x36);
    data[0]=0x00;
    LCD_WriteDat(data,1);
    // display initial set 6
    LCD_WriteReg(0x3a);
    data[0]=0x70;
    LCD_WriteDat(data,1);
    // display initial set 7-9
    LCD_WriteReg(0xb1);
    data[0]=0x12; data[1]=0x1f; data[2]=0x1c;
    LCD_WriteDat(data,3);
    // display initial set 10-11
    LCD_WriteReg(0xb2);
    data[0]=0x20; data[1]=0xc8;
    LCD_WriteDat(data,2);
    // display initial set 12
    LCD_WriteReg(0xb3);
    data[0]=0x00;
    LCD_WriteDat(data,1);
    // display initial set 13
    LCD_WriteReg(0xb4);
    data[0]=0x04;
    LCD_WriteDat(data,1);
    // display initial set 14-18
    LCD_WriteReg(0xb5);
    data[0]=0x12; data[1]=0x0f; data[2]=0x0f; data[3]=0x00; data[4]=0x00;
    LCD_WriteDat(data,5);
    // display initial set 19-24
    LCD_WriteReg(0xb6);
    data[0]=0x03; data[1]=0x18; data[2]=0x02; data[3]=0x40; data[4]=0x10; data[5]=0x33;
    LCD_WriteDat(data,6);
    // display initial set 25
    HAL_Delay(20);
    // display initial set 26-30
    LCD_WriteReg(0xc3);
    data[0]=0x07; data[1]=0x05; data[2]=0x04; data[3]=0x04; data[4]=0x03;
    LCD_WriteDat(data,5);
    // display initial set 31-36
    LCD_WriteReg(0xc4);
    data[0]=0x12; data[1]=0x34; data[2]=0x13; data[3]=0x13; data[4]=0x00; data[5]=0x0c;
    LCD_WriteDat(data,6);
    // display initial set 37
    HAL_Delay(20);
    // display initial set 38
    LCD_WriteReg(0xc5);
    data[0]=0x76;
    LCD_WriteDat(data,1);
    // display initial set 39
    HAL_Delay(20);
    // display initial set 40-42
    LCD_WriteReg(0xc6);
    data[0]=0x23; data[1]=0x50; data[2]=0x00;
    LCD_WriteDat(data,3);
    // display initial set 43
    HAL_Delay(20);
    // display initial set 44-45
    LCD_WriteReg(0xc7);
    data[0]=0x00; data[1]=0xff;
    LCD_WriteDat(data,2);
    // display initial set 53-61
    LCD_WriteReg(0xd0);
    data[0]=0x00; data[1]=0x04; data[2]=0x57; data[3]=0x02; data[4]=0x00; data[5]=0x00; data[6]=0x02; data[7]=0x00; data[8]=0x03;
    LCD_WriteDat(data,9);
    // display initial set 62-70
    LCD_WriteReg(0xd2);
    data[0]=0x00; data[1]=0x04; data[2]=0x57; data[3]=0x02; data[4]=0x00; data[5]=0x00; data[6]=0x02; data[7]=0x00; data[8]=0x03;
    LCD_WriteDat(data,9);
    // display initial set 71-79
    LCD_WriteReg(0xd4);
    data[0]=0x00; data[1]=0x04; data[2]=0x57; data[3]=0x02; data[4]=0x00; data[5]=0x00; data[6]=0x02; data[7]=0x00; data[8]=0x03;
    LCD_WriteDat(data,9);
    // display initial set 80-89
    LCD_WriteReg(0xd1);
    data[0]=0x00; data[1]=0x04; data[2]=0x57; data[3]=0x02; data[4]=0x00; data[5]=0x00; data[6]=0x00; data[7]=0x00; data[8]=0x03;
    LCD_WriteDat(data,9);
    // display initial set 90-98
    LCD_WriteReg(0xd3);
    data[0]=0x00; data[1]=0x04; data[2]=0x57; data[3]=0x02; data[4]=0x00; data[5]=0x00; data[6]=0x00; data[7]=0x00; data[8]=0x03;
    LCD_WriteDat(data,9);
    // display initial set 99-107
    LCD_WriteReg(0xd5);
    data[0]=0x00; data[1]=0x04; data[2]=0x57; data[3]=0x02; data[4]=0x00; data[5]=0x00; data[6]=0x00; data[7]=0x00; data[8]=0x03;
    LCD_WriteDat(data,9);

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);  //LED Indicator
}

void LCD_Sleep_Out_N_Display_On_Set(){

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);  //LED Indicator

    uint8_t data[9]={};
    // sleep out & display on set 1
    LCD_WriteReg(0x11);
    data[0]=0x00;
    LCD_WriteDat(data,1);
    // sleep out & display on set 2
    HAL_Delay(200);
    // sleep out & display on set 3
    LCD_WriteReg(0x29);
    data[0]=0x00;
    LCD_WriteDat(data,1);

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);  //LED Indicator

}
void LCD_Display_Off_N_Sleep_In_Set(){

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);  //LED Indicator

    uint8_t data[9]={};
    // display off & sleep in set 1
    LCD_WriteReg(0x10);
    data[0]=0x00;
    LCD_WriteDat(data,1);
    // display off & sleep in set 2
    LCD_WriteReg(0x28);
    data[0]=0x00;
    LCD_WriteDat(data,1);

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);  //LED Indicator

}

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
  MX_LTDC_Init();
  MX_SPI4_Init();
  /* USER CODE BEGIN 2 */
    HAL_Delay(100);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);  //SPI CS
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);  //LED Indicator
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);  //LED VCC

    HAL_Delay(100);
    LCD_Power_On_Set();
    //HAL_Delay(100);
    LCD_Display_Initial_Set();
    //HAL_Delay(100);
    LCD_Sleep_Out_N_Display_On_Set();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {


      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);  //LED Indicator
      HAL_Delay(500);
      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);  //LED Indicator
      HAL_Delay(500);
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);  //LED Indicator
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
