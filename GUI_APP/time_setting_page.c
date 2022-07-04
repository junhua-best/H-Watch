#include "DisplayPrivate.h"
#include "taskfun.h"
//#include "time_setting_page.h"


static lv_obj_t *cont; //页面容器

static lv_obj_t *label_title; //显示标题
static const char *title_text = "Setting";

static lv_obj_t *time_date_roller[7]; //时间日期设置roller
static const uint8_t roller_max[7] = {24, 60, 60, 30, 12, 31, 7};
static const char roller_string[] = 
{
	"00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n\
24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n\
48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59\n"
};

static const char week_list[] = {"Sun.\nMon.\nTue.\nWen.\nThu.\nFri.\nSat."};				 //周列表

static uint8_t foucs_roller_index = 0;		//当前选中的roller

static lv_obj_t *enter_btn;		//确认按键
static lv_obj_t *enter_btn_label;		//按键label

static lv_obj_t *time_point_label[2];		//时间的: label
static lv_obj_t *date_Hline_label[3];		//日期的20 和 - label

static lv_style_t style_roller; //几个选项框的style
static lv_style_t style_roller_select; //选中的选项框的style
static lv_style_t btn_style;//
static lv_style_t btn_label_style;//
 

//根据当前时间初始化roller
void app_setting_update_roller()
{
	lv_roller_set_selected(time_date_roller[0], sTime.Hours, LV_ANIM_OFF);		//时
	lv_roller_set_selected(time_date_roller[1], sTime.Minutes, LV_ANIM_OFF);			//分
	lv_roller_set_selected(time_date_roller[2], sTime.Seconds, LV_ANIM_OFF);			//秒
	
	lv_roller_set_selected(time_date_roller[3], sDate.Year, LV_ANIM_OFF);		//年
	lv_roller_set_selected(time_date_roller[4], sDate.Month - 1, LV_ANIM_OFF);			//月, 从1开始，因此 - 1
	lv_roller_set_selected(time_date_roller[5], sDate.Date - 1, LV_ANIM_OFF);			//日, 从1开始，因此 - 1
	
	lv_roller_set_selected(time_date_roller[6], sDate.WeekDay, LV_ANIM_OFF);			//周
}

//根据roller填充时间类型结构体
void app_setting_update_time()
{
	sTime.Hours = lv_roller_get_selected(time_date_roller[0]);		//获取时
	sTime.Minutes = lv_roller_get_selected(time_date_roller[1]);		//获取分
	sTime.Seconds = lv_roller_get_selected(time_date_roller[2]);		//获取秒
	
	sDate.Year = lv_roller_get_selected(time_date_roller[3]) + 2000;		//获取年
	sDate.Month = lv_roller_get_selected(time_date_roller[4]) + 1;		//获取月
	sDate.Date = lv_roller_get_selected(time_date_roller[5]) + 1;		//获取日
	
	sDate.WeekDay = lv_roller_get_selected(time_date_roller[6]);		//获取周
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
		Transient_vibration(); //震动提示
}

//创建设置界面
void app_setting_create(void)
{
	
	uint8_t i;
	char roller_buf[181];
	
	lv_obj_t *scr_time_setting;  //设置时间的界面窗口
	scr_time_setting = lv_scr_act(); //获取当前活跃的屏幕
  
	/*********************************************创建设置界面容器******************************************************/
	
	cont = lv_cont_create(scr_time_setting, NULL);															//创建容器
	lv_obj_set_size(cont, 240, 240);															//设置容器大小
	lv_obj_align(cont, scr_time_setting, LV_ALIGN_OUT_TOP_MID, 0, 0);										//设置容器在屏幕外部正上方
	//lv_obj_set_style_local_bg_opa(cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);		//设置背景透明
	//lv_obj_set_style_local_border_opa(cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP); //设置边框透明

	lv_cont_set_layout(cont, LV_LAYOUT_OFF); //设置容器的布局方式: 关闭自动布局。根据需要设置是否自动布局,否则程序卡死

	label_title = lv_label_create(cont, NULL);																   //创建标题标签
		
		// lv_label_set_style(label_title, LV_LABEL_STYLE_MAIN, &style_label);
	//lv_obj_set_style_local_text_font(label_title, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_32); //设置字体32号
	//lv_obj_set_style_local_text_color(label_title, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);		   //设置字体颜色
	lv_label_set_static_text(label_title, title_text);														   //设置标题
	lv_obj_align(label_title, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);											   //设置标题顶端內部对齐
	
	/*********************************************创建设置界面容器******************************************************/
	/*********************************************创建时间日期设置roller******************************************************/
	for(i = 0; i < 7; i++)
	{
		time_date_roller[i] = lv_roller_create(cont, NULL);																//创建时间设置roller
	//	lv_obj_set_style_local_text_font(time_date_roller[i], LV_ROLLER_PART_BG, LV_STATE_DEFAULT, &lv_font_montserrat_20); //设置20号字体
	//	lv_obj_set_style_local_text_color(time_date_roller[i], LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);		   //设置字体颜色
	//	lv_obj_set_style_local_border_color(time_date_roller[i], LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);			//设置控件边框白色
	//	lv_obj_set_style_local_bg_opa(time_date_roller[i], LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_OPA_TRANSP);			//设置控件背景透明
		 
		// lv_label_set_style(time_date_roller[i], LV_LABEL_STYLE_MAIN, &style_label);
		
	//	lv_obj_set_style_local_text_color(time_date_roller[i], LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);		   //设置选中项字体颜色
	//	lv_obj_set_style_local_border_opa(time_date_roller[i], LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_OPA_TRANSP);			//设置选中项边框透明
	//	lv_obj_set_style_local_bg_opa(time_date_roller[i], LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_OPA_TRANSP);			//设置选中项边框透明
		 style_roller = *lv_roller_get_style(time_date_roller[i], LV_LABEL_STYLE_MAIN);
      //style_roller.text.font = &lv_font_roboto_16;
      //style_roller.text.color = LV_COLOR_BLACK;
		style_roller.body.opa =LV_OPA_TRANSP; //背景透明
		//style_roller.body.border.color=LV_COLOR_GRAY;
		lv_roller_set_style(time_date_roller[i], LV_ROLLER_STYLE_SEL, &style_roller);//设置选中项样式
		lv_roller_set_style(time_date_roller[i], LV_ROLLER_STYLE_BG, &style_roller);//设置选中项样式
		lv_roller_set_visible_row_count(time_date_roller[i], 1);															//设置最多可见1行
		
		if(i != 4 && i != 5)			//若不是月和日roller
			memcpy(roller_buf, roller_string, roller_max[i] * 3);			//从00开始拷贝所有项的字符串
		else
			memcpy(roller_buf, roller_string + 3, roller_max[i] * 3);			//从01开始拷贝所有项的字符串
		
		roller_buf[roller_max[i] * 3] = '\0';
		
		lv_roller_set_options(time_date_roller[i], roller_buf, LV_ROLLER_MODE_NORMAL); //设置选项(正常滚动模式)
		lv_obj_align(time_date_roller[i], cont, LV_ALIGN_IN_TOP_LEFT, 43 + i * 55, 50);																					 //设置对齐
		lv_roller_set_anim_time(time_date_roller[i], 500);		//动画时间500ms
	}
	//lv_obj_set_style_local_border_color(time_date_roller[0], LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_RED);			//设置第一个控件边框红色
	 style_roller_select = *lv_roller_get_style(time_date_roller[0], LV_LABEL_STYLE_MAIN); 
	 style_roller_select.body.border.color=LV_COLOR_CYAN; //设置第一个控件边框颜色
	 lv_roller_set_style(time_date_roller[0], LV_ROLLER_STYLE_BG, &style_roller_select);//设置第一个项背景样式
	lv_roller_set_style(time_date_roller[0], LV_ROLLER_STYLE_SEL, &style_roller_select);//设置第一个项的选中样式
	
	for(i = 0; i < 2; i++)		//创建时间的 : label
	{
		time_point_label[i] = lv_label_create(cont, NULL);		//创建时间point label
	//	lv_obj_set_style_local_text_font(time_point_label[i], LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20); //设置字体20号
	//	lv_obj_set_style_local_text_color(time_point_label[i], LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);		   //设置字体颜色
		lv_label_set_text(time_point_label[i], ":");
		lv_obj_align(time_point_label[i], time_date_roller[i], LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	}
	/*********************************************创建时间日期设置roller******************************************************/
	
	/*********************************************调整日期设置roller位置******************************************************/
	for(i = 0; i < 3; i++)
	{
		lv_obj_align(time_date_roller[i + 3], cont, LV_ALIGN_IN_LEFT_MID, 55 + i * 55, 0);																					 //设置对齐
	}
	lv_roller_set_options(time_date_roller[6], week_list, LV_ROLLER_MODE_NORMAL); //设置周选项(正常滚动模式)
	lv_obj_align(time_date_roller[6], cont, LV_ALIGN_CENTER, 0, 40);																							 //设置周对齐
	
	for(i = 0; i < 3; i++)	//创建日期的 年20 和 - label
	{
		date_Hline_label[i] = lv_label_create(cont, NULL);		//创建日期的 年20 和 - label
	//	lv_obj_set_style_local_text_font(date_Hline_label[i], LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20); //设置字体20号
	//	lv_obj_set_style_local_text_color(date_Hline_label[i], LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);		   //设置字体颜色
		lv_label_set_text(date_Hline_label[i], i ? "-" : "20");
		lv_obj_align(date_Hline_label[i], time_date_roller[i + 3], LV_ALIGN_OUT_LEFT_MID, i ? -4 : 0, 0);
	}
	/*********************************************调整日期设置roller位置******************************************************/
	
	/*********************************************创建确认按键btn******************************************************/
	enter_btn = lv_btn_create(cont, NULL);
	lv_obj_set_size(enter_btn, 60, 22);		//设置按钮大小
	lv_obj_align(enter_btn, cont, LV_ALIGN_IN_BOTTOM_MID, 0, -10);		//设置按钮对齐
	//lv_obj_set_style_local_bg_opa(enter_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);		//设置背景透明
	//lv_obj_set_style_local_border_color(enter_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);		//设置边框颜色
	
	enter_btn_label = lv_label_create(enter_btn, NULL);		//创建按钮label
	lv_label_set_text(enter_btn_label, "Enter");           
	//lv_obj_set_style_local_text_color(enter_btn_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);		   //设置字体颜色
	lv_obj_align(enter_btn_label, enter_btn, LV_ALIGN_CENTER, 0, 0);		//设置按钮文本对齐
	lv_style_copy(&btn_style,enter_btn->style_p);
  lv_style_copy(&btn_label_style,enter_btn_label->style_p);
	 app_setting_update_roller();
}

//设置当前选中roller的项
void app_setting_change(uint8_t index)
{
	lv_roller_set_selected(time_date_roller[foucs_roller_index], index, LV_ANIM_ON);
}

//设置当前选中的roller
void app_setting_change_roller(uint8_t i)
{
	foucs_roller_index = i;
	
	//lv_obj_set_style_local_border_color(time_date_roller[i == 0 ? 6 : i - 1], LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);			//设置上一个选中的roller控件边框白色
		
		lv_roller_set_style(time_date_roller[i == 0 ? 6 : i - 1], LV_ROLLER_STYLE_SEL, &style_roller);//设置选中项样式
		lv_roller_set_style(time_date_roller[i == 0 ? 6 : i - 1], LV_ROLLER_STYLE_BG, &style_roller);//设置选中项样式
	if(i != 7)
	{
		//lv_obj_set_style_local_border_color(enter_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);		//设置边框颜色
		//lv_obj_set_style_local_border_color(time_date_roller[foucs_roller_index], LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_RED);			//设置当前选中的roller控件边框红色
		lv_roller_set_style(time_date_roller[foucs_roller_index], LV_ROLLER_STYLE_BG, &style_roller_select);//设置选择项背景 浅蓝色 
	  lv_roller_set_style(time_date_roller[foucs_roller_index], LV_ROLLER_STYLE_SEL, &style_roller_select);//设置选择项的选择部分背景 
	}
	else
	{
		//lv_obj_set_style_local_border_color(enter_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);		//设置边框颜色
	 
	}
}

//获取当前选中的roller的项索引
uint8_t app_setting_get_roller_index(void)
{
	return lv_roller_get_selected(time_date_roller[foucs_roller_index]);
}

//获取每个roller最大项保存地址
const uint8_t* app_setting_get_roller_max(void)
{
	return roller_max;
}




/*******************************************
* @function: 	设置时间界面滑动的动画
* @input:			
* @return:		
* @author:		Jovan
* @time:			2021.12
*******************************************/
void app_setting_anim_Vexit(bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_y(cont);		//获取起始值
	
	obj_add_anim(
								cont, 		//动画对象
								(lv_anim_exec_xcb_t)lv_obj_set_y,				//动画函数
								lv_anim_speed_to_time(100, 0, 42),			//动画速度
								start,																	//起始值
								start + (dir ? -240 : 240),							//结束值
								lv_anim_path_bounce											//动画特效:模拟弹性物体下落
								);

}





void set_enter (bool enter)
{
	
	
    if(enter) 
    {
			 btn_style.body.border.color=LV_COLOR_RED;
			 btn_label_style.text.color=LV_COLOR_RED;
			 lv_label_set_text(enter_btn_label, "Set!");		
			 //lv_label_set_style(enter_btn_label,LV_LABEL_STYLE_MAIN, &btn_label_style);
			 //lv_btn_set_style(enter_btn,LV_BTN_STYLE_REL , &btn_style);
			 
        //lv_obj_set_style_local_border_color(enter_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);		//设置边框颜色        
        //lv_obj_set_style_local_text_color(enter_btn_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);		   //设置字体颜色
     
    
    }
    else 
    {
       // lv_obj_set_style_local_border_color(enter_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);		//设置边框颜色        
        //lv_obj_set_style_local_text_color(enter_btn_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);		   //设置字体颜色   
       btn_style.body.border.color=LV_COLOR_CYAN;
			 btn_label_style.text.color=LV_COLOR_CYAN;
       //lv_label_set_style(enter_btn_label,LV_LABEL_STYLE_MAIN, &btn_label_style);
			 //lv_btn_set_style(enter_btn,LV_BTN_STYLE_REL , &btn_style);
			 lv_label_set_text(enter_btn_label, "Unset!");		
    }
		 
     
}
