#ifndef _APP_MEUN_H_
#define _APP_MEUN_H_
//#include "app_anim.h"
#include "lvgl.h"
#include "DisplayPrivate.h"

void app_menu_create(void);
void app_menu_anim_Vexit_setting_img( bool dir);
void app_menu_anim_Vexit_memo_img( bool dir);
void app_menu_anim_Vexit_about_img( bool dir);
void app_menu_anim_Vexit_light_img( bool dir);
void app_menu_anim_Vexit_stopwatch_img( bool dir);
void app_menu_tip_create(void);
void app_tip_anim_Vexit( bool dir);
void app_menu_anim_Hexit_setting_img( bool dir);
void app_menu_anim_Hexit_memo_img( bool dir);
void app_menu_anim_Hexit_about_img( bool dir);
void app_menu_anim_Hexit_stopwatch_img( bool dir);
void app_menu_anim_Hexit_light_img( bool dir);
#endif

