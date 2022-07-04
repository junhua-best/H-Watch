#ifndef taskFun_h
#define taskFun_h
#include "rtc.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "inv_mpu.h"
#include "mpu6050.h"

#define DO   0x01
#define DONE 0x00

#define Interface_main    0 //主界面
#define Interface_notify  1 //左侧第一个通知界面

#define Interface_informations  2 //左侧第一个通知界面
#define Interface_compass  3 //左侧第一个通知界面
#define Interface_tomato 4 //显示番茄钟
#define Interface_Pedometer 5 //计步器界面状态标志 Pedometer interface
#define Interface_Set 6 //设置工作状态界面
#define Interface_Memo 7 //设置备忘录状态界面

#define Interface_Menu   8//菜单
#define Interface_Setting   9//设置
#define Interface_About   10//菜单

#define DO   0x01
#define DONE 0x00
#define KEY_NO   1
#define KEY_PASS 0

extern int time_change_flag,date_change_flag,week_change_flag,second_change_flag,bat_change_flag;


extern uint8_t words_len1 ; //保存今天的天气信息的长度
extern uint8_t words_len2 ; //保存明天的天气信息的长度
extern uint8_t words_len3 ; //通知信息的长度
extern uint8_t words_len4 ; //通知备忘录的汉字的长度

extern uint8_t words1[6] ; //保存今天的天气
extern uint8_t words2[6] ; //保存明天的天气
extern unsigned char words3[40] ; //间接保存通知信息的内容的buffer
extern unsigned char words4[40] ; //间接保存备忘录信息的内容的buffer

extern uint8_t day_1_H ;//保存今天的最高气温
extern uint8_t day_1_L ;//保存今天的最低气温
extern uint8_t day_2_H ;//保存明天的最高气温
extern uint8_t day_2_L ;//保存明天的最低气温

extern uint8_t words_month ;//保存备忘录日期
extern uint8_t words_day ;//保存备忘录日期

extern unsigned long STEPS; //记录步数


extern float Curent_Angle; 
extern unsigned char Interface;
/*******************************************
* @function: 	使用STM32板载ADC获取电池电压值，将获取到的电压值储存在全局变量中，打开ADC1通道1 ，对应引脚PA1
* @input:			无
* @return:		输出成为全局变量直接使用
* @author:		田小花
* @time:			2020.10.2
*******************************************/
void init_task(void);
	





/*******************************************
* @function: 	使用STM32板载ADC获取电池电压值，将获取到的电压值储存在全局变量中，打开ADC1通道1 ，对应引脚PA1
* @input:			无
* @return:		输出成为全局变量直接使用
* @author:		田小花
* @time:			2020.10.2
*******************************************/
extern RTC_DateTypeDef sDate; //全局变量，外部可以直接调用使用 ，存放日期数据

extern RTC_TimeTypeDef sTime; //全局变量，外部可以直接调用使用 ，存放时间数据

void get_time(void);





/*******************************************
* @function: 	基础任务，任务优先级为最高的任务
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.4.26
*******************************************/

void task_base(void);







/*******************************************
* @function: 	基础任务，任务优先级为中等的任务
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.4.26
*******************************************/

void background_base(void);
	




/*******************************************
* @function: 	基础任务，任务优先级为最低的任务
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.4.26
*******************************************/

void other_fun(void);
	






/*******************************************
* @function: 	低功耗模式函数 使用PA0 wake up 引脚出发内部硬件唤醒复位单片机 低功耗模式下关闭所有IO时钟，设置IO为输入 进入待机模式
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2020.4.26
*******************************************/
void low_power_model(void);





void SystemClock_Config_20Mhz(void);
	

void SystemClock_Config_10Mhz(void);


extern unsigned char sleep_time_buffer[3];
extern unsigned char light_time;
extern int light_hight;
/*******************************************
* @function: 	隶属于设置工作模式界面储存函数
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/
void Interface_Set_write(void);
	
/*******************************************
* @function: 	隶属于次级界面的设置工作模式界面储存函数
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/
void Interface_Set_read(void);


/*******************************************
* @function: 	隶属于次级界面的设置工作模式界面屏幕亮度显示函数
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/
void set_screen_light(unsigned char lit);



/*******************************************
* @function: 	读取天气和通知备忘录等信息
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.28
*******************************************/
void notify_Set_read(void);

/*******************************************
* @function: 	通过定时器输出PWM方波控制震动的大小
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.5.21
*******************************************/
void Transient_vibration(void);

/*******************************************
* @function: 	按键处理任务
* @input:			无
* @return:		无
* @author:		Jovan
* @time:			2021.12.21
*******************************************/
void background_base_new(void);

/*******************************************
* @function: 	显示任务
* @input:			无
* @return:		无
* @author:		Jovan
* @time:			2021.12.21
*******************************************/
void task_base_new(void);

void display_test_task(); //测试任务

//获取电量
int get_vbat(void);

//进入睡眠
void Checking_into_sleep(void);
#endif

