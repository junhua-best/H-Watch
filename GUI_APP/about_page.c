#include "DisplayPrivate.h"

lv_obj_t *scr_about;  //about�Ľ��洰��

static lv_obj_t *cont; //ҳ������
static lv_obj_t *label_title;  //��ʾ����
static lv_obj_t *label_mcu;	   //��ʾmcu��Ϣ
static lv_obj_t *label_gui;	   //��ʾgui��Ϣ
static lv_obj_t *label_author; //��ʾ������Ϣ
static lv_obj_t *logo_img;	   //logo
static lv_obj_t *label_version;	   //�汾
static lv_obj_t *label_updatetime;	   //����ʱ��

static const  char *title_text = "About";
static const char *mcu_text = "MCU: STM32L431CCU6";
static const char *gui_text = "GUI: littleVGL V6";
static  char *author_text = "AUTHOR: Jovan.situ "; 
static  char *version_text = "HWatch official-V2.5";
static  char *time_text = "Update_Time:2022.5.8";

//�������ڽ���
void app_about_create(void)
{

	static lv_style_t cont_style;//
	scr_about = lv_scr_act();
	

	cont = lv_cont_create(scr_about, NULL);															//��������
	lv_obj_set_size(cont, 240, 160);															//����������С
	lv_obj_set_pos(cont, 240, 60);	
	//lv_obj_align(cont, scr_about, LV_ALIGN_OUT_TOP_MID, 0, 0);										//������������Ļ�ⲿ���Ϸ�
	 lv_style_copy(&cont_style,cont->style_p);
	//lv_obj_set_style_local_bg_opa(cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);		//���ñ���͸��
//	lv_obj_set_style_local_border_opa(cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP); //���ñ߿�͸��
	 cont_style.body.border.opa=LV_OPA_TRANSP;
	 cont_style.body.opa=LV_OPA_TRANSP;
   lv_cont_set_style(cont,LV_CONT_STYLE_MAIN,&cont_style);
	lv_cont_set_layout(cont, LV_LAYOUT_OFF); //���������Ĳ��ַ�ʽ: �ر��Զ����֡�������Ҫ�����Ƿ��Զ�����,���������				
	//label_title = lv_label_create(cont, NULL);																   //���������ǩ
	//lv_obj_set_style_local_text_font(label_title, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_32); //��������20��
	//lv_obj_set_style_local_text_color(label_title, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);		   //����������ɫ
	//lv_label_set_static_text(label_title, title_text);
	//lv_obj_align(label_title, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);
  
	label_mcu = lv_label_create(cont, NULL);														   //����MCU��ǩ
		
	//lv_obj_set_style_local_text_color(label_mcu, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW); //����������ɫ
	lv_label_set_static_text(label_mcu, mcu_text);
	lv_obj_align(label_mcu, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
 // lv_obj_set_pos(label_mcu, 0, 80);
	label_gui = lv_label_create(cont, NULL);														   //����GUI��ǩ
	//lv_obj_set_style_local_text_color(label_gui, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW); //����������ɫ
	lv_label_set_static_text(label_gui, gui_text);
	lv_obj_align(label_gui, label_mcu, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

	label_author = lv_label_create(cont, NULL);															  //����AUTHOR��ǩ
//	lv_obj_set_style_local_text_color(label_author, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW); //����������ɫ
	lv_label_set_static_text(label_author, author_text);
	lv_obj_align(label_author, label_gui, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
	
	label_version = lv_label_create(cont, NULL);															  //�����汾
//	lv_obj_set_style_local_text_color(label_author, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW); //����������ɫ
	lv_label_set_static_text(label_version , version_text);
	lv_obj_align(label_version, label_author, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
	
	label_updatetime = lv_label_create(cont, NULL);															  //����ʱ���ǩ
//	lv_obj_set_style_local_text_color(label_author, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW); //����������ɫ
	lv_label_set_static_text(label_updatetime, time_text);
	lv_obj_align(label_updatetime, label_version, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

	//logo_img = lv_img_create(cont, NULL); //����logo
//	lv_img_set_src(logo_img, &start);	  //logo��ʾͼƬ
	//lv_obj_align(logo_img, cont, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
	//lv_obj_set_style_local_image_opa(logo_img, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
	//lv_img_set_auto_size(logo_img, false); //��ʹ�ܴ�С�Զ�����
}

//about��ֱ�����ƶ�������˳�����
void app_about_anim_Vexit(bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_y(cont);		//��ȡ��ʼֵ
	
	obj_add_anim(
								cont, 		//��������
								(lv_anim_exec_xcb_t)lv_obj_set_y,				//��������
								lv_anim_speed_to_time(100, 0, 42),			//�����ٶ�
								start,																	//��ʼֵ
								start + (dir ? -260 : 260),							//����ֵ
								lv_anim_path_bounce											//������Ч:ģ�ⵯ����������
								);
}


//aboutˮƽ�����ƶ�������˳�����
void app_about_anim_Hexit(bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_x(cont);		//��ȡ��ʼֵ
	
	obj_add_anim(
								cont, 		//��������
								(lv_anim_exec_xcb_t)lv_obj_set_x,				//��������
								lv_anim_speed_to_time(100, 0, 42),			//�����ٶ�
								start,																	//��ʼֵ
								start + (dir ? -245 : 245),							//����ֵ
								lv_anim_path_bounce											//������Ч:ģ�ⵯ����������
								);
}