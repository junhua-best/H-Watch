#include "DisplayPrivate.h"


LV_IMG_DECLARE(people1);
LV_IMG_DECLARE(people3);
LV_IMG_DECLARE(people5);
LV_IMG_DECLARE(people7);
LV_IMG_DECLARE(people9);
LV_IMG_DECLARE(people11);
LV_IMG_DECLARE(people13);
LV_IMG_DECLARE(people15);
LV_IMG_DECLARE(people17);
LV_IMG_DECLARE(people19);
LV_IMG_DECLARE(people21);
LV_IMG_DECLARE(people23);
LV_IMG_DECLARE(people25);
LV_IMG_DECLARE(people27);
LV_IMG_DECLARE(people29);



LV_FONT_DECLARE(my_font_icon_num60);//�������� ��������
LV_FONT_DECLARE(my_font_icon_num20);//�������� ��������
LV_FONT_DECLARE(my_font_icon_22);//�������� ��������
LV_FONT_DECLARE(lv_font_roboto_22) ; //ϵͳ�Դ�����

//Cloudy, heavy rain, lightning, humidity, walking, temperature, sunny, Celsius,
#define MY_ICON_Cloudy "\xEE\x98\x80" //����
#define MY_ICON_rain "\xEE\x98\x81" //����
#define MY_ICON_lightning "\xEE\x98\x9E" //�׵�  
#define MY_ICON_humidity "\xEE\x98\x8F" //ʪ�� 
#define MY_ICON_walking "\xEE\x98\xA6" //���� 
#define MY_ICON_temperature "\xEE\x98\xAE" //�¶�ͼ�� 
#define MY_ICON_sunny "\xEE\x98\xB6" //���� 
#define MY_ICON_Celsius "\xEE\x9B\xAE" //���϶� 
#define MY_ICON_calendar "\xEE\xA6\xAB" //���� 
#define MY_ICON_battery "\xEE\x9A\x87" //����

#define MY_ICON_Nike "\xEE\x98\x97" //nike
// #define MY_ICON_Dunk "\xEE\x99\x9F" //����
#define MY_ICON_AJ "\xEE\x98\xA9" //AJ
#define MY_ICON_bluetooth "\xEE\x9A\x97" //����
#define MY_ICON_Watch "\xEE\xA4\x91" //�ֱ�
static lv_obj_t *start_img; //���忪ʼ����ͼƬimg
static lv_obj_t *main_img;	//����������ͼƬimg
static lv_obj_t *people_img;	//����������ͼƬimg
static lv_obj_t *bar_img;	//����������ͼƬimg
static lv_obj_t *charge_img;
static lv_obj_t *scr;
static lv_obj_t *watch_cont; //̫���˶�ͼ����
static lv_obj_t *temperature_cont; //��������
static lv_obj_t *date_cont; //��������
static lv_obj_t *week_cont; //��������
static lv_obj_t *bat_cont; //��������
static lv_obj_t *time_cont; //ʱ������
static lv_obj_t *entry_cont; //ʱ������
static lv_obj_t *step_cont; //������������
static lv_obj_t *humidity_cont; //ʪ������






static lv_obj_t* label_icon;
static lv_obj_t* label_week;
static lv_obj_t* label_time;
static lv_obj_t* label_second;
static lv_obj_t* label_date;
static lv_obj_t* label_date_icon;
static lv_obj_t* label_temperature;
static lv_obj_t* label_temperature_icon;
static lv_obj_t* label_temperature_celsius;
static lv_obj_t* label_bat_icon;
static lv_obj_t* label_bat;
static lv_obj_t* label_entry_icon;
static lv_obj_t* label_entry;
static lv_obj_t* label_step_icon;
static lv_obj_t* label_step;
static lv_obj_t* label_distance;
static lv_obj_t* label_distance_km;
static lv_obj_t* label_humidity_icon;
static lv_obj_t* label_humidity;
static lv_obj_t* square_line;
static lv_obj_t* up_line;
static lv_obj_t* up_line2;
static lv_obj_t* down_line;
static lv_obj_t* down_line2;
static lv_obj_t* down_line3;


const lv_point_t line_points[] = { {0, 65}, {100, 65},{100,180},{0,180} };
const lv_point_t line_points_up[] = { {0, 40}, {240, 40} };
const lv_point_t line_points_up2[] = { {160,0 }, {160, 40} };

const lv_point_t line_points_down[] = { {0, 200}, {240, 200} };
const lv_point_t line_points_down2[] = { {80,200 }, {80, 240} };
const lv_point_t line_points_down3[] = { {160,200 }, {160, 240} };
#define LINE_POINTS_NUM (sizeof(line_points)/sizeof(line_points[0]))

uint8_t i=1;
lv_task_t *astronauts_task;
//̫���˶�ͼ task
void astronauts_task_cb(lv_task_t* task)
{
    i=i+2;
    if(i>29) i=1;
    get_num(i);

}

char time_now[6] ; //Сʱ�ͷ���
char date_now[6] ; //���� 11.20
char second_now[3];
char bat_now[3];
char temperature_now[4];
char humidity_now[4];
char step_now[5];
lv_task_t *data_update_task;
void data_update_task_cb(lv_task_t* task)
{
    //printf("get in data_update_task cb \r\n");
    /*
    lv_label_set_static_text(label_BAT, bat);
    if(adcx>80)   lv_img_set_src(bar_img,LV_SYMBOL_DUMMY LV_SYMBOL_BATTERY_FULL);
    else if(adcx>60&&adcx<=80) lv_img_set_src(bar_img,LV_SYMBOL_DUMMY LV_SYMBOL_BATTERY_3);
    else if(adcx>40&&adcx<=60) lv_img_set_src(bar_img,LV_SYMBOL_DUMMY LV_SYMBOL_BATTERY_2);
    else if(adcx>20&&adcx<=40) lv_img_set_src(bar_img,LV_SYMBOL_DUMMY LV_SYMBOL_BATTERY_1);
    else  lv_img_set_src(bar_img,LV_SYMBOL_DUMMY LV_SYMBOL_BATTERY_EMPTY);
    if(Charge==1) lv_img_set_src(charge_img,LV_SYMBOL_DUMMY LV_SYMBOL_CHARGE);
    else          lv_img_set_src(charge_img,LV_SYMBOL_DUMMY LV_SYMBOL_GPS); */

    //���µ���
    if(bat_change_flag)
    {
        lv_label_set_static_text(label_bat, bat_now); //��������
        bat_change_flag=0;
    }

    //������ʪ��
    lv_label_set_static_text(label_temperature, temperature_now);
    lv_label_set_static_text(label_humidity, humidity_now);

    //���²���
     lv_label_set_static_text(label_step, step_now);
    //�ǵ�sprintf�󣬰�������������һλ��Ϊֹͣλ��������label����ʾ��ȫ
    if(date_change_flag)
    {
        lv_label_set_static_text(label_date, date_now); //��������
        date_change_flag=0;
    }
    if(time_change_flag)
    {
        lv_label_set_static_text(label_time, time_now); //����ʱ��


        time_change_flag=0;
    }
    if(second_change_flag)
    {
        lv_label_set_static_text(label_second, second_now); //��������
        second_change_flag=0;

    }

    //��������
    if(week_change_flag)
    {
        week_change_flag=0;
        switch (sDate.WeekDay)
        {
				case 0:
            lv_label_set_static_text(label_week,"Sunday");
						 break;
        case 1:
            lv_label_set_static_text(label_week,"Monday");
            break;
        case 2:
            lv_label_set_static_text(label_week,"Tuesday");
            break;
        case 3:
            lv_label_set_static_text(label_week,"Wednesday");
            break;
        case 4:
            lv_label_set_static_text(label_week,"Thursday");
            break;
        case 5:
            lv_label_set_static_text(label_week,"Friday");
            break;
        case 6:
            lv_label_set_static_text(label_week,"Saturday");
            break;
        
            
				default:
						lv_label_set_static_text(label_week,"Error");
						break;
        }
    }
    /*
    if(hours==23&&minutes==59&&seconds==59)
    {
    dmp_set_pedometer_step_count(0);
    dmp_set_pedometer_walk_time(0);
    }
    */
}

//�������ֱ���
void app_digital_clock_create(void)
{
    static lv_style_t label_time_style;
    static lv_style_t time_cont_style;
    static lv_style_t week_label_style;
    static lv_style_t label_icon_style;
    static lv_style_t label_num22_style;
    static lv_style_t label_second_style;
    scr = lv_scr_act();  //��ȡ��Ļ����


    /***********************����̫���˿ؼ�****************************/
    watch_cont = lv_cont_create(scr, NULL);															  //����̫��������
    //lv_obj_set_pos(watch_cont, 0, 0);																  //��������λ��
    lv_obj_set_size(watch_cont, 240, 240);															  //���������ߴ�
    lv_cont_set_layout(watch_cont, LV_LAYOUT_OFF);												  //���������Ĳ��ַ�ʽ: �����Զ�����
    people_img =lv_img_create(watch_cont,NULL);                                                          // //����ͼƬ
    lv_obj_align(people_img, watch_cont, LV_ALIGN_IN_LEFT_MID, 0, -40);
    /***********************����ʱ��ؼ�****************************/

    time_cont = lv_cont_create(watch_cont, NULL);													  //����ʱ������
    lv_obj_set_size(time_cont, 130, 115);															  //���������ߴ�
    lv_style_copy(&time_cont_style,time_cont->style_p);//��ʽ����
    time_cont_style.body.opa=LV_OPA_TRANSP;
    time_cont_style.body.border.opa=LV_OPA_TRANSP;
    lv_cont_set_style(time_cont,LV_CONT_STYLE_MAIN,&time_cont_style);//����ǩ����������ʽ


    label_time = lv_label_create(time_cont, NULL);
    lv_obj_align(label_time, time_cont, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_style_copy(&label_time_style,&lv_style_plain_color);//��ʽ����
    label_time_style.text.font = &my_font_icon_num60;//��������
    label_time_style.text.color=LV_COLOR_GRAY;
    lv_label_set_style(label_time,LV_LABEL_STYLE_MAIN,&label_time_style);//����ǩ����������ʽ
    //	lv_label_set_text(label_time,"05:43");
    lv_obj_align(time_cont, people_img, LV_ALIGN_OUT_RIGHT_TOP, 38, -10); //ʱ����������̫����

    //������label
    label_second= lv_label_create(time_cont, NULL);
    //lv_style_copy(&label_second_style,label_second->style_p);//��ʽ����
    // label_second_style.text.font = &lv_font_roboto_22;//��������
    //	label_second_style.text.color=LV_COLOR_GRAY;
    //lv_label_set_style(label_second,LV_LABEL_STYLE_MAIN,&label_second_style);//����ǩ����������ʽ
    lv_obj_align(label_second, time_cont, LV_ALIGN_IN_BOTTOM_RIGHT, 5, -10);

    //lv_label_set_text(label_second,"50");
    /***********************��������icon****************************/
    label_icon=lv_label_create(time_cont, NULL);
    lv_style_copy(&label_icon_style,label_icon->style_p);//��ʽ����
    label_icon_style.text.font = &my_font_icon_22;//��������
    //label_icon_style.body.opa=LV_OPA_TRANSP;
    //label_icon_style.body.border.opa=LV_OPA_TRANSP;
    //label_icon_style.text.color= LV_COLOR_CYAN;
    lv_label_set_style(label_icon,LV_LABEL_STYLE_MAIN,&label_icon_style);//����ǩ����������ʽ
    lv_obj_align(label_icon, time_cont, LV_ALIGN_IN_TOP_LEFT, 5, 10);
    lv_label_set_text(label_icon, MY_ICON_sunny MY_ICON_bluetooth);


    /***********************������������****************************/
    week_cont=lv_cont_create(watch_cont, NULL);
    lv_obj_set_pos(week_cont, 0, 200);
    lv_obj_set_size(week_cont, 80, 40);
    label_week = lv_label_create(week_cont, NULL);
    lv_obj_align( label_week, week_cont, LV_ALIGN_IN_LEFT_MID, 5, 0);
    //lv_label_set_text(label_week, "Saturday");

    /***********************�������ڿؼ�****************************/
    date_cont=lv_cont_create(watch_cont, NULL);
    lv_obj_set_pos(date_cont, 80, 200);
    lv_obj_set_size(date_cont, 80, 40);

    //��������ͼ��

    label_date_icon=lv_label_create(date_cont, NULL);
    lv_label_set_style( label_date_icon,LV_LABEL_STYLE_MAIN,&label_icon_style);//����ǩ����������ʽ
    lv_obj_align( label_date_icon, date_cont, LV_ALIGN_IN_LEFT_MID, 4, 0);
    lv_label_set_text(label_date_icon, MY_ICON_calendar);

    //�������ڱ�ǩ

    label_date = lv_label_create(date_cont, NULL);
    lv_obj_align( label_date, label_date_icon, LV_ALIGN_OUT_RIGHT_MID, 3, 0);
    // lv_obj_align( label_date, date_cont, LV_ALIGN_IN_RIGHT_MID, 0, 0);
    // lv_label_set_text(label_date, "11-20");

    /***********************�����¶���ؿؼ�****************************/
    temperature_cont=lv_cont_create(watch_cont, NULL);
    lv_obj_set_pos(temperature_cont, 160, 200);
    lv_obj_set_size(temperature_cont, 80, 40);
    //�¶�ͼ��
    label_temperature_icon=lv_label_create(temperature_cont, NULL);
    lv_label_set_style( label_temperature_icon,LV_LABEL_STYLE_MAIN,&label_icon_style);//����ǩ����������ʽ
    lv_obj_align( label_temperature_icon, temperature_cont, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_label_set_text(label_temperature_icon, MY_ICON_temperature);
    //�¶�����
    label_temperature = lv_label_create(temperature_cont, NULL);
    lv_style_copy(&label_num22_style,label_temperature->style_p);//��ʽ����
    label_num22_style.text.font = &lv_font_roboto_22;//��������
    lv_label_set_style(label_temperature,LV_LABEL_STYLE_MAIN,&label_num22_style);//����ǩ����������ʽ
    lv_obj_align(label_temperature, temperature_cont,LV_ALIGN_CENTER, 0, 0);
    //�¶ȡ�ͼ��
    label_temperature_celsius=lv_label_create(temperature_cont, NULL);
    lv_label_set_style( label_temperature_celsius,LV_LABEL_STYLE_MAIN,&label_icon_style);//����ǩ����������ʽ
    lv_obj_align( label_temperature_celsius, label_temperature, LV_ALIGN_OUT_RIGHT_MID, -15, 0);
    lv_label_set_text(label_temperature_celsius, MY_ICON_Celsius);

    /***********************�������ϽǴ�������****************************/
    entry_cont=lv_cont_create(watch_cont, NULL);
    lv_obj_set_pos(entry_cont, 0, 0);
    lv_obj_set_size(entry_cont, 160, 40);
    label_entry_icon = lv_label_create(entry_cont, NULL);
    lv_obj_align( label_entry_icon, entry_cont, LV_ALIGN_IN_LEFT_MID, 2, 0);
    lv_label_set_style(label_entry_icon,LV_LABEL_STYLE_MAIN,&label_icon_style);//����ǩ����������ʽ
    lv_label_set_text(label_entry_icon, MY_ICON_Watch); //�ֱ�ͼ��

    label_entry=lv_label_create(entry_cont, NULL);
    lv_label_set_long_mode(label_entry, LV_LABEL_LONG_BREAK); //���ó��ı�ģʽ ��ʾ���� �Զ����л���ǰ�����
    lv_obj_set_pos(label_entry, 28, 0);
    lv_obj_set_size(label_entry, 140, 40);

    lv_label_set_text(label_entry, "To be with time.\r   H-WATCH"); //��ʱ��ͬ��
    /***********************������������****************************/

    bat_cont=lv_cont_create(watch_cont, NULL);
    lv_obj_set_pos(bat_cont, 160, 0);
    lv_obj_set_size(bat_cont,80, 40);
    label_bat_icon = lv_label_create(bat_cont, NULL);
    lv_obj_align( label_bat_icon, bat_cont, LV_ALIGN_IN_LEFT_MID, 5, 0);
    lv_label_set_style(label_bat_icon,LV_LABEL_STYLE_MAIN,&label_icon_style);//����ǩ����������ʽ
    lv_label_set_text(label_bat_icon, MY_ICON_battery); //����ͼ��

    label_bat = lv_label_create(bat_cont, NULL);
    lv_label_set_style(label_bat,LV_LABEL_STYLE_MAIN,&label_num22_style);//����ǩ����������ʽ
    lv_obj_align(label_bat, label_bat_icon,LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_label_set_text(label_bat, "100");

    /***********************������������****************************/
    step_cont=lv_cont_create(watch_cont, NULL);
    lv_obj_set_pos(step_cont, 0, 160);
    lv_obj_set_size(step_cont,120, 30);
    label_step_icon = lv_label_create(step_cont, NULL);
    lv_obj_align( label_step_icon, step_cont, LV_ALIGN_IN_LEFT_MID, 20, 0);
    lv_label_set_style(label_step_icon,LV_LABEL_STYLE_MAIN,&label_icon_style);//����ǩ����������ʽ
    lv_label_set_text(label_step_icon, MY_ICON_walking); //����ͼ��

    label_step = lv_label_create(step_cont, NULL);
    lv_cont_set_style(step_cont,LV_CONT_STYLE_MAIN,&time_cont_style);//����ǩ����������ʽ
    //lv_label_set_style(label_bat,LV_LABEL_STYLE_MAIN,&label_num22_style);//����ǩ����������ʽ
    lv_obj_align(label_step, label_step_icon,LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_label_set_text(label_step, "3000");
    /*
    label_distance=lv_label_create(step_cont, NULL);
    lv_obj_align(label_distance, label_step,LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_label_set_text(label_distance, "0.18");

    label_distance_km=lv_label_create(step_cont, NULL);
    lv_obj_align(label_distance_km, label_distance,LV_ALIGN_OUT_RIGHT_MID, 2, 0);
    lv_label_set_text(label_distance_km, "km");
    */
    /***********************����ʪ������****************************/
    humidity_cont=lv_cont_create(watch_cont, NULL);
    lv_obj_set_pos( humidity_cont, 120, 160);
    lv_obj_set_size( humidity_cont,120, 30);
    label_humidity_icon = lv_label_create(humidity_cont, NULL);
    lv_obj_align( label_humidity_icon, humidity_cont, LV_ALIGN_IN_LEFT_MID, 10, 0);
    lv_label_set_style(label_humidity_icon,LV_LABEL_STYLE_MAIN,&label_icon_style);//����ǩ����������ʽ
    lv_label_set_text(label_humidity_icon, MY_ICON_humidity); //ʪ��

    label_humidity = lv_label_create(humidity_cont, NULL);
    //lv_label_set_style(label_bat,LV_LABEL_STYLE_MAIN,&label_num22_style);//����ǩ����������ʽ
    lv_obj_align(label_humidity, label_humidity_icon,LV_ALIGN_OUT_RIGHT_MID, 3, 0);
    lv_cont_set_style(humidity_cont,LV_CONT_STYLE_MAIN,&time_cont_style);//����ǩ����������ʽ
    lv_label_set_text(label_humidity, "80.7%");
    /***********************���������Ϸ���������****************************/
    up_line=lv_line_create(watch_cont, NULL);
    lv_line_set_points(up_line, line_points_up,2);

    up_line2=lv_line_create(watch_cont, NULL);
    lv_line_set_points(up_line2, line_points_up2,2);

    /***********************���������·���������****************************/
    down_line=lv_line_create(watch_cont, NULL);
    lv_line_set_points(down_line, line_points_down,2);
    down_line2=lv_line_create(watch_cont, NULL);
    lv_line_set_points(down_line2, line_points_down2,2);
    down_line3=lv_line_create(watch_cont, NULL);
    lv_line_set_points(down_line3, line_points_down3,2);


    /***********************����̫����task****************************/
    astronauts_task = lv_task_create(astronauts_task_cb,35,LV_TASK_PRIO_HIGH,NULL);
    lv_task_ready(astronauts_task);
    /***********************����ʱ���������task****************************/
    data_update_task = lv_task_create(data_update_task_cb,500,LV_TASK_PRIO_HIGH,NULL);
    lv_task_ready(data_update_task);
}
void get_num(int i)
{



    switch(i)
    {
    case 1:
        lv_img_set_src(people_img,&people1);
        break;
    //  case 2: lv_img_set_src(people_img,&people2);break;
    case 3:
        lv_img_set_src(people_img,&people3);
        break;
    //case 4: lv_img_set_src(people_img,&people4);break;
    case 5:
        lv_img_set_src(people_img,&people5);
        break;
    // case 6: lv_img_set_src(people_img,&people6);break;
    case 7:
        lv_img_set_src(people_img,&people7);
        break;
    // case 8: lv_img_set_src(people_img,&people8);break;
    case 9:
        lv_img_set_src(people_img,&people9);
        break;
    // case 10: lv_img_set_src(people_img,&people10);break;
    case 11:
        lv_img_set_src(people_img,&people11);
        break;
    //  case 12: lv_img_set_src(people_img,&people12);break;
    case 13:
        lv_img_set_src(people_img,&people13);
        break;
    // case 14: lv_img_set_src(people_img,&people14);break;
    case 15:
        lv_img_set_src(people_img,&people15);
        break;
    // case 16: lv_img_set_src(people_img,&people16);break;
    case 17:
        lv_img_set_src(people_img,&people17);
        break;
    //  case 18: lv_img_set_src(people_img,&people18);break;
    case 19:
        lv_img_set_src(people_img,&people19);
        break;
    //  case 20: lv_img_set_src(people_img,&people20);break;
    case 21:
        lv_img_set_src(people_img,&people21);
        break;
    // case 22: lv_img_set_src(people_img,&people22);break;
    case 23:
        lv_img_set_src(people_img,&people23);
        break;
    //   case 24: lv_img_set_src(people_img,&people24);break;
    case 25:
        lv_img_set_src(people_img,&people25);
        break;
    //   case 26: lv_img_set_src(people_img,&people26);break;
    case 27:
        lv_img_set_src(people_img,&people27);
        break;
    //   case 28: lv_img_set_src(people_img,&people28);break;
    case 29:
        lv_img_set_src(people_img,&people29);
        break;
        //  case 30: lv_img_set_src(people_img,&people30);break;

    }
}

//��������ʼ���������˳�����(true�����룬false���˳�)
void app_digital_clock_meun_re(bool dir)
{

    uint16_t start;
    start = lv_obj_get_y(watch_cont);		//��ȡ��ʼֵ
    /***************************�ϰ���Ļ**********************************/
    obj_add_anim(
        watch_cont, 		                        //��������
        (lv_anim_exec_xcb_t)lv_obj_set_y,		//��������
        lv_anim_speed_to_time(100, 0, 42),		//�����ٶ�
        start,									//��ʼֵ
        start + (dir ? 240 : -240),							//����ֵ
        lv_anim_path_bounce						//������Ч:ģ�ⵯ����������
    );


}
