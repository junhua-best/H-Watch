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
* @function: 	ʹ��STM32����ADC��ȡ��ص�ѹֵ������ȡ���ĵ�ѹֵ������ȫ�ֱ����У���ADC1ͨ��1 ����Ӧ����PA1
* @input:			��
* @return:		�����Ϊȫ�ֱ���ֱ��ʹ��
* @author:		��С��
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

    //��W26Q64��Ƭѡ�źţ�
    //�ر���Ļ��Ƭѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
    printf("W26Q64 init ok -------------------------------------->\r\n");
    ReadID();
    printf("readID is %x%x\r\n",W25Q64_ID[3],W25Q64_ID[4]);



    notify_Set_read(); //��flash�ڶ�ȡ֪ͨ�������Ϣ
//	/*������Ļ����*/
    Interface_Set_read();
    set_screen_light(light_hight);
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //������ʱ��PWMͨ�� ��ʱ��2 ͨ��3
//	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 400); //����PWM��ռ�ձȣ����ڱ���
    printf("LCD black init ok ----------------------------->\r\n");


    //�ر�W26Q64��Ƭѡ�źţ�
    //����Ļ��Ƭѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //�͵�ƽΪѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); //�͵�ƽΪѡ��
    LCD_Init();//LCD��ʼ��
    printf("LCD init ok ---------------->\r\n");
    //LCD_ShowPicture(0,0,240,240,gImage_dahuai); //��ʾ��ձ���

    LCD_Fill(0,0,240,240,0xDF3E);


    /*������*/
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //������ʱ��PWMͨ�� ��ʱ��2 ͨ��3
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000); //����PWM��ռ�ձȣ�������ǿ��
    HAL_Delay(60);



    /*������*/
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //������ʱ��PWMͨ�� ��ʱ��2 ͨ��3
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0); //����PWM��ռ�ձȣ�������ǿ��
    printf("motor init ok ------------->\r\n");

    get_time();
    sDate.Year=22;
    printf("get_time %d %d %d 20%d %d %d %d\r\n",sTime.Hours,sTime.Minutes,sTime.Seconds,sDate.Year,sDate.Month,sDate.Date,sDate.WeekDay);



//	MPU_Init();					//��ʼ��MPU6050
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
* @function: 	ʹ��STM32����ADC��ȡ��ص�ѹֵ������ȡ���ĵ�ѹֵ������ȫ�ֱ����У���ADC1ͨ��1 ����Ӧ����PA1
* @input:			��
* @return:		�����Ϊȫ�ֱ���ֱ��ʹ��
* @author:		��С��
* @time:			2020.10.2
*******************************************/
RTC_DateTypeDef sDate; //ȫ�ֱ������ⲿ����ֱ�ӵ���ʹ�� �������������

RTC_TimeTypeDef sTime; //ȫ�ֱ������ⲿ����ֱ�ӵ���ʹ�� �����ʱ������

void get_time(void)
{
    /*ʱ���ȡ����*/
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN); //��ȡʱ��
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN); //��ȡ����

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
    HAL_GPIO_WritePin(GET_VBAT_GPIO_Port, GPIO_PIN_12, GPIO_PIN_SET);//����

    for(j=0; j<10; j++)
    {
        HAL_ADC_Start(&hadc1);  //��ADC1

        HAL_ADC_PollForConversion(&hadc1, 15); //

        if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
        {
            ADC_Value_BATTERY = HAL_ADC_GetValue(&hadc1); //��ȡADCֵ�����ADC_Value_BATTERY��
        }
        ADC_Value_BATTERY = ADC_Value_BATTERY*3.3f/4096;	//��صĵ�ѹֵ


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
        HAL_ADC_Start(&hadc1);  //��ADC1

        HAL_ADC_PollForConversion(&hadc1, 15); //

        if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
        {
            ADC_Value_BATTERY = HAL_ADC_GetValue(&hadc1); //��ȡADCֵ�����ADC_Value_BATTERY��
        }
        ADC_Value_BATTERY = ADC_Value_BATTERY*3.3f/4096;	//��صĵ�ѹֵ


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
* @function: 	ʹ��RCT�����ӽ������������жϣ���������RTC�жϻ���
* @input:			ʱ����
* @return:		��
* @author:		��С��
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

    printf("����ʱ���趨���ˣ�%d %d %d\r\n",hour, min, sec);
}


/*******************************************
* @function: 	ͨ����ʱ�����PWM���������𶯵Ĵ�С
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.5.21
*******************************************/
void Transient_vibration(void)
{
    /*������*/
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //������ʱ��PWMͨ�� ��ʱ��2 ͨ��3
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000); //����PWM��ռ�ձȣ�������ǿ��
    HAL_Delay(40);

    /*������*/
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //������ʱ��PWMͨ�� ��ʱ��2 ͨ��3
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0); //����PWM��ռ�ձȣ�������ǿ��
}






/*******************************************
* @function: 	�͹���ģʽ���� ʹ��PA0 wake up ���ų����ڲ�Ӳ�����Ѹ�λ��Ƭ�� �͹���ģʽ�¹ر�����IOʱ�ӣ�����IOΪ���� �������ģʽ
* @input:			��
* @return:		��
* @author:		��С��
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


//	//����ʹ�õ��ݴ���оƬ��оƬ����������Ǹߵ�ƽ��������ó�Ϊ�������ģʽ���Լ�СIO�ĵ���
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


    //����ʹ�õ��ݴ���оƬ��оƬ����������Ǹߵ�ƽ��������ó�Ϊ�������ģʽ���Լ�СIO�ĵ���
    GPIO_InitStructure.Pin = GPIO_PIN_All;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);//
    __HAL_RCC_GPIOA_CLK_DISABLE();

    /* Ϊ��Լ���ģ��ʵ����ö�Ӧ��GPIO
    	�������ţ�����������ƼĴ���������APCλ */

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
    //	printf("Ӧ�û��ѵ�ʱ��	%d %d %d\r\n",hour_sleep,min_sleep,sec_sleep);

    HAL_PWR_EnterSTANDBYMode();
}





/*******************************************
* @function: 	������ʾ�������ϵ�����ͼƬ��������flash�ڶ�ȡ��ͼƬ�����ݣ��ٽ�������ʾ����Ļ��
* @input:			Ҫ��ʾ��ʱ�䣬Ҫ��ʾ��ͼƬ��λ��
* @return:		��
* @author:		��С��
* @time:			2021.5.28
*******************************************/
void LCD_Print_Time(unsigned char number_time, unsigned char number_location)
{
    //��W26Q64��Ƭѡ�źţ��ر���Ļ��Ƭѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);

    switch (number_time)
    {
    case 0 :
        ReadData_4096(Pictures_address_NUM0);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��
        break;
    case 1 :
        ReadData_4096(Pictures_address_NUM1);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��
        break;
    case 2 :
        ReadData_4096(Pictures_address_NUM2);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��
        break;
    case 3 :
        ReadData_4096(Pictures_address_NUM3);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��
        break;
    case 4 :
        ReadData_4096(Pictures_address_NUM4);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��
        break;
    case 5 :
        ReadData_4096(Pictures_address_NUM5);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��
        break;
    case 6 :
        ReadData_4096(Pictures_address_NUM6);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��
        break;
    case 7 :
        ReadData_4096(Pictures_address_NUM7);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��
        break;
    case 8 :
        ReadData_4096(Pictures_address_NUM8);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��
        break;
    case 9 :
        ReadData_4096(Pictures_address_NUM9);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��
        break;
    }

    //��W26Q64��Ƭѡ�źţ��ر���Ļ��Ƭѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//�͵�ƽѡ��

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
* @function: 	�������ʱ��ˢ��
* @input:			��
* @return:		��
* @author:		��С��
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

        //����ǲ����ڳ�硣����״̬����ʾС���磬û���ʱ��ʾ��ص����ٷֱ�
        if(HAL_GPIO_ReadPin(usb_in_sign_GPIO_Port,usb_in_sign_Pin) == 1)
        {
            LCD_ShowPicture(182+26,216,16,16,gImage_shadian);
        }
        else
        {
            LCD_ShowIntNum(182+26,216,get_vbat(),2,BLACK,0XFF80,16,0); //ˢ�µ�ص���
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
* @function: 	�������ˢ�����񣬰���ˢ��ʱ���ˢ�¶���
* @input:			��
* @return:		��
* @author:		��С��
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

    Task_Time(); //��ʾʱ��

    LCD_ShowPicture(62,111,33,33,gImage_dahuaiseye);//����
    osDelay(60);
    LCD_ShowPicture(62,111,33,33,gImage_dahuaiseye2); //����
    osDelay(240);

}









void Interface_mian_read(int add)
{
    //�����ݴ��FLASH;

    //��FLASH��Ƭѡ�źţ��ر���ĻƬѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//�͵�ƽѡ��

    //��ֹд������ʱ����������ݣ��Ƚ�����ȡ���������������º��ٽ����ݴ��FLASH��
    ReadData_4096(add);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��

    //����Ļ��Ƭѡ�źţ��ر�W26Q64��Ƭѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//�͵�ƽѡ��
}





void LCD_Draw_Interface_main2(void)
{
    uint16_t x = 100;
    uint16_t y = 80;
    uint16_t length = 90;

    uint16_t width = 100;

    //��ʾ����ĳ�ʼ��//��ʾ��ʼ��ͼ��

    if(LCD_Draw_flag == 1)
    {

        LCD_Draw_flag = 0;
        //Interface_main_write();

        notify_Set_read(); //��flash�ڶ�ȡ֪ͨ�������Ϣ

        //����Ļ��Ƭѡ�źţ��ر�W26Q64��Ƭѡ�źš�
        HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //�͵�ƽѡ��
        HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//�͵�ƽѡ��

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


        LCD_show_Chinese(6, 9, BLACK, "���", 2, 24); //��ʾ����
        LCD_ShowIntNum(48,8,get_vbat(),2,BLACK,0xDF3E,24,0); //ˢ�µ�ص���

        LCD_ShowString(75,8,"%", BLACK, 0xDF3E, 24, 0);

        //LCD_show_Chinese(110, 3, BLACK, "����һ", 3, 32); //��ʾ����
        switch (sDate.WeekDay)
        {
        case RTC_WEEKDAY_MONDAY    :
            LCD_show_Chinese(110, 3, BLACK, "����һ", 3, 32);
            break; //��ʾ���� break;
        case RTC_WEEKDAY_TUESDAY   :
            LCD_show_Chinese(110, 3, BLACK, "���ڶ�", 3, 32);
            break;//��ʾ���� break;
        case RTC_WEEKDAY_WEDNESDAY :
            LCD_show_Chinese(110, 3, BLACK, "������", 3, 32);
            break;//��ʾ���� break;
        case RTC_WEEKDAY_THURSDAY  :
            LCD_show_Chinese(110, 3, BLACK, "������", 3, 32);
            break;//��ʾ���� break;
        case RTC_WEEKDAY_FRIDAY    :
            LCD_show_Chinese(110, 3, BLACK, "������", 3, 32);
            break;//��ʾ���� break;
        case RTC_WEEKDAY_SATURDAY  :
            LCD_show_Chinese(110, 3, BLACK, "������", 3, 32);
            break;//��ʾ���� break;
        case RTC_WEEKDAY_SUNDAY  	 :
            LCD_show_Chinese(110, 3, BLACK, "������", 3, 32);
            break;//��ʾ���� break;
        }

//		LCD_show_Chinese(3, 110, BLACK, "��", 1, 24); //��ʾ����
//		LCD_ShowIntNum(5, 140, 32, 2, BLACK, 0xDF3E,	24, 0);
//		LCD_ShowString(30,140,"-", BLACK, 0xDF3E, 24, 0);
//		LCD_ShowIntNum(45, 140, 18, 2, BLACK, 0xDF3E,	24, 0);
//		LCD_show_Chinese(68, 140, BLACK, "��", 1, 24);

        LCD_show_Chinese(3, 110, BLACK,words1,words_len1,24);
        LCD_ShowIntNum(5, 140, day_1_H, 2, BLACK, 0xDF3E, 24,0);//��ʾ��������
        LCD_ShowString(30, 140, "-", BLACK, 0xDF3E, 24, 0);
        LCD_ShowIntNum(45, 140, day_1_L, 2, BLACK, 0xDF3E, 24, 0);//��ʾ��������
        LCD_show_Chinese(68, 140, BLACK, "��", 1, 24);


        LCD_show_Chinese(13, 200, BLACK, "�¶�", 2, 24); //��ʾ����
        LCD_show_Chinese(187, 200, BLACK, "ʪ��", 2, 24); //��ʾ����

        measure_Si7021();//ÿ���������

        printf("�¶�:%.2f ʪ��:%.2f \r\n",TEMP_buf,Humi_buf);

        LCD_ShowFloatNum1(105-40,180+3,	TEMP_buf-4,4,BLACK,0xDF3E,24,0);//��ʾ��������
        LCD_ShowFloatNum1(100,210+3,	Humi_buf,4,BLACK,0xDF3E,24,0);//��ʾ��������

        LCD_show_Chinese(120-40+48,180+5,BLACK,"��",1,24);
        LCD_show_Chinese(110+48,210+5,BLACK,"��",1,24);

    }


    get_time();

    if(history_time[2] != sTime.Seconds )
    {
        history_time[2] = sTime.Seconds;

        LCD_ShowIntNum(160, 62, sTime.Seconds, 2, BLACK, 0xDF3E,	16, 0);

        //����ǲ����ڳ�硣����״̬����ʾС���磬û���ʱ��ʾ��ص����ٷֱ�
        if(HAL_GPIO_ReadPin(usb_in_sign_GPIO_Port,usb_in_sign_Pin) == 1)
        {

        }
        else
        {
            LCD_ShowIntNum(48,8,get_vbat(),2,BLACK,0xDF3E,24,0); //ˢ�µ�ص��� ����150kflash
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
    //��ʾ��11��ͼƬ

    LCD_Address_Set(x,y,x+length-1,y+width-1);//�����ַ

    Interface_mian_read(Space_Picture_addressK1);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������


    Interface_mian_read(Space_Picture_addressK2);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressK3);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressK4);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressK5);
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//��������


    osDelay(88);
    //��ʾ��10��ͼƬ
    LCD_Address_Set(x,y,x+length-1,y+width-1); //�����ַ

    Interface_mian_read(Space_Picture_addressJ1); //��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressJ2);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressJ3);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressJ4);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressJ5);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//�������ݡ�


    osDelay(88);
    //��ʾ��9��ͼƬ
    LCD_Address_Set(x,y,x+length-1,y+width-1);//�����ַ

    Interface_mian_read(Space_Picture_addressI1);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressI2);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressI3);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressI4);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressI5);
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//�������ݡ�

    LCD_Fill(110, 60, 115, 65, BLACK);
    LCD_Fill(110, 72, 115, 77, BLACK);

    osDelay(88);
    //��ʾ��8��ͼƬ
    LCD_Address_Set(x,y,x+length-1,y+width-1); //�����ַ

    Interface_mian_read(Space_Picture_addressH1); //��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressH2);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressH3);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressH4);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressH5);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//�������ݡ�


    osDelay(88);
    //��ʾ��7��ͼƬ
    LCD_Address_Set(x,y,x+length-1,y+width-1);//�����ַ

    Interface_mian_read(Space_Picture_addressG1);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressG2);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressG3);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressG4);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressG5);
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//�������ݡ�


    osDelay(88);
    //��ʾ��6��ͼƬ
    LCD_Address_Set(x,y,x+length-1,y+width-1); //�����ַ

    Interface_mian_read(Space_Picture_addressF1); //��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressF2);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressF3);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressF4);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressF5);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//�������ݡ�



    osDelay(88);
    //��ʾ��5��ͼƬ
    LCD_Address_Set(x,y,x+length-1,y+width-1);//�����ַ

    Interface_mian_read(Space_Picture_addressE1);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressE2);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressE3);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressE4);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressE5);
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//�������ݡ�
    osDelay(88);


    //��ʾ��4��ͼƬ
    LCD_Address_Set(x,y,x+length-1,y+width-1); //�����ַ

    Interface_mian_read(Space_Picture_addressD1); //��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressD2);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressD3);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressD4);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressD5);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//�������ݡ�


    osDelay(88);
    //��ʾ��3��ͼƬ
    LCD_Address_Set(x,y,x+length-1,y+width-1);//�����ַ

    Interface_mian_read(Space_Picture_addressC1);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressC2);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressC3);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressC4);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressC5);
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//�������ݡ�


    osDelay(88);
    //��ʾ��2��ͼƬ
    LCD_Address_Set(x,y,x+length-1,y+width-1); //�����ַ

    Interface_mian_read(Space_Picture_addressB1); //��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressB2);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressB3);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressB4);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressB5);//��ȡͼƬ
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//��������


    osDelay(88);
    //��ʾ��һ��ͼƬ
    LCD_Address_Set(x,y,x+length-1,y+width-1);

    Interface_mian_read(Space_Picture_addressA1);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressA2);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressA3);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressA4);
    HAL_SPI_Transmit(&hspi1,rdata,4096,100);//��������

    Interface_mian_read(Space_Picture_addressA5);
    HAL_SPI_Transmit(&hspi1,rdata,1616,100);//�������ݡ�


}









uint8_t words_len1 = 0; //��������������Ϣ�ĳ���
uint8_t words_len2 = 0; //���������������Ϣ�ĳ���
uint8_t words_len3 = 0; //֪ͨ��Ϣ�ĳ���
uint8_t words_len4 = 0; //֪ͨ����¼�ĺ��ֵĳ���

uint8_t words1[6] = {0}; //������������
uint8_t words2[6] = {0}; //�������������
unsigned char words3[40] = {0}; //��ӱ���֪ͨ��Ϣ�����ݵ�buffer
unsigned char words4[40] = {0}; //��ӱ��汸��¼��Ϣ�����ݵ�buffer

uint8_t day_1_H = 0;//���������������
uint8_t day_1_L = 0;//���������������
uint8_t day_2_H = 0;//����������������
uint8_t day_2_L = 0;//����������������

uint8_t words_month = 0;//���汸��¼����
uint8_t words_day = 0;//���汸��¼����

/*******************************************
* @function: 	д��������֪ͨ����¼����Ϣ ������ĵ�30λ��ʼ������������
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.5.28
*******************************************/
void notify_Set_write(void)
{
    unsigned char i=0;

    //�����ݴ��FLASH;

    //��FLASH��Ƭѡ�źţ��ر���ĻƬѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//�͵�ƽѡ��

    //��ֹд������ʱ����������ݣ��Ƚ�����ȡ���������������º��ٽ����ݴ��FLASH��
    ReadData_4096(Data_address);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��

    rdata[30] = words_len1; //������������ݳ��� ��ֵ��ΧΪ2��6

    rdata[31] = words1[0]; //������������� �������� �� ����
    rdata[32] = words1[1];
    rdata[33] = words1[2];
    rdata[34] = words1[3];
    rdata[35] = words1[4];
    rdata[36] = words1[5];

    rdata[37] = day_1_H; //����������������
    rdata[38] = day_1_L; //����������������

    rdata[40] = words_len2; //������������ݳ��� ��ֵ��ΧΪ2��6

    rdata[41] = words2[0]; //������������� �������� �� ����
    rdata[42] = words2[1];
    rdata[43] = words2[2];
    rdata[44] = words2[3];
    rdata[45] = words2[4];
    rdata[46] = words2[5];

    rdata[47] = day_2_H; //����������������
    rdata[48] = day_2_L; //����������������

    rdata[50] = words_len3; //֪ͨ��Ϣ���ȣ���ֵ��Χ��2��40
    for(i=0; i<40; i++)  //����֪ͨ��Ϣ
    {
        rdata[51+i] = words3[i];
    }

    rdata[100] = words_len4; //����¼�ĺ���˫���ĳ��ȣ���ֵ��Χ��2��40
    for(i=0; i<40; i++)  //���汸��¼�ĺ���GB2312��������
    {
        rdata[101+i] = words4[i];
    }

    rdata[150] = words_month; //���汸��¼����
    rdata[151] = words_day;  //���汸��¼����

    WriteData_4096(Data_address,rdata);//����ȡ��������д��洢��


    //����Ļ��Ƭѡ�źţ��ر�W26Q64��Ƭѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//�͵�ƽѡ��

//	printf("����д�����%s ����¶�%d ����¶�%d\r\n",words1, day_1_H, day_1_L);
//	printf("����д������%s ����¶�%d ����¶�%d\r\n",words1, day_2_H, day_2_L);
//	printf("FLASH write notify��%s notify length��%d\r\n",words3, words_len3);
    printf("FLASH write memo��%s memo length��%d\r\n",words4, words_len4);

}





/*******************************************
* @function: 	��ȡ������֪ͨ����¼����Ϣ
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.5.28
*******************************************/

unsigned char buffer_notify[40] = {0};

void notify_Set_read(void)
{
    unsigned char i=0;

    //�����ݴ��FLASH;

    //��FLASH��Ƭѡ�źţ��ر���ĻƬѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//�͵�ƽѡ��

    //��ֹд������ʱ����������ݣ��Ƚ�����ȡ���������������º��ٽ����ݴ��FLASH��
    ReadData_4096(Data_address);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��

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

    words_len3 = rdata[50]; //֪ͨ��Ϣ���ȣ���ֵ��Χ��2��40

    for(i=0; i<40; i++)  //����֪ͨ��Ϣ
    {
        words3[i] = rdata[51+i];
    }

    words_len4 = rdata[100]; //֪ͨ��Ϣ���ȣ���ֵ��Χ��2��40

    for(i=0; i<40; i++)  //����֪ͨ��Ϣ
    {
        words4[i] = rdata[101+i];
    }

    words_month = rdata[150]; //���汸��¼����
    words_day = rdata[151];  //���汸��¼����

    //����Ļ��Ƭѡ�źţ��ر�W26Q64��Ƭѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//�͵�ƽѡ��
//	printf("����д�����%s ����¶�%d ����¶�%d\r\n",words1, day_1_H, day_1_L);
//	printf("����д������%s ����¶�%d ����¶�%d\r\n",words1, day_2_H, day_2_L);
//	printf("FLASH read notify��%s notify length��%d\r\n",words3, words_len3);
    printf("FLASH read memo��%s memo length��%d\r\n",words4, words_len4);
}



/*******************************************
* @function: 	�μ�������ʾ֪ͨ��Ϣ
* @input:			��
* @return:		��
* @author:		��С��
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
		printf("�����˴�һ������\r\n");

		notify_Set_read(); //��flash�ڶ�ȡ֪ͨ�������Ϣ

		get_time();

		LCD_Draw_flag = 0;

		//R_falg = 0;

		LCD_Fill(0,0,240,240,LGRAYBLUE);

		//			��ʾ���ڲ���
		LCD_ShowIntNum(5,6,sDate.Month,2,WHITE,LGRAYBLUE,32,0);//��ʾ��������

		LCD_show_Chinese(5+32,6,WHITE,"��",1,32);

		LCD_ShowIntNum(5+32+32,6,sDate.Date,2,WHITE,LGRAYBLUE,32,0);//��ʾ��������

		LCD_show_Chinese(5+32+32+32,6,WHITE,"��",1,32);

		switch (sDate.WeekDay)
		{
			case RTC_WEEKDAY_MONDAY    : LCD_show_Chinese(5+32+32+32+32,14,WHITE,"����һ",3,24); break;
			case RTC_WEEKDAY_TUESDAY   : LCD_show_Chinese(5+32+32+32+32,14,WHITE,"���ڶ�",3,24); break;
			case RTC_WEEKDAY_WEDNESDAY : LCD_show_Chinese(5+32+32+32+32,14,WHITE,"������",3,24); break;
			case RTC_WEEKDAY_THURSDAY  : LCD_show_Chinese(5+32+32+32+32,14,WHITE,"������",3,24); break;
			case RTC_WEEKDAY_FRIDAY    : LCD_show_Chinese(5+32+32+32+32,14,WHITE,"������",3,24); break;
			case RTC_WEEKDAY_SATURDAY  : LCD_show_Chinese(5+32+32+32+32,14,WHITE,"������",3,24); break;
			case RTC_WEEKDAY_SUNDAY  	 : LCD_show_Chinese(5+32+32+32+32,14,WHITE,"������",3,24); break;
		}

		//��ʾ��������
		LCD_show_Chinese(5,34,WHITE,"����",2,24);
		LCD_show_Chinese(5+40,34,WHITE,words1,words_len1,24);

		LCD_ShowIntNum(5,54,day_1_H,2,WHITE,LGRAYBLUE,24,0);//��ʾ��������

		LCD_show_Chinese(5+24,54,WHITE,"��",1,24);

		LCD_ShowIntNum(5+24+24,54,day_1_L,2,WHITE,LGRAYBLUE,24,0);//��ʾ��������

		LCD_show_Chinese(5+24+24+24,54,WHITE,"��",1,24);


		LCD_show_Chinese(110,34,WHITE,"����",2,24);
		LCD_show_Chinese(110+40,34,WHITE,words2,words_len2,24);

		LCD_ShowIntNum(110,54,day_2_H,2,WHITE,LGRAYBLUE,24,0);//��ʾ��������

		LCD_show_Chinese(110+24,54,WHITE,"��",1,24);

		LCD_ShowIntNum(110+24+24,54,day_2_L,2,WHITE,LGRAYBLUE,24,0);//��ʾ��������

		LCD_show_Chinese(110+24+24+24,54,WHITE,"��",1,24);

		//��ʾ�Ʋ�������



		//	��ʾ��Ϣ����

		LCD_show_Chinese(18,96,WHITE,"��Ϣ֪ͨ��",5,24);

		//ÿ�����ʮ����
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

//�������Ե�  ����flash
void LCD_Draw_Interface_notify(void)
{
    unsigned char i =0;

    if(LCD_Draw_flag == 1)
    {
        printf("�����˴�һ������\r\n");


        get_time();
        LCD_Draw_flag = 0;

        //R_falg = 0;

        LCD_Fill(0,0,240,240,LGRAYBLUE);
        //LCD_ShowIntNum(5,54,day_1_H,2,WHITE,LGRAYBLUE,24,0);//��ʾ��������

    }
    osDelay(300);
}

/*******************************************
* @function: 	�ζ������棬��ʾ����¼����
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.5.28
*******************************************/

//���� ����flash
void LCD_Draw_Interface_Memo(void)
{
    unsigned char i =0;

    unsigned char words_buffer_reve[40] = {0};

    if(LCD_Draw_flag == 1)
    {
        LCD_Draw_flag = 0;

        printf("��ʾ����¼����\r\n");

        //	notify_Set_read(); //��flash�ڶ�ȡ֪ͨ�������Ϣ

        LCD_Fill(0,0,240,240,YELLOW);

        LCD_show_Chinese(18,10,BLACK,"����¼",3,32);

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

	printf("��ʾ����¼����\r\n");

	notify_Set_read(); //��flash�ڶ�ȡ֪ͨ�������Ϣ

	LCD_Fill(0,0,240,240,YELLOW);

	LCD_show_Chinese(18,10,BLACK,"����¼",3,32);

	LCD_DrawLine(20,104,220,104,RED);
	LCD_DrawLine(20,144,220,144,RED);
	LCD_DrawLine(105,204,220,204,RED);

	//ÿ�����ʮ����
	if(words_len4 > 20)
	{
		for(i=0; i<20; i++)
		{
			words_buffer_reve[i] = words4[i];
		}
		LCD_show_Chinese(18,80,BLACK,words_buffer_reve,10,24); //��ʾ��һ�к���

		for(i=20; i<words_len4; i++)
		{
			words_buffer_reve[i-20] = words4[i];
		}
		LCD_show_Chinese(18,120,BLACK,words_buffer_reve,(words_len4-20)/2,24);	//��ʾ�ڶ��к���
	}

	LCD_ShowIntNum(120,180,words_month,2,BLACK,WHITE,24,1);

	LCD_show_Chinese(120+24,180,BLACK,"��",1,24);

	LCD_ShowIntNum(120+24+20,180,words_day,2,BLACK,WHITE,24,1);

	LCD_show_Chinese(120+24+20+24,180,BLACK,"��",1,24);

}
osDelay(300);
}
*/




/*******************************************
* @function: 	�μ����棬��ʾָ����
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.5.28
*******************************************/
float pitch,roll,yaw; 		//ŷ����
float hyaw; 		//ŷ����
short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;	//������ԭʼ����
short temp;					//�¶�
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

        LCD_show_Chinese(80,180,BLACK,"����",2,32);

        LCD_ShowIntNum(128,180,0,3,BLACK,WHITE,32,1);

        MPU_Init();					//��ʼ��MPU6050

        while(mpu_dmp_init())
        {
            printf("MPU6050 error\r\n");
            HAL_Delay(10);
        }
        printf("MPU6050 init ok ------>\r\n");

    }



    if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
    {
        temp=MPU_Get_Temperature();	//�õ��¶�ֵ

        MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������

//			printf("%d %d %d \r\n",aacx,aacy,aacz);

        MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������

        printf("���������� %.1f %.1f %.1f \r\n",pitch,roll,yaw);
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
//		LCD_DrawLine(120,100,120+(int)x,100-(int)y,WHITE); //�����ʷ
//
//		x = sin(yaw*PI/180)*65;
//		y = cos(yaw*PI/180)*65;
//		LCD_DrawLine(120,100,120+(int)x,100-(int)y,RED);

    }


    //��ȡָ����Ƕ�
//	Curent_Angle = HMC5883_Get_Angle();
//
//	printf("ָ���� %.0f %d %d \r\n",Curent_Angle,HMC_X,HMC_Y,HMC_Z);

    osDelay(10);

    //LCD_DrawLine(120,100,120+(int)hx,100-(int)hy,WHITE); //�����ʷ
}




/*******************************************
* @function: 	�μ��������ʪ����Ϣ��ʾ����
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.5.28
*******************************************/
unsigned char history_time_seconds=0;

void LCD_Draw_Interface_informations(void)
{
    if(LCD_Draw_flag == 1)
    {
        LCD_Draw_flag = 0;

        LCD_Fill(0,0,240,240,BLACK);

        LCD_show_Chinese(5,60,WHITE,"��",1,32);
        LCD_show_Chinese(5,140,WHITE,"ʪ",1,32);

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

        LCD_show_Chinese(45+5+48+12,64,WHITE,"��",1,24);
        LCD_show_Chinese(45+5+48+12,144,WHITE,"��",1,24);
    }

    get_time();

    if(	history_time_seconds != sTime.Seconds)
    {
        history_time_seconds = sTime.Seconds;

        measure_Si7021();//ÿ���������

        printf("�¶�:%.2f ʪ��:%.2f \r\n",TEMP_buf,Humi_buf);

        LCD_ShowFloatNum1(45+5,64,	TEMP_buf,4,WHITE,GREEN,24,0);//��ʾ��������
        LCD_ShowFloatNum1(45+5,144,	Humi_buf,4,WHITE,BLUE,24,0);//��ʾ��������
    }
    osDelay(300);
}




/*******************************************
* @function: 	�μ�����ķ����ӵ�ԲȦ��״ˢ����ʾͼ��
* @input:			���ֱ�����ɫ�����ӣ�����
* @return:		��
* @author:		��С��
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
    LCD_ShowIntNum(120-24-12,100-12,stime_min,2,RED,color,32,0);//ˢ��ͼ���еļ�ʱʱ��ķ������֡�

    LCD_ShowIntNum(120+12,100-12,stime_sec,2,RED,color,32,0);//ˢ��ͼ���еļ�ʱʱ����������֡�


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
* @function: 	�μ���������ʾ����ĳ�ʼ����ʾ����ʾһȦԲȦ
* @input:			��
* @return:		��
* @author:		��С��
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

    LCD_show_Chinese(120,100-12,RED,"��",1,24);

    LCD_ShowIntNum(120+12,100-12,0,2,RED,0,32,1);
}




/*******************************************
* @function: 	�����ڴμ�����ķ�������ʾ�����ʱ�䴢�溯��
* @input:			��
* @return:		��
* @author:		��С��
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
    //printf("����:%d:%d:%d   ʱ��:%d:%d:%d \r\n",sDate.Year,	sDate.Month,	sDate.Date,	sTime.Hours, sTime.Minutes, sTime.Seconds);

    //�����ݴ��FLASH;

    //����ĻƬѡ�źţ��ر�FLASH��Ƭѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//�͵�ƽѡ��

    //��ֹд������ʱ����������ݣ��Ƚ�����ȡ���������������º��ٽ����ݴ��FLASH��
    ReadData_4096(Data_address);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��

    rdata[0] = sDate.Month;
    rdata[1] = sDate.Date;
    rdata[2] = sTime.Hours;
    rdata[3] = sTime.Minutes;
    rdata[4] = sTime.Seconds;

    WriteData_4096(Data_address,rdata);//����ȡ��������д��洢��

    ReadData_4096(Data_address);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��
    time_buffer[0] = rdata[0];
    time_buffer[1] = rdata[1];
    time_buffer[2] = rdata[2];
    time_buffer[3] = rdata[3];
    time_buffer[4] = rdata[4];

    an[0] = rdata[2];
    an[1] = rdata[3];
    an[2] = rdata[4];
    //printf("����:%d:%d %d:%d:%d \r\n",time_buffer[0],	time_buffer[1],	time_buffer[2],	time_buffer[3], time_buffer[4]);

    //��W26Q64��Ƭѡ�źţ��ر���Ļ��Ƭѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//�͵�ƽѡ��

    return an;
}


/********  https://blog.csdn.net/qq_44179528/article/details/117339038?spm=1001.2014.3001.5501
* @function: 	����ʱ����֮���ʱ��
* @input:			���ڵ�ʱ������飬���֮���ʱ������飬����˳��ֱ���ʱ����
* @return:		֮���ʱ�������
* @author:		��С��
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
    //printf("������%d %d %d\r\n",an[0], an[1], an[2]);

    return an;
}




/********  https://blog.csdn.net/qq_44179528/article/details/117339038?spm=1001.2014.3001.5501
* @function: 	�������ڵ�ʱ���֮ǰ��ʱ���ֵ
* @input:			��ȥ��ʱ������飬���ڵ�ʱ������飬����˳��ֱ���ʱ����
* @return:		��ֵʱ�������
* @author:		��С��
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
    printf("ʱ���Ϊ��%d %d %d \r\n",hours,mins,seconds);
    //return(seconds+mins*60+hours*60*60);
    return time;
}


/*
	������. �����ӣ��Ǹ��ݷ��ѹ��������۽��п�����һ��㡢ʵ�õ��ճ̹��������.
	�ǰѹ�������ֽ�ɰ�Сʱ���ң����о�������25���ӣ�Ȼ����Ϣ5���ӣ�
	���������һ�������ѡ����������ѹ���������������ʹ��һ��30���Ӹ��ж�����
*/
//����һ��ʼ����ʱ����ͣ�е�״̬�������󰴼�����������״̬���ٳԳ����󰴼�������ͣ��״̬��ѭ��
//�����Ұ����������¼�ʱ
#define stop_state 0
#define working_state 1
#define sleeping_state 2

unsigned char tomato_state = stop_state;

unsigned char begin_sign = 0; //���������������˵ı�־
unsigned char work_state = 0;
unsigned char history_seconds = 0;

#define dieoutTime 30 //�ڴ���Ƽ�ʱʱ�� ����ʱ25����Ϊ1500����
#define dieoutTime2 50 //�ڴ���Ƽ�ʱʱ�� ����ʱ25����Ϊ1500����


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

            LCD_show_Chinese(120-20-8,200,BLACK,"��ͣ��",3,24);
        }

        if(work_state == 1)
        {
            LCD_Fill(0,0,240,240,YELLOW);

            LCD_Draw_Interface_tomato_init();

            LCD_show_Chinese(120-20-8,200,BLACK,"������",3,24);
        }

        if(work_state == 2)
        {
            LCD_Fill(0,0,240,240,GREEN);

            LCD_Draw_Interface_tomato_init();

            LCD_show_Chinese(120-20-8,200,BLACK,"��Ϣ��",3,24);
        }
    }


    osDelay(300);

    if( Interface == Interface_tomato )
        switch(work_state)
        {
        //��ͣ״̬
        case stop_state:
        {
            if(begin_sign == 1) //��⵽�г��������¼����� ��ת����״̬
            {
                begin_sign = 0;  //������������������˵ı�־


                work_state = working_state;//��ת״̬����ת������״̬
                printf("�����ӿ�ʼ��ʱ��\r\n");

                //���½�����ʾ
                LCD_Fill(0,0,240,240,YELLOW);
                LCD_Draw_Interface_tomato_init();
                LCD_show_Chinese(120-20-8,200,BLACK,"������",3,24);

                get_time();
                flash_buffer[0] = sTime.Hours;
                flash_buffer[1] = sTime.Minutes;
                flash_buffer[2] = sTime.Seconds;

                //printf("һ��ʼ��ʱ�� flash_buffer %d %d %d \r\n",flash_buffer[0], flash_buffer[1], flash_buffer[2]);

                //flash_buffer = LCD_Draw_Interface_tomato_Update_timing(); //��ʼ����һ��ʼ�����ȼ�¼��ʼ������ʱ�䡣���� time_buffer ��

                get_time();	//��ȡ��ǰʱ��
                now_time_buffer[0] = sTime.Hours;//����ǰʱ���ŵ�������
                now_time_buffer[1] = sTime.Minutes;
                now_time_buffer[2] = sTime.Seconds;

                time_difference = GET_Time_after(now_time_buffer,centre_time_buffer);//����ӵ�ǰʱ�俪ʼ֮��Ĺ���ʱ��Ľ���ʱ�䡣��ŵ�������
//
//				hour_sleep = time_difference[0];
//				min_sleep = time_difference[1];
//				sec_sleep = time_difference[2];

                set_Alarm(time_difference[0], time_difference[1], time_difference[2]);//����RTC���ӵ�ʱ��
            }


            break;
        }

        //����״̬
        case working_state:
        {
            if(begin_sign == 1)//��⵽�г��������¼����� ��ת����״̬ //��ת����ͣ״̬
            {
                begin_sign = 0;  //������������������˵ı�־

                work_state = stop_state; //��ת״̬����ת����ͣ״̬

                //���½�����ʾ ��ͣ����
                LCD_Fill(0,0,240,240,BLUE);
                LCD_Draw_Interface_tomato_init();
                LCD_show_Chinese(120-20-8,200,BLACK,"��ͣ��",3,24);

                break;
            }


            get_time();		//��ȡ��ǰʱ��
            now_time_buffer[0] = sTime.Hours;
            now_time_buffer[1] = sTime.Minutes;
            now_time_buffer[2] = sTime.Seconds;
            //printf("����ʱ�� flash_buffer %d %d %d \r\n",flash_buffer[0], flash_buffer[1], flash_buffer[2]);

            time_difference = GET_Time_difference(flash_buffer,now_time_buffer);//��������ʱ��֮��Ĳ�

            LCD_Draw_Interface_tomato_Print(YELLOW, time_difference[1], time_difference[2]); //��ʾ����ʣ��ʱ��״̬�Ķ���


            //��⵽�Ѿ����˹���ʱ��
            if( (time_difference[2]+time_difference[1]*60+time_difference[0]*60*60) > dieoutTime)
            {
                printf("��⵽�Ѿ����˹���ʱ�� %d %d %d \r\n",time_difference[0], time_difference[1], time_difference[2]);

                work_state = sleeping_state; //��ת״̬����ת����Ϣ״̬

                Transient_vibration(); //����ʾ

                //���½���
                LCD_Fill(0,0,240,240,GREEN);
                LCD_Draw_Interface_tomato_init();
                LCD_show_Chinese(120-20-8,200,BLACK,"��Ϣ��",3,24);
            }
            break;
        }

        //��Ϣ״̬
        case sleeping_state:
        {
            if(begin_sign == 1)//��⵽�г��������¼����� ��ת����״̬ //��ת����ͣ״̬
            {
                begin_sign = 0;  //������������������˵ı�־

                work_state = stop_state; //��ת״̬����ת����ͣ״̬

                //���½�����ʾ ��ͣ����
                LCD_Fill(0,0,240,240,BLUE);
                LCD_Draw_Interface_tomato_init();
                LCD_show_Chinese(120-20-8,200,BLACK,"��ͣ��",3,24);

                break;
            }

            get_time();		//��ȡ��ǰʱ��
            now_time_buffer[0] = sTime.Hours;
            now_time_buffer[1] = sTime.Minutes;
            now_time_buffer[2] = sTime.Seconds;

            //printf("��Ϣʱ�� flash_buffer %d %d %d \r\n",flash_buffer[0], flash_buffer[1], flash_buffer[2]);

            time_difference = GET_Time_difference(flash_buffer,now_time_buffer);
            LCD_Draw_Interface_tomato_Print(GREEN, time_difference[1], time_difference[2]); //��ʾ����ʣ��ʱ��״̬�Ķ���


            //��⵽�Ѿ�������Ϣʱ��
            if( (time_difference[2]+time_difference[1]*60+time_difference[0]*60*60) > dieoutTime2)
            {
                printf("��⵽�Ѿ�������Ϣʱ�� %d %d %d \r\n\r\n\r\n",time_difference[0], time_difference[1], time_difference[2]);

                work_state = working_state; //��ת״̬����ת�������е�״̬

                Transient_vibration(); //����ʾ

                get_time();		//��ȡ��ǰʱ��
                flash_buffer[0] = sTime.Hours;
                flash_buffer[1] = sTime.Minutes;
                flash_buffer[2] = sTime.Seconds;

                //flash_buffer = LCD_Draw_Interface_tomato_Update_timing(); //��ʼ����һ��ʼ�����ȼ�¼��ʼ������ʱ�䡣���� time_buffer ��

                LCD_Fill(0,0,240,240,YELLOW);
                LCD_Draw_Interface_tomato_init();
                LCD_show_Chinese(120-20-8,200,BLACK,"������",3,24);
            }

            break;
        }
        }
}





/*******************************************
* @function: 	�����ڴμ�����ķ�������ʾ�����ʱ�䴢�溯��
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.5.28
*******************************************/
void LCD_Draw_Interface_Pedometer_write_STEPS(unsigned long step1, unsigned long step2, unsigned long step3)
{

    //�����ݴ��FLASH;

    //��FLASH��Ƭѡ�źţ��ر���ĻƬѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//�͵�ƽѡ��

    //��ֹд������ʱ����������ݣ��Ƚ�����ȡ���������������º��ٽ����ݴ��FLASH��
    ReadData_4096(Data_address);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��

    rdata[10] = step1/1000;
    rdata[11] = (step1 - rdata[10]*1000)/10;
    rdata[12] = (step1 - rdata[10]*1000 - rdata[11]*10);


    rdata[13] = step2/1000;
    rdata[14] = (step2 - rdata[13]*1000)/10;
    rdata[15] = (step2 - rdata[13]*1000 - rdata[14]*10);

    rdata[16] = step3/1000;
    rdata[17] = (step3 - rdata[16]*1000)/10;
    rdata[18] = (step3 - rdata[16]*1000 - rdata[17]*10);

    printf("д�뵽������\r\n");
    printf("%d%d%d \r\n",rdata[10], rdata[11], rdata[12]);
    printf("%d%d%d \r\n",rdata[13], rdata[14], rdata[15]);
    printf("%d%d%d \r\n\r\n",rdata[16], rdata[17], rdata[18]);

    WriteData_4096(Data_address,rdata);//����ȡ��������д��洢��


    //����Ļ��Ƭѡ�źţ��ر�W26Q64��Ƭѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//�͵�ƽѡ��

}





/*******************************************
* @function: 	�����ڴμ�����ķ�������ʾ�����ʱ�䴢�溯��
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.5.28
*******************************************/

unsigned long step_buffer[3];

void LCD_Draw_Interface_Pedometer_read_STEPS(void)
{

    //�����ݴ��FLASH;

    //��FLASH��Ƭѡ�źţ��ر���ĻƬѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//�͵�ƽѡ��

    //��ֹд������ʱ����������ݣ��Ƚ�����ȡ���������������º��ٽ����ݴ��FLASH��
    ReadData_4096(Data_address);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��

    step_buffer[0] = (unsigned long)(rdata[10]*1000 + rdata[11]*10 + rdata[12]);

    step_buffer[1] = (unsigned long)(rdata[13]*1000 + rdata[14]*10 + rdata[15]);

    step_buffer[2] = (unsigned long)(rdata[16]*1000 + rdata[17]*10 + rdata[18]);


    //����Ļ��Ƭѡ�źţ��ر�W26Q64��Ƭѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//�͵�ƽѡ��

    printf("����������");
    printf("%d %d %d \r\n",(int)step_buffer[0], (int)step_buffer[1], (int)step_buffer[2]);
}



float Pedometer_height = 0;

int STEPS_value = 0; //��¼����

unsigned long STEPS_history = 0;

void LCD_Draw_Interface_Pedometer(void)
{
    //��ʾ����ĳ�ʼ��//��ʾ��ʼ��ͼ��
    if(LCD_Draw_flag == 1)
    {
        LCD_Draw_flag = 0;

        LCD_Fill(0,0,240,90,WHITE);
        LCD_Fill(0,90,240,240,BLUE);

        LCD_show_Chinese(20,20,BLACK,"�����ǳ�ʼ����",7,24); //��ʾ����
        LCD_show_Chinese(20,52,BLACK,"�뱣��ˮƽ����",7,24); //��ʾ����

        MPU_Init();					//��ʼ��MPU6050
        while(mpu_dmp_init())
        {
            printf("MPU6050 error\r\n");
            HAL_Delay(10);
        }
        printf("MPU6050 init ok ------>\r\n");

        dmp_get_pedometer_step_count(&STEPS); //�Ʋ�����ȡ������

        LCD_Fill(0,0,240,90,WHITE);
        LCD_Fill(0,90,240,240,BLUE);


        LCD_DrawLine(20,210,20,100,WHITE);
        LCD_DrawLine(20,210,220,210,WHITE);

        LCD_DrawLine(20,100,20+6,100+6,WHITE);
        LCD_DrawLine(20,100,20-6,100+6,WHITE);

        LCD_DrawLine(220,210,220-6,210-6,WHITE);
        LCD_DrawLine(220,210,220-6,210+6,WHITE);


        LCD_Draw_Interface_Pedometer_read_STEPS(); //��������flash�ж�ȡ�� step_buffer ��
        //printf("%d %d %d \r\n",step_buffer[0], step_buffer[1], step_buffer[2]);

        step_buffer[1] = 45;//Ϊ�˷�����ʾ
        step_buffer[2] = 2; //Ϊ�˷�����ʾ
        //step_buffer[0] = STEPS+step_buffer[0];


        //LCD_show_Chinese(10,92,WHITE,"��ʷ��¼",4,24); //��ʾ����
        LCD_ShowIntNum(40,240-24,step_buffer[0],5,WHITE,WHITE,16,1);//��ʾ��������
        LCD_ShowIntNum(100,240-24,step_buffer[1],5,WHITE,WHITE,16,1);//��ʾ��������
        LCD_ShowIntNum(160,240-24,step_buffer[2],5,WHITE,WHITE,16,1);//��ʾ��������


        LCD_show_Chinese(30,15,BLACK,"������",3,32); //��ʾ����
        LCD_ShowIntNum(32*3+25,15,step_buffer[0],5,BLACK,WHITE,32,0);//��ʾ�������� ����Ĳ���ͳ��

        LCD_show_Chinese(24*4+22+5,57,BLACK,"ǧ��·��",4,24); //��ʾ����
        LCD_ShowFloatNum1(50,57, step_buffer[0]*0.04, 5,BLACK,WHITE,24,0);//��ʾ��·��

//		LCD_Fill(45,105,75,210,RED);
//		LCD_Fill(105,105,105+30,210,RED);
//		LCD_Fill(165,105,165+30,210,RED);


        if(step_buffer[0] > 20000)
        {
            LCD_Fill(45,105,75,210,RED);	//����Ĳ���ͳ����״ͼ
        }
        else if(step_buffer[0] > 10000)
        {
            Pedometer_height = step_buffer[0]*105/20000-1;
            LCD_Fill(45,210-(int)Pedometer_height,75,210,RED);	//����Ĳ���ͳ����״ͼ
            //printf("%d \r\n",(int)Pedometer_height1);
        }
        else
        {
            Pedometer_height = step_buffer[0]*105/20000-1;
            LCD_Fill(45,210-(int)Pedometer_height+20,75,210,RED);	//����Ĳ���ͳ����״ͼ
            //printf("%d \r\n",(int)Pedometer_height1);
        }



        if(step_buffer[1] > 20000)
        {
            LCD_Fill(105,105,105+30,210,RED);	//����Ĳ���ͳ����״ͼ
        }
        else if(step_buffer[1] > 10000)
        {
            Pedometer_height = step_buffer[1]*105/20000-1;
            LCD_Fill(105,210-(int)Pedometer_height,105+30,210,RED);	//����Ĳ���ͳ����״ͼ
            //printf("%d \r\n",(int)Pedometer_height1);
        }
        else
        {
            Pedometer_height = step_buffer[1]*105/20000-1;
            LCD_Fill(105,210-(int)Pedometer_height-20,105+30,210,RED);	//����Ĳ���ͳ����״ͼ
            //printf("%d \r\n",(int)Pedometer_height1);
        }



        if(step_buffer[2] > 20000)
        {
            LCD_Fill(165,105,165+30,210,RED);	//����Ĳ���ͳ����״ͼ
        }
        else if(step_buffer[2] > 10000)
        {
            Pedometer_height = step_buffer[2]*105/20000-1;
            LCD_Fill(165,210-(int)Pedometer_height,165+30,210,RED);	//����Ĳ���ͳ����״ͼ
            //printf("%d \r\n",(int)Pedometer_height1);
        }
        else
        {
            Pedometer_height = step_buffer[2]*105/20000-1;
            LCD_Fill(165,210-(int)Pedometer_height-20,165+30,210,RED);	//����Ĳ���ͳ����״ͼ
            //printf("%d \r\n",(int)Pedometer_height1);
        }
    }


    if(STEPS_history != STEPS)
    {
        STEPS_history = STEPS;

        step_buffer[0] = STEPS+step_buffer[0];

        LCD_Draw_Interface_Pedometer_write_STEPS(step_buffer[0],234,4);

        LCD_ShowIntNum(32*3+25,15,(uint16_t)step_buffer[0],5,BLACK,WHITE,32,0);//��ʾ�������� ����Ĳ���ͳ��

        LCD_ShowFloatNum1(50,57,(uint16_t)step_buffer[0]*0.04,5,BLACK,WHITE,24,0);//��ʾ��·��

        LCD_ShowIntNum(40,240-24,(uint16_t)step_buffer[0],5,WHITE,BLUE,16,0);//��ʾ��������

        step_buffer[0] = (unsigned long)STEPS;

        //����ͳ��ͼ
        if(step_buffer[0] > 20000)
        {
            LCD_Fill(45,105,75,210,RED);	//����Ĳ���ͳ����״ͼ
        }
        else if(step_buffer[0] > 10000)
        {
            Pedometer_height = step_buffer[0]*105/20000-1;
            LCD_Fill(45,210-(int)Pedometer_height,75,210,RED);	//����Ĳ���ͳ����״ͼ
            //printf("%d \r\n",(int)Pedometer_height1);
        }
        else
        {
            Pedometer_height = step_buffer[0]*105/20000-1;
            LCD_Fill(45,210-(int)Pedometer_height-20,75,210,RED);	//����Ĳ���ͳ����״ͼ
            //printf("%d \r\n",(int)Pedometer_height);
        }
    }

    dmp_get_pedometer_step_count(&STEPS); //�Ʋ�����ȡ������
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
//	LCD_Fill(45,210-Pedometer_height1,75,210,RED);	//����Ĳ���ͳ����״ͼ

//	osDelay(400);
}









unsigned char light_time = 2;
int light_hight = 0;
/*******************************************
* @function: 	���������ù���ģʽ���洢�溯��
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.5.28
*******************************************/
void Interface_Set_write(void)
{

    //�����ݴ��FLASH;

    //��FLASH��Ƭѡ�źţ��ر���ĻƬѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//�͵�ƽѡ��

    //��ֹд������ʱ����������ݣ��Ƚ�����ȡ���������������º��ٽ����ݴ��FLASH��
    ReadData_4096(Data_address);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��

    rdata[20] = light_time;
    rdata[21] = light_hight;

    WriteData_4096(Data_address,rdata);//����ȡ��������д��洢��


    //����Ļ��Ƭѡ�źţ��ر�W26Q64��Ƭѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//�͵�ƽѡ��

    printf("   light_hight:%d light_time:%d\r\n",light_hight,light_time);
}





/*******************************************
* @function: 	�����ڴμ���������ù���ģʽ���洢�溯��
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.5.28
*******************************************/
void Interface_Set_read(void)
{
    //�����ݴ��FLASH;

    //��FLASH��Ƭѡ�źţ��ر���ĻƬѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);//�͵�ƽѡ��

    //��ֹд������ʱ����������ݣ��Ƚ�����ȡ���������������º��ٽ����ݴ��FLASH��
    ReadData_4096(Data_address);//�Ӵ洢����ȡ���ݵ��ڴ����� rdata ��

    light_time = rdata[20];

    light_hight = rdata[21];

    //����Ļ��Ƭѡ�źţ��ر�W26Q64��Ƭѡ�źš�
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET); //�͵�ƽѡ��
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//�͵�ƽѡ��

    printf("   light_hight:%d   light_time:%d\r\n",light_hight,light_time);
}



void set_screen_light(unsigned char lit)
{
    switch (lit)
    {
    case 0:
    {
        /*������Ļ����*/
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //������ʱ��PWMͨ�� ��ʱ��2 ͨ��3
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 400); //����PWM��ռ�ձȣ����ڱ���
        printf("400 \r\n");
        break;
    }
    case 1:
    {
        /*������Ļ����*/
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //������ʱ��PWMͨ�� ��ʱ��2 ͨ��3
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 300); //����PWM��ռ�ձȣ����ڱ���
        printf("300 \r\n");
        break;
    }
    case 2:
    {
        /*������Ļ����*/
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //������ʱ��PWMͨ�� ��ʱ��2 ͨ��3
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 250); //����PWM��ռ�ձȣ����ڱ���
        printf("250 \r\n");
        break;
    }
    case 3:
    {
        /*������Ļ����*/
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //������ʱ��PWMͨ�� ��ʱ��2 ͨ��3
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 150); //����PWM��ռ�ձȣ����ڱ���
        printf("150 \r\n");
        break;
    }
    }
}


/*******************************************
* @function: 	���ù���ģʽ����//����������������ʱ�䣬������Ļ����
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.5.28
*******************************************/

unsigned char key_long_flag = 0;

unsigned char Interface_Set_while_flag = 0;

unsigned char light_flag = 0;

unsigned char Set_while_flag = 0;



void LCD_Draw_Interface_Set(void)
{
    //��ʾ����ĳ�ʼ��//��ʾ��ʼ��ͼ��
    if(LCD_Draw_flag == 1)
    {
        LCD_Draw_flag = 0;

        LCD_Fill(0,0,240,240,BLACK); //�������ɫ

        LCD_show_Chinese(20,0,WHITE,"����",2,32); //��ʾ����

        LCD_Fill(0,0+80,20,20+80,LGRAY);

        LCD_show_Chinese(40,50,WHITE,"��Ļ����",4,32); //��ʾ����


        LCD_Fill(0,0+160,20,20+160,LGRAY);

        LCD_show_Chinese(40,130,WHITE,"����ʱ��",4,32); //��ʾ����

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

        Interface_Set_while_flag = 1; //�ý�����뵽ѭ���в��ٳ�ȥ

        printf("��⵽��ѭ������\r\n");
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

            //������һ��ѭ��
            if(key_long_flag == 1)
            {
                key_long_flag = 0;

                printf("������һ��ѭ��\r\n");
                LCD_Fill(0,0+160,20,20+160,WHITE);
                Set_while_flag = 1;
            }

            //������水��������������ʱ��Ķ���
            if(key_long_flag == 3)
            {
                key_long_flag = 0;

                printf("������������\r\n");

                light_hight--;

                if(light_hight<0)
                {
                    light_hight = 0;
                }
                set_screen_light(light_hight);

                Interface_Set_write(); //д�����falsh�ڵ���������

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

                printf("������Ļ����\r\n");
                light_hight++;

                if(light_hight>3)
                {
                    light_hight = 3;
                }
                set_screen_light(light_hight);
                Interface_Set_write(); //д�����falsh�ڵ���������
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
            //������һ��ѭ��
            if(key_long_flag == 1)
            {
                key_long_flag = 0;
                printf("������һ��ѭ��\r\n");
                LCD_Fill(0,0+160,20,20+160,WHITE);
                Set_while_flag = 0;
            }

            if(key_long_flag == 3)
            {
                key_long_flag = 0;

                printf("����Ϣ��ʱ��\r\n");

                light_time--;

                if(light_time<1)
                {
                    light_time=1;
                }
                Interface_Set_write(); //д�����falsh�ڵ���������

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

                printf("����Ϣ��ʱ��\r\n");
                light_time++;
                if(light_time>4)
                {
                    light_time=4;
                }
                Interface_Set_write(); //д�����falsh�ڵ���������

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

            Interface_Set_while_flag = 0; //�ָ�ѭ��ѭ��

            LCD_Fill(0,0+80,20,20+80,LGRAY);
            LCD_Fill(0,0+160,20,20+160,LGRAY);

            printf("�˳�������ѭ��\r\n");
        }
    }
}





unsigned char sleep_time_buffer[3] = {0,0,5};
unsigned char time_seconds_history[3] = {0};
unsigned char *time_seconds_history_;
void updata_sleep_time(void)
{
    get_time();	//��ȡ��ǰʱ��
    time_seconds_history[0] = sTime.Hours;//����ǰʱ���ŵ�������
    time_seconds_history[1] = sTime.Minutes;
    time_seconds_history[2] = sTime.Seconds;
    time_seconds_history_ = GET_Time_after(time_seconds_history,sleep_time_buffer);//����ӵ�ǰʱ�俪ʼ֮��Ĺ���ʱ��Ľ���ʱ�䡣��ŵ�������
}



void Checking_into_sleep(void)
{
    get_time();	//��ȡ��ǰʱ��
    if(sTime.Hours == time_seconds_history_[0] && sTime.Minutes >= time_seconds_history_[1] && sTime.Seconds >= time_seconds_history_[2])
    {
        low_power_model();
    }

}

unsigned char flash_write_flage = 0; //��ȡ�����豸����Ϣ�����flash�����ı�־λ

/*******************************************
* @function: 	���������������ȼ�Ϊ��͵�����
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.4.26
*******************************************/


unsigned char key_value[2];

unsigned char key_value_handle_flag = DONE;

unsigned char Interface = Interface_main;
//unsigned char Interface = Interface_Menu;
unsigned char Interface_Flag =0;

void task_base(void)
{
    //�鿴���������Ĵ���2 �Ƿ���յ�������
    while(1)
    {
        switch(Interface)
        {
        case Interface_main:
            LCD_Draw_Interface_main2();
            key_value_handle_flag = DONE;
            break;//Checking_into_sleep(); break; //������

        case Interface_notify:
            LCD_Draw_Interface_notify();
            key_value_handle_flag = DONE;
            break;  //����֪ͨ����

        case Interface_Memo:
            LCD_Draw_Interface_Memo();
            key_value_handle_flag = DONE;
            break;//Checking_into_sleep();break;  //���뱸��¼����

        case Interface_informations:
            LCD_Draw_Interface_informations();
            key_value_handle_flag = DONE;
            break;//Checking_into_sleep(); break;  //������ʪ����ʾ����

        case Interface_tomato :
            LCD_Draw_Interface_tomato();
            key_value_handle_flag = DONE;
            break;  //���뷬���ӽ���

        case Interface_Pedometer :
            LCD_Draw_Interface_Pedometer();
            key_value_handle_flag = DONE;
            break;  //����Ʋ�������

        case Interface_Set :
            LCD_Draw_Interface_Set();
            key_value_handle_flag = DONE;
            break;  //�������ý���
        }
        if(flash_write_flage == 1)
        {
            flash_write_flage = 0;

            notify_Set_write(); //д�����falsh�ڵ���������
        }
        //printf("����1 -->\r\n");

        //����͹��Ĵ���ģʽ
//		HAL_GPIO_WritePin(SYS_ON_GPIO_Port, SYS_ON_Pin, GPIO_PIN_RESET); //�ر�ϵͳ
//		low_power_model();
    }
}



/*******************************************
* @function: 	���������������ȼ�Ϊ��͵�����
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.4.26
*******************************************/
unsigned long STEPS = 0; //��¼����

unsigned char USB_in_SIGN = 0; //��ȡUSB����

unsigned char KEY_1; //����ֵ �������жϺ����ļ���
unsigned char KEY_2; //����ֵ

float pitch,roll,yaw; 		//ŷ����
short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;	//������ԭʼ����
short temp;					//�¶�

int key_times1=0; //����1���µ�ʱ��
int key_times2=0;	//����2���µ�ʱ��

unsigned char key_continuous_value = 0; //���泤���İ���ֵ


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
        //�鿴���������Ĵ���2 �Ƿ���յ�������
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
            {
                buferRX[i] = Rx2_Buf[i];
            }
            bufferLen = Rx2_Len;

            //HAL_UART_Transmit_DMA(&huart1, Rx2_Buf, Rx2_Len);

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

                    LCD_Draw_flag = 1; //����֪ͨ�������ʾ

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
                    sDate.Year = 21;

                    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
                    {
                        Error_Handler();
                    }
                    Transient_vibration(); //����ʾ


                    break;
                }
                case 't':
                {
                    printf("��ȡ��������\r\n");

                    LCD_Draw_flag = 1; //����֪ͨ�������ʾ
                    flash_write_flage = 1; //��ȡ�����豸����Ϣ�����flash�����ı�־λ
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
                        flash_write_flage = 1; //��ȡ�����豸����Ϣ�����flash�����ı�־λ

                        printf("֪ͨ��%s ֪ͨ���ȣ�%d\r\n",words3, words_len3);

                        break;
                    }
                    }
                    LCD_Draw_flag = 1; //����֪ͨ�������ʾ

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
                        flash_write_flage = 1; //��ȡ�����豸����Ϣ�����flash�����ı�־λ

                        LCD_Draw_flag = 1; //����ˢ�µı�־λ

                        get_time();

                        words_month = sDate.Month; //���汸��¼����
                        words_day = sDate.Date;  //���汸��¼����

                        printf("momo��%s momo length��%d  %d  %d\r\n",words4, words_len4,words_month, words_day);

                        break;
                    }
                    }
                    LCD_Draw_flag = 1; //����֪ͨ�������ʾ

                    Transient_vibration(); //����ʾ

                    break;
                }
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
                MX_USART2_UART_Init();;
            }
        }



        //���ݰ��°�����ʱ��ĳ����ж��ǲ��ǳ�������
        if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 1)
        {
            printf("get in task background ���ݰ��°���1��ʱ��ĳ����ж��ǲ��ǳ������� ");
            while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 1)
            {
                printf("%d\r\n",key_times1);
                osDelay(1);
                key_times1++;
                if(key_times1 > 400)
                {
                    printf("����1 %d\r\n",key_times1);
                    key_continuous_value = 1;
                    //Transient_vibration(); //˲����
                    osDelay(50);
                    //break;
                }
            }
        }

        //���İ岻��PA11��Ϊkey2 ������ʱע���� ����PB12 ��ʱע��
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
        			//printf("����2 %d\r\n",key_times2);
        			key_continuous_value = 2;
        			//Transient_vibration(); //˲����
        			osDelay(50);
        			//break;
        		}
        	}

        }
        */
        //���İ��key2
        if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12) == 1)
        {
            printf("get in task background ���ݰ��°���2��ʱ��ĳ����ж��ǲ��ǳ������� ");
            while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12) == 1)
            {
                printf("%d\r\n",key_times2);
                osDelay(1);
                key_times2++;
                if(key_times2 > 400)
                {
                    printf("����2 %d\r\n",key_times2);
                    key_continuous_value = 2;
                    //Transient_vibration(); //˲����
                    osDelay(50);
                    //break;
                }
            }

        }
        //���ݰ����ж��ж��ǲ����а������£�
        //ͬʱ�жϰ����¼��ǲ��Ǵ�����ɣ��ڴ�����ɰ���֮ǰ�����ٶ԰����¼��з�Ӧ
        if((key_continuous_value != 0 || KEY_1 == 1 || KEY_2 == 1 )&& key_value_handle_flag == DONE)
        {

            key_times2 = 0; //���㳤������
            key_times1 = 0;	//���㳤������

            key_value_handle_flag = DO; //ˢ�°�������״̬��־

            Transient_vibration(); //��

            switch(Interface) //���ݵ�ǰ����ѡ���ڰ��������Ժ�ȥ���ĸ�����
            {
            //������
            case Interface_main:
            {
                //��⵽�̰����������뵽��Ϣ֪ͨ����
                if(KEY_1 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_notify;
                    LCD_Draw_flag = 1;
                }
                //��⵽�̰����������뵽��Ϣ��ʾ����
                if(KEY_2 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_informations;
                    LCD_Draw_flag = 1;
                }
                //��⵽�������������뷬��ʱ�ӽ���
                if(key_continuous_value == 1)
                {
                    Interface = Interface_tomato;
                    LCD_Draw_flag = 1;
                }
                //��⵽��������������Ʋ�������
                if(key_continuous_value == 2)
                {
                    Interface = Interface_Pedometer;
                    LCD_Draw_flag = 1;
                }
                break;
            }

            //��Ϣ֪ͨ����
            case Interface_notify:
            {
                //��⵽�̰����������ص�������
                if(KEY_2 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_main;
                    LCD_Draw_flag = 1;
                }
                //��⵽�̰����������뱸��¼����
                if(KEY_1 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_Memo;
                    LCD_Draw_flag = 1;
                }
                break;
            }
            //��ʾ����¼����
            case Interface_Memo:
            {
                //��⵽�̰����������ص�������
                if(KEY_1 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_main;
                    LCD_Draw_flag = 1;
                }
                //��⵽�̰�����������֪ͨ����
                if(KEY_2 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_notify;
                    LCD_Draw_flag = 1;
                }
                break;
            }


            //��ʪ����Ϣ��ʾ����
            case Interface_informations:
            {
                //��⵽�̰����������ص�������
                if(KEY_1 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_main;
                    LCD_Draw_flag = 1;
                }
                //��⵽�̰����� ȥ�����ý���
                if(KEY_2 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_Set;
                    LCD_Draw_flag = 1;
                }
                break;
            }

            //��������Ϣʱ����ʾ����
            case Interface_tomato:
            {
                //��⵽�̰����������ص�������
                if(KEY_2 == 1 && key_continuous_value == 0)
                {
                    Interface = Interface_main;
                    LCD_Draw_flag = 1;
                }
                //��⵽�����������ڷ������ڲ����н�����ת
                if(key_continuous_value == 1)
                {
                    begin_sign = 1; //���������������˵ı�־
                }
                break;
            }


            //�Ʋ�����ʾ����
            case Interface_Pedometer:
            {
                //��⵽�̰����������ص�������
                if(KEY_1 == 1 && key_continuous_value == 0)
                {
                    //LCD_Draw_Interface_Pedometer_write_STEPS(step_buffer[0], 3782, 1879); //���������ڴ���д�뵽falsh��
                    Interface = Interface_main;
                    LCD_Draw_flag = 1;
                }
                if(KEY_2 == 1 && key_continuous_value == 0)
                {
                    //LCD_Draw_Interface_Pedometer_write_STEPS(step_buffer[0], 3782, 1879); //���������ڴ���д�뵽falsh��
                    Interface = Interface_main;
                    LCD_Draw_flag = 1;

                }
            }

            //��ʾ���ý���
            case Interface_Set:
            {
                //��⵽�̰����������ص���ʪ����Ϣ����
                if(KEY_1 == 1 && key_continuous_value == 0)
                {
                    key_long_flag = 3;
                    if(0 == Interface_Set_while_flag)
                    {
                        Interface = Interface_informations;
                        LCD_Draw_flag = 1;
                    }

                }
                //��⵽�̰����������ص�������
                if(KEY_2 == 1 && key_continuous_value == 0)
                {
                    key_long_flag = 4;
                    if(0 == Interface_Set_while_flag)
                    {
                        Interface = Interface_main;
                        LCD_Draw_flag = 1;
                    }
                }
                //��⵽�����������ڷ������ڲ����н�����ת
                if(key_continuous_value == 1)
                {
                    key_long_flag = 1; //���������������˵ı�־
                }
                //��⵽�����������ڷ������ڲ����н�����ת
                if(key_continuous_value == 2)
                {
                    key_long_flag = 2; //���������������˵ı�־
                }
            }



            }

            //printf("\r\n�ı���� %d  %d  %d \r\n",key_continuous_value,KEY_1,KEY_2);

            KEY_2 = 0;
            KEY_1 = 0;
            key_continuous_value = 0;

            updata_sleep_time();
        }

        dmp_get_pedometer_step_count(&STEPS); //�Ʋ�����ȡ������





//	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//	{
//		temp=MPU_Get_Temperature();	//�õ��¶�ֵ
//
//		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
//
////			printf("%d %d %d \r\n",aacx,aacy,aacz);
//
//		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
//
//		printf("���������� %.1f %.1f %.1f ",pitch,roll,yaw);
//	}
//
//
//		printf("����2 -->\r\n");
//		//HAL_Delay(50);
//		//osDelay(50);
//

    }
    //while(1)����
}









/*******************************************
* @function: 	���������������ȼ�Ϊ��͵�����
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.4.26
*******************************************/



void other_fun(void)
{
    //measure_Si7021();//ÿ���������

    //printf("�¶�:%.2f ʪ��:%.2f ",TEMP_buf,Humi_buf);


    USB_in_SIGN = HAL_GPIO_ReadPin(usb_in_sign_GPIO_Port,usb_in_sign_Pin);

    //printf("USB����%d ",USB_in_SIGN);
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
            //lv_disp_load_scr(scr_menu); //�л�����
            break;
        }
        }
    }
}


