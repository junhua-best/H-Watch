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
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stdio.h"


#include "lcd_init.h"
#include "lcd.h"
#include "getword.h"
//#include "pic.h"
//#include "picmain.h"


#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

#include "hmc5883l.h"

#include "SI7021.h"

#include "w25q64.h"

#include "taskfun.h"


#include "string.h"
#include "ymodel.h"
//lvgl相关的库
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_apps\demo\demo.h"

#include "testtaskFun.h" //测试类的函数库 //里面的函数全部都用来做测试
#include "guitest.h"  //lvgl的相关测试
#include "app_menu.h" //菜单目录
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    //具体哪个串口可以更改huart1为其它串口
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
    return ch;
}

void SystemClock_Config(void);
void MX_FREERTOS_Init(void);

int main(void)
{
    /* USER CODE BEGIN 1 */
//#define	test_model
#ifdef test_model
    int i;

    float pitch,roll,yaw; 		//欧拉角
    short aacx,aacy,aacz;		//加速度传感器原始数据
    short gyrox,gyroy,gyroz;	//陀螺仪原始数据
    short temp;					//温度

    float Curent_Angle;

    unsigned char USB_in_SIGN = 0;
    unsigned char KEY_1 = 0;
    unsigned char KEY_2 = 0;

    float ADC_Value_BATTERY = 0;

    int Hp = 0;
    unsigned char GBH = 0x00;
    unsigned char GBL = 0x00;
    unsigned char size = 128;
    int t=0;
#endif


    HAL_Init();


    SystemClock_Config();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_SPI1_Init();
    MX_USART1_UART_Init();
    MX_TIM1_Init();
    MX_I2C1_Init();
    MX_I2C2_Init();
    MX_USART2_UART_Init();
    MX_TIM2_Init();
    MX_RTC_Init();
    MX_ADC1_Init();
    //把 lv_tick_inc(1)放在了 vApplicationTickHook里面 所以tim7不用了
		// MX_TIM7_Init();
    /* USER CODE BEGIN 2 */
    init_task();
    lv_init();						//lvgl系统初始化
    lv_port_disp_init();	//lvgl显示接口初始化,放在lv_init()的后面
    lv_port_indev_init();	//lvgl输入接口初始化,放在lv_init()的后面
    osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
    MX_FREERTOS_Init();
    osKernelStart();
    while(1)
    {

    }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 2;
    //RCC_OscInitStruct.PLL.PLLM = 1;  //改成1才是80m
    RCC_OscInitStruct.PLL.PLLN = 10;
    //RCC_OscInitStruct.PLL.PLLN = 36; //核心板的hse为8m 所以这里设置为40 最后就能得到80m 但实际上配置成40 flash读不出数据，所以设置为35
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1
                                         |RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1
                                         |RCC_PERIPHCLK_I2C2|RCC_PERIPHCLK_ADC;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
    PeriphClkInit.PLLSAI1.PLLSAI1M = 2;
    PeriphClkInit.PLLSAI1.PLLSAI1N = 8; // 8Mhse这里要改成32
    //PeriphClkInit.PLLSAI1.PLLSAI1N = 32;
    PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
    PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure the main internal regulator output voltage
    */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        Error_Handler();
    }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM6) {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{

}

