#ifndef taskFun_h
#define taskFun_h
#include "rtc.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "inv_mpu.h"
#include "mpu6050.h"

#define DO   0x01
#define DONE 0x00

#define Interface_main    0 //������
#define Interface_notify  1 //����һ��֪ͨ����

#define Interface_informations  2 //����һ��֪ͨ����
#define Interface_compass  3 //����һ��֪ͨ����
#define Interface_tomato 4 //��ʾ������
#define Interface_Pedometer 5 //�Ʋ�������״̬��־ Pedometer interface
#define Interface_Set 6 //���ù���״̬����
#define Interface_Memo 7 //���ñ���¼״̬����

#define Interface_Menu   8//�˵�
#define Interface_Setting   9//����
#define Interface_About   10//�˵�

#define DO   0x01
#define DONE 0x00
#define KEY_NO   1
#define KEY_PASS 0

extern int time_change_flag,date_change_flag,week_change_flag,second_change_flag,bat_change_flag;


extern uint8_t words_len1 ; //��������������Ϣ�ĳ���
extern uint8_t words_len2 ; //���������������Ϣ�ĳ���
extern uint8_t words_len3 ; //֪ͨ��Ϣ�ĳ���
extern uint8_t words_len4 ; //֪ͨ����¼�ĺ��ֵĳ���

extern uint8_t words1[6] ; //������������
extern uint8_t words2[6] ; //�������������
extern unsigned char words3[40] ; //��ӱ���֪ͨ��Ϣ�����ݵ�buffer
extern unsigned char words4[40] ; //��ӱ��汸��¼��Ϣ�����ݵ�buffer

extern uint8_t day_1_H ;//���������������
extern uint8_t day_1_L ;//���������������
extern uint8_t day_2_H ;//����������������
extern uint8_t day_2_L ;//����������������

extern uint8_t words_month ;//���汸��¼����
extern uint8_t words_day ;//���汸��¼����

extern unsigned long STEPS; //��¼����


extern float Curent_Angle; 
extern unsigned char Interface;
/*******************************************
* @function: 	ʹ��STM32����ADC��ȡ��ص�ѹֵ������ȡ���ĵ�ѹֵ������ȫ�ֱ����У���ADC1ͨ��1 ����Ӧ����PA1
* @input:			��
* @return:		�����Ϊȫ�ֱ���ֱ��ʹ��
* @author:		��С��
* @time:			2020.10.2
*******************************************/
void init_task(void);
	





/*******************************************
* @function: 	ʹ��STM32����ADC��ȡ��ص�ѹֵ������ȡ���ĵ�ѹֵ������ȫ�ֱ����У���ADC1ͨ��1 ����Ӧ����PA1
* @input:			��
* @return:		�����Ϊȫ�ֱ���ֱ��ʹ��
* @author:		��С��
* @time:			2020.10.2
*******************************************/
extern RTC_DateTypeDef sDate; //ȫ�ֱ������ⲿ����ֱ�ӵ���ʹ�� �������������

extern RTC_TimeTypeDef sTime; //ȫ�ֱ������ⲿ����ֱ�ӵ���ʹ�� �����ʱ������

void get_time(void);





/*******************************************
* @function: 	���������������ȼ�Ϊ��ߵ�����
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.4.26
*******************************************/

void task_base(void);







/*******************************************
* @function: 	���������������ȼ�Ϊ�еȵ�����
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.4.26
*******************************************/

void background_base(void);
	




/*******************************************
* @function: 	���������������ȼ�Ϊ��͵�����
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.4.26
*******************************************/

void other_fun(void);
	






/*******************************************
* @function: 	�͹���ģʽ���� ʹ��PA0 wake up ���ų����ڲ�Ӳ�����Ѹ�λ��Ƭ�� �͹���ģʽ�¹ر�����IOʱ�ӣ�����IOΪ���� �������ģʽ
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2020.4.26
*******************************************/
void low_power_model(void);





void SystemClock_Config_20Mhz(void);
	

void SystemClock_Config_10Mhz(void);


extern unsigned char sleep_time_buffer[3];
extern unsigned char light_time;
extern int light_hight;
/*******************************************
* @function: 	���������ù���ģʽ���洢�溯��
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.5.28
*******************************************/
void Interface_Set_write(void);
	
/*******************************************
* @function: 	�����ڴμ���������ù���ģʽ���洢�溯��
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.5.28
*******************************************/
void Interface_Set_read(void);


/*******************************************
* @function: 	�����ڴμ���������ù���ģʽ������Ļ������ʾ����
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.5.28
*******************************************/
void set_screen_light(unsigned char lit);



/*******************************************
* @function: 	��ȡ������֪ͨ����¼����Ϣ
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.5.28
*******************************************/
void notify_Set_read(void);

/*******************************************
* @function: 	ͨ����ʱ�����PWM���������𶯵Ĵ�С
* @input:			��
* @return:		��
* @author:		��С��
* @time:			2021.5.21
*******************************************/
void Transient_vibration(void);

/*******************************************
* @function: 	������������
* @input:			��
* @return:		��
* @author:		Jovan
* @time:			2021.12.21
*******************************************/
void background_base_new(void);

/*******************************************
* @function: 	��ʾ����
* @input:			��
* @return:		��
* @author:		Jovan
* @time:			2021.12.21
*******************************************/
void task_base_new(void);

void display_test_task(); //��������

//��ȡ����
int get_vbat(void);

//����˯��
void Checking_into_sleep(void);
#endif

