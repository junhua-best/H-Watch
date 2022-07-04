//#include "common.h"
#include "DisplayPrivate.h"

//��ʾͼƬ���� ÿ��5k



LV_IMG_DECLARE(setting);
LV_IMG_DECLARE(about);
LV_IMG_DECLARE(memo); 
LV_IMG_DECLARE(stopwatch);
LV_IMG_DECLARE(light);


static lv_obj_t *setting_img;	//����������ͼƬimg
static lv_obj_t *about_img;	//����������ͼƬimg
static lv_obj_t *memo_img;	//����������ͼƬimg
static lv_obj_t *stopwatch_img;	//����������ͼƬimg
static lv_obj_t *light_img;	//����������ͼƬimg


lv_obj_t *tip_cont; //��ʾ����
lv_obj_t *label_tip;//��ʾ��ǩ
lv_obj_t* tip_line;  //��ʾ����

const lv_point_t tip_line_points[] = { {10, 15}, {230, 15} };
#define TIP_LINE_POINTS_NUM (sizeof(tip_line_points)/sizeof(tip_line_points[0]))


//�����˵����� ͼƬ������Ļ�ײ�
void app_menu_create(void)
{
    lv_obj_t *scr = lv_scr_act(); //��ȡ��ǰ��Ծ����Ļ

    setting_img = lv_img_create(scr, NULL);						  //����img��ʾ�˵�
    lv_img_set_src(setting_img, &setting);			  //������ʾͼƬ
    lv_obj_align(setting_img, scr, LV_ALIGN_OUT_BOTTOM_MID, 0, 0); //����ͼƬ�������
    
    about_img = lv_img_create(scr, NULL);						  //����img��ʾ�˵�
    lv_img_set_src(about_img, &about);			  //������ʾͼƬ
    lv_obj_align(about_img, scr, LV_ALIGN_OUT_BOTTOM_MID, 0, 0); //����ͼƬ�������    
    
    memo_img = lv_img_create(scr, NULL);						  //����img��ʾ�˵�
    lv_img_set_src(memo_img, &memo);			  //������ʾͼƬ
    lv_obj_align(memo_img, scr, LV_ALIGN_OUT_BOTTOM_MID, 0, 0); //����ͼƬ�������

    stopwatch_img = lv_img_create(scr, NULL);						  //����img��ʾ�˵�
    lv_img_set_src(stopwatch_img, &stopwatch);			  //������ʾͼƬ
    lv_obj_align(stopwatch_img, scr, LV_ALIGN_OUT_BOTTOM_MID, 0, 0); //����ͼƬ�������    
	
	  light_img = lv_img_create(scr, NULL);						  //����img��ʾ�˵�
    lv_img_set_src(light_img, &light);			  //������ʾͼƬ
    lv_obj_align(light_img, scr, LV_ALIGN_OUT_BOTTOM_MID, 0, 0); //����ͼƬ�������    
}



//����ͼ�괹ֱ�����ƶ�������˳�����
void app_menu_anim_Vexit_setting_img( bool dir)
{
	uint16_t start;
	start = lv_obj_get_y(setting_img);		//��ȡ��ʼֵ
	obj_add_anim(
								setting_img, 		//��������
								(lv_anim_exec_xcb_t)lv_obj_set_y,				//��������
								lv_anim_speed_to_time(100, 0, 42),			//�����ٶ�
								start,																	//��ʼֵ
								start + (dir ? -160 : 160),							//����ֵ
								lv_anim_path_bounce											//������Ч:ģ�ⵯ����������
								);
    if(dir)        lv_label_set_static_text(label_tip,"Setting");
    
}
//����¼ͼ�괹ֱ�����ƶ�������˳�����
void app_menu_anim_Vexit_memo_img( bool dir)
{
	uint16_t start;
	start = lv_obj_get_y(memo_img);		//��ȡ��ʼֵ
	obj_add_anim(
								memo_img, 		//��������
								(lv_anim_exec_xcb_t)lv_obj_set_y,				//��������
								lv_anim_speed_to_time(100, 0, 42),			//�����ٶ�
								start,																	//��ʼֵ
								start + (dir ? -160 : 160),							//����ֵ
								lv_anim_path_bounce											//������Ч:ģ�ⵯ����������
								);
        if(dir)        lv_label_set_static_text(label_tip,"Memo");
}

//����ͼ�괹ֱ�����ƶ�������˳�����
void app_menu_anim_Vexit_about_img( bool dir)
{
	uint16_t start;
	start = lv_obj_get_y(about_img);		//��ȡ��ʼֵ
	obj_add_anim(
								about_img, 		//��������
								(lv_anim_exec_xcb_t)lv_obj_set_y,				//��������
								lv_anim_speed_to_time(100, 0, 42),			//�����ٶ�
								start,																	//��ʼֵ
								start + (dir ? -160 : 160),							//����ֵ
								lv_anim_path_bounce											//������Ч:ģ�ⵯ����������
								);
        if(dir)        lv_label_set_static_text(label_tip,"About");
}

//���ͼ�괹ֱ�����ƶ�������˳�����
void app_menu_anim_Vexit_stopwatch_img( bool dir)
{
	uint16_t start;
	start = lv_obj_get_y(stopwatch_img);		//��ȡ��ʼֵ
	obj_add_anim(
								stopwatch_img, 		//��������
								(lv_anim_exec_xcb_t)lv_obj_set_y,				//��������
								lv_anim_speed_to_time(100, 0, 42),			//�����ٶ�
								start,																	//��ʼֵ
								start + (dir ? -160 : 160),							//����ֵ
								lv_anim_path_bounce											//������Ч:ģ�ⵯ����������
								);
        if(dir)        lv_label_set_static_text(label_tip,"Stopwatch");
}

//������ͼ�괹ֱ�����ƶ�������˳�����
void app_menu_anim_Vexit_light_img( bool dir)
{
	uint16_t start;
	start = lv_obj_get_y(light_img);		//��ȡ��ʼֵ
	obj_add_anim(
								light_img, 		//��������
								(lv_anim_exec_xcb_t)lv_obj_set_y,				//��������
								lv_anim_speed_to_time(100, 0, 42),			//�����ٶ�
								start,																	//��ʼֵ
								start + (dir ? -160 : 160),							//����ֵ
								lv_anim_path_bounce											//������Ч:ģ�ⵯ����������
								);
        if(dir)        lv_label_set_static_text(label_tip,"Light");
}

/****************************************************************************************************************/
//settingͼ��ˮƽ�����ƶ�������˳�����
void app_menu_anim_Hexit_setting_img( bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_x(setting_img);		//��ȡ��ʼֵ
	
	obj_add_anim(
								setting_img, 		//��������
								(lv_anim_exec_xcb_t)lv_obj_set_x,				//��������
								lv_anim_speed_to_time(100, 0, 42),			//�����ٶ�
								start,																	//��ʼֵ
								start + (dir ? -130 : 130),							//����ֵ
								lv_anim_path_bounce											//������Ч:ģ�ⵯ����������
								);
}

//memoͼ��ˮƽ�����ƶ�������˳�����
void app_menu_anim_Hexit_memo_img( bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_x(memo_img);		//��ȡ��ʼֵ
	
	obj_add_anim(
								memo_img, 		//��������
								(lv_anim_exec_xcb_t)lv_obj_set_x,				//��������
								lv_anim_speed_to_time(100, 0, 42),			//�����ٶ�
								start,																	//��ʼֵ
								start + (dir ? -145 : 145),							//����ֵ
								lv_anim_path_bounce											//������Ч:ģ�ⵯ����������
								);
}


//����ͼ��ˮƽ�����ƶ�������˳�����
void app_menu_anim_Hexit_about_img( bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_x(about_img);		//��ȡ��ʼֵ
	
	obj_add_anim(
								about_img, 		//��������
								(lv_anim_exec_xcb_t)lv_obj_set_x,				//��������
								lv_anim_speed_to_time(100, 0, 42),			//�����ٶ�
								start,																	//��ʼֵ
								start + (dir ? -240 : 240),							//����ֵ
								lv_anim_path_bounce											//������Ч:ģ�ⵯ����������
								);
}


//stopwatchͼ��ˮƽ�����ƶ�������˳�����
void app_menu_anim_Hexit_stopwatch_img( bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_x(stopwatch_img);		//��ȡ��ʼֵ
	
	obj_add_anim(
								stopwatch_img, 		//��������
								(lv_anim_exec_xcb_t)lv_obj_set_x,				//��������
								lv_anim_speed_to_time(100, 0, 42),			//�����ٶ�
								start,																	//��ʼֵ
								start + (dir ? -240 : 240),							//����ֵ
								lv_anim_path_bounce											//������Ч:ģ�ⵯ����������
								);
}

//lightͼ��ˮƽ�����ƶ�������˳����� 
void app_menu_anim_Hexit_light_img( bool dir)
{
	uint16_t start;
	
	start = lv_obj_get_x(light_img);		//��ȡ��ʼֵ
	
	obj_add_anim(
								light_img, 		//��������
								(lv_anim_exec_xcb_t)lv_obj_set_x,				//��������
								lv_anim_speed_to_time(100, 0, 42),			//�����ٶ�
								start,																	//��ʼֵ
								start + (dir ? -240 : 240),							//����ֵ
								lv_anim_path_bounce											//������Ч:ģ�ⵯ����������
								);
}


/******************************************************************************************************/
  
//�����˵���ʾ����
void app_menu_tip_create(void)
{
    lv_obj_t *scr = lv_scr_act(); //��ȡ��ǰ��Ծ����Ļ	
	  static lv_style_t style_line;                     //������ʽ
	  static lv_style_t style_cont;                     //������ʽ
    
    /***********************������ʾ�ؼ�****************************/
    tip_cont = lv_cont_create(scr, NULL);															  //������ʾ����
    lv_obj_set_pos(tip_cont, 0, -60);																  //��������λ��
    lv_obj_set_size(tip_cont, 240, 60);															  //���������ߴ�
	  style_cont=*(lv_cont_get_style(tip_cont,LV_CONT_STYLE_MAIN));
	  style_cont.body.border.opa=LV_OPA_TRANSP;  //͸��
	  lv_cont_set_style(tip_cont, LV_CONT_STYLE_MAIN, &style_cont);        //����ʽ��ӵ�����
   // lv_obj_set_style_local_bg_opa(tip_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);	  //���ñ���͸��
   // lv_obj_set_style_local_border_opa(tip_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP); //���ñ߿�͸��
    //lv_cont_set_layout(tip_cont, LV_LAYOUT_OFF);												  //���������Ĳ��ַ�ʽ: �����Զ�����
   // lv_obj_set_style_local_pad_inner(tip_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);			  //�����Ӷ���֮��ļ��
    /***********************������ʾ�ؼ�****************************/
    
 	/***********************������ʾ����****************************/ 
    tip_line = lv_line_create(tip_cont, NULL);//������������
    lv_obj_set_pos(tip_line,0,30);
    lv_line_set_points(tip_line, tip_line_points, TIP_LINE_POINTS_NUM);
	 
	 
	  style_line=  *(lv_line_get_style(tip_line,LV_LINE_STYLE_MAIN));
    style_line.line.width=4  ;        //����ֱ����ʽ���
    style_line.line.rounded=1;   //����ֱ����ʽԲ�ε�(ĩ��) 
    //style_line.line.color=LV_COLOR_RED;
	  
  
    lv_line_set_style(tip_line, LV_LINE_STYLE_MAIN, &style_line);        //����ʽ��ӵ�ֱ��
   
   
	 

   // lv_obj_set_style_local_line_color(tip_line, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);		  //������ɫ
  //  lv_obj_set_style_local_line_width(tip_line, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 4);		  //�����߿�
  //  lv_obj_set_style_local_line_rounded(tip_line, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 1);		  //����Բ�� 
 	/***********************������ʾ����****************************/     
    
    
    label_tip = lv_label_create(tip_cont, NULL); 
    lv_obj_align(label_tip, tip_line, LV_ALIGN_OUT_TOP_MID, 0, -1);
   // lv_obj_set_style_local_text_font(label_tip, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_16); //��������
   // lv_obj_set_style_local_text_color(label_tip, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);		  //������ɫ

}


void app_tip_anim_Vexit( bool dir)
{
	uint16_t start;
	start = lv_obj_get_y(tip_cont);		//��ȡ��ʼֵ
	obj_add_anim(
								tip_cont, 		//��������
								(lv_anim_exec_xcb_t)lv_obj_set_y,				//��������
								lv_anim_speed_to_time(100, 0, 42),			//�����ٶ�
								start,																	//��ʼֵ
								start + (dir ? 60 : -60),							//����ֵ
								lv_anim_path_bounce											//������Ч:ģ�ⵯ����������
								);
}



