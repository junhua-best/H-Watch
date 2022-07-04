#include "DisplayPrivate.h"

lv_obj_t *scr_about;  //about的界面窗口

static lv_obj_t *cont; //页面容器
static lv_obj_t *label_title;  //显示标题
static lv_obj_t *label_mcu;	   //显示mcu信息
static lv_obj_t *label_gui;	   //显示gui信息
static lv_obj_t *label_author; //显示作者信息
static lv_obj_t *logo_img;	   //logo
static lv_obj_t *label_version;	   //版本
static lv_obj_t *label_updatetime;	   //更新时间

static const  char *title_text = "About";
static const char *mcu_text = "MCU: STM32L431CCU6";
static const char *gui_text = "GUI: littleVGL V6";
static  char *author_text = "AUTHOR: Jovan.situ "; 
static  char *version_text = "HWatch official-V2.5";
static  char *time_text = "Update_Time:2022.5.8";

//创建关于界面
void app_about_create(void)
{

	static lv_style_t cont_style;//
	scr_about = lv_scr_act();
	

	cont = lv_cont_create(scr_about, NULL);															//创建容器
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
	//lv_label_set_static_text(label_title, title_text);
	//lv_obj_align(label_title, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);
  
	label_mcu = lv_label_create(cont, NULL);														   //创建MCU标签
		
	//lv_obj_set_style_local_text_color(label_mcu, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW); //设置字体颜色
	lv_label_set_static_text(label_mcu, mcu_text);
	lv_obj_align(label_mcu, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
 // lv_obj_set_pos(label_mcu, 0, 80);
	label_gui = lv_label_create(cont, NULL);														   //创建GUI标签
	//lv_obj_set_style_local_text_color(label_gui, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW); //设置字体颜色
	lv_label_set_static_text(label_gui, gui_text);
	lv_obj_align(label_gui, label_mcu, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

	label_author = lv_label_create(cont, NULL);															  //创建AUTHOR标签
//	lv_obj_set_style_local_text_color(label_author, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW); //设置字体颜色
	lv_label_set_static_text(label_author, author_text);
	lv_obj_align(label_author, label_gui, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
	
	label_version = lv_label_create(cont, NULL);															  //创建版本
//	lv_obj_set_style_local_text_color(label_author, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW); //设置字体颜色
	lv_label_set_static_text(label_version , version_text);
	lv_obj_align(label_version, label_author, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
	
	label_updatetime = lv_label_create(cont, NULL);															  //创建时间标签
//	lv_obj_set_style_local_text_color(label_author, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW); //设置字体颜色
	lv_label_set_static_text(label_updatetime, time_text);
	lv_obj_align(label_updatetime, label_version, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

	//logo_img = lv_img_create(cont, NULL); //创建logo
//	lv_img_set_src(logo_img, &start);	  //logo显示图片
	//lv_obj_align(logo_img, cont, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
	//lv_obj_set_style_local_image_opa(logo_img, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
	//lv_img_set_auto_size(logo_img, false); //不使能大小自动适配
}

//about垂直方向移动进入或退出中心
void app_about_anim_Vexit(bool dir)
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


//about水平方向移动进入或退出中心
void app_about_anim_Hexit(bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_x(cont);		//获取起始值
	
	obj_add_anim(
								cont, 		//动画对象
								(lv_anim_exec_xcb_t)lv_obj_set_x,				//动画函数
								lv_anim_speed_to_time(100, 0, 42),			//动画速度
								start,																	//起始值
								start + (dir ? -245 : 245),							//结束值
								lv_anim_path_bounce											//动画特效:模拟弹性物体下落
								);
}