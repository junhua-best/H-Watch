/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "lv_port_indev.h"
#include "stdio.h"
#include "stm32l4xx_it.h"
#include "lvgl.h"
#include "key.h"
#include "menu_page.h"
#include "guitest.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "taskfun.h"
#include "SI7021.h"

#include "usart.h"



osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 550 * 2
};
/* Definitions for button_task */
osThreadId_t Button_Task_handler;
const osThreadAttr_t button_task_attributes = {
    .name = "button_task",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 550 * 2
};

//LVGL task
osThreadId_t lv_task_handler_task_handle;
const osThreadAttr_t lv_task_handler_task_attributes = {
    .name = "lv_task_handler_task",
    .priority = (osPriority_t) osPriorityAboveNormal,
    .stack_size = 512 *4
};

//时间数据更新task
osThreadId_t Data_Task_handler;
const osThreadAttr_t data_task_attributes = {
    .name = "data_task",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 550 * 2
};

//其他传感器数据更新task
osThreadId_t Sensor_Task_handler;
const osThreadAttr_t sensor_task_attributes = {
    .name = "sensor_task",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 550 * 2
};

//检测是否该进入休眠模式task
osThreadId_t Sleep_Task_handler;
const osThreadAttr_t sleep_task_attributes = {
    .name = "sleep_task",
    .priority = (osPriority_t) 25,
    .stack_size = 550 * 2
};


//蓝牙数据检测
osThreadId_t Ble_Task_handler;
const osThreadAttr_t ble_task_attributes = {
    .name = "ble_task",
    .priority = (osPriority_t) 26,
    .stack_size = 550 * 2
};
/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Button_task(void *argument);
void lv_task_handler_task( void *argument );
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */
void Data_task(void *argument);
void Sensor_task(void *argument);
void Sleep_task(void *argument);
void Ble_task(void *argument);


/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {

// defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    /* creation of button_task */
    Button_Task_handler = osThreadNew(Button_task, NULL, &button_task_attributes);

    lv_task_handler_task_handle = osThreadNew(lv_task_handler_task, NULL, &lv_task_handler_task_attributes);

    Data_Task_handler = osThreadNew(Data_task, NULL, &data_task_attributes);

    Ble_Task_handler = osThreadNew(Ble_task, NULL, &ble_task_attributes);
    Sleep_Task_handler = osThreadNew(Sleep_task, NULL, &sleep_task_attributes);
    Sensor_Task_handler = osThreadNew(Sensor_task, NULL, &sensor_task_attributes);



    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for(;;)
    {
        task_base();

        //display_test_task();
        //printf("out of task task_base");
        osDelay(1);
    }
    /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Button_task */
/**
* @brief Function implementing the button_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Button_task */
void Button_task(void *argument)
{
    int button=0;
    printf("button=%d",button);
    /* USER CODE BEGIN Button_task */
    /* Infinite loop */
    for(;;)
    {

        button=button_scan();
        LCD_display(button);

        if(button!=0)
            printf("button=%d",button);
        osDelay(100);
    }
    /* USER CODE END Button_task */
}

void lv_task_handler_task( void *argument )
{
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS( 5 );
    lv_theme_t * theme = lv_theme_mono_init(210,NULL); //创建 mono 主题 消耗很少的ram
    lv_theme_set_current(theme); //使用主题;
    app_menu_create(); //菜单创建
    app_menu_tip_create();
    app_setting_create();
    app_about_create(); 
    app_lighting_create();
		LmeterSec_Create();
    app_digital_clock_create();
		app_memo_create();

    // 使用当前时间初始化变量xLastWakeTime ,注意这和vTaskDelay()函数不同
    xLastWakeTime = xTaskGetTickCount();

    for(;;)
    {
        /* 调用系统延时函数,周期性阻塞5ms */
        vTaskDelayUntil( &xLastWakeTime,xPeriod );
        //osDelay(500);
        lv_task_handler();
        /*******/
    }
    vTaskDelete(NULL);
}



int time_change_flag=1,date_change_flag=1,week_change_flag=1,second_change_flag=1,bat_change_flag=1;

int pre_time=-1,pre_date=-1,pre_week=-1,pre_second=-1,pre_bat=-1;
void Data_task(void *argument)
{


    for(;;)
    {

        //获取rtc时间
        get_time();
        ////记得sprintf后，把两个数组的最后一位置为停止位，否则别的label会显示不全
        
        if(pre_date!=sDate.Date)
        {
            sprintf((char*)date_now,"%02d-%02d",sDate.Month,sDate.Date);
            //sprintf((char*)date_now,"%02d",sDate.Date);
            pre_date=sDate.Date;
            date_change_flag=1;
            date_now[5]='\0';
            printf("m=%d date=%d  string=%s \r\n",sDate.Month,sDate.Date,date_now);
        }
        if(pre_time!=sTime.Minutes)
        {
						printf("get in data_task pre_min!=sTime.min \r\n");
            sprintf((char*)time_now,"%02d:%02d",sTime.Hours,sTime.Minutes);
            pre_time=sTime.Minutes;
            time_change_flag=1;
            time_now[5]='\0';

        }
        if(pre_second!=sTime.Seconds)
        {  
            second_change_flag=1;
            pre_second=sTime.Seconds;
            sprintf((char*)second_now,"%02d",sTime.Seconds);
            second_now[2]='\0';
        }

        if(pre_week!=sDate.WeekDay)
        {
            pre_week=sDate.WeekDay;
            week_change_flag=1;
        }



        osDelay(100);
    }

}

unsigned long stepcount=0;
void Sensor_task(void *argument)
{

    int temp=0,humi=0;
    int mpu6050_try_times=0; //最多允许重试初始化5次
    int step_ret=-1;

    for(;;)
    {

        /*
        	if(pre_bat!=get_vbat())
        	{
        		pre_bat= get_vbat();//电量; //变化的频率不需要大，取平均值
        		sprintf((char*)bat_now,"%02d",pre_bat);
        		bat_change_flag=1;
        	}
        	*/

        if(pre_bat!=get_vbat())
        {
            pre_bat= get_vbat();//电量; //变化的频率不需要大，取平均值
            sprintf((char*)bat_now,"%02d",pre_bat);
            bat_change_flag=1;
            bat_now[2]='\0';
        }
        //温湿度
        measure_Si7021();//每秒采样数据
        temp=(int)TEMP_buf;
        humi=(int)Humi_buf;
        //printf("温度:%.2f 湿度:%.2f \r\n",TEMP_buf,Humi_buf);
        sprintf((char*)temperature_now,"%02d",temp);
        sprintf((char*)humidity_now,"%02d%",humi);
        temperature_now[2]='\0';
        humidity_now[3]='\0';

        //步数 初始化函数写在这里会影响其他传感器的读取，考虑再建一个task？

        printf("MPU6050 init ok ------>\r\n");
        step_ret=dmp_get_pedometer_step_count(&stepcount); //计步器获取到步数
        if(step_ret==0)
        {
            sprintf((char*)step_now,"%04lu",stepcount);
        }
        else
        {
            MPU_Init();					//初始化MPU6050
            while(mpu_dmp_init())
            {
                printf("MPU6050 error\r\n");
                mpu6050_try_times++;
                if(	mpu6050_try_times>5)
                {
                    mpu6050_try_times=0;
                    break;
                }
                osDelay(10);
            }
        }
        step_now[4]='\0';

        osDelay(1000); //1s
    }

}


void Sleep_task(void *argument)
{
    int tick_for_sleep=0;

    for(;;)
    {
        if(current_page==Page_Watch)
        {
            //持续没按键按下超过5s就休眠
            while(KEY_1 ==0 && KEY_2==0)
            {
                tick_for_sleep+=10;
                if(tick_for_sleep>=5000)
                {
                    //sleep
                    tick_for_sleep=0;
                    //Checking_into_sleep();
                    low_power_model();

                }
                osDelay(10);
            }
        }
        tick_for_sleep=0;
        osDelay(800);
    }

}

uint8_t  buferRX[200] = {0};
uint8_t  bufferLen = 0;
void Ble_task(void *argument)
{
    int i=0;
    //uint8_t  buferRX[128] = {0};
    //uint8_t  bufferLen = 0;
    uint8_t BLE_time[6] = {0};

    uint8_t message_indicate[64] = {0};
    uint8_t message_indicate_len = 0;
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    // Interface_Set_read(); //读flash似乎会导致不能休眠 并且启动瞬间有花屏 待排查.
    //sleep_time_buffer[2] = light_time;
    //updata_sleep_time();

    for(;;)
    {


        //查看连接蓝牙的串口2 是否接收到了数据
        //为什么要判断两次?因为协议问题?
			/*
        if(Rx1_Flag)    	// Receive flag
        {
            Rx1_Flag=0;	// clean flag
            HAL_UART_Transmit_DMA(&huart2, Rx1_Buf, Rx1_Len);

        }

        //查看连接蓝牙的串口2 是否接收到了数据
        if(Rx1_Flag)    	// Receive flag
        {
            Rx1_Flag=0;	// clean flag
            HAL_UART_Transmit_DMA(&huart2, Rx1_Buf, Rx1_Len);
        }
*/
        /*
        	定义规范：以接收到的buffer的第一个数字为标志位，
        	当接收到的第一个字节为c认为蓝牙连接上了
        	当接收到的第一个字节为d认为蓝牙断开了
        	当接收到的第一个字节是等于号时候，认为有蓝牙数据消息从手机发送过来了
        	=!!"通知" 单次最多发ll送10个汉字
        	=!2"长通知" 代表发送的第1条汉字通知消息，之后还有2调消息
        	=!1"长通知" 代表发送的第2条汉字通知消息，之后还有1调消息
        	=!0"长通知" 代表发送的第3条汉字通知消息，之后还有0调消息
        	=s!"设置时间" 时间排布顺序：month day week hours minutes seconds   73 21 06 08 02 10 0E 27
        	=t!t!晴!!!!多云!! 天气预报消息 C7 E7 21 21 21 21 1E 13 B6 E0 D4 C6 21 21 1D 13
        	=m!"备忘录汉字"
        */
        if(Rx2_Flag)    	// Receive flag
        {


            Rx2_Flag=0;	// clean flag

            for(i=0; i<Rx2_Len; i++)
						//for(i=0; i<100; i++)
            {
                buferRX[i] = Rx2_Buf[i];
            }
            bufferLen = Rx2_Len;

            //HAL_UART_Transmit_DMA(&huart1, Rx2_Buf, Rx2_Len);
						printf("buferRX[0]=%c buferRX=%s rxlen=%d bufflen=%d",buferRX[0],buferRX,Rx2_Len,strlen(buferRX));
            switch(buferRX[0])
            {
							
            case 'c' :
            {
                printf("蓝牙已经连接\r\n");
                Transient_vibration(); //震动提示
                break;
            }
            case '=' :
            {
                //HAL_UART_Transmit_DMA(&huart1, usart2_rev_buffer, Rx2_Len);
                switch(buferRX[1])
                {
                case 's':
                {
                    printf("获取到了时间\r\n");

                    //   LCD_Draw_flag = 1; //更新通知界面的显示

                    BLE_time[0] = buferRX[3]; //月份
                    BLE_time[1] = buferRX[4]; //日期
                    BLE_time[2] = buferRX[5]; //星期
                    BLE_time[3] = buferRX[6]; //小时
                    BLE_time[4] = buferRX[7]; //分钟
                    BLE_time[5] = buferRX[8]; //秒.

                    printf("%d月 %d日 星期%d %d：%d：%d\r\n",BLE_time[0],BLE_time[1],BLE_time[2],BLE_time[3],BLE_time[4],BLE_time[5]);

                    sTime.Hours = BLE_time[3];
                    sTime.Minutes = BLE_time[4];
                    sTime.Seconds = BLE_time[5];
                    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
                    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
                    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
                    {
                        Error_Handler();
                    }
                    sDate.WeekDay = BLE_time[2];
                    sDate.Month = BLE_time[0];
                    sDate.Date = BLE_time[1];
                    sDate.Year = 22;

                    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
                    {
                        Error_Handler();
                    }
                    Transient_vibration(); //震动提示


                    break;
                }
								case 'm' :
								{
										switch (buferRX[2])
										{
											case '1':
												memo_update(1);
											Transient_vibration(); //震动提示
											break;
											case '2':
												memo_update(2);
											Transient_vibration(); //震动提示
											break;
											case '3':
												memo_update(3);
											Transient_vibration(); //震动提示
											break;
											case '4':
												memo_update(4);
											Transient_vibration(); //震动提示
											break;											
										}
										break;
								}									
                /*
                case 't':
                {
                printf("获取到了天气\r\n");

                //       LCD_Draw_flag = 1; //更新通知界面的显示
                //      flash_write_flage = 1; //获取蓝牙设备的信息后更新flash操作的标志位
                if(buferRX[5] == '!')
                {
                words1[0] = buferRX[3];
                words1[1] = buferRX[4];
                words_len1 = 1;
                }
                else if(buferRX[7] == '!')
                {
                words1[0] = buferRX[3];
                words1[1] = buferRX[4];
                words1[2] = buferRX[5];
                words1[3] = buferRX[6];
                words_len1 = 2;
                }
                else
                {
                words1[0] = buferRX[3];
                words1[1] = buferRX[4];
                words1[2] = buferRX[5];
                words1[3] = buferRX[6];
                words1[4] = buferRX[7];
                words1[5] = buferRX[8];
                words_len1 = 3;
                }

                day_1_H = buferRX[9];
                day_1_L = buferRX[10];

                printf("今天%s 最高温度%d 最低温度%d\r\n",words1, buferRX[9], buferRX[10]);

                if(buferRX[13] == '!')
                {
                words2[0] = buferRX[11];
                words2[1] = buferRX[12];
                words_len2 = 1;
                }
                else if(buferRX[15] == '!')
                {
                words2[0] = buferRX[11];
                words2[1] = buferRX[12];
                words2[2] = buferRX[13];
                words2[3] = buferRX[14];
                words_len2 = 2;
                }
                else
                {
                words2[0] = buferRX[11];
                words2[1] = buferRX[12];
                words2[2] = buferRX[13];
                words2[3] = buferRX[14];
                words2[4] = buferRX[15];
                words2[5] = buferRX[16];
                words_len2 = 3;
                }

                day_2_H = buferRX[17];
                day_2_L = buferRX[18];


                printf("明天%s 最高温度%d 最低温度%d\r\n",words2, buferRX[17], buferRX[18]);

                //HAL_UART_Transmit_DMA(&huart1, buferRX, bufferLen);
                Transient_vibration(); //震动提示

                break;
                }
                */
                /*
                case '!':
                {
                switch(buferRX[2])
                {
                case '1':
                {
                for(i=3; i<bufferLen; i++)
                {
                message_indicate[i-3] = buferRX[i];
                }
                message_indicate_len = i-3-2; //去除掉换行符

                break;
                }
                case '0':
                {
                for(i=3; i<bufferLen; i++)
                {
                message_indicate[message_indicate_len+i-3] = buferRX[i];
                }

                for(i=0; i<64; i++) //计算汉字部分的长度 得到的长度是实际汉字长度*2
                {
                words3[i] = message_indicate[i]; //转存纯汉字部分

                if(message_indicate[i] == 0)
                {
                words_len3 = i;
                break;
                }
                }
                //      flash_write_flage = 1; //获取蓝牙设备的信息后更新flash操作的标志位

                printf("通知：%s 通知长度：%d\r\n",words3, words_len3);

                break;
                }
                }
                //    LCD_Draw_flag = 1; //更新通知界面的显示

                Transient_vibration(); //震动提示

                break;
                }
                case '#':
                {
                switch(buferRX[2])
                {
                case '1':
                {
                for(i=3; i<bufferLen; i++)
                {
                message_indicate[i-3] = buferRX[i];
                }
                message_indicate_len = i-3-4; //去除掉换行符

                break;
                }
                case '0':
                {
                for(i=3; i<bufferLen; i++)
                {
                message_indicate[message_indicate_len+i-3] = buferRX[i];
                }

                for(i=0; i<64; i++) //计算汉字部分的长度 得到的长度是实际汉字长度*2
                {
                words4[i] = message_indicate[i]; //转存纯汉字部分

                if(message_indicate[i] == 0)
                {
                words_len4 = i;
                break;
                }
                }
                //      flash_write_flage = 1; //获取蓝牙设备的信息后更新flash操作的标志位

                //      LCD_Draw_flag = 1; //界面刷新的标志位

                get_time();

                words_month = sDate.Month; //保存备忘录日期
                words_day = sDate.Date;  //保存备忘录日期

                printf("momo：%s momo length：%d  %d  %d\r\n",words4, words_len4,words_month, words_day);

                break;
                }
                }
                //    LCD_Draw_flag = 1; //更新通知界面的显示

                Transient_vibration(); //震动提示

                break;
                }
                */
                default:
                    printf("nothing\r\n");
                }
                break;
            }
            case 'd' :
            {
                printf("断开蓝牙断开接\r\n");
                Transient_vibration(); //震动提示
                break;
            }
            default:
                MX_USART2_UART_Init();
            }
        }

        osDelay(1000);
    }




}