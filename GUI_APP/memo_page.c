#include "DisplayPrivate.h"

extern uint8_t  buferRX[200];
extern uint8_t  bufferLen;
extern uint8_t			Rx2_Buf[1024];

lv_obj_t *scr_memo;  //memo的界面窗口

static lv_obj_t *cont; //页面容器
static lv_obj_t *label_title;  //显示标题
static lv_obj_t *label_line1;	   //显示mcu信息
static lv_obj_t *label_line2;	   //显示gui信息
static lv_obj_t *label_line3; //显示作者信息
static lv_obj_t *logo_img;	   //logo
static lv_obj_t *label_line4;	   //版本
static lv_obj_t *label_updatetime;	   //更新时间


static const char *memo_line1 = "1.Don't eat breakfast on an empty stomach.";
static const char *memo_line2 = "2.Submit paper on May 6th.";
static  char *memo_line3 = "3.Learn the principle of Linux kernel memory scheduling.";
static  char *memo_line4 = "4.Learn MIPI and GMSL.";

//创建关于界面
void app_memo_create(void)
{

static lv_style_t cont_style;//
	scr_memo = lv_scr_act();

	cont = lv_cont_create(scr_memo, NULL);															//创建容器
	lv_obj_set_size(cont, 240, 160);															//设置容器大小
	lv_obj_set_pos(cont, 240, 60);
	//lv_obj_align(cont, scr_about, LV_ALIGN_OUT_TOP_MID, 0, 0);										//设置容器在屏幕外部正上方
	 lv_style_copy(&cont_style,cont->style_p);
	//lv_obj_set_style_local_bg_opa(cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);		//设置背景透明
//	lv_obj_set_style_local_border_opa(cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP); //设置边框透明
	 cont_style.body.border.opa=LV_OPA_TRANSP;
	 cont_style.body.opa=LV_OPA_TRANSP;
   lv_cont_set_style(cont,LV_CONT_STYLE_MAIN,&cont_style);
	lv_cont_set_layout(cont, LV_LAYOUT_OFF); //设置容器的布局方式: 关闭自动布局。根据需要设置是否自动布局,否则程序卡死
	//label_title = lv_label_create(cont, NULL);																   //创建标题标签
	//lv_obj_set_style_local_text_font(label_title, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_32); //设置字体20号
	//lv_obj_set_style_local_text_color(label_title, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);		   //设置字体颜色
	//lv_label_set_static_text(label_title, memo_line1);
	//lv_obj_align(label_title, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);

	label_line1 = lv_label_create(cont, NULL);														   //创建MCU标签
	lv_label_set_long_mode(label_line1,LV_LABEL_LONG_BREAK);
	lv_obj_set_size(label_line1,220,0);//宽度为 100 像素,高度是无效的,随意设置吧
	//lv_obj_set_style_local_text_color(label_line1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW); //设置字体颜色
	lv_label_set_static_text(label_line1, memo_line1);
	lv_obj_align(label_line1, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 0);
 // lv_obj_set_pos(label_line1, 0, 80);
	label_line2 = lv_label_create(cont, NULL);														   //创建GUI标签
	lv_label_set_long_mode(label_line2,LV_LABEL_LONG_BREAK);
	lv_obj_set_size(label_line2,220,0);//宽度为 100 像素,高度是无效的,随意设置吧
	//lv_obj_set_style_local_text_color(label_line2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW); //设置字体颜色
	lv_label_set_static_text(label_line2, memo_line2);
	lv_obj_align(label_line2, label_line1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);

	label_line3 = lv_label_create(cont, NULL);															  //创建AUTHOR标签
	lv_label_set_long_mode(label_line3,LV_LABEL_LONG_BREAK);
	lv_obj_set_size(label_line3,220,0);//宽度为 100 像素,高度是无效的,随意设置吧
	lv_label_set_static_text(label_line3, memo_line3);
	lv_obj_align(label_line3, label_line2, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);

	label_line4 = lv_label_create(cont, NULL);															  //创建版本
	lv_label_set_long_mode(label_line4,LV_LABEL_LONG_BREAK);
	lv_obj_set_size(label_line4,220,0);//宽度为 100 像素,高度是无效的,随意设置吧
	lv_label_set_static_text(label_line4 , memo_line4);
	lv_obj_align(label_line4, label_line3, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
 
}

void memo_update(int line_index)
{

	switch(line_index)
	{
		case 1:
			 lv_label_set_array_text(label_line1, (const char *)buferRX,bufferLen);
			//lv_label_set_static_text(label_line1, (const char *)buferRX);
			break;
		case 2:  
			 lv_label_set_array_text(label_line2, &buferRX[2],bufferLen);
		//lv_label_set_static_text(label_line1, (const char *)Rx2_Buf);
			break;
		case 3:
			 lv_label_set_array_text(label_line3, &buferRX[2],bufferLen);
			break;
		case 4:
			 lv_label_set_array_text(label_line4, &buferRX[2],bufferLen);
			break;
	}
    
}

//memo垂直方向移动进入或退出中心
void app_memo_anim_Vexit(bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_y(cont);		//获取起始值
	
	obj_add_anim(
								cont, 		//动画对象
								(lv_anim_exec_xcb_t)lv_obj_set_y,				//动画函数
								lv_anim_speed_to_time(100, 0, 42),			//动画速度
								start,																	//起始值
								start + (dir ? -260 : 260),							//结束值
								lv_anim_path_bounce											//动画特效:模拟弹性物体下落
								);
}


//memo水平方向移动进入或退出中心
void app_memo_anim_Hexit(bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_x(cont);		//获取起始值
	
	obj_add_anim(
								cont, 		//动画对象
								(lv_anim_exec_xcb_t)lv_obj_set_x,				//动画函数
								lv_anim_speed_to_time(100, 0, 42),			//动画速度
								start,																	//起始值
								start + (dir ? -240 : 240),							//结束值
								lv_anim_path_bounce											//动画特效:模拟弹性物体下落
								);
}