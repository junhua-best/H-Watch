#ifndef __TIME_SETTING_PAGE_H
#define __TIME_SETTING_PAGE_H


//ʱ�����ý���
void app_setting_create(void);
//���õ�ǰѡ��roller����
void app_setting_change(uint8_t index);
	
//���õ�ǰѡ�е�roller
void app_setting_change_roller(uint8_t i);

//��ȡ��ǰѡ�е�roller��������
uint8_t app_setting_get_roller_index(void);

//��ȡÿ��roller�������ַ
const uint8_t* app_setting_get_roller_max(void);
void app_setting_anim_Vexit(bool dir);
//����rtcʱ��
void app_setting_update_time();
//���ð�ť��ɫ
void set_enter (bool enter);

#endif