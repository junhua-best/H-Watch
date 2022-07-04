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

//ʱ�����ݸ���task
osThreadId_t Data_Task_handler;
const osThreadAttr_t data_task_attributes = {
    .name = "data_task",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 550 * 2
};

//�������������ݸ���task
osThreadId_t Sensor_Task_handler;
const osThreadAttr_t sensor_task_attributes = {
    .name = "sensor_task",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 550 * 2
};

//����Ƿ�ý�������ģʽtask
osThreadId_t Sleep_Task_handler;
const osThreadAttr_t sleep_task_attributes = {
    .name = "sleep_task",
    .priority = (osPriority_t) 25,
    .stack_size = 550 * 2
};


//�������ݼ��
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
    lv_theme_t * theme = lv_theme_mono_init(210,NULL); //���� mono ���� ���ĺ��ٵ�ram
    lv_theme_set_current(theme); //ʹ������;
    app_menu_create(); //�˵�����
    app_menu_tip_create();
    app_setting_create();
    app_about_create(); 
    app_lighting_create();
		LmeterSec_Create();
    app_digital_clock_create();
		app_memo_create();

    // ʹ�õ�ǰʱ���ʼ������xLastWakeTime ,ע�����vTaskDelay()������ͬ
    xLastWakeTime = xTaskGetTickCount();

    for(;;)
    {
        /* ����ϵͳ��ʱ����,����������5ms */
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

        //��ȡrtcʱ��
        get_time();
        ////�ǵ�sprintf�󣬰�������������һλ��Ϊֹͣλ��������label����ʾ��ȫ
        
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
    int mpu6050_try_times=0; //����������Գ�ʼ��5��
    int step_ret=-1;

    for(;;)
    {

        /*
        	if(pre_bat!=get_vbat())
        	{
        		pre_bat= get_vbat();//����; //�仯��Ƶ�ʲ���Ҫ��ȡƽ��ֵ
        		sprintf((char*)bat_now,"%02d",pre_bat);
        		bat_change_flag=1;
        	}
        	*/

        if(pre_bat!=get_vbat())
        {
            pre_bat= get_vbat();//����; //�仯��Ƶ�ʲ���Ҫ��ȡƽ��ֵ
            sprintf((char*)bat_now,"%02d",pre_bat);
            bat_change_flag=1;
            bat_now[2]='\0';
        }
        //��ʪ��
        measure_Si7021();//ÿ���������
        temp=(int)TEMP_buf;
        humi=(int)Humi_buf;
        //printf("�¶�:%.2f ʪ��:%.2f \r\n",TEMP_buf,Humi_buf);
        sprintf((char*)temperature_now,"%02d",temp);
        sprintf((char*)humidity_now,"%02d%",humi);
        temperature_now[2]='\0';
        humidity_now[3]='\0';

        //���� ��ʼ������д�������Ӱ�������������Ķ�ȡ�������ٽ�һ��task��

        printf("MPU6050 init ok ------>\r\n");
        step_ret=dmp_get_pedometer_step_count(&stepcount); //�Ʋ�����ȡ������
        if(step_ret==0)
        {
            sprintf((char*)step_now,"%04lu",stepcount);
        }
        else
        {
            MPU_Init();					//��ʼ��MPU6050
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
            //����û�������³���5s������
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

    // Interface_Set_read(); //��flash�ƺ��ᵼ�²������� ��������˲���л��� ���Ų�.
    //sleep_time_buffer[2] = light_time;
    //updata_sleep_time();

    for(;;)
    {


        //�鿴���������Ĵ���2 �Ƿ���յ�������
        //ΪʲôҪ�ж�����?��ΪЭ������?
			/*
        if(Rx1_Flag)    	// Receive flag
        {
            Rx1_Flag=0;	// clean flag
            HAL_UART_Transmit_DMA(&huart2, Rx1_Buf, Rx1_Len);

        }

        //�鿴���������Ĵ���2 �Ƿ���յ�������
        if(Rx1_Flag)    	// Receive flag
        {
            Rx1_Flag=0;	// clean flag
            HAL_UART_Transmit_DMA(&huart2, Rx1_Buf, Rx1_Len);
        }
*/
        /*
        	����淶���Խ��յ���buffer�ĵ�һ������Ϊ��־λ��
        	�����յ��ĵ�һ���ֽ�Ϊc��Ϊ������������
        	�����յ��ĵ�һ���ֽ�Ϊd��Ϊ�����Ͽ���
        	�����յ��ĵ�һ���ֽ��ǵ��ں�ʱ����Ϊ������������Ϣ���ֻ����͹�����
        	=!!"֪ͨ" ������෢ll��10������
        	=!2"��֪ͨ" �����͵ĵ�1������֪ͨ��Ϣ��֮����2����Ϣ
        	=!1"��֪ͨ" �����͵ĵ�2������֪ͨ��Ϣ��֮����1����Ϣ
        	=!0"��֪ͨ" �����͵ĵ�3������֪ͨ��Ϣ��֮����0����Ϣ
        	=s!"����ʱ��" ʱ���Ų�˳��month day week hours minutes seconds   73 21 06 08 02 10 0E 27
        	=t!t!��!!!!����!! ����Ԥ����Ϣ C7 E7 21 21 21 21 1E 13 B6 E0 D4 C6 21 21 1D 13
        	=m!"����¼����"
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
                printf("�����Ѿ�����\r\n");
                Transient_vibration(); //����ʾ
                break;
            }
            case '=' :
            {
                //HAL_UART_Transmit_DMA(&huart1, usart2_rev_buffer, Rx2_Len);
                switch(buferRX[1])
                {
                case 's':
                {
                    printf("��ȡ����ʱ��\r\n");

                    //   LCD_Draw_flag = 1; //����֪ͨ�������ʾ

                    BLE_time[0] = buferRX[3]; //�·�
                    BLE_time[1] = buferRX[4]; //����
                    BLE_time[2] = buferRX[5]; //����
                    BLE_time[3] = buferRX[6]; //Сʱ
                    BLE_time[4] = buferRX[7]; //����
                    BLE_time[5] = buferRX[8]; //��.

                    printf("%d�� %d�� ����%d %d��%d��%d\r\n",BLE_time[0],BLE_time[1],BLE_time[2],BLE_time[3],BLE_time[4],BLE_time[5]);

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
                    Transient_vibration(); //����ʾ


                    break;
                }
								case 'm' :
								{
										switch (buferRX[2])
										{
											case '1':
												memo_update(1);
											Transient_vibration(); //����ʾ
											break;
											case '2':
												memo_update(2);
											Transient_vibration(); //����ʾ
											break;
											case '3':
												memo_update(3);
											Transient_vibration(); //����ʾ
											break;
											case '4':
												memo_update(4);
											Transient_vibration(); //����ʾ
											break;											
										}
										break;
								}									
                /*
                case 't':
                {
                printf("��ȡ��������\r\n");

                //       LCD_Draw_flag = 1; //����֪ͨ�������ʾ
                //      flash_write_flage = 1; //��ȡ�����豸����Ϣ�����flash�����ı�־λ
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

                printf("����%s ����¶�%d ����¶�%d\r\n",words1, buferRX[9], buferRX[10]);

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


                printf("����%s ����¶�%d ����¶�%d\r\n",words2, buferRX[17], buferRX[18]);

                //HAL_UART_Transmit_DMA(&huart1, buferRX, bufferLen);
                Transient_vibration(); //����ʾ

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
                message_indicate_len = i-3-2; //ȥ�������з�

                break;
                }
                case '0':
                {
                for(i=3; i<bufferLen; i++)
                {
                message_indicate[message_indicate_len+i-3] = buferRX[i];
                }

                for(i=0; i<64; i++) //���㺺�ֲ��ֵĳ��� �õ��ĳ�����ʵ�ʺ��ֳ���*2
                {
                words3[i] = message_indicate[i]; //ת�洿���ֲ���

                if(message_indicate[i] == 0)
                {
                words_len3 = i;
                break;
                }
                }
                //      flash_write_flage = 1; //��ȡ�����豸����Ϣ�����flash�����ı�־λ

                printf("֪ͨ��%s ֪ͨ���ȣ�%d\r\n",words3, words_len3);

                break;
                }
                }
                //    LCD_Draw_flag = 1; //����֪ͨ�������ʾ

                Transient_vibration(); //����ʾ

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
                message_indicate_len = i-3-4; //ȥ�������з�

                break;
                }
                case '0':
                {
                for(i=3; i<bufferLen; i++)
                {
                message_indicate[message_indicate_len+i-3] = buferRX[i];
                }

                for(i=0; i<64; i++) //���㺺�ֲ��ֵĳ��� �õ��ĳ�����ʵ�ʺ��ֳ���*2
                {
                words4[i] = message_indicate[i]; //ת�洿���ֲ���

                if(message_indicate[i] == 0)
                {
                words_len4 = i;
                break;
                }
                }
                //      flash_write_flage = 1; //��ȡ�����豸����Ϣ�����flash�����ı�־λ

                //      LCD_Draw_flag = 1; //����ˢ�µı�־λ

                get_time();

                words_month = sDate.Month; //���汸��¼����
                words_day = sDate.Date;  //���汸��¼����

                printf("momo��%s momo length��%d  %d  %d\r\n",words4, words_len4,words_month, words_day);

                break;
                }
                }
                //    LCD_Draw_flag = 1; //����֪ͨ�������ʾ

                Transient_vibration(); //����ʾ

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
                printf("�Ͽ������Ͽ���\r\n");
                Transient_vibration(); //����ʾ
                break;
            }
            default:
                MX_USART2_UART_Init();
            }
        }

        osDelay(1000);
    }




}