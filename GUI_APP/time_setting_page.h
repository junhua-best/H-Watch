#ifndef __TIME_SETTING_PAGE_H
#define __TIME_SETTING_PAGE_H


//时间设置界面
void app_setting_create(void);
//设置当前选中roller的项
void app_setting_change(uint8_t index);
	
//设置当前选中的roller
void app_setting_change_roller(uint8_t i);

//获取当前选中的roller的项索引
uint8_t app_setting_get_roller_index(void);

//获取每个roller最大项保存地址
const uint8_t* app_setting_get_roller_max(void);
void app_setting_anim_Vexit(bool dir);
//更新rtc时间
void app_setting_update_time();
//设置按钮颜色
void set_enter (bool enter);

#endif