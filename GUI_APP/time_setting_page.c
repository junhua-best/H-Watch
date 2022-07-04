#include "DisplayPrivate.h"
#include "taskfun.h"
//#include "time_setting_page.h"


static lv_obj_t *cont; //ҳ������

static lv_obj_t *label_title; //��ʾ����
static const char *title_text = "Setting";

static lv_obj_t *time_date_roller[7]; //ʱ����������roller
static const uint8_t roller_max[7] = {24, 60, 60, 30, 12, 31, 7};
static const char roller_string[] = 
{
	"00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n\
24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n\
48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59\n"
};

static const char week_list[] = {"Sun.\nMon.\nTue.\nWen.\nThu.\nFri.\nSat."};				 //���б�

static uint8_t foucs_roller_index = 0;		//��ǰѡ�е�roller

static lv_obj_t *enter_btn;		//ȷ�ϰ���
static lv_obj_t *enter_btn_label;		//����label

static lv_obj_t *time_point_label[2];		//ʱ���: label
static lv_obj_t *date_Hline_label[3];		//���ڵ�20 �� - label

static lv_style_t style_roller; //����ѡ����style
static lv_style_t style_roller_select; //ѡ�е�ѡ����style
static lv_style_t btn_style;//
static lv_style_t btn_label_style;//
 

//���ݵ�ǰʱ���ʼ��roller
void app_setting_update_roller()
{
	lv_roller_set_selected(time_date_roller[0], sTime.Hours, LV_ANIM_OFF);		//ʱ
	lv_roller_set_selected(time_date_roller[1], sTime.Minutes, LV_ANIM_OFF);			//��
	lv_roller_set_selected(time_date_roller[2], sTime.Seconds, LV_ANIM_OFF);			//��
	
	lv_roller_set_selected(time_date_roller[3], sDate.Year, LV_ANIM_OFF);		//��
	lv_roller_set_selected(time_date_roller[4], sDate.Month - 1, LV_ANIM_OFF);			//��, ��1��ʼ����� - 1
	lv_roller_set_selected(time_date_roller[5], sDate.Date - 1, LV_ANIM_OFF);			//��, ��1��ʼ����� - 1
	
	lv_roller_set_selected(time_date_roller[6], sDate.WeekDay, LV_ANIM_OFF);			//��
}

//����roller���ʱ�����ͽṹ��
void app_setting_update_time()
{
	sTime.Hours = lv_roller_get_selected(time_date_roller[0]);		//��ȡʱ
	sTime.Minutes = lv_roller_get_selected(time_date_roller[1]);		//��ȡ��
	sTime.Seconds = lv_roller_get_selected(time_date_roller[2]);		//��ȡ��
	
	sDate.Year = lv_roller_get_selected(time_date_roller[3]) + 2000;		//��ȡ��
	sDate.Month = lv_roller_get_selected(time_date_roller[4]) + 1;		//��ȡ��
	sDate.Date = lv_roller_get_selected(time_date_roller[5]) + 1;		//��ȡ��
	
	sDate.WeekDay = lv_roller_get_selected(time_date_roller[6]);		//��ȡ��
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
		Transient_vibration(); //����ʾ
}

//�������ý���
void app_setting_create(void)
{
	
	uint8_t i;
	char roller_buf[181];
	
	lv_obj_t *scr_time_setting;  //����ʱ��Ľ��洰��
	scr_time_setting = lv_scr_act(); //��ȡ��ǰ��Ծ����Ļ
  
	/*********************************************�������ý�������******************************************************/
	
	cont = lv_cont_create(scr_time_setting, NULL);															//��������
	lv_obj_set_size(cont, 240, 240);															//����������С
	lv_obj_align(cont, scr_time_setting, LV_ALIGN_OUT_TOP_MID, 0, 0);										//������������Ļ�ⲿ���Ϸ�
	//lv_obj_set_style_local_bg_opa(cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);		//���ñ���͸��
	//lv_obj_set_style_local_border_opa(cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP); //���ñ߿�͸��

	lv_cont_set_layout(cont, LV_LAYOUT_OFF); //���������Ĳ��ַ�ʽ: �ر��Զ����֡�������Ҫ�����Ƿ��Զ�����,���������

	label_title = lv_label_create(cont, NULL);																   //���������ǩ
		
		// lv_label_set_style(label_title, LV_LABEL_STYLE_MAIN, &style_label);
	//lv_obj_set_style_local_text_font(label_title, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_32); //��������32��
	//lv_obj_set_style_local_text_color(label_title, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);		   //����������ɫ
	lv_label_set_static_text(label_title, title_text);														   //���ñ���
	lv_obj_align(label_title, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);											   //���ñ��ⶥ�˃Ȳ�����
	
	/*********************************************�������ý�������******************************************************/
	/*********************************************����ʱ����������roller******************************************************/
	for(i = 0; i < 7; i++)
	{
		time_date_roller[i] = lv_roller_create(cont, NULL);																//����ʱ������roller
	//	lv_obj_set_style_local_text_font(time_date_roller[i], LV_ROLLER_PART_BG, LV_STATE_DEFAULT, &lv_font_montserrat_20); //����20������
	//	lv_obj_set_style_local_text_color(time_date_roller[i], LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);		   //����������ɫ
	//	lv_obj_set_style_local_border_color(time_date_roller[i], LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);			//���ÿؼ��߿��ɫ
	//	lv_obj_set_style_local_bg_opa(time_date_roller[i], LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_OPA_TRANSP);			//���ÿؼ�����͸��
		 
		// lv_label_set_style(time_date_roller[i], LV_LABEL_STYLE_MAIN, &style_label);
		
	//	lv_obj_set_style_local_text_color(time_date_roller[i], LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);		   //����ѡ����������ɫ
	//	lv_obj_set_style_local_border_opa(time_date_roller[i], LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_OPA_TRANSP);			//����ѡ����߿�͸��
	//	lv_obj_set_style_local_bg_opa(time_date_roller[i], LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_OPA_TRANSP);			//����ѡ����߿�͸��
		 style_roller = *lv_roller_get_style(time_date_roller[i], LV_LABEL_STYLE_MAIN);
      //style_roller.text.font = &lv_font_roboto_16;
      //style_roller.text.color = LV_COLOR_BLACK;
		style_roller.body.opa =LV_OPA_TRANSP; //����͸��
		//style_roller.body.border.color=LV_COLOR_GRAY;
		lv_roller_set_style(time_date_roller[i], LV_ROLLER_STYLE_SEL, &style_roller);//����ѡ������ʽ
		lv_roller_set_style(time_date_roller[i], LV_ROLLER_STYLE_BG, &style_roller);//����ѡ������ʽ
		lv_roller_set_visible_row_count(time_date_roller[i], 1);															//�������ɼ�1��
		
		if(i != 4 && i != 5)			//�������º���roller
			memcpy(roller_buf, roller_string, roller_max[i] * 3);			//��00��ʼ������������ַ���
		else
			memcpy(roller_buf, roller_string + 3, roller_max[i] * 3);			//��01��ʼ������������ַ���
		
		roller_buf[roller_max[i] * 3] = '\0';
		
		lv_roller_set_options(time_date_roller[i], roller_buf, LV_ROLLER_MODE_NORMAL); //����ѡ��(��������ģʽ)
		lv_obj_align(time_date_roller[i], cont, LV_ALIGN_IN_TOP_LEFT, 43 + i * 55, 50);																					 //���ö���
		lv_roller_set_anim_time(time_date_roller[i], 500);		//����ʱ��500ms
	}
	//lv_obj_set_style_local_border_color(time_date_roller[0], LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_RED);			//���õ�һ���ؼ��߿��ɫ
	 style_roller_select = *lv_roller_get_style(time_date_roller[0], LV_LABEL_STYLE_MAIN); 
	 style_roller_select.body.border.color=LV_COLOR_CYAN; //���õ�һ���ؼ��߿���ɫ
	 lv_roller_set_style(time_date_roller[0], LV_ROLLER_STYLE_BG, &style_roller_select);//���õ�һ�������ʽ
	lv_roller_set_style(time_date_roller[0], LV_ROLLER_STYLE_SEL, &style_roller_select);//���õ�һ�����ѡ����ʽ
	
	for(i = 0; i < 2; i++)		//����ʱ��� : label
	{
		time_point_label[i] = lv_label_create(cont, NULL);		//����ʱ��point label
	//	lv_obj_set_style_local_text_font(time_point_label[i], LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20); //��������20��
	//	lv_obj_set_style_local_text_color(time_point_label[i], LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);		   //����������ɫ
		lv_label_set_text(time_point_label[i], ":");
		lv_obj_align(time_point_label[i], time_date_roller[i], LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	}
	/*********************************************����ʱ����������roller******************************************************/
	
	/*********************************************������������rollerλ��******************************************************/
	for(i = 0; i < 3; i++)
	{
		lv_obj_align(time_date_roller[i + 3], cont, LV_ALIGN_IN_LEFT_MID, 55 + i * 55, 0);																					 //���ö���
	}
	lv_roller_set_options(time_date_roller[6], week_list, LV_ROLLER_MODE_NORMAL); //������ѡ��(��������ģʽ)
	lv_obj_align(time_date_roller[6], cont, LV_ALIGN_CENTER, 0, 40);																							 //�����ܶ���
	
	for(i = 0; i < 3; i++)	//�������ڵ� ��20 �� - label
	{
		date_Hline_label[i] = lv_label_create(cont, NULL);		//�������ڵ� ��20 �� - label
	//	lv_obj_set_style_local_text_font(date_Hline_label[i], LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_20); //��������20��
	//	lv_obj_set_style_local_text_color(date_Hline_label[i], LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);		   //����������ɫ
		lv_label_set_text(date_Hline_label[i], i ? "-" : "20");
		lv_obj_align(date_Hline_label[i], time_date_roller[i + 3], LV_ALIGN_OUT_LEFT_MID, i ? -4 : 0, 0);
	}
	/*********************************************������������rollerλ��******************************************************/
	
	/*********************************************����ȷ�ϰ���btn******************************************************/
	enter_btn = lv_btn_create(cont, NULL);
	lv_obj_set_size(enter_btn, 60, 22);		//���ð�ť��С
	lv_obj_align(enter_btn, cont, LV_ALIGN_IN_BOTTOM_MID, 0, -10);		//���ð�ť����
	//lv_obj_set_style_local_bg_opa(enter_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);		//���ñ���͸��
	//lv_obj_set_style_local_border_color(enter_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);		//���ñ߿���ɫ
	
	enter_btn_label = lv_label_create(enter_btn, NULL);		//������ťlabel
	lv_label_set_text(enter_btn_label, "Enter");           
	//lv_obj_set_style_local_text_color(enter_btn_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);		   //����������ɫ
	lv_obj_align(enter_btn_label, enter_btn, LV_ALIGN_CENTER, 0, 0);		//���ð�ť�ı�����
	lv_style_copy(&btn_style,enter_btn->style_p);
  lv_style_copy(&btn_label_style,enter_btn_label->style_p);
	 app_setting_update_roller();
}

//���õ�ǰѡ��roller����
void app_setting_change(uint8_t index)
{
	lv_roller_set_selected(time_date_roller[foucs_roller_index], index, LV_ANIM_ON);
}

//���õ�ǰѡ�е�roller
void app_setting_change_roller(uint8_t i)
{
	foucs_roller_index = i;
	
	//lv_obj_set_style_local_border_color(time_date_roller[i == 0 ? 6 : i - 1], LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);			//������һ��ѡ�е�roller�ؼ��߿��ɫ
		
		lv_roller_set_style(time_date_roller[i == 0 ? 6 : i - 1], LV_ROLLER_STYLE_SEL, &style_roller);//����ѡ������ʽ
		lv_roller_set_style(time_date_roller[i == 0 ? 6 : i - 1], LV_ROLLER_STYLE_BG, &style_roller);//����ѡ������ʽ
	if(i != 7)
	{
		//lv_obj_set_style_local_border_color(enter_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);		//���ñ߿���ɫ
		//lv_obj_set_style_local_border_color(time_date_roller[foucs_roller_index], LV_ROLLER_PART_BG, LV_STATE_DEFAULT, LV_COLOR_RED);			//���õ�ǰѡ�е�roller�ؼ��߿��ɫ
		lv_roller_set_style(time_date_roller[foucs_roller_index], LV_ROLLER_STYLE_BG, &style_roller_select);//����ѡ����� ǳ��ɫ 
	  lv_roller_set_style(time_date_roller[foucs_roller_index], LV_ROLLER_STYLE_SEL, &style_roller_select);//����ѡ�����ѡ�񲿷ֱ��� 
	}
	else
	{
		//lv_obj_set_style_local_border_color(enter_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);		//���ñ߿���ɫ
	 
	}
}

//��ȡ��ǰѡ�е�roller��������
uint8_t app_setting_get_roller_index(void)
{
	return lv_roller_get_selected(time_date_roller[foucs_roller_index]);
}

//��ȡÿ��roller�������ַ
const uint8_t* app_setting_get_roller_max(void)
{
	return roller_max;
}




/*******************************************
* @function: 	����ʱ����滬���Ķ���
* @input:			
* @return:		
* @author:		Jovan
* @time:			2021.12
*******************************************/
void app_setting_anim_Vexit(bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_y(cont);		//��ȡ��ʼֵ
	
	obj_add_anim(
								cont, 		//��������
								(lv_anim_exec_xcb_t)lv_obj_set_y,				//��������
								lv_anim_speed_to_time(100, 0, 42),			//�����ٶ�
								start,																	//��ʼֵ
								start + (dir ? -240 : 240),							//����ֵ
								lv_anim_path_bounce											//������Ч:ģ�ⵯ����������
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
			 
        //lv_obj_set_style_local_border_color(enter_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);		//���ñ߿���ɫ        
        //lv_obj_set_style_local_text_color(enter_btn_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);		   //����������ɫ
     
    
    }
    else 
    {
       // lv_obj_set_style_local_border_color(enter_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);		//���ñ߿���ɫ        
        //lv_obj_set_style_local_text_color(enter_btn_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);		   //����������ɫ   
       btn_style.body.border.color=LV_COLOR_CYAN;
			 btn_label_style.text.color=LV_COLOR_CYAN;
       //lv_label_set_style(enter_btn_label,LV_LABEL_STYLE_MAIN, &btn_label_style);
			 //lv_btn_set_style(enter_btn,LV_BTN_STYLE_REL , &btn_style);
			 lv_label_set_text(enter_btn_label, "Unset!");		
    }
		 
     
}
