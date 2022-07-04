#ifndef __DISPLAYPRIVATE_H
#define __DISPLAYPRIVATE_H


#include "lvgl/lvgl.h"
#include "stdio.h"
#include "menu_page.h"
#include "time_setting_page.h"
#include "about_page.h"
#include "light_setting_page.h"
#include "stopwatch_page.h"
#include "watch_page.h"
#include "stopwatch_page.h"
#include  "gui_display.h"
#include  "memo_page.h"


typedef enum
{
    /*保留*/
    PAGE_NONE,
    /*用户页面*/
    PAGE_StopWatch,
    PAGE_MainMenu,
    PAGE_Setting,
    PAGE_Memo,
    PAGE_Sensor,
    PAGE_About,
   // PAGE_Game,
    /*保留*/
    PAGE_MAX
} Page_Type;

#define LV_ANIM_TIME_DEFAULT 200

void lv_obj_add_anim(
    lv_obj_t * obj, lv_anim_t * a,
    lv_anim_exec_xcb_t exec_cb, 
    int32_t start, int32_t end,
    uint16_t time ,
    lv_anim_ready_cb_t ready_cb ,
    lv_anim_path_cb_t path_cb 
);

void obj_add_anim(lv_obj_t *obj, lv_anim_exec_xcb_t exec_cb, uint16_t time, lv_coord_t start, lv_coord_t end, lv_anim_path_cb_t path_cb);

#define LV_OBJ_ADD_ANIM(obj,attr,target,time)\
do{\
    static lv_anim_t a;\
    lv_obj_add_anim(\
        (obj), &a,\
        (lv_anim_exec_xcb_t)lv_obj_set_##attr,\
        lv_obj_get_##attr(obj),\
        (target),\
        (time),\
				(NULL),\
				(NULL)\
    );\
}while(0)

/*AppWindow*/
void AppWindow_Create();
lv_obj_t * AppWindow_GetCont(uint8_t pageID);
lv_coord_t AppWindow_GetHeight();
lv_coord_t AppWindow_GetWidth();
#define APP_WIN_HEIGHT AppWindow_GetHeight()
#define APP_WIN_WIDTH  AppWindow_GetWidth()

//菜单界面初始化
//extern  int8_t ICON_NowSelIndex;
 void Setup_Menu();
void ICON_Grp_Move(int8_t dir); //菜单界面移动
uint8_t Get_Pageid();



#endif