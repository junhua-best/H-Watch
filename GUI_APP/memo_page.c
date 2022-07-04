#include "DisplayPrivate.h"

extern uint8_t  buferRX[200];
extern uint8_t  bufferLen;
extern uint8_t			Rx2_Buf[1024];

lv_obj_t *scr_memo;  //memo�Ľ��洰��

static lv_obj_t *cont; //ҳ������
static lv_obj_t *label_title;  //��ʾ����
static lv_obj_t *label_line1;	   //��ʾmcu��Ϣ
static lv_obj_t *label_line2;	   //��ʾgui��Ϣ
static lv_obj_t *label_line3; //��ʾ������Ϣ
static lv_obj_t *logo_img;	   //logo
static lv_obj_t *label_line4;	   //�汾
static lv_obj_t *label_updatetime;	   //����ʱ��


static const char *memo_line1 = "1.Don't eat breakfast on an empty stomach.";
static const char *memo_line2 = "2.Submit paper on May 6th.";
static  char *memo_line3 = "3.Learn the principle of Linux kernel memory scheduling.";
static  char *memo_line4 = "4.Learn MIPI and GMSL.";

//�������ڽ���
void app_memo_create(void)
{

static lv_style_t cont_style;//
	scr_memo = lv_scr_act();

	cont = lv_cont_create(scr_memo, NULL);															//��������
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
	//lv_label_set_static_text(label_title, memo_line1);
	//lv_obj_align(label_title, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);

	label_line1 = lv_label_create(cont, NULL);														   //����MCU��ǩ
	lv_label_set_long_mode(label_line1,LV_LABEL_LONG_BREAK);
	lv_obj_set_size(label_line1,220,0);//���Ϊ 100 ����,�߶�����Ч��,�������ð�
	//lv_obj_set_style_local_text_color(label_line1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW); //����������ɫ
	lv_label_set_static_text(label_line1, memo_line1);
	lv_obj_align(label_line1, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 0);
 // lv_obj_set_pos(label_line1, 0, 80);
	label_line2 = lv_label_create(cont, NULL);														   //����GUI��ǩ
	lv_label_set_long_mode(label_line2,LV_LABEL_LONG_BREAK);
	lv_obj_set_size(label_line2,220,0);//���Ϊ 100 ����,�߶�����Ч��,�������ð�
	//lv_obj_set_style_local_text_color(label_line2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW); //����������ɫ
	lv_label_set_static_text(label_line2, memo_line2);
	lv_obj_align(label_line2, label_line1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);

	label_line3 = lv_label_create(cont, NULL);															  //����AUTHOR��ǩ
	lv_label_set_long_mode(label_line3,LV_LABEL_LONG_BREAK);
	lv_obj_set_size(label_line3,220,0);//���Ϊ 100 ����,�߶�����Ч��,�������ð�
	lv_label_set_static_text(label_line3, memo_line3);
	lv_obj_align(label_line3, label_line2, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);

	label_line4 = lv_label_create(cont, NULL);															  //�����汾
	lv_label_set_long_mode(label_line4,LV_LABEL_LONG_BREAK);
	lv_obj_set_size(label_line4,220,0);//���Ϊ 100 ����,�߶�����Ч��,�������ð�
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

//memo��ֱ�����ƶ�������˳�����
void app_memo_anim_Vexit(bool dir)
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


//memoˮƽ�����ƶ�������˳�����
void app_memo_anim_Hexit(bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_x(cont);		//��ȡ��ʼֵ
	
	obj_add_anim(
								cont, 		//��������
								(lv_anim_exec_xcb_t)lv_obj_set_x,				//��������
								lv_anim_speed_to_time(100, 0, 42),			//�����ٶ�
								start,																	//��ʼֵ
								start + (dir ? -240 : 240),							//����ֵ
								lv_anim_path_bounce											//������Ч:ģ�ⵯ����������
								);
}