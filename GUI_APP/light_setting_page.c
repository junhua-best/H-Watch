
#include "DisplayPrivate.h"
#include "tim.h"

static lv_obj_t *scr;
static lv_obj_t *set_lighting_cont; //页面容器
static lv_obj_t * arcBright;/*亮度指示弧*/
static lv_obj_t *label_lighting;//提示标签


//创建时钟界面界面
void app_lighting_create(void)
{

    static lv_style_t cont_style;//
    static lv_style_t arc_style;//
    static lv_style_t label_style;
    scr = lv_scr_act(); //获取当前活跃的屏幕

    /***********************创建时钟设置控件****************************/
    set_lighting_cont = lv_cont_create(scr, NULL);															  //创建提示容器
    lv_obj_set_pos(set_lighting_cont, 240, 60);																  //设置容器位置
    lv_obj_set_size(set_lighting_cont, 240, 140);															  //设置容器尺寸
    lv_style_copy(&cont_style,set_lighting_cont->style_p);
    cont_style.body.border.opa=LV_OPA_TRANSP;
    cont_style.body.opa=LV_OPA_TRANSP;
    lv_cont_set_style(set_lighting_cont,LV_CONT_STYLE_MAIN,&cont_style);
    //lv_obj_set_style_local_bg_opa(set_lighting_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);	  //设置背景透明
    //lv_obj_set_style_local_border_opa(set_lighting_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP); //设置边框透明
    // lv_cont_set_layout(set_lighting_cont, LV_LAYOUT_OFF);												  //设置容器的布局方式: 开启自动布局
    //  lv_obj_set_style_local_pad_inner(set_lighting_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);			  //设置子对象之间的间距
    /***********************创建时钟设置控件****************************/
    arcBright = lv_arc_create(set_lighting_cont, NULL);
    lv_style_copy(&arc_style,arcBright->style_p);
    arc_style.line.width =8;
    arc_style.line.opa=LV_OPA_TRANSP;
    lv_arc_set_style(arcBright,LV_ARC_STYLE_MAIN,&arc_style);
    //lv_obj_set_style_local_line_width(arcBright, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 8);		  //设置线宽
    //lv_obj_set_style_local_line_rounded(arcBright, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 1);		  //设置圆角
    //lv_obj_set_style_local_bg_opa(arcBright, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);	  //设置背景透明
    //lv_obj_set_style_local_border_opa(arcBright, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP); //设置边框透明
    //lv_arc_set_bg_angles(arcBright, 0, 0);
    lv_arc_set_angles(arcBright, 0, 0);
    lv_obj_set_size(arcBright, 100, 100); ;
    lv_obj_align(arcBright, set_lighting_cont, LV_ALIGN_CENTER, 0, 0);



    label_lighting = lv_label_create(set_lighting_cont, NULL);
    lv_obj_align(label_lighting, arcBright, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_style_local_text_font(label_lighting, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_16); //设置字体
    //lv_obj_set_style_local_text_color(label_lighting, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);		  //设置颜色
}

char bgcode[5];

void set_bglighting(uint16_t bglight)
{
    // TIM_SetCompare3(TIM1,360-bglight);	//修改比较值，修改占空比
    //lv_arc_set_end_angle(arcBright,bglight);
    lv_arc_set_angles(arcBright, 0, bglight);
    sprintf((char*)bgcode,"%3d",(int)(bglight/3.6));
    lv_label_set_static_text(label_lighting,bgcode);
    /*调节屏幕背光*/
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //开启定时器PWM通道 定时器2 通道3
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500-bglight); //设置PWM的占空比，调节背光
}







//light设置图标水平方向移动进入或退出中心
void app_lighting_anim_Hexit_img( bool dir)
{
    uint16_t start;

    start = lv_obj_get_x(set_lighting_cont);		//获取起始值

    obj_add_anim(
        set_lighting_cont, 		//动画对象
        (lv_anim_exec_xcb_t)lv_obj_set_x,				//动画函数
        lv_anim_speed_to_time(100, 0, 42),			//动画速度
        start,																	//起始值
        start + (dir ? -240 : 240),							//结束值
        lv_anim_path_bounce											//动画特效:模拟弹性物体下落
    );
}
