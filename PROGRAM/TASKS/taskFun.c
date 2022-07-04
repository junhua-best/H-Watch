#include "taskfun.h"

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


#include "stdio.h"


#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
#include "picmain.h"


#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

#include "hmc5883l.h"

#include "SI7021.h"

#include "w25q64.h"

#include "ymodel.h"

#include "testtaskFunPictures.h"

#include "stm32l4xx_it.h"

#include "getword.h"

#include "math.h"

#include "testtaskFun.h"
#include "DisplayPrivate.h"

unsigned char LCD_Draw_flag = 1;



/*******************************************
* @function: 	使用STM32板载ADC获取电池电压值，将获取到的电压值储存在全局变量中，打开ADC1通道1 ，对应引脚PA1
* @input:			无
* @return:		输出成为全局变量直接使用
* @author:		田小花
* @time:			2020.10.2
*******************************************/
void init_task(void)
{
//	RTC_AlarmTypeDef sAlarm = {0};
//	RTC_HandleTypeDef RTCHandle;
//
//	HAL_RTC_DeactivateAlarm(&hrtc,RTC_ALARM_A);
//
//	 /* Disable all used wakeup sources*/
//  HAL_RTCEx_DeactivateWakeUpTimer(&RTCHandle);
//	  /* Clear all related wakeup flags */
//  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
//
    printf("begin------------------------------------------------------------->\r\n");

    /*Enable DMA IRQ*/
    HAL_UART_Receive_DMA(&huart1, Rx1_Buf, Rx1_Max);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    printf("usart1 DMA ok ------------------------------------------------>\r\n");

    /*Enable DMA IRQ*/
    HAL_UART_Receive_DMA(&huart2, Rx2_Buf, Rx2_Max);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    printf("usart2 DMA ok ------------------------------------------->\r\n");


    HAL_GPIO_WritePin(SYS_ON_GPIO_Port, SYS_ON_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BLE_ON_GPIO_Port, BLE_ON_Pin, GPIO_PIN_SET);

    //打开W26Q64的片选信号，
    //关闭屏幕的片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
    printf("W26Q64 init ok -------------------------------------->\r\n");
    ReadID();
    printf("readID is %x%x\r\n",W25Q64_ID[3],W25Q64_ID[4]);



    notify_Set_read(); //从flash内读取通知等相关信息
//	/*调节屏幕背光*/
    Interface_Set_read();
    set_screen_light(light_hight);
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //开启定时器PWM通道 定时器2 通道3
//	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 400); //设置PWM的占空比，调节背光
    printf("LCD black init ok ----------------------------->\r\n");


    //关闭W26Q64的片选信号，
    //打开屏幕的片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //低电平为选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); //低电平为选中
    LCD_Init();//LCD初始化
    printf("LCD init ok ---------------->\r\n");
    //LCD_ShowPicture(0,0,240,240,gImage_dahuai); //显示天空背景

    LCD_Fill(0,0,240,240,0xDF3E);


    /*调节震动*/
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //开启定时器PWM通道 定时器2 通道3
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000); //设置PWM的占空比，调节震动强度
    HAL_Delay(60);



    /*调节震动*/
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //开启定时器PWM通道 定时器2 通道3
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0); //设置PWM的占空比，调节震动强度
    printf("motor init ok ------------->\r\n");

    get_time();
    sDate.Year=22;
    printf("get_time %d %d %d 20%d %d %d %d\r\n",sTime.Hours,sTime.Minutes,sTime.Seconds,sDate.Year,sDate.Month,sDate.Date,sDate.WeekDay);



//	MPU_Init();					//初始化MPU6050
//	while(mpu_dmp_init())
//	{
//		printf("MPU6050 error\r\n");
//		HAL_Delay(10);
//	}
//	printf("MPU6050 init ok ------>\r\n");



//	HMC5883_Init();
//	printf("HMC5883 init ok -->\r\n");

    printf(">\r\n");

}

/*******************************************
* @function: 	使用STM32板载ADC获取电池电压值，将获取到的电压值储存在全局变量中，打开ADC1通道1 ，对应引脚PA1
* @input:			无
* @return:		输出成为全局变量直接使用
* @author:		田小花
* @time:			2020.10.2
*******************************************/
RTC_DateTypeDef sDate; //全局变量，外部可以直接调用使用 ，存放日期数据

RTC_TimeTypeDef sTime; //全局变量，外部可以直接调用使用 ，存放时间数据

void get_time(void)
{
    /*时间获取部分*/
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN); //获取时间
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN); //获取日期

}


#define ARRAY_DIM(a) (sizeof(a) / sizeof((a)[0]))


static int Battery_Level_Percent_Table[11] = {3000, 3650, 3700, 3740, 3760, 3795, 3840, 3910, 3980, 4070, 4150};



int toPercentage(int voltage)
{
    int i = 0;
    if(voltage < Battery_Level_Percent_Table[0])
        return 0;

    for(i=0; i<ARRAY_DIM(Battery_Level_Percent_Table); i++) {
        if(voltage < Battery_Level_Percent_Table[i])
            return i*10 - (10UL * (int)(Battery_Level_Percent_Table[i] - voltage)) /
                   (int)(Battery_Level_Percent_Table[i] - Battery_Level_Percent_Table[i-1]);;
    }
    return 99;
}


float ADC_Value_BATTERY_an[10] = {0};
float ADC_Value_BATTERY_bn[10] = {0};



int get_vbat(void)
{
    float ADC_Value_BATTERY = 0;
    unsigned char i=0,j=0;;
    HAL_GPIO_WritePin(GET_VBAT_GPIO_Port, GPIO_PIN_12, GPIO_PIN_SET);//开关

    for(j=0; j<10; j++)
    {
        HAL_ADC_Start(&hadc1);  //打开ADC1

        HAL_ADC_PollForConversion(&hadc1, 15); //

        if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
        {
            ADC_Value_BATTERY = HAL_ADC_GetValue(&hadc1); //读取ADC值存放在ADC_Value_BATTERY中
        }
        ADC_Value_BATTERY = ADC_Value_BATTERY*3.3f/4096;	//电池的电压值


        ADC_Value_BATTERY_an[0] = ADC_Value_BATTERY_an[1];
        ADC_Value_BATTERY_an[1] = ADC_Value_BATTERY_an[2];
        ADC_Value_BATTERY_an[2] = ADC_Value_BATTERY_an[3];
        ADC_Value_BATTERY_an[3] = ADC_Value_BATTERY_an[4];
        ADC_Value_BATTERY_an[4] = ADC_Value_BATTERY_an[5];
        ADC_Value_BATTERY_an[5] = ADC_Value_BATTERY_an[6];
        ADC_Value_BATTERY_an[6] = ADC_Value_BATTERY_an[7];
        ADC_Value_BATTERY_an[7] = ADC_Value_BATTERY_an[8];
        ADC_Value_BATTERY_an[8] = ADC_Value_BATTERY_an[9];
        ADC_Value_BATTERY_an[9] = ADC_Value_BATTERY;
    }

    for(j=0; j<10; j++)
    {
        HAL_ADC_Start(&hadc1);  //打开ADC1

        HAL_ADC_PollForConversion(&hadc1, 15); //

        if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
        {
            ADC_Value_BATTERY = HAL_ADC_GetValue(&hadc1); //读取ADC值存放在ADC_Value_BATTERY中
        }
        ADC_Value_BATTERY = ADC_Value_BATTERY*3.3f/4096;	//电池的电压值


        ADC_Value_BATTERY_an[0] = ADC_Value_BATTERY_an[1];
        ADC_Value_BATTERY_an[1] = ADC_Value_BATTERY_an[2];
        ADC_Value_BATTERY_an[2] = ADC_Value_BATTERY_an[3];
        ADC_Value_BATTERY_an[3] = ADC_Value_BATTERY_an[4];
        ADC_Value_BATTERY_an[4] = ADC_Value_BATTERY_an[5];
        ADC_Value_BATTERY_an[5] = ADC_Value_BATTERY_an[6];
        ADC_Value_BATTERY_an[6] = ADC_Value_BATTERY_an[7];
        ADC_Value_BATTERY_an[7] = ADC_Value_BATTERY_an[8];
        ADC_Value_BATTERY_an[8] = ADC_Value_BATTERY_an[9];
        ADC_Value_BATTERY_an[9] = ADC_Value_BATTERY;

        for(i=0; i<10; i++)
        {
            ADC_Value_BATTERY = ADC_Value_BATTERY_an[i]+ADC_Value_BATTERY;
        }
        ADC_Value_BATTERY_bn[j] = ADC_Value_BATTERY/10;

        //printf("111ADC %.1f  %.1f %.1f \r\n",ADC_Value_BATTERY_bn[j], ADC_Value_BATTERY_bn[j]+0.2 ,(ADC_Value_BATTERY_bn[j]+0.2)*2);
    }
    ADC_Value_BATTERY = 0;
    for(i=0; i<10; i++)
    {
        ADC_Value_BATTERY = ADC_Value_BATTERY_bn[i]+ ADC_Value_BATTERY;
    }
    ADC_Value_BATTERY = ADC_Value_BATTERY/10;

//	printf("ADC %.1f  %.1f %.1f %d\r\n",ADC_Value_BATTERY, ADC_Value_BATTERY+0.2 ,(ADC_Value_BATTERY+0.2)*2, toPercentage(((ADC_Value_BATTERY+0.2)*2)*1000));

    return toPercentage(((ADC_Value_BATTERY+0.2)*2)*1000);
}



/*******************************************
* @function: 	使用RCT的闹钟进行设置闹钟中断，可以用于RTC中断唤醒
* @input:			时分秒
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/

unsigned char hour_sleep = 0;
unsigned char min_sleep = 0;
unsigned char sec_sleep = 0;

void set_Alarm(unsigned char hour, unsigned char min, unsigned char sec)
{
    RTC_AlarmTypeDef sAlarm = {0};
    RTC_HandleTypeDef RTCHandle;

    /* Disable all used wakeup sources*/
    HAL_RTCEx_DeactivateWakeUpTimer(&RTCHandle);

    /* Clear all related wakeup flags */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);


    /*
    * Enable the Alarm A
    */
    sAlarm.AlarmTime.Hours = hour;
    sAlarm.AlarmTime.Minutes = min;
    sAlarm.AlarmTime.Seconds = sec;
    sAlarm.AlarmTime.SubSeconds = 0;
    sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
    sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
    sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
    sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    sAlarm.AlarmDateWeekDay = 1;
    sAlarm.Alarm = RTC_ALARM_A;
    if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }

    hour_sleep = hour;
    min_sleep = min;
    sec_sleep = sec;

    printf("闹钟时间设定在了：%d %d %d\r\n",hour, min, sec);
}


/*******************************************
* @function: 	通过定时器输出PWM方波控制震动的大小
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.21
*******************************************/
void Transient_vibration(void)
{
    /*调节震动*/
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //开启定时器PWM通道 定时器2 通道3
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000); //设置PWM的占空比，调节震动强度
    HAL_Delay(40);

    /*调节震动*/
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //开启定时器PWM通道 定时器2 通道3
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0); //设置PWM的占空比，调节震动强度
}






/*******************************************
* @function: 	低功耗模式函数 使用PA0 wake up 引脚出发内部硬件唤醒复位单片机 低功耗模式下关闭所有IO时钟，设置IO为输入 进入待机模式
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2020.4.26
*******************************************/
 void low_power_model()
{
    RTC_AlarmTypeDef sAlarm = {0};
    GPIO_InitTypeDef GPIO_InitStructure;

//	__HAL_RCC_GPIOB_CLK_ENABLE();
//	__HAL_RCC_GPIOC_CLK_ENABLE();
//	__HAL_RCC_GPIOH_CLK_ENABLE();

//	/* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
//	/* Note: Debug using ST-Link is not possible during the execution of this   */
//	/*       example because communication between ST-link and the device       */
//	/*       under test is done through UART. All GPIO pins are disabled (set   */
//	/*       to analog input mode) including  UART I/O pins.           */
//	GPIO_InitStructure.Pin = GPIO_PIN_All;
//	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//	GPIO_InitStructure.Pull = GPIO_NOPULL;


//	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
//	HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);

//	/* Disable GPIOs clock */

//	__HAL_RCC_GPIOB_CLK_DISABLE();
//	__HAL_RCC_GPIOC_CLK_DISABLE();
//	__HAL_RCC_GPIOH_CLK_DISABLE();


//	//由于使用电容触摸芯片，芯片输出的引脚是高电平，因此设置成为上拉输出模式可以减小IO的电流
//	GPIO_InitStructure.Pin = GPIO_PIN_All;
//	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//	GPIO_InitStructure.Pull = GPIO_NOPULL;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);//
//	__HAL_RCC_GPIOA_CLK_DISABLE();

//	/*  For power consumption's sake, appropriately configure the GPIO corresponding to
//	the wake-up pin, fill up the pull-down control register and set the APC bit. */

//	__HAL_RCC_GPIOA_CLK_ENABLE();
//	GPIO_InitStructure.Pin = GPIO_PIN_0;
//	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStructure.Pull = GPIO_NOPULL;
//	//GPIO_NOPULL
//	//GPIO_PULLDOWN
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

//	HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_A, PWR_GPIO_BIT_0);
//	HAL_PWREx_EnablePullUpPullDownConfig();

//	/* Disable used wakeup source: PWR_WAKEUP_PIN1 */
//	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);

//	/* Clear all related wakeup flags */
//	//__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

//	/* Enable wakeup pin WKUP1 */
//	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1_HIGH);
//	//PWR_WAKEUP_PIN1_LOW
//	//PWR_WAKEUP_PIN1_HIGH

//	/*
//	* Enable the Alarm A
//	*/
//	sAlarm.AlarmTime.Hours = hour_sleep;
//	sAlarm.AlarmTime.Minutes = min_sleep;
//	sAlarm.AlarmTime.Seconds = sec_sleep;
//	sAlarm.AlarmTime.SubSeconds = 0;
//	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
//	sAlarm.AlarmMask = RTC_ALARMMASK_HOURS|RTC_ALARMMASK_MINUTES;
//	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
//	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
//	sAlarm.AlarmDateWeekDay = 1;
//	sAlarm.Alarm = RTC_ALARM_A;
//	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
//	{
//		Error_Handler();
//	}
//
//	/* Enter the Standby mode */
//	HAL_PWR_EnterSTANDBYMode();

    get_time();
    //printf("%d %d %d\r\n",sTime.Hours,sTime.Minutes,sTime.Seconds);

    /* Enable GPIOs clock */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIO_InitStructure.Pin = GPIO_PIN_All;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = GPIO_NOPULL;


    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);

    /* Disable GPIOs clock */

    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();


    //由于使用电容触摸芯片，芯片输出的引脚是高电平，因此设置成为上拉输出模式可以减小IO的电流
    GPIO_InitStructure.Pin = GPIO_PIN_All;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);//
    __HAL_RCC_GPIOA_CLK_DISABLE();

    /* 为节约功耗，适当配置对应的GPIO
    	唤醒引脚，填充下拉控制寄存器并设置APC位 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    //GPIO_NOPULL
    //GPIO_PULLDOWN
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_A, PWR_GPIO_BIT_0);
    HAL_PWREx_EnablePullUpPullDownConfig();

    /* Disable used wakeup source: PWR_WAKEUP_PIN1 */
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);

    /* Clear all related wakeup flags */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

    /* Enable wakeup pin WKUP1 */
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1_HIGH);

//
//		/** Enable the Alarm A
//		*/
//		sAlarm.AlarmTime.Hours = hour_sleep;
//		sAlarm.AlarmTime.Minutes = min_sleep;
//		sAlarm.AlarmTime.Seconds = sec_sleep;
//		sAlarm.AlarmTime.SubSeconds = 0;
//		sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//		sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
//		sAlarm.AlarmMask = RTC_ALARMMASK_HOURS|RTC_ALARMMASK_MINUTES;
//		sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
//		sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
//		sAlarm.AlarmDateWeekDay = 1;
//		sAlarm.Alarm = RTC_ALARM_A;
//		if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
//		{
//			Error_Handler();
//		}
    //	printf("应该唤醒的时间	%d %d %d\r\n",hour_sleep,min_sleep,sec_sleep);

    HAL_PWR_EnterSTANDBYMode();
}





/*******************************************
* @function: 	用于显示主界面上的数字图片函数，从flash内读取到图片的数据，再将数据显示到屏幕上
* @input:			要显示的时间，要显示的图片的位置
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/
void LCD_Print_Time(unsigned char number_time, unsigned char number_location)
{
    //打开W26Q64的片选信号，关闭屏幕的片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);

    switch (number_time)
    {
    case 0 :
        ReadData_4096(Pictures_address_NUM0);//从存储器读取数据到内存数组 rdata 中
        break;
    case 1 :
        ReadData_4096(Pictures_address_NUM1);//从存储器读取数据到内存数组 rdata 中
        break;
    case 2 :
        ReadData_4096(Pictures_address_NUM2);//从存储器读取数据到内存数组 rdata 中
        break;
    case 3 :
        ReadData_4096(Pictures_address_NUM3);//从存储器读取数据到内存数组 rdata 中
        break;
    case 4 :
        ReadData_4096(Pictures_address_NUM4);//从存储器读取数据到内存数组 rdata 中
        break;
    case 5 :
        ReadData_4096(Pictures_address_NUM5);//从存储器读取数据到内存数组 rdata 中
        break;
    case 6 :
        ReadData_4096(Pictures_address_NUM6);//从存储器读取数据到内存数组 rdata 中
        break;
    case 7 :
        ReadData_4096(Pictures_address_NUM7);//从存储器读取数据到内存数组 rdata 中
        break;
    case 8 :
        ReadData_4096(Pictures_address_NUM8);//从存储器读取数据到内存数组 rdata 中
        break;
    case 9 :
        ReadData_4096(Pictures_address_NUM9);//从存储器读取数据到内存数组 rdata 中
        break;
    }

    //打开W26Q64的片选信号，关闭屏幕的片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//低电平选中

    switch (number_location)
    {
    case 0:
        LCD_ShowPicture_no_wite(0,185,40,50,rdata);
        break;
    case 1:
        LCD_ShowPicture_no_wite(40,185,40,50,rdata);
        break;
    case 2:
        LCD_ShowPicture_no_wite(100,185,40,50,rdata);
        break;
    case 3:
        LCD_ShowPicture_no_wite(140,185,40,50,rdata);
        break;
    }
}


/*******************************************
* @function: 	主界面的时间刷新
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/
unsigned char history_time [3] = {0};

unsigned char Begin_Flag  = 1;

void Task_Time(void)
{
    get_time();

    if(history_time[2] != sTime.Seconds || Begin_Flag)
    {
        history_time[2] = sTime.Seconds;

        LCD_ShowIntNum(182,210,sTime.Seconds,2,BLUE,0XFF80,24,0);

        //检测是不是在充电。充电的状态下显示小闪电，没充电时显示电池电量百分比
        if(HAL_GPIO_ReadPin(usb_in_sign_GPIO_Port,usb_in_sign_Pin) == 1)
        {
            LCD_ShowPicture(182+26,216,16,16,gImage_shadian);
        }
        else
        {
            LCD_ShowIntNum(182+26,216,get_vbat(),2,BLACK,0XFF80,16,0); //刷新电池电量
        }


        if(history_time[1] != sTime.Minutes || Begin_Flag)
        {
            history_time[0] = sTime.Hours;
            history_time[1] = sTime.Minutes;

            if(!Begin_Flag) LCD_ShowPicture(3,181,193,59,gImage_shuaxin);

            LCD_Print_Time(sTime.Hours/10,0);
            LCD_Print_Time(sTime.Hours%10,1);
            LCD_ShowPicture_no_wite(80,195,20,30,gImage_maohao);
            LCD_Print_Time(sTime.Minutes/10,2);
            LCD_Print_Time(sTime.Minutes%10,3);

            LCD_ShowIntNum(182,210,sTime.Seconds,2,BLUE,0XFF80,24,0);
            Begin_Flag = 0;
        }
    }
}







/*******************************************
* @function: 	主界面的刷新任务，包括刷新时间和刷新动画
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.22
*******************************************/
void LCD_Draw_Interface_main(void)
{
    if(LCD_Draw_flag == 1)
    {
        LCD_Draw_flag = 0;

        LCD_ShowPicture(0,0,240,240,gImage_dahuai);

        Begin_Flag = 1;
    }

    Task_Time(); //显示时间

    LCD_ShowPicture(62,111,33,33,gImage_dahuaiseye);//闭眼
    osDelay(60);
    LCD_ShowPicture(62,111,33,33,gImage_dahuaiseye2); //睁眼
    osDelay(240);

}









void Interface_mian_read(int add)
{
    //把数据存进FLASH;

    //打开FLASH的片选信号，关闭屏幕片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//低电平选中

    //防止写入数据时错误擦除数据，先将数据取出来，存进数组更新后再将数据存进FLASH内
    ReadData_4096(add);//从存储器读取数据到内存数组 rdata 中

    //打开屏幕的片选信号，关闭W26Q64的片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//低电平选中
}





void LCD_Draw_Interface_main2(void)
{
    uint16_t x = 100;
    uint16_t y = 80;
    uint16_t length = 90;

    uint16_t width = 100;

    //显示界面的初始化//显示初始的图案

    if(LCD_Draw_flag == 1)
    {

        LCD_Draw_flag = 0;
        //Interface_main_write();

        notify_Set_read(); //从flash内读取通知等相关信息

        //打开屏幕的片选信号，关闭W26Q64的片选信号。
        HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //低电平选中
        HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//低电平选中

        //Interface_main_write();
        LCD_Fill(0,0,240,240,0xDF3E);

        LCD_ShowIntNum(70, 50, sTime.Hours, 2, BLACK, 0xDF3E,	32, 0);
        LCD_Fill(110, 60, 115, 65, BLACK);
        LCD_Fill(110, 72, 115, 77, BLACK);
        LCD_ShowIntNum(125, 50, sTime.Minutes, 2, BLACK, 0xDF3E,	32, 0);
        LCD_ShowIntNum(160, 62, sTime.Seconds, 2, BLACK, 0xDF3E,	16, 0);



        LCD_DrawLine(0, 40, 240, 40, BLACK);
        LCD_DrawLine(100, 0, 100, 40, BLACK);
//		LCD_DrawLine(0, 100, 90, 100, BLACK);
//		LCD_DrawLine(90, 100, 90, 180, BLACK);
        LCD_DrawLine(0, 180, 240, 180, BLACK);

        LCD_DrawLine(120+40, 180, 120+40, 210, BLACK);
        LCD_DrawLine(120+40, 210, 120-40, 210, BLACK);
        LCD_DrawLine(120-40, 210, 120-40, 240, BLACK);


        LCD_show_Chinese(6, 9, BLACK, "电池", 2, 24); //显示汉字
        LCD_ShowIntNum(48,8,get_vbat(),2,BLACK,0xDF3E,24,0); //刷新电池电量

        LCD_ShowString(75,8,"%", BLACK, 0xDF3E, 24, 0);

        //LCD_show_Chinese(110, 3, BLACK, "星期一", 3, 32); //显示汉字
        switch (sDate.WeekDay)
        {
        case RTC_WEEKDAY_MONDAY    :
            LCD_show_Chinese(110, 3, BLACK, "星期一", 3, 32);
            break; //显示汉字 break;
        case RTC_WEEKDAY_TUESDAY   :
            LCD_show_Chinese(110, 3, BLACK, "星期二", 3, 32);
            break;//显示汉字 break;
        case RTC_WEEKDAY_WEDNESDAY :
            LCD_show_Chinese(110, 3, BLACK, "星期三", 3, 32);
            break;//显示汉字 break;
        case RTC_WEEKDAY_THURSDAY  :
            LCD_show_Chinese(110, 3, BLACK, "星期四", 3, 32);
            break;//显示汉字 break;
        case RTC_WEEKDAY_FRIDAY    :
            LCD_show_Chinese(110, 3, BLACK, "星期五", 3, 32);
            break;//显示汉字 break;
        case RTC_WEEKDAY_SATURDAY  :
            LCD_show_Chinese(110, 3, BLACK, "星期六", 3, 32);
            break;//显示汉字 break;
        case RTC_WEEKDAY_SUNDAY  	 :
            LCD_show_Chinese(110, 3, BLACK, "星期日", 3, 32);
            break;//显示汉字 break;
        }

//		LCD_show_Chinese(3, 110, BLACK, "晴", 1, 24); //显示汉字
//		LCD_ShowIntNum(5, 140, 32, 2, BLACK, 0xDF3E,	24, 0);
//		LCD_ShowString(30,140,"-", BLACK, 0xDF3E, 24, 0);
//		LCD_ShowIntNum(45, 140, 18, 2, BLACK, 0xDF3E,	24, 0);
//		LCD_show_Chinese(68, 140, BLACK, "℃", 1, 24);

        LCD_show_Chinese(3, 110, BLACK,words1,words_len1,24);
        LCD_ShowIntNum(5, 140, day_1_H, 2, BLACK, 0xDF3E, 24,0);//显示整数变量
        LCD_ShowString(30, 140, "-", BLACK, 0xDF3E, 24, 0);
        LCD_ShowIntNum(45, 140, day_1_L, 2, BLACK, 0xDF3E, 24, 0);//显示整数变量
        LCD_show_Chinese(68, 140, BLACK, "℃", 1, 24);


        LCD_show_Chinese(13, 200, BLACK, "温度", 2, 24); //显示汉字
        LCD_show_Chinese(187, 200, BLACK, "湿度", 2, 24); //显示汉字

        measure_Si7021();//每秒采样数据

        printf("温度:%.2f 湿度:%.2f \r\n",TEMP_buf,Humi_buf);

        LCD_ShowFloatNum1(105-40,180+3,	TEMP_buf-4,4,BLACK,0xDF3E,24,0);//显示整数变量
        LCD_ShowFloatNum1(100,210+3,	Humi_buf,4,BLACK,0xDF3E,24,0);//显示整数变量

        LCD_show_Chinese(120-40+48,180+5,BLACK,"℃",1,24);
        LCD_show_Chinese(110+48,210+5,BLACK,"％",1,24);

    }


    get_time();

    if(history_time[2] != sTime.Seconds )
    {
        history_time[2] = sTime.Seconds;

        LCD_ShowIntNum(160, 62, sTime.Seconds, 2, BLACK, 0xDF3E,	16, 0);

        //检测是不是在充电。充电的状态下显示小闪电，没充电时显示电池电量百分比
        if(HAL_GPIO_ReadPin(usb_in_sign_GPIO_Port,usb_in_sign_Pin) == 1)
        {

        }
        else
        {
            LCD_ShowIntNum(48,8,get_vbat(),2,BLACK,0xDF3E,24,0); //刷新电池电量 消耗150kflash
        }


        if(history_time[1] != sTime.Minutes)
        {
            history_time[0] = sTime.Hours;
            history_time[1] = sTime.Minutes;

            LCD_ShowIntNum(125, 50, sTime.Minutes, 2, BLACK, 0xDF3E,	32, 0);
            LCD_ShowIntNum(70, 50, sTime.Hours, 2, BLACK, 0xDF3E,	32, 0);
        }
    }



    LCD_Fill(110, 60, 115, 65, 0xDF3E);
    LCD_Fill(110, 72, 115, 77, 0xDF3E);

    osDelay(88);
    //显示第11张图片

    LCD_Address_Set(x,y,x+length-1,y+width-1);//分配地址

    Interface_mian_read(Space_Picture_addressK1);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据


    Interface_mian_read(Space_Picture_addressK2);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressK3);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressK4);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressK5);
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//发送数据


    osDelay(88);
    //显示第10张图片
    LCD_Address_Set(x,y,x+length-1,y+width-1); //分配地址

    Interface_mian_read(Space_Picture_addressJ1); //读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressJ2);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressJ3);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressJ4);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressJ5);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//发送数据、


    osDelay(88);
    //显示第9张图片
    LCD_Address_Set(x,y,x+length-1,y+width-1);//分配地址

    Interface_mian_read(Space_Picture_addressI1);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressI2);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressI3);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressI4);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressI5);
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//发送数据、

    LCD_Fill(110, 60, 115, 65, BLACK);
    LCD_Fill(110, 72, 115, 77, BLACK);

    osDelay(88);
    //显示第8张图片
    LCD_Address_Set(x,y,x+length-1,y+width-1); //分配地址

    Interface_mian_read(Space_Picture_addressH1); //读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressH2);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressH3);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressH4);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressH5);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//发送数据、


    osDelay(88);
    //显示第7张图片
    LCD_Address_Set(x,y,x+length-1,y+width-1);//分配地址

    Interface_mian_read(Space_Picture_addressG1);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressG2);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressG3);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressG4);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressG5);
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//发送数据、


    osDelay(88);
    //显示第6张图片
    LCD_Address_Set(x,y,x+length-1,y+width-1); //分配地址

    Interface_mian_read(Space_Picture_addressF1); //读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressF2);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressF3);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressF4);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressF5);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//发送数据、



    osDelay(88);
    //显示第5张图片
    LCD_Address_Set(x,y,x+length-1,y+width-1);//分配地址

    Interface_mian_read(Space_Picture_addressE1);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressE2);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressE3);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressE4);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressE5);
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//发送数据、
    osDelay(88);


    //显示第4张图片
    LCD_Address_Set(x,y,x+length-1,y+width-1); //分配地址

    Interface_mian_read(Space_Picture_addressD1); //读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressD2);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressD3);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressD4);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressD5);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//发送数据、


    osDelay(88);
    //显示第3张图片
    LCD_Address_Set(x,y,x+length-1,y+width-1);//分配地址

    Interface_mian_read(Space_Picture_addressC1);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressC2);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressC3);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressC4);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressC5);
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//发送数据、


    osDelay(88);
    //显示第2张图片
    LCD_Address_Set(x,y,x+length-1,y+width-1); //分配地址

    Interface_mian_read(Space_Picture_addressB1); //读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressB2);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressB3);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressB4);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressB5);//读取图片
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//发送数据


    osDelay(88);
    //显示第一张图片
    LCD_Address_Set(x,y,x+length-1,y+width-1);

    Interface_mian_read(Space_Picture_addressA1);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressA2);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressA3);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressA4);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//发送数据

    Interface_mian_read(Space_Picture_addressA5);
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//发送数据、


}









uint8_t words_len1 = 0; //保存今天的天气信息的长度
uint8_t words_len2 = 0; //保存明天的天气信息的长度
uint8_t words_len3 = 0; //通知信息的长度
uint8_t words_len4 = 0; //通知备忘录的汉字的长度

uint8_t words1[6] = {0}; //保存今天的天气
uint8_t words2[6] = {0}; //保存明天的天气
unsigned char words3[40] = {0}; //间接保存通知信息的内容的buffer
unsigned char words4[40] = {0}; //间接保存备忘录信息的内容的buffer

uint8_t day_1_H = 0;//保存今天的最高气温
uint8_t day_1_L = 0;//保存今天的最低气温
uint8_t day_2_H = 0;//保存明天的最高气温
uint8_t day_2_L = 0;//保存明天的最低气温

uint8_t words_month = 0;//保存备忘录日期
uint8_t words_day = 0;//保存备忘录日期

/*******************************************
* @function: 	写入天气和通知备忘录等信息 从数组的第30位开始保存今天的天气
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/
void notify_Set_write(void)
{
    unsigned char i=0;

    //把数据存进FLASH;

    //打开FLASH的片选信号，关闭屏幕片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//低电平选中

    //防止写入数据时错误擦除数据，先将数据取出来，存进数组更新后再将数据存进FLASH内
    ReadData_4096(Data_address);//从存储器读取数据到内存数组 rdata 中

    rdata[30] = words_len1; //今天的天气数据长度 数值范围为2至6

    rdata[31] = words1[0]; //今天的天气数据 汉字例子 晴 大雨
    rdata[32] = words1[1];
    rdata[33] = words1[2];
    rdata[34] = words1[3];
    rdata[35] = words1[4];
    rdata[36] = words1[5];

    rdata[37] = day_1_H; //今天的最高气温数据
    rdata[38] = day_1_L; //今天的最低气温数据

    rdata[40] = words_len2; //明天的天气数据长度 数值范围为2至6

    rdata[41] = words2[0]; //明天的天气数据 汉字例子 晴 大雨
    rdata[42] = words2[1];
    rdata[43] = words2[2];
    rdata[44] = words2[3];
    rdata[45] = words2[4];
    rdata[46] = words2[5];

    rdata[47] = day_2_H; //明天的最高气温数据
    rdata[48] = day_2_L; //明天的最高气温数据

    rdata[50] = words_len3; //通知信息长度，数值范围在2至40
    for(i=0; i<40; i++)  //保存通知信息
    {
        rdata[51+i] = words3[i];
    }

    rdata[100] = words_len4; //备忘录的汉字双倍的长度，数值范围在2至40
    for(i=0; i<40; i++)  //保存备忘录的汉字GB2312编码数据
    {
        rdata[101+i] = words4[i];
    }

    rdata[150] = words_month; //保存备忘录日期
    rdata[151] = words_day;  //保存备忘录日期

    WriteData_4096(Data_address,rdata);//将读取到的数据写入存储器


    //打开屏幕的片选信号，关闭W26Q64的片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//低电平选中

//	printf("数据写入今天%s 最高温度%d 最低温度%d\r\n",words1, day_1_H, day_1_L);
//	printf("数据写入明天%s 最高温度%d 最低温度%d\r\n",words1, day_2_H, day_2_L);
//	printf("FLASH write notify：%s notify length：%d\r\n",words3, words_len3);
    printf("FLASH write memo：%s memo length：%d\r\n",words4, words_len4);

}





/*******************************************
* @function: 	读取天气和通知备忘录等信息
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/

unsigned char buffer_notify[40] = {0};

void notify_Set_read(void)
{
    unsigned char i=0;

    //把数据存进FLASH;

    //打开FLASH的片选信号，关闭屏幕片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//低电平选中

    //防止写入数据时错误擦除数据，先将数据取出来，存进数组更新后再将数据存进FLASH内
    ReadData_4096(Data_address);//从存储器读取数据到内存数组 rdata 中

    words_len1 = rdata[30];

    words1[0] = 	rdata[31];
    words1[1] = 	rdata[32];
    words1[2] = 	rdata[33];
    words1[3] = 	rdata[34];
    words1[4] = 	rdata[35];
    words1[5] = 	rdata[36];

    day_1_H = 		rdata[37];
    day_1_L = 		rdata[38];

    words_len2 = rdata[40];

    words2[0] = 	rdata[41];
    words2[1] = 	rdata[42];
    words2[2] = 	rdata[43];
    words2[3] = 	rdata[44];
    words2[4] = 	rdata[45];
    words2[5] = 	rdata[46];

    day_2_H =			rdata[47];
    day_2_L = 		rdata[48];

    words_len3 = rdata[50]; //通知信息长度，数值范围在2至40

    for(i=0; i<40; i++)  //保存通知信息
    {
        words3[i] = rdata[51+i];
    }

    words_len4 = rdata[100]; //通知信息长度，数值范围在2至40

    for(i=0; i<40; i++)  //保存通知信息
    {
        words4[i] = rdata[101+i];
    }

    words_month = rdata[150]; //保存备忘录日期
    words_day = rdata[151];  //保存备忘录日期

    //打开屏幕的片选信号，关闭W26Q64的片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//低电平选中
//	printf("数据写入今天%s 最高温度%d 最低温度%d\r\n",words1, day_1_H, day_1_L);
//	printf("数据写入明天%s 最高温度%d 最低温度%d\r\n",words1, day_2_H, day_2_L);
//	printf("FLASH read notify：%s notify length：%d\r\n",words3, words_len3);
    printf("FLASH read memo：%s memo length：%d\r\n",words4, words_len4);
}



/*******************************************
* @function: 	次级界面显示通知信息
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.20
*******************************************/

//unsigned char R_falg = 0;


/*
 void LCD_Draw_Interface_notify(void)
{
	unsigned char i =0;
	unsigned char words_buffer_reve[40] = {0};

	if(LCD_Draw_flag == 1)
	{
		printf("进入了次一级界面\r\n");

		notify_Set_read(); //从flash内读取通知等相关信息

		get_time();

		LCD_Draw_flag = 0;

		//R_falg = 0;

		LCD_Fill(0,0,240,240,LGRAYBLUE);

		//			显示日期部分
		LCD_ShowIntNum(5,6,sDate.Month,2,WHITE,LGRAYBLUE,32,0);//显示整数变量

		LCD_show_Chinese(5+32,6,WHITE,"月",1,32);

		LCD_ShowIntNum(5+32+32,6,sDate.Date,2,WHITE,LGRAYBLUE,32,0);//显示整数变量

		LCD_show_Chinese(5+32+32+32,6,WHITE,"日",1,32);

		switch (sDate.WeekDay)
		{
			case RTC_WEEKDAY_MONDAY    : LCD_show_Chinese(5+32+32+32+32,14,WHITE,"星期一",3,24); break;
			case RTC_WEEKDAY_TUESDAY   : LCD_show_Chinese(5+32+32+32+32,14,WHITE,"星期二",3,24); break;
			case RTC_WEEKDAY_WEDNESDAY : LCD_show_Chinese(5+32+32+32+32,14,WHITE,"星期三",3,24); break;
			case RTC_WEEKDAY_THURSDAY  : LCD_show_Chinese(5+32+32+32+32,14,WHITE,"星期四",3,24); break;
			case RTC_WEEKDAY_FRIDAY    : LCD_show_Chinese(5+32+32+32+32,14,WHITE,"星期五",3,24); break;
			case RTC_WEEKDAY_SATURDAY  : LCD_show_Chinese(5+32+32+32+32,14,WHITE,"星期六",3,24); break;
			case RTC_WEEKDAY_SUNDAY  	 : LCD_show_Chinese(5+32+32+32+32,14,WHITE,"星期日",3,24); break;
		}

		//显示天气部分
		LCD_show_Chinese(5,34,WHITE,"今天",2,24);
		LCD_show_Chinese(5+40,34,WHITE,words1,words_len1,24);

		LCD_ShowIntNum(5,54,day_1_H,2,WHITE,LGRAYBLUE,24,0);//显示整数变量

		LCD_show_Chinese(5+24,54,WHITE,"至",1,24);

		LCD_ShowIntNum(5+24+24,54,day_1_L,2,WHITE,LGRAYBLUE,24,0);//显示整数变量

		LCD_show_Chinese(5+24+24+24,54,WHITE,"℃",1,24);


		LCD_show_Chinese(110,34,WHITE,"明天",2,24);
		LCD_show_Chinese(110+40,34,WHITE,words2,words_len2,24);

		LCD_ShowIntNum(110,54,day_2_H,2,WHITE,LGRAYBLUE,24,0);//显示整数变量

		LCD_show_Chinese(110+24,54,WHITE,"至",1,24);

		LCD_ShowIntNum(110+24+24,54,day_2_L,2,WHITE,LGRAYBLUE,24,0);//显示整数变量

		LCD_show_Chinese(110+24+24+24,54,WHITE,"℃",1,24);

		//显示计步器步数



		//	显示消息部分

		LCD_show_Chinese(18,96,WHITE,"消息通知：",5,24);

		//每行最多十个字
		if(words_len3 > 20)
		{
			for(i=0; i<20; i++)
			{
				words_buffer_reve[i] = words3[i];
			}
			LCD_show_Chinese(18,126,WHITE,words_buffer_reve,10,24);

			for(i=20; i<words_len3; i++)
			{
				words_buffer_reve[i-20] = words3[i];
			}
			LCD_show_Chinese(18,150,WHITE,words_buffer_reve,(words_len3-20)/2,24);
		}
		else
		{
			for(i=0; i<words_len3; i++)
				{
					words_buffer_reve[i] = words3[i];
				}
				LCD_show_Chinese(18,126,WHITE,words_buffer_reve,(words_len3-20)/2,24);
		}
	}

	osDelay(300);
}
*/

//拿来测试的  不用flash
void LCD_Draw_Interface_notify(void)
{
    unsigned char i =0;

    if(LCD_Draw_flag == 1)
    {
        printf("进入了次一级界面\r\n");


        get_time();
        LCD_Draw_flag = 0;

        //R_falg = 0;

        LCD_Fill(0,0,240,240,LGRAYBLUE);
        //LCD_ShowIntNum(5,54,day_1_H,2,WHITE,LGRAYBLUE,24,0);//显示整数变量

    }
    osDelay(300);
}

/*******************************************
* @function: 	次二级界面，显示备忘录界面
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/

//测试 不用flash
void LCD_Draw_Interface_Memo(void)
{
    unsigned char i =0;

    unsigned char words_buffer_reve[40] = {0};

    if(LCD_Draw_flag == 1)
    {
        LCD_Draw_flag = 0;

        printf("显示备忘录界面\r\n");

        //	notify_Set_read(); //从flash内读取通知等相关信息

        LCD_Fill(0,0,240,240,YELLOW);

        LCD_show_Chinese(18,10,BLACK,"备忘录",3,32);

        LCD_DrawLine(20,104,220,104,RED);
        LCD_DrawLine(20,144,220,144,RED);
        LCD_DrawLine(105,204,220,204,RED);



    }
    osDelay(300);
}
/*
void LCD_Draw_Interface_Memo(void)
{
unsigned char i =0;

unsigned char words_buffer_reve[40] = {0};

if(LCD_Draw_flag == 1)
{
	LCD_Draw_flag = 0;

	printf("显示备忘录界面\r\n");

	notify_Set_read(); //从flash内读取通知等相关信息

	LCD_Fill(0,0,240,240,YELLOW);

	LCD_show_Chinese(18,10,BLACK,"备忘录",3,32);

	LCD_DrawLine(20,104,220,104,RED);
	LCD_DrawLine(20,144,220,144,RED);
	LCD_DrawLine(105,204,220,204,RED);

	//每行最多十个字
	if(words_len4 > 20)
	{
		for(i=0; i<20; i++)
		{
			words_buffer_reve[i] = words4[i];
		}
		LCD_show_Chinese(18,80,BLACK,words_buffer_reve,10,24); //显示第一行汉字

		for(i=20; i<words_len4; i++)
		{
			words_buffer_reve[i-20] = words4[i];
		}
		LCD_show_Chinese(18,120,BLACK,words_buffer_reve,(words_len4-20)/2,24);	//显示第二行汉字
	}

	LCD_ShowIntNum(120,180,words_month,2,BLACK,WHITE,24,1);

	LCD_show_Chinese(120+24,180,BLACK,"月",1,24);

	LCD_ShowIntNum(120+24+20,180,words_day,2,BLACK,WHITE,24,1);

	LCD_show_Chinese(120+24+20+24,180,BLACK,"日",1,24);

}
osDelay(300);
}
*/




/*******************************************
* @function: 	次级界面，显示指南针
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/
float pitch,roll,yaw; 		//欧拉角
float hyaw; 		//欧拉角
short aacx,aacy,aacz;		//加速度传感器原始数据
short gyrox,gyroy,gyroz;	//陀螺仪原始数据
short temp;					//温度
#define PI 3.14159265
float Curent_Angle =0;

void LCD_Draw_Interface_compass(void)
{
//	int x,y;
//	int hx,hy;
    if(LCD_Draw_flag == 1)
    {
        LCD_Draw_flag = 0;

        //LCD_Fill(0,0,240,240,LGRAYBLUE);
        LCD_Fill(0,0,240,240,WHITE);

        Draw_Circle(120,100,70,BLACK);
        Draw_Circle(120,100,71,BLACK);
        Draw_Circle(120,100,72,BLACK);


        //LCD_DrawLine(120,100,150,160,RED);

        Draw_Circle(120,100,2,BLACK);
        Draw_Circle(120,100,4,BLACK);

        LCD_show_Chinese(80,180,BLACK,"东：",2,32);

        LCD_ShowIntNum(128,180,0,3,BLACK,WHITE,32,1);

        MPU_Init();					//初始化MPU6050

        while(mpu_dmp_init())
        {
            printf("MPU6050 error\r\n");
            HAL_Delay(10);
        }
        printf("MPU6050 init ok ------>\r\n");

    }



    if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
    {
        temp=MPU_Get_Temperature();	//得到温度值

        MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据

//			printf("%d %d %d \r\n",aacx,aacy,aacz);

        MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据

        printf("陀螺仪数据 %.1f %.1f %.1f \r\n",pitch,roll,yaw);
    }

    if(yaw<0)
    {
        LCD_ShowIntNum(128,180,360+(int)yaw,3,BLACK,WHITE,32,1);
    }
    else
    {
        LCD_ShowIntNum(128,180,(int)yaw,3,BLACK,WHITE,32,1);
    }


    if(yaw<90 && yaw>0 && hyaw!=yaw)
    {
        hyaw = yaw;
//		x = sin(hyaw*PI/180)*65;
//		y = cos(hyaw*PI/180)*65;
//		LCD_DrawLine(120,100,120+(int)x,100-(int)y,WHITE); //清除历史
//
//		x = sin(yaw*PI/180)*65;
//		y = cos(yaw*PI/180)*65;
//		LCD_DrawLine(120,100,120+(int)x,100-(int)y,RED);

    }


    //读取指南针角度
//	Curent_Angle = HMC5883_Get_Angle();
//
//	printf("指南针 %.0f %d %d \r\n",Curent_Angle,HMC_X,HMC_Y,HMC_Z);

    osDelay(10);

    //LCD_DrawLine(120,100,120+(int)hx,100-(int)hy,WHITE); //清除历史
}




/*******************************************
* @function: 	次级界面的温湿度信息显示界面
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/
unsigned char history_time_seconds=0;

void LCD_Draw_Interface_informations(void)
{
    if(LCD_Draw_flag == 1)
    {
        LCD_Draw_flag = 0;

        LCD_Fill(0,0,240,240,BLACK);

        LCD_show_Chinese(5,60,WHITE,"温",1,32);
        LCD_show_Chinese(5,140,WHITE,"湿",1,32);

        LCD_DrawLine(40,40,40,200,WHITE);
        LCD_DrawLine(40,40,200,40,WHITE);

        LCD_DrawLine(40,200,40+6,200-6,WHITE);
        LCD_DrawLine(40,200,40-6,200-6,WHITE);

        LCD_DrawLine(200,40,200-6,40-6,WHITE);
        LCD_DrawLine(200,40,200-6,40+6,WHITE);


        LCD_DrawRectangle(45, 58, 180, 62+32,LGRAY);
        LCD_DrawRectangle(45, 138,180, 142+32,LGRAY);

        LCD_Fill(45+2, 58+2, 180-1, 62+32-1,GREEN);
        LCD_Fill(45+2, 138+2,180-1, 142+32-1,BLUE);

        LCD_show_Chinese(45+5+48+12,64,WHITE,"℃",1,24);
        LCD_show_Chinese(45+5+48+12,144,WHITE,"％",1,24);
    }

    get_time();

    if(	history_time_seconds != sTime.Seconds)
    {
        history_time_seconds = sTime.Seconds;

        measure_Si7021();//每秒采样数据

        printf("温度:%.2f 湿度:%.2f \r\n",TEMP_buf,Humi_buf);

        LCD_ShowFloatNum1(45+5,64,	TEMP_buf,4,WHITE,GREEN,24,0);//显示整数变量
        LCD_ShowFloatNum1(45+5,144,	Humi_buf,4,WHITE,BLUE,24,0);//显示整数变量
    }
    osDelay(300);
}




/*******************************************
* @function: 	次级界面的番茄钟的圆圈形状刷新显示图案
* @input:			数字背景颜色，分钟，秒钟
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/

int LCD_Draw_Interface_tomato_i=0;

void LCD_Draw_Interface_tomato_Print(uint32_t color, uint8_t stime_min, uint8_t stime_sec)
{

    LCD_Draw_Interface_tomato_i++;

    if(LCD_Draw_Interface_tomato_i>19)
    {
        LCD_Draw_Interface_tomato_i=0;
    }
    LCD_ShowIntNum(120-24-12,100-12,stime_min,2,RED,color,32,0);//刷新图像中的计时时间的分钟数字。

    LCD_ShowIntNum(120+12,100-12,stime_sec,2,RED,color,32,0);//刷新图像中的计时时间的秒钟数字。


    switch(LCD_Draw_Interface_tomato_i)
    {
    case 0:
        Draw_Circle(120,20,4,RED);
        Draw_Circle(120,20,3,RED);
        Draw_Circle(120,20,2,RED);
        Draw_Circle(120-24,100-76,4,YELLOW);
        Draw_Circle(120-24,100-76,3,YELLOW);
        Draw_Circle(120-24,100-76,2,YELLOW);
        break;


    case 1:
        Draw_Circle(120+24,100-76,4,RED);
        Draw_Circle(120+24,100-76,3,RED);
        Draw_Circle(120+24,100-76,2,RED);
        Draw_Circle(120,20,4,YELLOW);
        Draw_Circle(120,20,3,YELLOW);
        Draw_Circle(120,20,2,YELLOW);
        break;

    case 2:
        Draw_Circle(120+47,100-64,4,RED);
        Draw_Circle(120+47,100-64,3,RED);
        Draw_Circle(120+47,100-64,2,RED);
        Draw_Circle(120+24,100-76,4,YELLOW);
        Draw_Circle(120+24,100-76,3,YELLOW);
        Draw_Circle(120+24,100-76,2,YELLOW);
        break;

    case 3:
        Draw_Circle(120+64,100-47,4,RED);
        Draw_Circle(120+64,100-47,3,RED);
        Draw_Circle(120+64,100-47,2,RED);
        Draw_Circle(120+47,100-64,4,YELLOW);
        Draw_Circle(120+47,100-64,3,YELLOW);
        Draw_Circle(120+47,100-64,2,YELLOW);
        break;

    case 4:
        Draw_Circle(120+76,100-24,4,RED);
        Draw_Circle(120+76,100-24,3,RED);
        Draw_Circle(120+76,100-24,2,RED);
        Draw_Circle(120+64,100-47,4,YELLOW);
        Draw_Circle(120+64,100-47,3,YELLOW);
        Draw_Circle(120+64,100-47,2,YELLOW);
        break;


    case 5:
        Draw_Circle(200,100,4,RED);
        Draw_Circle(200,100,3,RED);
        Draw_Circle(200,100,2,RED);
        Draw_Circle(120+76,100-24,4,YELLOW);
        Draw_Circle(120+76,100-24,3,YELLOW);
        Draw_Circle(120+76,100-24,2,YELLOW);
        break;


    case 6:
        Draw_Circle(120+76,100+24,4,RED);
        Draw_Circle(120+76,100+24,3,RED);
        Draw_Circle(120+76,100+24,2,RED);
        Draw_Circle(200,100,4,YELLOW);
        Draw_Circle(200,100,3,YELLOW);
        Draw_Circle(200,100,2,YELLOW);
        break;

    case 7:
        Draw_Circle(120+64,100+47,4,RED);
        Draw_Circle(120+64,100+47,3,RED);
        Draw_Circle(120+64,100+47,2,RED);
        Draw_Circle(120+76,100+24,4,YELLOW);
        Draw_Circle(120+76,100+24,3,YELLOW);
        Draw_Circle(120+76,100+24,2,YELLOW);
        break;

    case 8:
        Draw_Circle(120+47,100+64,4,RED);
        Draw_Circle(120+47,100+64,3,RED);
        Draw_Circle(120+47,100+64,2,RED);
        Draw_Circle(120+64,100+47,4,YELLOW);
        Draw_Circle(120+64,100+47,3,YELLOW);
        Draw_Circle(120+64,100+47,2,YELLOW);
        break;

    case 9:
        Draw_Circle(120+24,100+76,4,RED);
        Draw_Circle(120+24,100+76,3,RED);
        Draw_Circle(120+24,100+76,2,RED);
        Draw_Circle(120+47,100+64,4,YELLOW);
        Draw_Circle(120+47,100+64,3,YELLOW);
        Draw_Circle(120+47,100+64,2,YELLOW);
        break;


    case 10:
        Draw_Circle(120,180,4,RED);
        Draw_Circle(120,180,3,RED);
        Draw_Circle(120,180,2,RED);
        Draw_Circle(120+24,100+76,4,YELLOW);
        Draw_Circle(120+24,100+76,3,YELLOW);
        Draw_Circle(120+24,100+76,2,YELLOW);
        break;


    case 11:
        Draw_Circle(120-24,100+76,4,RED);
        Draw_Circle(120-24,100+76,3,RED);
        Draw_Circle(120-24,100+76,2,RED);
        Draw_Circle(120,180,4,YELLOW);
        Draw_Circle(120,180,3,YELLOW);
        Draw_Circle(120,180,2,YELLOW);
        break;

    case 12:
        Draw_Circle(120-47,100+64,4,RED);
        Draw_Circle(120-47,100+64,3,RED);
        Draw_Circle(120-47,100+64,2,RED);
        Draw_Circle(120-24,100+76,4,YELLOW);
        Draw_Circle(120-24,100+76,3,YELLOW);
        Draw_Circle(120-24,100+76,2,YELLOW);
        break;

    case 13:
        Draw_Circle(120-64,100+47,4,RED);
        Draw_Circle(120-64,100+47,3,RED);
        Draw_Circle(120-64,100+47,2,RED);
        Draw_Circle(120-47,100+64,4,YELLOW);
        Draw_Circle(120-47,100+64,3,YELLOW);
        Draw_Circle(120-47,100+64,2,YELLOW);
        break;

    case 14:
        Draw_Circle(120-76,100+24,4,RED);
        Draw_Circle(120-76,100+24,3,RED);
        Draw_Circle(120-76,100+24,2,RED);
        Draw_Circle(120-64,100+47,4,YELLOW);
        Draw_Circle(120-64,100+47,3,YELLOW);
        Draw_Circle(120-64,100+47,2,YELLOW);
        break;


    case 15:
        Draw_Circle(40,100,4,RED);
        Draw_Circle(40,100,3,RED);
        Draw_Circle(40,100,2,RED);
        Draw_Circle(120-76,100+24,4,YELLOW);
        Draw_Circle(120-76,100+24,3,YELLOW);
        Draw_Circle(120-76,100+24,2,YELLOW);
        break;


    case 16:
        Draw_Circle(120-76,100-24,4,RED);
        Draw_Circle(120-76,100-24,3,RED);
        Draw_Circle(120-76,100-24,2,RED);
        Draw_Circle(40,100,4,YELLOW);
        Draw_Circle(40,100,3,YELLOW);
        Draw_Circle(40,100,2,YELLOW);
        break;

    case 17:
        Draw_Circle(120-64,100-47,4,RED);
        Draw_Circle(120-64,100-47,3,RED);
        Draw_Circle(120-64,100-47,2,RED);
        Draw_Circle(120-76,100-24,4,YELLOW);
        Draw_Circle(120-76,100-24,3,YELLOW);
        Draw_Circle(120-76,100-24,2,YELLOW);
        break;

    case 18:
        Draw_Circle(120-47,100-64,4,RED);
        Draw_Circle(120-47,100-64,3,RED);
        Draw_Circle(120-47,100-64,2,RED);
        Draw_Circle(120-64,100-47,4,YELLOW);
        Draw_Circle(120-64,100-47,3,YELLOW);
        Draw_Circle(120-64,100-47,2,YELLOW);
        break;

    case 19:
        Draw_Circle(120-24,100-76,4,RED);
        Draw_Circle(120-24,100-76,3,RED);
        Draw_Circle(120-24,100-76,2,RED);
        Draw_Circle(120-47,100-64,4,YELLOW);
        Draw_Circle(120-47,100-64,3,YELLOW);
        Draw_Circle(120-47,100-64,2,YELLOW);
        break;

    default:
        ;

    }

}




/*******************************************
* @function: 	次级番茄钟显示界面的初始化显示，显示一圈圆圈
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/
void LCD_Draw_Interface_tomato_init (void)
{
    Draw_Circle(120,20,5,RED);

    Draw_Circle(120+24,100-76,5,RED);
    Draw_Circle(120+47,100-64,5,RED);
    Draw_Circle(120+64,100-47,5,RED);
    Draw_Circle(120+76,100-24,5,RED);

    Draw_Circle(200,100,5,RED); //5

    Draw_Circle(120+24,100+76,5,RED);
    Draw_Circle(120+47,100+64,5,RED);
    Draw_Circle(120+64,100+47,5,RED);
    Draw_Circle(120+76,100+24,5,RED); //9

    Draw_Circle(120,180,5,RED); //10

    Draw_Circle(120-24,100-76,5,RED); //11
    Draw_Circle(120-47,100-64,5,RED); //12
    Draw_Circle(120-64,100-47,5,RED); //13
    Draw_Circle(120-76,100-24,5,RED); //14

    Draw_Circle(40,100,5,RED); //15

    Draw_Circle(120-24,100+76,5,RED); //16
    Draw_Circle(120-47,100+64,5,RED); //17
    Draw_Circle(120-64,100+47,5,RED); //18
    Draw_Circle(120-76,100+24,5,RED);


    LCD_ShowIntNum(120-24-12,100-12,0,2,RED,0,32,1);

    LCD_show_Chinese(120,100-12,RED,"：",1,24);

    LCD_ShowIntNum(120+12,100-12,0,2,RED,0,32,1);
}




/*******************************************
* @function: 	隶属于次级界面的番茄钟显示界面的时间储存函数
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/
unsigned char time_buffer[5] = {0};

unsigned char* LCD_Draw_Interface_tomato_Update_timing(void)
{
    unsigned char * an;
    get_time();

    an[0] = sTime.Hours;
    an[1] = sTime.Minutes;
    an[2] = sTime.Seconds;
    //printf("日期:%d:%d:%d   时间:%d:%d:%d \r\n",sDate.Year,	sDate.Month,	sDate.Date,	sTime.Hours, sTime.Minutes, sTime.Seconds);

    //讲数据存进FLASH;

    //打开屏幕片选信号，关闭FLASH的片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//低电平选中

    //防止写入数据时错误擦除数据，先将数据取出来，存进数组更新后再将数据存进FLASH内
    ReadData_4096(Data_address);//从存储器读取数据到内存数组 rdata 中

    rdata[0] = sDate.Month;
    rdata[1] = sDate.Date;
    rdata[2] = sTime.Hours;
    rdata[3] = sTime.Minutes;
    rdata[4] = sTime.Seconds;

    WriteData_4096(Data_address,rdata);//将读取到的数据写入存储器

    ReadData_4096(Data_address);//从存储器读取数据到内存数组 rdata 中
    time_buffer[0] = rdata[0];
    time_buffer[1] = rdata[1];
    time_buffer[2] = rdata[2];
    time_buffer[3] = rdata[3];
    time_buffer[4] = rdata[4];

    an[0] = rdata[2];
    an[1] = rdata[3];
    an[2] = rdata[4];
    //printf("日期:%d:%d %d:%d:%d \r\n",time_buffer[0],	time_buffer[1],	time_buffer[2],	time_buffer[3], time_buffer[4]);

    //打开W26Q64的片选信号，关闭屏幕的片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//低电平选中

    return an;
}


/********  https://blog.csdn.net/qq_44179528/article/details/117339038?spm=1001.2014.3001.5501
* @function: 	计算时分秒之后的时间
* @input:			现在的时间的数组，多久之后的时间的数组，数组顺序分别是时分秒
* @return:		之后的时间的数组
* @author:		田小花
* @time:			2021.5.28
*******************************************/
unsigned char* GET_Time_after(unsigned char* now_time, unsigned char* later_time)
{
    unsigned char* an;

    if((now_time[0]+later_time[0])<24)
    {
        an[0] = now_time[0]+later_time[0];
    }
    else
    {
        an[0] = now_time[0]+later_time[0] - 24;
    }

    if((now_time[1]+later_time[1])<59)
    {
        an[1] = now_time[1]+later_time[1];
    }
    else
    {
        an[1] = now_time[1]+later_time[1] - 60;
        an[0]++;
    }

    if((now_time[2]+later_time[2])<59)
    {
        an[2] = now_time[2]+later_time[2];
    }
    else
    {
        an[2] = now_time[2]+later_time[2] - 60;
        an[1]++;
    }
    //printf("函数内%d %d %d\r\n",an[0], an[1], an[2]);

    return an;
}




/********  https://blog.csdn.net/qq_44179528/article/details/117339038?spm=1001.2014.3001.5501
* @function: 	计算现在的时间和之前的时间差值
* @input:			过去的时间的数组，现在的时间的数组，数组顺序分别是时分秒
* @return:		差值时间的数组
* @author:		田小花
* @time:			2021.5.28
*******************************************/
unsigned char* GET_Time_difference(unsigned char *History_time, unsigned char * now_time)
{
    int seconds = 0;
    int mins = 0;
    int hours = 0;
    unsigned char *time;
    if(now_time[0] >= History_time[0])
    {
        hours = now_time[0] - History_time[0];
    }

    if(now_time[1] > History_time[1])
    {
        mins = now_time[1] - History_time[1];
    }
    else if(now_time[1] < History_time[1])
    {
        mins = 60 - History_time[1] + now_time[1];
        hours--;
        if(mins>60)
        {
            mins = mins -60;
        }
    }

    if(now_time[2] > History_time[2])
    {
        seconds = now_time[2] - History_time[2];
    }
    else if(now_time[2] < History_time[2])
    {
        seconds = 60 - History_time[2] + now_time[2];
        mins--;
        if(seconds>60)
        {
            seconds = seconds -60;
        }
    }


    time[0] = hours;
    time[1] = mins;
    time[2] = seconds;
    printf("时间差为：%d %d %d \r\n",hours,mins,seconds);
    //return(seconds+mins*60+hours*60*60);
    return time;
}


/*
	番茄钟. 番茄钟，是根据番茄工作法理论进行开发的一款方便、实用的日程管理软件。.
	是把工作任务分解成半小时左右，集中精力工作25分钟，然后休息5分钟，
	如此视作种一个“番茄”，而“番茄工作法”的流程能使下一个30分钟更有动力。
*/
//程序一开始进入时在暂停中的状态，长按左按键触发工作中状态，再吃长按左按键触发暂停中状态，循环
//长按右按键触发重新计时
#define stop_state 0
#define working_state 1
#define sleeping_state 2

unsigned char tomato_state = stop_state;

unsigned char begin_sign = 0; //长按按键被按下了的标志
unsigned char work_state = 0;
unsigned char history_seconds = 0;

#define dieoutTime 30 //在此设计计时时间 。计时25分钟为1500秒钟
#define dieoutTime2 50 //在此设计计时时间 。计时25分钟为1500秒钟


unsigned char centre_time_buffer[3] = {0,0,30};
unsigned char centre_time_buffer1[3] = {0,0,20};
unsigned char * time_difference;
unsigned char history_time_buffer[3];
unsigned char now_time_buffer[3];
unsigned char * after_time_buffer;

unsigned char flash_buffer[3] = {0};

void LCD_Draw_Interface_tomato(void)
{

    if(LCD_Draw_flag == 1)
    {
        LCD_Draw_flag = 0;

        if(work_state == 0)
        {
            LCD_Fill(0,0,240,240,BLUE);

            LCD_Draw_Interface_tomato_init();

            LCD_show_Chinese(120-20-8,200,BLACK,"暂停中",3,24);
        }

        if(work_state == 1)
        {
            LCD_Fill(0,0,240,240,YELLOW);

            LCD_Draw_Interface_tomato_init();

            LCD_show_Chinese(120-20-8,200,BLACK,"工作中",3,24);
        }

        if(work_state == 2)
        {
            LCD_Fill(0,0,240,240,GREEN);

            LCD_Draw_Interface_tomato_init();

            LCD_show_Chinese(120-20-8,200,BLACK,"休息中",3,24);
        }
    }


    osDelay(300);

    if( Interface == Interface_tomato )
        switch(work_state)
        {
        //暂停状态
        case stop_state:
        {
            if(begin_sign == 1) //检测到有长按按键事件发生 跳转工作状态
            {
                begin_sign = 0;  //清楚长按按键被按下了的标志


                work_state = working_state;//跳转状态，跳转到工作状态
                printf("番茄钟开始计时！\r\n");

                //更新界面显示
                LCD_Fill(0,0,240,240,YELLOW);
                LCD_Draw_Interface_tomato_init();
                LCD_show_Chinese(120-20-8,200,BLACK,"工作中",3,24);

                get_time();
                flash_buffer[0] = sTime.Hours;
                flash_buffer[1] = sTime.Minutes;
                flash_buffer[2] = sTime.Seconds;

                //printf("一开始的时间 flash_buffer %d %d %d \r\n",flash_buffer[0], flash_buffer[1], flash_buffer[2]);

                //flash_buffer = LCD_Draw_Interface_tomato_Update_timing(); //开始工作一开始，首先记录开始工作的时间。存在 time_buffer 中

                get_time();	//获取当前时间
                now_time_buffer[0] = sTime.Hours;//将当前时间存放到数组内
                now_time_buffer[1] = sTime.Minutes;
                now_time_buffer[2] = sTime.Seconds;

                time_difference = GET_Time_after(now_time_buffer,centre_time_buffer);//计算从当前时间开始之后的工作时间的结束时间。存放到数组中
//
//				hour_sleep = time_difference[0];
//				min_sleep = time_difference[1];
//				sec_sleep = time_difference[2];

                set_Alarm(time_difference[0], time_difference[1], time_difference[2]);//设置RTC闹钟的时间
            }


            break;
        }

        //工作状态
        case working_state:
        {
            if(begin_sign == 1)//检测到有长按按键事件发生 跳转工作状态 //跳转回暂停状态
            {
                begin_sign = 0;  //清楚长按按键被按下了的标志

                work_state = stop_state; //跳转状态，跳转到暂停状态

                //更新界面显示 暂停界面
                LCD_Fill(0,0,240,240,BLUE);
                LCD_Draw_Interface_tomato_init();
                LCD_show_Chinese(120-20-8,200,BLACK,"暂停中",3,24);

                break;
            }


            get_time();		//获取当前时间
            now_time_buffer[0] = sTime.Hours;
            now_time_buffer[1] = sTime.Minutes;
            now_time_buffer[2] = sTime.Seconds;
            //printf("工作时间 flash_buffer %d %d %d \r\n",flash_buffer[0], flash_buffer[1], flash_buffer[2]);

            time_difference = GET_Time_difference(flash_buffer,now_time_buffer);//计算两次时间之间的差

            LCD_Draw_Interface_tomato_Print(YELLOW, time_difference[1], time_difference[2]); //显示工作剩余时间状态的动画


            //检测到已经过了工作时间
            if( (time_difference[2]+time_difference[1]*60+time_difference[0]*60*60) > dieoutTime)
            {
                printf("检测到已经过了工作时间 %d %d %d \r\n",time_difference[0], time_difference[1], time_difference[2]);

                work_state = sleeping_state; //跳转状态，跳转到休息状态

                Transient_vibration(); //震动提示

                //更新界面
                LCD_Fill(0,0,240,240,GREEN);
                LCD_Draw_Interface_tomato_init();
                LCD_show_Chinese(120-20-8,200,BLACK,"休息中",3,24);
            }
            break;
        }

        //休息状态
        case sleeping_state:
        {
            if(begin_sign == 1)//检测到有长按按键事件发生 跳转工作状态 //跳转回暂停状态
            {
                begin_sign = 0;  //清楚长按按键被按下了的标志

                work_state = stop_state; //跳转状态，跳转到暂停状态

                //更新界面显示 暂停界面
                LCD_Fill(0,0,240,240,BLUE);
                LCD_Draw_Interface_tomato_init();
                LCD_show_Chinese(120-20-8,200,BLACK,"暂停中",3,24);

                break;
            }

            get_time();		//获取当前时间
            now_time_buffer[0] = sTime.Hours;
            now_time_buffer[1] = sTime.Minutes;
            now_time_buffer[2] = sTime.Seconds;

            //printf("休息时间 flash_buffer %d %d %d \r\n",flash_buffer[0], flash_buffer[1], flash_buffer[2]);

            time_difference = GET_Time_difference(flash_buffer,now_time_buffer);
            LCD_Draw_Interface_tomato_Print(GREEN, time_difference[1], time_difference[2]); //显示工作剩余时间状态的动画


            //检测到已经过了休息时间
            if( (time_difference[2]+time_difference[1]*60+time_difference[0]*60*60) > dieoutTime2)
            {
                printf("检测到已经过了休息时间 %d %d %d \r\n\r\n\r\n",time_difference[0], time_difference[1], time_difference[2]);

                work_state = working_state; //跳转状态，跳转到工作中的状态

                Transient_vibration(); //震动提示

                get_time();		//获取当前时间
                flash_buffer[0] = sTime.Hours;
                flash_buffer[1] = sTime.Minutes;
                flash_buffer[2] = sTime.Seconds;

                //flash_buffer = LCD_Draw_Interface_tomato_Update_timing(); //开始工作一开始，首先记录开始工作的时间。存在 time_buffer 中

                LCD_Fill(0,0,240,240,YELLOW);
                LCD_Draw_Interface_tomato_init();
                LCD_show_Chinese(120-20-8,200,BLACK,"工作中",3,24);
            }

            break;
        }
        }
}





/*******************************************
* @function: 	隶属于次级界面的番茄钟显示界面的时间储存函数
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/
void LCD_Draw_Interface_Pedometer_write_STEPS(unsigned long step1, unsigned long step2, unsigned long step3)
{

    //把数据存进FLASH;

    //打开FLASH的片选信号，关闭屏幕片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//低电平选中

    //防止写入数据时错误擦除数据，先将数据取出来，存进数组更新后再将数据存进FLASH内
    ReadData_4096(Data_address);//从存储器读取数据到内存数组 rdata 中

    rdata[10] = step1/1000;
    rdata[11] = (step1 - rdata[10]*1000)/10;
    rdata[12] = (step1 - rdata[10]*1000 - rdata[11]*10);


    rdata[13] = step2/1000;
    rdata[14] = (step2 - rdata[13]*1000)/10;
    rdata[15] = (step2 - rdata[13]*1000 - rdata[14]*10);

    rdata[16] = step3/1000;
    rdata[17] = (step3 - rdata[16]*1000)/10;
    rdata[18] = (step3 - rdata[16]*1000 - rdata[17]*10);

    printf("写入到的数据\r\n");
    printf("%d%d%d \r\n",rdata[10], rdata[11], rdata[12]);
    printf("%d%d%d \r\n",rdata[13], rdata[14], rdata[15]);
    printf("%d%d%d \r\n\r\n",rdata[16], rdata[17], rdata[18]);

    WriteData_4096(Data_address,rdata);//将读取到的数据写入存储器


    //打开屏幕的片选信号，关闭W26Q64的片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//低电平选中

}





/*******************************************
* @function: 	隶属于次级界面的番茄钟显示界面的时间储存函数
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/

unsigned long step_buffer[3];

void LCD_Draw_Interface_Pedometer_read_STEPS(void)
{

    //把数据存进FLASH;

    //打开FLASH的片选信号，关闭屏幕片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//低电平选中

    //防止写入数据时错误擦除数据，先将数据取出来，存进数组更新后再将数据存进FLASH内
    ReadData_4096(Data_address);//从存储器读取数据到内存数组 rdata 中

    step_buffer[0] = (unsigned long)(rdata[10]*1000 + rdata[11]*10 + rdata[12]);

    step_buffer[1] = (unsigned long)(rdata[13]*1000 + rdata[14]*10 + rdata[15]);

    step_buffer[2] = (unsigned long)(rdata[16]*1000 + rdata[17]*10 + rdata[18]);


    //打开屏幕的片选信号，关闭W26Q64的片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//低电平选中

    printf("读出的数据");
    printf("%d %d %d \r\n",(int)step_buffer[0], (int)step_buffer[1], (int)step_buffer[2]);
}



float Pedometer_height = 0;

int STEPS_value = 0; //记录步数

unsigned long STEPS_history = 0;

void LCD_Draw_Interface_Pedometer(void)
{
    //显示界面的初始化//显示初始的图案
    if(LCD_Draw_flag == 1)
    {
        LCD_Draw_flag = 0;

        LCD_Fill(0,0,240,90,WHITE);
        LCD_Fill(0,90,240,240,BLUE);

        LCD_show_Chinese(20,20,BLACK,"陀螺仪初始化中",7,24); //显示汉字
        LCD_show_Chinese(20,52,BLACK,"请保持水平不动",7,24); //显示汉字

        MPU_Init();					//初始化MPU6050
        while(mpu_dmp_init())
        {
            printf("MPU6050 error\r\n");
            HAL_Delay(10);
        }
        printf("MPU6050 init ok ------>\r\n");

        dmp_get_pedometer_step_count(&STEPS); //计步器获取到步数

        LCD_Fill(0,0,240,90,WHITE);
        LCD_Fill(0,90,240,240,BLUE);


        LCD_DrawLine(20,210,20,100,WHITE);
        LCD_DrawLine(20,210,220,210,WHITE);

        LCD_DrawLine(20,100,20+6,100+6,WHITE);
        LCD_DrawLine(20,100,20-6,100+6,WHITE);

        LCD_DrawLine(220,210,220-6,210-6,WHITE);
        LCD_DrawLine(220,210,220-6,210+6,WHITE);


        LCD_Draw_Interface_Pedometer_read_STEPS(); //将步数从flash中读取到 step_buffer 中
        //printf("%d %d %d \r\n",step_buffer[0], step_buffer[1], step_buffer[2]);

        step_buffer[1] = 45;//为了方便演示
        step_buffer[2] = 2; //为了方便演示
        //step_buffer[0] = STEPS+step_buffer[0];


        //LCD_show_Chinese(10,92,WHITE,"历史记录",4,24); //显示汉字
        LCD_ShowIntNum(40,240-24,step_buffer[0],5,WHITE,WHITE,16,1);//显示整数变量
        LCD_ShowIntNum(100,240-24,step_buffer[1],5,WHITE,WHITE,16,1);//显示整数变量
        LCD_ShowIntNum(160,240-24,step_buffer[2],5,WHITE,WHITE,16,1);//显示整数变量


        LCD_show_Chinese(30,15,BLACK,"步数：",3,32); //显示汉字
        LCD_ShowIntNum(32*3+25,15,step_buffer[0],5,BLACK,WHITE,32,0);//显示整数变量 今天的步数统计

        LCD_show_Chinese(24*4+22+5,57,BLACK,"千卡路里",4,24); //显示汉字
        LCD_ShowFloatNum1(50,57, step_buffer[0]*0.04, 5,BLACK,WHITE,24,0);//显示卡路里

//		LCD_Fill(45,105,75,210,RED);
//		LCD_Fill(105,105,105+30,210,RED);
//		LCD_Fill(165,105,165+30,210,RED);


        if(step_buffer[0] > 20000)
        {
            LCD_Fill(45,105,75,210,RED);	//今天的步数统计条状图
        }
        else if(step_buffer[0] > 10000)
        {
            Pedometer_height = step_buffer[0]*105/20000-1;
            LCD_Fill(45,210-(int)Pedometer_height,75,210,RED);	//今天的步数统计条状图
            //printf("%d \r\n",(int)Pedometer_height1);
        }
        else
        {
            Pedometer_height = step_buffer[0]*105/20000-1;
            LCD_Fill(45,210-(int)Pedometer_height+20,75,210,RED);	//今天的步数统计条状图
            //printf("%d \r\n",(int)Pedometer_height1);
        }



        if(step_buffer[1] > 20000)
        {
            LCD_Fill(105,105,105+30,210,RED);	//明天的步数统计条状图
        }
        else if(step_buffer[1] > 10000)
        {
            Pedometer_height = step_buffer[1]*105/20000-1;
            LCD_Fill(105,210-(int)Pedometer_height,105+30,210,RED);	//今天的步数统计条状图
            //printf("%d \r\n",(int)Pedometer_height1);
        }
        else
        {
            Pedometer_height = step_buffer[1]*105/20000-1;
            LCD_Fill(105,210-(int)Pedometer_height-20,105+30,210,RED);	//今天的步数统计条状图
            //printf("%d \r\n",(int)Pedometer_height1);
        }



        if(step_buffer[2] > 20000)
        {
            LCD_Fill(165,105,165+30,210,RED);	//后天的步数统计条状图
        }
        else if(step_buffer[2] > 10000)
        {
            Pedometer_height = step_buffer[2]*105/20000-1;
            LCD_Fill(165,210-(int)Pedometer_height,165+30,210,RED);	//今天的步数统计条状图
            //printf("%d \r\n",(int)Pedometer_height1);
        }
        else
        {
            Pedometer_height = step_buffer[2]*105/20000-1;
            LCD_Fill(165,210-(int)Pedometer_height-20,165+30,210,RED);	//今天的步数统计条状图
            //printf("%d \r\n",(int)Pedometer_height1);
        }
    }


    if(STEPS_history != STEPS)
    {
        STEPS_history = STEPS;

        step_buffer[0] = STEPS+step_buffer[0];

        LCD_Draw_Interface_Pedometer_write_STEPS(step_buffer[0],234,4);

        LCD_ShowIntNum(32*3+25,15,(uint16_t)step_buffer[0],5,BLACK,WHITE,32,0);//显示整数变量 今天的步数统计

        LCD_ShowFloatNum1(50,57,(uint16_t)step_buffer[0]*0.04,5,BLACK,WHITE,24,0);//显示卡路里

        LCD_ShowIntNum(40,240-24,(uint16_t)step_buffer[0],5,WHITE,BLUE,16,0);//显示整数变量

        step_buffer[0] = (unsigned long)STEPS;

        //绘制统计图
        if(step_buffer[0] > 20000)
        {
            LCD_Fill(45,105,75,210,RED);	//今天的步数统计条状图
        }
        else if(step_buffer[0] > 10000)
        {
            Pedometer_height = step_buffer[0]*105/20000-1;
            LCD_Fill(45,210-(int)Pedometer_height,75,210,RED);	//今天的步数统计条状图
            //printf("%d \r\n",(int)Pedometer_height1);
        }
        else
        {
            Pedometer_height = step_buffer[0]*105/20000-1;
            LCD_Fill(45,210-(int)Pedometer_height-20,75,210,RED);	//今天的步数统计条状图
            //printf("%d \r\n",(int)Pedometer_height);
        }
    }

    dmp_get_pedometer_step_count(&STEPS); //计步器获取到步数
    osDelay(50);

    if(STEPS>20000)
    {
        STEPS = 20000;
    }

//	STEPS_value = STEPS_value + 200;
//
//	if(STEPS_value > 20000)
//	{
//		STEPS_value = 20000;
//	}
//	else
//	{
//		Pedometer_height1 = STEPS_value*105/20000-1;
//	}
//
//
//	LCD_Fill(45,210-Pedometer_height1,75,210,RED);	//今天的步数统计条状图

//	osDelay(400);
}









unsigned char light_time = 2;
int light_hight = 0;
/*******************************************
* @function: 	隶属于设置工作模式界面储存函数
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/
void Interface_Set_write(void)
{

    //把数据存进FLASH;

    //打开FLASH的片选信号，关闭屏幕片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//低电平选中

    //防止写入数据时错误擦除数据，先将数据取出来，存进数组更新后再将数据存进FLASH内
    ReadData_4096(Data_address);//从存储器读取数据到内存数组 rdata 中

    rdata[20] = light_time;
    rdata[21] = light_hight;

    WriteData_4096(Data_address,rdata);//将读取到的数据写入存储器


    //打开屏幕的片选信号，关闭W26Q64的片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//低电平选中

    printf("   light_hight:%d light_time:%d\r\n",light_hight,light_time);
}





/*******************************************
* @function: 	隶属于次级界面的设置工作模式界面储存函数
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/
void Interface_Set_read(void)
{
    //把数据存进FLASH;

    //打开FLASH的片选信号，关闭屏幕片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//低电平选中

    //防止写入数据时错误擦除数据，先将数据取出来，存进数组更新后再将数据存进FLASH内
    ReadData_4096(Data_address);//从存储器读取数据到内存数组 rdata 中

    light_time = rdata[20];

    light_hight = rdata[21];

    //打开屏幕的片选信号，关闭W26Q64的片选信号。
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //低电平选中
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//低电平选中

    printf("   light_hight:%d   light_time:%d\r\n",light_hight,light_time);
}



void set_screen_light(unsigned char lit)
{
    switch (lit)
    {
    case 0:
    {
        /*调节屏幕背光*/
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //开启定时器PWM通道 定时器2 通道3
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 400); //设置PWM的占空比，调节背光
        printf("400 \r\n");
        break;
    }
    case 1:
    {
        /*调节屏幕背光*/
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //开启定时器PWM通道 定时器2 通道3
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 300); //设置PWM的占空比，调节背光
        printf("300 \r\n");
        break;
    }
    case 2:
    {
        /*调节屏幕背光*/
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //开启定时器PWM通道 定时器2 通道3
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 250); //设置PWM的占空比，调节背光
        printf("250 \r\n");
        break;
    }
    case 3:
    {
        /*调节屏幕背光*/
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //开启定时器PWM通道 定时器2 通道3
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 150); //设置PWM的占空比，调节背光
        printf("150 \r\n");
        break;
    }
    }
}


/*******************************************
* @function: 	设置工作模式界面//按照需求，设置亮屏时间，设置屏幕亮度
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/

unsigned char key_long_flag = 0;

unsigned char Interface_Set_while_flag = 0;

unsigned char light_flag = 0;

unsigned char Set_while_flag = 0;



void LCD_Draw_Interface_Set(void)
{
    //显示界面的初始化//显示初始的图案
    if(LCD_Draw_flag == 1)
    {
        LCD_Draw_flag = 0;

        LCD_Fill(0,0,240,240,BLACK); //整体风格黑色

        LCD_show_Chinese(20,0,WHITE,"设置",2,32); //显示汉字

        LCD_Fill(0,0+80,20,20+80,LGRAY);

        LCD_show_Chinese(40,50,WHITE,"屏幕亮度",4,32); //显示汉字


        LCD_Fill(0,0+160,20,20+160,LGRAY);

        LCD_show_Chinese(40,130,WHITE,"亮屏时间",4,32); //显示汉字

        Interface_Set_read();

        switch(light_hight)
        {
        case 0:
        {
            LCD_Fill(40,100,70,110,WHITE);
            LCD_Fill(80,100,110,110,BROWN);
            LCD_Fill(120,100,150,110,BROWN);
            LCD_Fill(160,100,190,110,BROWN);
            break;
        }
        case 1:
        {
            LCD_Fill(40,100,70,110,WHITE);
            LCD_Fill(80,100,110,110,WHITE);
            LCD_Fill(120,100,150,110,BROWN);
            LCD_Fill(160,100,190,110,BROWN);
            break;
        }
        case 2:
        {
            LCD_Fill(40,100,70,110,WHITE);
            LCD_Fill(80,100,110,110,WHITE);
            LCD_Fill(120,100,150,110,WHITE);
            LCD_Fill(160,100,190,110,BROWN);
            break;
        }
        case 3:
        {
            LCD_Fill(40,100,70,110,WHITE);
            LCD_Fill(80,100,110,110,WHITE);
            LCD_Fill(120,100,150,110,WHITE);
            LCD_Fill(160,100,190,110,WHITE);
            break;
        }
        }
        switch( light_time)
        {
        case 1:
        {
            LCD_Fill(40,180,70,190,WHITE);
            LCD_Fill(80,180,110,190,BROWN);
            LCD_Fill(120,180,150,190,BROWN);
            LCD_Fill(160,180,190,190,BROWN);
            break;
        }
        case 2:
        {
            LCD_Fill(40,180,70,190,WHITE);
            LCD_Fill(80,180,110,190,WHITE);
            LCD_Fill(120,180,150,190,BROWN);
            LCD_Fill(160,180,190,190,BROWN);
            break;
        }
        case 3:
        {
            LCD_Fill(40,180,70,190,WHITE);
            LCD_Fill(80,180,110,190,WHITE);
            LCD_Fill(120,180,150,190,WHITE);
            LCD_Fill(160,180,190,190,BROWN);
            break;
        }
        case 4:
        {
            LCD_Fill(40,180,70,190,WHITE);
            LCD_Fill(80,180,110,190,WHITE);
            LCD_Fill(120,180,150,190,WHITE);
            LCD_Fill(160,180,190,190,WHITE);
            break;
        }
        }
    }

    osDelay(200);

    light_flag = !light_flag;

    if(key_long_flag == 1)
    {
        if(Interface_Set_while_flag!=1)
            key_long_flag = 0;

        Interface_Set_while_flag = 1; //让界面进入到循环中不再出去

        printf("检测到了循环条件\r\n");
    }

    if(Interface_Set_while_flag == 1)
    {
        switch (Set_while_flag)
        {
        case 0:
        {
            if(light_flag)
            {
                LCD_Fill(0,0+80,20,20+80,WHITE);
            }
            else
            {
                LCD_Fill(0,0+80,20,20+80,BLACK);
            }

            //进入另一个循环
            if(key_long_flag == 1)
            {
                key_long_flag = 0;

                printf("进入另一个循环\r\n");
                LCD_Fill(0,0+160,20,20+160,WHITE);
                Set_while_flag = 1;
            }

            //检测左面按键，减少亮屏的时间的动画
            if(key_long_flag == 3)
            {
                key_long_flag = 0;

                printf("减少亮屏亮度\r\n");

                light_hight--;

                if(light_hight<0)
                {
                    light_hight = 0;
                }
                set_screen_light(light_hight);

                Interface_Set_write(); //写入更新falsh内的设置数据

                switch(light_hight)
                {
                case 0:
                {
                    LCD_Fill(40,100,70,110,WHITE);
                    LCD_Fill(80,100,110,110,BROWN);
                    LCD_Fill(120,100,150,110,BROWN);
                    LCD_Fill(160,100,190,110,BROWN);
                    break;
                }
                case 1:
                {
                    LCD_Fill(40,100,70,110,WHITE);
                    LCD_Fill(80,100,110,110,WHITE);
                    LCD_Fill(120,100,150,110,BROWN);
                    LCD_Fill(160,100,190,110,BROWN);
                    break;
                }
                case 2:
                {
                    LCD_Fill(40,100,70,110,WHITE);
                    LCD_Fill(80,100,110,110,WHITE);
                    LCD_Fill(120,100,150,110,WHITE);
                    LCD_Fill(160,100,190,110,BROWN);
                    break;
                }
                case 3:
                {
                    LCD_Fill(40,100,70,110,WHITE);
                    LCD_Fill(80,100,110,110,WHITE);
                    LCD_Fill(120,100,150,110,WHITE);
                    LCD_Fill(160,100,190,110,WHITE);
                    break;
                }
                }

            }

            if(key_long_flag == 4)
            {
                key_long_flag = 0;

                printf("增加屏幕亮度\r\n");
                light_hight++;

                if(light_hight>3)
                {
                    light_hight = 3;
                }
                set_screen_light(light_hight);
                Interface_Set_write(); //写入更新falsh内的设置数据
                switch( light_hight)
                {
                case 0:
                {
                    LCD_Fill(40,100,70,110,WHITE);
                    LCD_Fill(80,100,110,110,BROWN);
                    LCD_Fill(120,100,150,110,BROWN);
                    LCD_Fill(160,100,190,110,BROWN);
                    break;
                }
                case 1:
                {
                    LCD_Fill(40,100,70,110,WHITE);
                    LCD_Fill(80,100,110,110,WHITE);
                    LCD_Fill(120,100,150,110,BROWN);
                    LCD_Fill(160,100,190,110,BROWN);
                    break;
                }
                case 2:
                {
                    LCD_Fill(40,100,70,110,WHITE);
                    LCD_Fill(80,100,110,110,WHITE);
                    LCD_Fill(120,100,150,110,WHITE);
                    LCD_Fill(160,100,190,110,BROWN);
                    break;
                }
                case 3:
                {
                    LCD_Fill(40,100,70,110,WHITE);
                    LCD_Fill(80,100,110,110,WHITE);
                    LCD_Fill(120,100,150,110,WHITE);
                    LCD_Fill(160,100,190,110,WHITE);
                    break;
                }
                }

            }

            break;
        }


        case 1:
        {
            if(light_flag)
            {
                LCD_Fill(0,0+160,20,20+160,WHITE);
            }
            else
            {
                LCD_Fill(0,0+160,20,20+160,BLACK);
            }
            //进入另一个循环
            if(key_long_flag == 1)
            {
                key_long_flag = 0;
                printf("进入另一个循环\r\n");
                LCD_Fill(0,0+160,20,20+160,WHITE);
                Set_while_flag = 0;
            }

            if(key_long_flag == 3)
            {
                key_long_flag = 0;

                printf("减少息屏时间\r\n");

                light_time--;

                if(light_time<1)
                {
                    light_time=1;
                }
                Interface_Set_write(); //写入更新falsh内的设置数据

                switch(light_time)
                {
                case 1:
                {
                    LCD_Fill(40,180,70,190,WHITE);
                    LCD_Fill(80,180,110,190,BROWN);
                    LCD_Fill(120,180,150,190,BROWN);
                    LCD_Fill(160,180,190,190,BROWN);
                    break;
                }
                case 2:
                {
                    LCD_Fill(40,180,70,190,WHITE);
                    LCD_Fill(80,180,110,190,WHITE);
                    LCD_Fill(120,180,150,190,BROWN);
                    LCD_Fill(160,180,190,190,BROWN);
                    break;
                }
                case 3:
                {
                    LCD_Fill(40,180,70,190,WHITE);
                    LCD_Fill(80,180,110,190,WHITE);
                    LCD_Fill(120,180,150,190,WHITE);
                    LCD_Fill(160,180,190,190,BROWN);
                    break;
                }
                case 4:
                {
                    LCD_Fill(40,180,70,190,WHITE);
                    LCD_Fill(80,180,110,190,WHITE);
                    LCD_Fill(120,180,150,190,WHITE);
                    LCD_Fill(160,180,190,190,WHITE);
                    break;
                }
                }

            }

            if(key_long_flag == 4)
            {
                key_long_flag = 0;

                printf("增加息屏时间\r\n");
                light_time++;
                if(light_time>4)
                {
                    light_time=4;
                }
                Interface_Set_write(); //写入更新falsh内的设置数据

                switch(light_time)
                {
                case 1:
                {
                    LCD_Fill(40,180,70,190,WHITE);
                    LCD_Fill(80,180,110,190,BROWN);
                    LCD_Fill(120,180,150,190,BROWN);
                    LCD_Fill(160,180,190,190,BROWN);
                    break;
                }
                case 2:
                {
                    LCD_Fill(40,180,70,190,WHITE);
                    LCD_Fill(80,180,110,190,WHITE);
                    LCD_Fill(120,180,150,190,BROWN);
                    LCD_Fill(160,180,190,190,BROWN);
                    break;
                }
                case 3:
                {
                    LCD_Fill(40,180,70,190,WHITE);
                    LCD_Fill(80,180,110,190,WHITE);
                    LCD_Fill(120,180,150,190,WHITE);
                    LCD_Fill(160,180,190,190,BROWN);
                    break;
                }
                case 4:
                {
                    LCD_Fill(40,180,70,190,WHITE);
                    LCD_Fill(80,180,110,190,WHITE);
                    LCD_Fill(120,180,150,190,WHITE);
                    LCD_Fill(160,180,190,190,WHITE);
                    break;
                }
                }
            }
            break;
        }
        }



        if(key_long_flag == 2)
        {
            key_long_flag = 0;

            Interface_Set_while_flag = 0; //恢复循环循环

            LCD_Fill(0,0+80,20,20+80,LGRAY);
            LCD_Fill(0,0+160,20,20+160,LGRAY);

            printf("退出了设置循环\r\n");
        }
    }
}





unsigned char sleep_time_buffer[3] = {0,0,5};
unsigned char time_seconds_history[3] = {0};
unsigned char *time_seconds_history_;
void updata_sleep_time(void)
{
    get_time();	//获取当前时间
    time_seconds_history[0] = sTime.Hours;//将当前时间存放到数组内
    time_seconds_history[1] = sTime.Minutes;
    time_seconds_history[2] = sTime.Seconds;
    time_seconds_history_ = GET_Time_after(time_seconds_history,sleep_time_buffer);//计算从当前时间开始之后的工作时间的结束时间。存放到数组中
}



void Checking_into_sleep(void)
{
    get_time();	//获取当前时间
    if(sTime.Hours == time_seconds_history_[0] && sTime.Minutes >= time_seconds_history_[1] && sTime.Seconds >= time_seconds_history_[2])
    {
        low_power_model();
    }

}

unsigned char flash_write_flage = 0; //获取蓝牙设备的信息后更新flash操作的标志位

/*******************************************
* @function: 	基础任务，任务优先级为最低的任务
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.4.26
*******************************************/


unsigned char key_value[2];

unsigned char key_value_handle_flag = DONE;

unsigned char Interface = Interface_main;
//unsigned char Interface = Interface_Menu;
unsigned char Interface_Flag =0;

void task_base(void)
{
    //查看连接蓝牙的串口2 是否接收到了数据
    while(1)
    {
        switch(Interface)
        {
        case Interface_main:
            LCD_Draw_Interface_main2();
            key_value_handle_flag = DONE;
            break;//Checking_into_sleep(); break; //主界面

        case Interface_notify:
            LCD_Draw_Interface_notify();
            key_value_handle_flag = DONE;
            break;  //进入通知界面

        case Interface_Memo:
            LCD_Draw_Interface_Memo();
            key_value_handle_flag = DONE;
            break;//Checking_into_sleep();break;  //进入备忘录界面

        case Interface_informations:
            LCD_Draw_Interface_informations();
            key_value_handle_flag = DONE;
            break;//Checking_into_sleep(); break;  //进入温湿度显示界面

        case Interface_tomato :
            LCD_Draw_Interface_tomato();
            key_value_handle_flag = DONE;
            break;  //进入番茄钟界面

        case Interface_Pedometer :
            LCD_Draw_Interface_Pedometer();
            key_value_handle_flag = DONE;
            break;  //进入计步器界面

        case Interface_Set :
            LCD_Draw_Interface_Set();
            key_value_handle_flag = DONE;
            break;  //进入设置界面
        }
        if(flash_write_flage == 1)
        {
            flash_write_flage = 0;

            notify_Set_write(); //写入更新falsh内的设置数据
        }
        //printf("任务1 -->\r\n");

        //进入低功耗待机模式
//		HAL_GPIO_WritePin(SYS_ON_GPIO_Port, SYS_ON_Pin, GPIO_PIN_RESET); //关闭系统
//		low_power_model();
    }
}



/*******************************************
* @function: 	基础任务，任务优先级为最低的任务
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.4.26
*******************************************/
unsigned long STEPS = 0; //记录步数

unsigned char USB_in_SIGN = 0; //获取USB连接

unsigned char KEY_1; //按键值 关联在中断函数文件内
unsigned char KEY_2; //按键值

float pitch,roll,yaw; 		//欧拉角
short aacx,aacy,aacz;		//加速度传感器原始数据
short gyrox,gyroy,gyroz;	//陀螺仪原始数据
short temp;					//温度

int key_times1=0; //按键1按下的时间
int key_times2=0;	//按键2按下的时间

unsigned char key_continuous_value = 0; //保存长按的按键值


void background_base(void)
{
    
    int i=0;
    uint8_t  buferRX[128] = {0};
    uint8_t  bufferLen = 0;
    uint8_t BLE_time[6] = {0};

    uint8_t message_indicate[64] = {0};
    uint8_t message_indicate_len = 0;
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    Interface_Set_read();
    sleep_time_buffer[2] = light_time;
    updata_sleep_time();

    while(1)
    {
        //printf("get in task background loop");
        //查看连接蓝牙的串口2 是否接收到了数据
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
            {
                buferRX[i] = Rx2_Buf[i];
            }
            bufferLen = Rx2_Len;

            //HAL_UART_Transmit_DMA(&huart1, Rx2_Buf, Rx2_Len);

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

                    LCD_Draw_flag = 1; //更新通知界面的显示

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
                    sDate.Year = 21;

                    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
                    {
                        Error_Handler();
                    }
                    Transient_vibration(); //震动提示


                    break;
                }
                case 't':
                {
                    printf("获取到了天气\r\n");

                    LCD_Draw_flag = 1; //更新通知界面的显示
                    flash_write_flage = 1; //获取蓝牙设备的信息后更新flash操作的标志位
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
                        flash_write_flage = 1; //获取蓝牙设备的信息后更新flash操作的标志位

                        printf("通知：%s 通知长度：%d\r\n",words3, words_len3);

                        break;
                    }
                    }
                    LCD_Draw_flag = 1; //更新通知界面的显示

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
                        flash_write_flage = 1; //获取蓝牙设备的信息后更新flash操作的标志位

                        LCD_Draw_flag = 1; //界面刷新的标志位

                        get_time();

                        words_month = sDate.Month; //保存备忘录日期
                        words_day = sDate.Date;  //保存备忘录日期

                        printf("momo：%s momo length：%d  %d  %d\r\n",words4, words_len4,words_month, words_day);

                        break;
                    }
                    }
                    LCD_Draw_flag = 1; //更新通知界面的显示

                    Transient_vibration(); //震动提示

                    break;
                }
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
                MX_USART2_UART_Init();;
            }
        }



        //根据按下按键的时间的长短判断是不是长按操作
        if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 1)
        {
            printf("get in task background 根据按下按键1的时间的长短判断是不是长按操作 ");
            while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 1)
            {
                printf("%d\r\n",key_times1);
                osDelay(1);
                key_times1++;
                if(key_times1 > 400)
                {
                    printf("长按1 %d\r\n",key_times1);
                    key_continuous_value = 1;
                    //Transient_vibration(); //瞬变振动
                    osDelay(50);
                    //break;
                }
            }
        }

        //核心板不用PA11作为key2 所以暂时注释这 换成PB12 暂时注释
        /*
        if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11) == 0)
        {

        	while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11) == 0)
        	{
        		printf("%d\r\n",key_times2);
        		osDelay(1);
        		key_times2++;
        		if(key_times2 > 400)
        		{
        			//printf("长按2 %d\r\n",key_times2);
        			key_continuous_value = 2;
        			//Transient_vibration(); //瞬变振动
        			osDelay(50);
        			//break;
        		}
        	}

        }
        */
        //核心板的key2
        if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12) == 1)
        {
            printf("get in task background 根据按下按键2的时间的长短判断是不是长按操作 ");
            while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12) == 1)
            {
                printf("%d\r\n",key_times2);
                osDelay(1);
                key_times2++;
                if(key_times2 > 400)
                {
                    printf("长按2 %d\r\n",key_times2);
                    key_continuous_value = 2;
                    //Transient_vibration(); //瞬变振动
                    osDelay(50);
                    //break;
                }
            }

        }
        //根据按键中断判断是不是有按键按下！
        //同时判断按键事件是不是处理完成，在处理完成按键之前不会再对按键事件有反应
        if((key_continuous_value != 0 || KEY_1 == 1 || KEY_2 == 1 )&& key_value_handle_flag == DONE)
        {

            key_times2 = 0; //归零长按计数
            key_times1 = 0;	//归零长按计数

            key_value_handle_flag = DO; //刷新按键按下状态标志

            Transient_vibration(); //振动

            switch(Interface) //根据当前界面选择在按键按下以后去到哪个界面
            {
            //主界面
            case Interface_main:
            {
                //检测到短按操作，进入到消息通知界面
                if(KEY_1 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_notify;
                    LCD_Draw_flag = 1;
                }
                //检测到短按操作，进入到信息显示界面
                if(KEY_2 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_informations;
                    LCD_Draw_flag = 1;
                }
                //检测到长按操作，进入番茄时钟界面
                if(key_continuous_value == 1)
                {
                    Interface = Interface_tomato;
                    LCD_Draw_flag = 1;
                }
                //检测到长按操作，进入计步器界面
                if(key_continuous_value == 2)
                {
                    Interface = Interface_Pedometer;
                    LCD_Draw_flag = 1;
                }
                break;
            }

            //消息通知界面
            case Interface_notify:
            {
                //检测到短按操作，返回到主界面
                if(KEY_2 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_main;
                    LCD_Draw_flag = 1;
                }
                //检测到短按操作，进入备忘录界面
                if(KEY_1 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_Memo;
                    LCD_Draw_flag = 1;
                }
                break;
            }
            //显示备忘录界面
            case Interface_Memo:
            {
                //检测到短按操作，返回到主界面
                if(KEY_1 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_main;
                    LCD_Draw_flag = 1;
                }
                //检测到短按操作，进入通知界面
                if(KEY_2 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_notify;
                    LCD_Draw_flag = 1;
                }
                break;
            }


            //温湿度信息显示界面
            case Interface_informations:
            {
                //检测到短按操作，返回到主界面
                if(KEY_1 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_main;
                    LCD_Draw_flag = 1;
                }
                //检测到短按操作 去到设置界面
                if(KEY_2 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_Set;
                    LCD_Draw_flag = 1;
                }
                break;
            }

            //番茄钟作息时间显示界面
            case Interface_tomato:
            {
                //检测到短按操作，返回到主界面
                if(KEY_2 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_main;
                    LCD_Draw_flag = 1;
                }
                //检测到长按操作，在番茄钟内部进行界面跳转
                if(key_continuous_value == 1)
                {
                    begin_sign = 1; //长按按键被按下了的标志
                }
                break;
            }


            //计步器显示界面
            case Interface_Pedometer:
            {
                //检测到短按操作，返回到主界面
                if(KEY_1 == 1 && key_continuous_value == 0)
                {
                    //LCD_Draw_Interface_Pedometer_write_STEPS(step_buffer[0], 3782, 1879); //将步数从内存中写入到falsh中
                    Interface = Interface_main;
                    LCD_Draw_flag = 1;
                }
                if(KEY_2 == 1 && key_continuous_value == 0)
                {
                    //LCD_Draw_Interface_Pedometer_write_STEPS(step_buffer[0], 3782, 1879); //将步数从内存中写入到falsh中
                    Interface = Interface_main;
                    LCD_Draw_flag = 1;

                }
            }

            //显示设置界面
            case Interface_Set:
            {
                //检测到短按操作，返回到温湿度信息界面
                if(KEY_1 == 1 && key_continuous_value == 0)
                {
                    key_long_flag = 3;
                    if(0 == Interface_Set_while_flag)
                    {
                        Interface = Interface_informations;
                        LCD_Draw_flag = 1;
                    }

                }
                //检测到短按操作，返回到主界面
                if(KEY_2 == 1 && key_continuous_value == 0)
                {
                    key_long_flag = 4;
                    if(0 == Interface_Set_while_flag)
                    {
                        Interface = Interface_main;
                        LCD_Draw_flag = 1;
                    }
                }
                //检测到长按操作，在番茄钟内部进行界面跳转
                if(key_continuous_value == 1)
                {
                    key_long_flag = 1; //长按按键被按下了的标志
                }
                //检测到长按操作，在番茄钟内部进行界面跳转
                if(key_continuous_value == 2)
                {
                    key_long_flag = 2; //长按按键被按下了的标志
                }
            }



            }

            //printf("\r\n改变界面 %d  %d  %d \r\n",key_continuous_value,KEY_1,KEY_2);

            KEY_2 = 0;
            KEY_1 = 0;
            key_continuous_value = 0;

            updata_sleep_time();
        }

        dmp_get_pedometer_step_count(&STEPS); //计步器获取到步数





//	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//	{
//		temp=MPU_Get_Temperature();	//得到温度值
//
//		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
//
////			printf("%d %d %d \r\n",aacx,aacy,aacz);
//
//		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
//
//		printf("陀螺仪数据 %.1f %.1f %.1f ",pitch,roll,yaw);
//	}
//
//
//		printf("任务2 -->\r\n");
//		//HAL_Delay(50);
//		//osDelay(50);
//

    }
    //while(1)结束
}









/*******************************************
* @function: 	基础任务，任务优先级为最低的任务
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.4.26
*******************************************/



void other_fun(void)
{
    //measure_Si7021();//每秒采样数据

    //printf("温度:%.2f 湿度:%.2f ",TEMP_buf,Humi_buf);


    USB_in_SIGN = HAL_GPIO_ReadPin(usb_in_sign_GPIO_Port,usb_in_sign_Pin);

    //printf("USB输入%d ",USB_in_SIGN);
}








void SystemClock_Config_20Mhz(void)
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
    RCC_OscInitStruct.PLL.PLLN = 10;
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
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
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
    PeriphClkInit.PLLSAI1.PLLSAI1N = 8;
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











void SystemClock_Config_10Mhz(void)
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
    RCC_OscInitStruct.PLL.PLLN = 10;
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
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV8;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
    PeriphClkInit.PLLSAI1.PLLSAI1N = 8;
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



void display_test_task(void)
{
    int i=0;
    while(1) {
        if(i==3)
            i=1;
        else i++;

        osDelay(1000);
        //printf("come in\r\n");
        switch (i)
        {
        case 1:
        {
            printf("come in\r\n");

            break;
        }
        case 2:
        {
            printf("come in2\r\n");

            break;
        }
        case 3:
        {
            //lv_disp_load_scr(scr_menu); //切换界面
            break;
        }
        }
    }
}


