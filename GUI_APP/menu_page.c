//#include "common.h"
#include "DisplayPrivate.h"

//显示图片声明 每张5k



LV_IMG_DECLARE(setting);
LV_IMG_DECLARE(about);
LV_IMG_DECLARE(memo); 
LV_IMG_DECLARE(stopwatch);
LV_IMG_DECLARE(light);


static lv_obj_t *setting_img;	//定义主背景图片img
static lv_obj_t *about_img;	//定义主背景图片img
static lv_obj_t *memo_img;	//定义主背景图片img
static lv_obj_t *stopwatch_img;	//定义主背景图片img
static lv_obj_t *light_img;	//定义主背景图片img


lv_obj_t *tip_cont; //提示容器
lv_obj_t *label_tip;//提示标签
lv_obj_t* tip_line;  //提示线条

const lv_point_t tip_line_points[] = { {10, 15}, {230, 15} };
#define TIP_LINE_POINTS_NUM (sizeof(tip_line_points)/sizeof(tip_line_points[0]))


//创建菜单界面 图片都在屏幕底部
void app_menu_create(void)
{
    lv_obj_t *scr = lv_scr_act(); //获取当前活跃的屏幕

    setting_img = lv_img_create(scr, NULL);						  //创建img显示菜单
    lv_img_set_src(setting_img, &setting);			  //设置显示图片
    lv_obj_align(setting_img, scr, LV_ALIGN_OUT_BOTTOM_MID, 0, 0); //设置图片中央对齐
    
    about_img = lv_img_create(scr, NULL);						  //创建img显示菜单
    lv_img_set_src(about_img, &about);			  //设置显示图片
    lv_obj_align(about_img, scr, LV_ALIGN_OUT_BOTTOM_MID, 0, 0); //设置图片中央对齐    
    
    memo_img = lv_img_create(scr, NULL);						  //创建img显示菜单
    lv_img_set_src(memo_img, &memo);			  //设置显示图片
    lv_obj_align(memo_img, scr, LV_ALIGN_OUT_BOTTOM_MID, 0, 0); //设置图片中央对齐

    stopwatch_img = lv_img_create(scr, NULL);						  //创建img显示菜单
    lv_img_set_src(stopwatch_img, &stopwatch);			  //设置显示图片
    lv_obj_align(stopwatch_img, scr, LV_ALIGN_OUT_BOTTOM_MID, 0, 0); //设置图片中央对齐    
	
	  light_img = lv_img_create(scr, NULL);						  //创建img显示菜单
    lv_img_set_src(light_img, &light);			  //设置显示图片
    lv_obj_align(light_img, scr, LV_ALIGN_OUT_BOTTOM_MID, 0, 0); //设置图片中央对齐    
}



//设置图标垂直方向移动进入或退出中心
void app_menu_anim_Vexit_setting_img( bool dir)
{
	uint16_t start;
	start = lv_obj_get_y(setting_img);		//获取起始值
	obj_add_anim(
								setting_img, 		//动画对象
								(lv_anim_exec_xcb_t)lv_obj_set_y,				//动画函数
								lv_anim_speed_to_time(100, 0, 42),			//动画速度
								start,																	//起始值
								start + (dir ? -160 : 160),							//结束值
								lv_anim_path_bounce											//动画特效:模拟弹性物体下落
								);
    if(dir)        lv_label_set_static_text(label_tip,"Setting");
    
}
//备忘录图标垂直方向移动进入或退出中心
void app_menu_anim_Vexit_memo_img( bool dir)
{
	uint16_t start;
	start = lv_obj_get_y(memo_img);		//获取起始值
	obj_add_anim(
								memo_img, 		//动画对象
								(lv_anim_exec_xcb_t)lv_obj_set_y,				//动画函数
								lv_anim_speed_to_time(100, 0, 42),			//动画速度
								start,																	//起始值
								start + (dir ? -160 : 160),							//结束值
								lv_anim_path_bounce											//动画特效:模拟弹性物体下落
								);
        if(dir)        lv_label_set_static_text(label_tip,"Memo");
}

//关于图标垂直方向移动进入或退出中心
void app_menu_anim_Vexit_about_img( bool dir)
{
	uint16_t start;
	start = lv_obj_get_y(about_img);		//获取起始值
	obj_add_anim(
								about_img, 		//动画对象
								(lv_anim_exec_xcb_t)lv_obj_set_y,				//动画函数
								lv_anim_speed_to_time(100, 0, 42),			//动画速度
								start,																	//起始值
								start + (dir ? -160 : 160),							//结束值
								lv_anim_path_bounce											//动画特效:模拟弹性物体下落
								);
        if(dir)        lv_label_set_static_text(label_tip,"About");
}

//秒表图标垂直方向移动进入或退出中心
void app_menu_anim_Vexit_stopwatch_img( bool dir)
{
	uint16_t start;
	start = lv_obj_get_y(stopwatch_img);		//获取起始值
	obj_add_anim(
								stopwatch_img, 		//动画对象
								(lv_anim_exec_xcb_t)lv_obj_set_y,				//动画函数
								lv_anim_speed_to_time(100, 0, 42),			//动画速度
								start,																	//起始值
								start + (dir ? -160 : 160),							//结束值
								lv_anim_path_bounce											//动画特效:模拟弹性物体下落
								);
        if(dir)        lv_label_set_static_text(label_tip,"Stopwatch");
}

//传感器图标垂直方向移动进入或退出中心
void app_menu_anim_Vexit_light_img( bool dir)
{
	uint16_t start;
	start = lv_obj_get_y(light_img);		//获取起始值
	obj_add_anim(
								light_img, 		//动画对象
								(lv_anim_exec_xcb_t)lv_obj_set_y,				//动画函数
								lv_anim_speed_to_time(100, 0, 42),			//动画速度
								start,																	//起始值
								start + (dir ? -160 : 160),							//结束值
								lv_anim_path_bounce											//动画特效:模拟弹性物体下落
								);
        if(dir)        lv_label_set_static_text(label_tip,"Light");
}

/****************************************************************************************************************/
//setting图标水平方向移动进入或退出中心
void app_menu_anim_Hexit_setting_img( bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_x(setting_img);		//获取起始值
	
	obj_add_anim(
								setting_img, 		//动画对象
								(lv_anim_exec_xcb_t)lv_obj_set_x,				//动画函数
								lv_anim_speed_to_time(100, 0, 42),			//动画速度
								start,																	//起始值
								start + (dir ? -130 : 130),							//结束值
								lv_anim_path_bounce											//动画特效:模拟弹性物体下落
								);
}

//memo图标水平方向移动进入或退出中心
void app_menu_anim_Hexit_memo_img( bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_x(memo_img);		//获取起始值
	
	obj_add_anim(
								memo_img, 		//动画对象
								(lv_anim_exec_xcb_t)lv_obj_set_x,				//动画函数
								lv_anim_speed_to_time(100, 0, 42),			//动画速度
								start,																	//起始值
								start + (dir ? -145 : 145),							//结束值
								lv_anim_path_bounce											//动画特效:模拟弹性物体下落
								);
}


//关于图标水平方向移动进入或退出中心
void app_menu_anim_Hexit_about_img( bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_x(about_img);		//获取起始值
	
	obj_add_anim(
								about_img, 		//动画对象
								(lv_anim_exec_xcb_t)lv_obj_set_x,				//动画函数
								lv_anim_speed_to_time(100, 0, 42),			//动画速度
								start,																	//起始值
								start + (dir ? -240 : 240),							//结束值
								lv_anim_path_bounce											//动画特效:模拟弹性物体下落
								);
}


//stopwatch图标水平方向移动进入或退出中心
void app_menu_anim_Hexit_stopwatch_img( bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_x(stopwatch_img);		//获取起始值
	
	obj_add_anim(
								stopwatch_img, 		//动画对象
								(lv_anim_exec_xcb_t)lv_obj_set_x,				//动画函数
								lv_anim_speed_to_time(100, 0, 42),			//动画速度
								start,																	//起始值
								start + (dir ? -240 : 240),							//结束值
								lv_anim_path_bounce											//动画特效:模拟弹性物体下落
								);
}

//light图标水平方向移动进入或退出中心 
void app_menu_anim_Hexit_light_img( bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_x(light_img);		//获取起始值
	
	obj_add_anim(
								light_img, 		//动画对象
								(lv_anim_exec_xcb_t)lv_obj_set_x,				//动画函数
								lv_anim_speed_to_time(100, 0, 42),			//动画速度
								start,																	//起始值
								start + (dir ? -240 : 240),							//结束值
								lv_anim_path_bounce											//动画特效:模拟弹性物体下落
								);
}


/******************************************************************************************************/
  
//创建菜单提示界面
void app_menu_tip_create(void)
{
    lv_obj_t *scr = lv_scr_act(); //获取当前活跃的屏幕	
	  static lv_style_t style_line;                     //创建样式
	  static lv_style_t style_cont;                     //创建样式
    
    /***********************创建提示控件****************************/
    tip_cont = lv_cont_create(scr, NULL);															  //创建提示容器
    lv_obj_set_pos(tip_cont, 0, -60);																  //设置容器位置
    lv_obj_set_size(tip_cont, 240, 60);															  //设置容器尺寸
	  style_cont=*(lv_cont_get_style(tip_cont,LV_CONT_STYLE_MAIN));
	  style_cont.body.border.opa=LV_OPA_TRANSP;  //透明
	  lv_cont_set_style(tip_cont, LV_CONT_STYLE_MAIN, &style_cont);        //将样式添加到容器
   // lv_obj_set_style_local_bg_opa(tip_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);	  //设置背景透明
   // lv_obj_set_style_local_border_opa(tip_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP); //设置边框透明
    //lv_cont_set_layout(tip_cont, LV_LAYOUT_OFF);												  //设置容器的布局方式: 开启自动布局
   // lv_obj_set_style_local_pad_inner(tip_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);			  //设置子对象之间的间距
    /***********************创建提示控件****************************/
    
 	/***********************创建提示线条****************************/ 
    tip_line = lv_line_create(tip_cont, NULL);//创建线条对象
    lv_obj_set_pos(tip_line,0,30);
    lv_line_set_points(tip_line, tip_line_points, TIP_LINE_POINTS_NUM);
	 
	 
	  style_line=  *(lv_line_get_style(tip_line,LV_LINE_STYLE_MAIN));
    style_line.line.width=4  ;        //设置直线样式宽度
    style_line.line.rounded=1;   //设置直线样式圆形的(末端) 
    //style_line.line.color=LV_COLOR_RED;
	  
  
    lv_line_set_style(tip_line, LV_LINE_STYLE_MAIN, &style_line);        //将样式添加到直线
   
   
	 

   // lv_obj_set_style_local_line_color(tip_line, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);		  //设置颜色
  //  lv_obj_set_style_local_line_width(tip_line, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 4);		  //设置线宽
  //  lv_obj_set_style_local_line_rounded(tip_line, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 1);		  //设置圆角 
 	/***********************创建提示线条****************************/     
    
    
    label_tip = lv_label_create(tip_cont, NULL); 
    lv_obj_align(label_tip, tip_line, LV_ALIGN_OUT_TOP_MID, 0, -1);
   // lv_obj_set_style_local_text_font(label_tip, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_16); //设置字体
   // lv_obj_set_style_local_text_color(label_tip, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);		  //设置颜色

}


void app_tip_anim_Vexit( bool dir)
{
	uint16_t start;
	start = lv_obj_get_y(tip_cont);		//获取起始值
	obj_add_anim(
								tip_cont, 		//动画对象
								(lv_anim_exec_xcb_t)lv_obj_set_y,				//动画函数
								lv_anim_speed_to_time(100, 0, 42),			//动画速度
								start,																	//起始值
								start + (dir ? 60 : -60),							//结束值
								lv_anim_path_bounce											//动画特效:模拟弹性物体下落
								);
}



