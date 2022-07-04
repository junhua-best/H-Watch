
#include "DisplayPrivate.h"

LV_FONT_DECLARE(my_font_icon_num20); 
static lv_obj_t *stopwatch_cont; //ҳ������
static lv_obj_t * arcBright;/*����ָʾ��*/
static lv_obj_t *label_lighting;//��ʾ��ǩ
  

/*������*/
static lv_obj_t * labelTitle;

/*�������ָ���*/
static lv_obj_t * lineTitle;

/*���߱�*/
static lv_obj_t * lmeterSec;
/*ʱ��ǩ*/
static lv_obj_t * labelHour;
/*�����ǩ*/
static lv_obj_t * labelMinSec;
/*�����ǩ*/
static lv_obj_t * labelMs;

/*��ʷ��¼����*/
static lv_obj_t * contRecord;
/*��¼3����ʷ*/
#define SW_RECORD_MAX 3
/*��ʷ��¼��ǩ*/
static lv_obj_t * labelRecord_Grp[SW_RECORD_MAX];

extern unsigned long int global_tick; //��ʱ��tick 1ms
extern int stopwatch_tick_switch;
/*��ʷ��¼����*/
static uint8_t  SW_RecordIndex = 0;
/*��ʷ��¼*/
static uint32_t  SW_RecordMs[SW_RECORD_MAX];

/*����ǩˢ��������*/
static lv_task_t * taskLabelUpdate;

static void SW_Init();

static lv_obj_t *scr1;

/**
  * @brief  �������߱�
  * @param  ��
  * @retval ��
  */
 void LmeterSec_Create()
{
	  scr1 = lv_scr_act(); //
		static lv_style_t cont_style;
		static lv_style_t style_lmeter;

    /***********************����ʱ�����ÿؼ�****************************/
    stopwatch_cont = lv_cont_create(scr1, NULL);															  //������ʾ����
    lv_obj_set_pos(stopwatch_cont, 240, 60);																  //��������λ��
    lv_obj_set_size(stopwatch_cont, 240, 180);															  //���������ߴ�
	  cont_style.body.border.opa=LV_OPA_TRANSP;
    cont_style.body.opa=LV_OPA_TRANSP;
		lv_cont_set_style(stopwatch_cont,LV_CONT_STYLE_MAIN,&cont_style);
    style_lmeter = lv_style_pretty_color; 
    
    style_lmeter.body.main_color = LV_COLOR_BLUE;
    style_lmeter.body.grad_color = LV_COLOR_BLUE;
    style_lmeter.body.padding.left = 10;
    style_lmeter.line.width = 1;
    style_lmeter.line.color = LV_COLOR_SILVER;

    lmeterSec = lv_lmeter_create(stopwatch_cont, NULL);
    lv_lmeter_set_range(lmeterSec, 0, 59);
    //lv_lmeter_set_angle_offset(lmeterSec, 180);
    lv_lmeter_set_value(lmeterSec, 50);
    lv_lmeter_set_scale(lmeterSec, 360, 31);
    lv_lmeter_set_style(lmeterSec, LV_LMETER_STYLE_MAIN, &style_lmeter);
    lv_obj_set_size(lmeterSec, 110, 110);
    //lv_obj_set_opa_scale_enable(lmeterSec, true);
    //lv_obj_set_opa_scale(lmeterSec, LV_OPA_TRANSP);
    lv_obj_align(lmeterSec, stopwatch_cont,  LV_ALIGN_IN_TOP_MID, 0, 0);
		
	//����м��ʱ���ǩ
	  labelMinSec = lv_label_create(lmeterSec, NULL);
    
    static lv_style_t style_label1;
    style_label1 = *lv_label_get_style(labelMinSec, LV_LABEL_STYLE_MAIN);
    style_label1.text.font = &my_font_icon_num20;
    style_label1.text.color = LV_COLOR_BLACK;
    lv_label_set_style(labelMinSec, LV_LABEL_STYLE_MAIN, &style_label1);
    
    lv_label_set_recolor(labelMinSec, true);
    lv_label_set_text(labelMinSec, "00#FF0000 :#00");
    lv_obj_align(labelMinSec, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_auto_realign(labelMinSec, true);
    
    labelHour = lv_label_create(lmeterSec, NULL);
    
    static lv_style_t style_label2;
    style_label2 = *lv_label_get_style(labelHour, LV_LABEL_STYLE_MAIN);
    style_label2.text.color = LV_COLOR_GRAY;
    lv_label_set_style(labelHour, LV_LABEL_STYLE_MAIN, &style_label2);
    
    lv_label_set_text(labelHour, "00");
    lv_obj_align(labelHour, labelMinSec, LV_ALIGN_OUT_TOP_MID, 0, -3);
    lv_obj_set_auto_realign(labelHour, true);
    
    labelMs = lv_label_create(lmeterSec, NULL);
    lv_label_set_style(labelMs, LV_LABEL_STYLE_MAIN, &style_label2);
    lv_label_set_text(labelMs, ".000");
    lv_obj_align(labelMs, labelMinSec, LV_ALIGN_OUT_BOTTOM_MID, 0, 3);
    lv_obj_set_auto_realign(labelMs, true);
		//�������ļ�¼
		const lv_coord_t y_mod[SW_RECORD_MAX] = {-15, 0, 15};
    
    for(int i = 0; i < SW_RECORD_MAX; i++)
    {
        lv_obj_t * label = lv_label_create(stopwatch_cont, NULL);
        lv_label_set_text(label, "--:--:--.---");
        lv_obj_align(label, lmeterSec, LV_ALIGN_OUT_BOTTOM_MID, 0, y_mod[i] + 22); 
        lv_obj_set_auto_realign(label, true);
        //lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &style_label);
        labelRecord_Grp[i] = label;
			  lv_label_set_style(labelRecord_Grp[i], LV_LABEL_STYLE_MAIN, &style_label2);
    }
		SW_Init();//����ˢ�����ʱ������
}


/**
  * @brief  ����ǩˢ������
  * @param  task:������
  * @retval ��
  */
char stopwatch_hour[3];
char stopwatch_min_sec[6];
char stopwatch_ms[5];
static void SW_LabelUpdate(lv_task_t * task)
{
    uint16_t hh = (global_tick / (3600 * 1000)) % 100;
    uint16_t mm = (global_tick / (60 * 1000)) % 60;
    uint16_t ss = (global_tick / 1000) % 60;
    uint16_t ms = global_tick % 1000;
    
    static uint8_t cnt;
    if(cnt++ > 20)
    {
				//lv_label_set_static_text(label_updatetime, time_text);
				sprintf((char*)stopwatch_hour,"%02d",hh); stopwatch_hour[2]='\0';
			sprintf((char*)stopwatch_min_sec,"%02d:%02d",mm,ss); stopwatch_min_sec[5]='\0';
			//	sprintf((char*)stopwatch_sec,"%02d",ss); stopwatch_sec[2]='\0';
				sprintf((char*)stopwatch_ms,".%03d",ms); stopwatch_ms[4]='\0';
        lv_lmeter_set_value(lmeterSec, ss);
        lv_label_set_static_text(labelHour,stopwatch_hour);
        lv_label_set_static_text(labelMinSec,stopwatch_min_sec);
        cnt = 0;
    }
    lv_label_set_static_text(labelMs, stopwatch_ms);
}

char stopwatch_record_hour[3];
char stopwatch_record_min[3];
char stopwatch_record_sec[3];
char stopwatch_record_ms[4];
char stopwatch_record_time[14];
/**
  * @brief  �����ʷ��¼����
  * @param  ��
  * @retval ��
  */

/*
void SW_RecordUpdate()
{
    for(int i = 0; i < SW_RECORD_MAX; i++)
    {
        uint32_t ms = SW_RecordMs[i];
        if(ms > 0)
        {
            uint16_t hh = (ms / (3600 * 1000)) % 100;
            uint16_t mm = (ms / (60 * 1000)) % 60;
            uint16_t ss = (ms / 1000) % 60;
            ms = ms % 1000;
					sprintf((char*)stopwatch_record_time,".%02d:%2d:%2d:%3d",hh,mm,ss,ms); stopwatch_record_time[13]='\0';
            //lv_label_set_text_fmt(labelRecord_Grp[i], "%02d:%02d:%02d.%03d", hh, mm, ss, ms);
					lv_label_set_static_text(labelRecord_Grp[i], stopwatch_record_time);
        }
        else
        {
            lv_label_set_text(labelRecord_Grp[i], "--:--:--.---");
        }
    }
}
*/
void SW_RecordUpdate()
{

        uint32_t ms = SW_RecordMs[SW_RecordIndex];
        if(ms > 0)
        {
            uint16_t hh = (ms / (3600 * 1000)) % 100;
            uint16_t mm = (ms / (60 * 1000)) % 60;
            uint16_t ss = (ms / 1000) % 60;
            ms = ms % 1000;
					/*
						sprintf((char*)stopwatch_record_hour,".%02d",ms); stopwatch_record_hour[3]='\0';
						sprintf((char*)stopwatch_record_min,".%02d",ms); stopwatch_record_min[3]='\0';
						sprintf((char*)stopwatch_record_sec,".%02d",ms); stopwatch_record_sec[3]='\0';
						sprintf((char*)stopwatch_record_ms,".%03d",ms); stopwatch_record_ms[4]='\0';
					*/
					sprintf((char*)stopwatch_record_time,".%02d:%2d:%2d:%3d",hh,mm,ss,ms); stopwatch_record_time[13]='\0';
            //lv_label_set_text_fmt(labelRecord_Grp[i], "%02d:%02d:%02d.%03d", hh, mm, ss, ms);
					lv_label_set_static_text(labelRecord_Grp[SW_RecordIndex], stopwatch_record_time);  
				}					
        else
        {
            lv_label_set_text(labelRecord_Grp[SW_RecordIndex], "--:--:--.---");
        }
    
}
static void SW_Init()
{
    /*ˢ����ʷ��¼*/
    SW_RecordUpdate();
    /*ע������*/
    taskLabelUpdate = lv_task_create(SW_LabelUpdate, 11, LV_TASK_PRIO_HIGH, NULL);
    SW_LabelUpdate(taskLabelUpdate);
}

/**
  * @brief  ���λ
  * @param  ��
  * @retval ��
  */
void SW_Reset()
{
   
        global_tick = 0;
	      stopwatch_tick_switch=0;
        SW_RecordIndex = 0;
        for(int i = 0; i < SW_RECORD_MAX; i++)
        {
            SW_RecordMs[i] = 0;
        }
				//SW_RecordUpdate();
				lv_label_set_text(labelRecord_Grp[0], "--:--:--.---");
				lv_label_set_text(labelRecord_Grp[1], "--:--:--.---");
				lv_label_set_text(labelRecord_Grp[2], "--:--:--.---");
}

/**
  * @brief  ����¼
  * @param  ��
  * @retval ��
  */
void SW_Record()
{
	if(stopwatch_tick_switch==0)
	{
		stopwatch_tick_switch=1;
		return ;
	}

        if(SW_RecordIndex < SW_RECORD_MAX)
        {
            SW_RecordMs[SW_RecordIndex] = global_tick;
            SW_RecordUpdate();
            SW_RecordIndex++;
        }
}

//ʱ������ͼ��ˮƽ�����ƶ�������˳�����
void app_stopwatch_anim_Hexit_img( bool dir)
{
    uint16_t start;

    start = lv_obj_get_x(stopwatch_cont);		//��ȡ��ʼֵ

    obj_add_anim(
        stopwatch_cont, 		//��������
        (lv_anim_exec_xcb_t)lv_obj_set_x,				//��������
        lv_anim_speed_to_time(100, 0, 42),			//�����ٶ�
        start,																	//��ʼֵ
        start + (dir ? -240 : 240),							//����ֵ
        lv_anim_path_bounce											//������Ч:ģ�ⵯ����������
    );
}
