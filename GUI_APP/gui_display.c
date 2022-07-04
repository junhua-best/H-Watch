
#include "DisplayPrivate.h"
#include "stm32l4xx_hal.h"


const uint8_t *roller_max;
uint8_t select_roller_index;
uint8_t Clock_Index;
uint8_t nowIndex=0;
uint8_t LastIndex=0;
uint16_t bglight=360;
int current_page=Page_Watch;

Key_index_struct const Key_table[15]=
{
    //��ǰ, ��, ��, ȷ��, ����
    {0, 1, 0, 0, 0,(*mainWindow)},//һ������ ����
    {1, 2, 0, 8, 1, (*menu1)},//���������һ��  ����
    {2, 3, 1, 9, 2,(*menu2)},//��������ڶ���  ����
    {3, 4, 2, 10, 3,(*menu3)},//�������������  ���
    {4, 5, 3, 11, 4,(*menu4)},//�������������  ����¼
    {5, 1, 4, 12, 5,(*menu5)},//�������������  ����

    {6, 3, 7, 13, 6,(*menu6)},//
    {7, 2, 0, 14, 7,(*menu7)},//
    {8, 8, 8, 1, 1,(*menu2_1)},//�������������       ���ý��� ����ȷ���ͷ��ض����Է���
    {9, 9, 9, 2, 2,(*menu2_2)},//�������������       ���ڽ���  ����ȷ���ͷ��ض����Է���
    {10, 10, 10, 3, 3,(*menu2_3)},//�������������  ������ ����ȷ���ͷ��ض����Է���
    {11, 11, 11, 4, 4,(*menu2_4)},//�������������  ����¼���� ����ȷ���ͷ��ض����Է���
    {12, 12, 12, 5, 5,(*menu2_5)},//�������������  ���Ƚ���    ����ȷ���ͷ��ض����Է���
    {13, 13, 13, 13, 6,(*menu2_6)},//�������������
    {14, 14, 14, 14, 7,(*menu2_7)},//�������������

};

/***************************LCD����ѡ����*****************************************/

void LCD_display(uint8_t press)
{
    if(press!=0)
    {

        LastIndex = nowIndex;

        if(press==KEY1_PRESS)       // key1 ��
        {
            nowIndex=Key_table[nowIndex].down_index;
            Key_table[nowIndex].operate(press);

        }
        else if(press==KEY2_PRESS)  //key2 ��
        {
            nowIndex=Key_table[nowIndex].up_index;
            Key_table[nowIndex].operate(press);

        }
        else if(press==KEY1_LONG_PRESS)  //key1���� ȷ��
        {
            nowIndex=Key_table[nowIndex].enter_index;
            Key_table[nowIndex].operate(press);


        }
        else if(press==KEY2_LONG_PRESS)  //key2���� �˳�
        {
            nowIndex=Key_table[nowIndex].esc_index;
            Key_table[nowIndex].operate(press);

        }
        printf("nowindex=%d\r\n",nowIndex);
    }

}



/**********************************���˵�*****************************************/

void mainWindow(uint8_t press)
{
    if(LastIndex!=0)
    {
        //printf("get in menu 0");
        app_digital_clock_meun_re(false); //���ָ̻�
        app_menu_anim_Vexit_setting_img(false); //����ͼ���˳�
        app_tip_anim_Vexit(false); //��ʾ�˳�
			current_page=Page_Watch;
    }
}




void menu1(uint8_t press)//�˵�1�б�
{

    if(LastIndex==0 )
    {

        app_digital_clock_meun_re(true);    //����ɢ��
        app_menu_anim_Vexit_setting_img(true); //����ʱ��ͼ�괹ֱ����
        app_tip_anim_Vexit(true);            //��ʾ����
    }
    else if(LastIndex==8)  //������ҳ�����
    {
        app_menu_anim_Hexit_setting_img(false); //����ͼ��ˮƽ����
        app_setting_anim_Vexit(true); //����ʱ������˳�
        set_enter(0);
    }
    else if(LastIndex==5)  //�����һ��ͼ�����
    {
        app_menu_anim_Vexit_light_img(false);  //light�˳�
        app_menu_anim_Vexit_setting_img(true); //����ʱ��ͼ�괹ֱ����
    }
    else if(LastIndex==2)  //��aboutͼ�����
    {
        app_menu_anim_Vexit_about_img(false); //����ͼ���˳�
        app_menu_anim_Vexit_setting_img(true); //����ʱ��ͼ�괹ֱ����
    }
		current_page=Page_menu_setting;
}
void menu2(uint8_t press)//�˵�2�б�
{
    if(LastIndex==1)
    {

        app_menu_anim_Vexit_setting_img(false);  //����ʱ��ͼ���˳�
        app_menu_anim_Vexit_about_img(true); //����ͼ�����
    }
    else if(LastIndex==9) //�ӹ��ڽ������
    {
        app_menu_anim_Hexit_about_img(false); //����ͼ��ˮƽ����
        app_about_anim_Hexit(false);
        //app_about_anim_Vexit(true); //���ڽ��洹ֱ�˳�
    }
    else if(LastIndex==3)
    {

        app_menu_anim_Vexit_stopwatch_img(false);     //stopwatch�˳�
        app_menu_anim_Vexit_about_img(true); //����ͼ�����
    }
		current_page=Page_menu_about;
}
void menu3(uint8_t press)//�˵�3�б�
{
    if(LastIndex==2)
    {

        app_menu_anim_Vexit_about_img(false); //�����˳�
        app_menu_anim_Vexit_stopwatch_img(true);     //stopwatch����
    }
    else if(LastIndex==10)
    {
        //
        //app_menu_anim_Vexit_stopwatch_img(true);     //stopwatch����
			  app_stopwatch_anim_Hexit_img(false);
        app_menu_anim_Hexit_stopwatch_img(false);  //stopwatc����
    }
    else if(LastIndex==4)
    {

        app_menu_anim_Vexit_memo_img(false);  //memo�˳�
        app_menu_anim_Vexit_stopwatch_img(true);     //stopwatch����
    }
		current_page=Page_menu_stopwatch;
}
void menu4(uint8_t press)//�˵�4�б�
{
    if(LastIndex==3)
    {
        app_menu_anim_Vexit_stopwatch_img(false); //stopwatch�˳�
        app_menu_anim_Vexit_memo_img(true);  //memo����
    }
    else if(LastIndex==11)
    {
        app_menu_anim_Hexit_memo_img(false); //memoͼ��ˮƽ����
        app_memo_anim_Hexit(false);
    }
    else if( LastIndex==5)
    {
        app_menu_anim_Vexit_light_img(false);  //light�˳�
        app_menu_anim_Vexit_memo_img(true);  //memo����
    }
		current_page=Page_menu_memo;
}

void menu5(uint8_t press)//�˵�5�б�
{
    if(LastIndex==4)
    {
        app_menu_anim_Vexit_memo_img(false); //memo�˳�
        app_menu_anim_Vexit_light_img(true);  //light����

    }
    if(LastIndex==12)
    {

        app_lighting_anim_Hexit_img(false);
        app_menu_anim_Hexit_light_img(false);  //light����

    }
		current_page=Page_menu_light;

}

void menu6(uint8_t press)//�˵�6�б�
{

}

void menu7(uint8_t press)//�˵�6�б�
{

}

void menu2_1(uint8_t press)//���ý���
{
    if(LastIndex!=8)
    {
        printf("get in setting page");
        app_menu_anim_Hexit_setting_img(true); //����ͼ��ˮƽ�Ƴ�
        //app_menu_anim_Vexit_setting_img(false);
        app_setting_anim_Vexit(false); //����ʱ������˳�  //���ý��洹ֱ����
        roller_max = app_setting_get_roller_max();
    }
    if(press==KEY1_PRESS)
    {
        Clock_Index++;
        Clock_Index%=8;
        app_setting_change_roller(Clock_Index);
    }
    if(press==KEY2_PRESS)
    {

        switch(Clock_Index)
        {
        case 0 :
            select_roller_index ++;
            select_roller_index%=24;
            app_setting_change(select_roller_index);		                                //���õ�ǰѡ��roller����(�ݼ�)
            break;
        case 1 :
            select_roller_index ++;
            select_roller_index%=60;
            app_setting_change(select_roller_index);
            break;
        case 2 :
            select_roller_index ++;
            select_roller_index%=60;
            app_setting_change(select_roller_index);
            break;
        case 3 :
            select_roller_index ++;
            select_roller_index%=30;
            app_setting_change(select_roller_index);
            break;
        case 4 :
            select_roller_index ++;
            select_roller_index%=12;
            app_setting_change(select_roller_index);
            break;
        case 5 :
            select_roller_index ++;
            select_roller_index%=31;
            app_setting_change(select_roller_index);
            break;
        case 6 :
            select_roller_index ++;
            select_roller_index%=7;
            app_setting_change(select_roller_index);
            break;
        case 7 :
            app_setting_update_time();
            set_enter(1);
            break;
        }
    }
		current_page=Page_Setting;
}

void menu2_2(uint8_t press)//
{
    if(LastIndex!=9)
    {
        //app_menu_anim_Vexit_about_img(true);
        app_menu_anim_Hexit_about_img(true); //����ͼ���˳�
        // app_about_anim_Vexit(false); //���ڽ������
        app_about_anim_Hexit(true);
    }
		current_page=Page_About;
}

void menu2_3(uint8_t press)//�����б�
{
		if(LastIndex!=10)
	{
			app_menu_anim_Hexit_stopwatch_img(true);
			app_stopwatch_anim_Hexit_img(true);
	}
	else
	{
			if(press==KEY2_PRESS)
			{
					SW_Reset();
			}
			if(press==KEY1_PRESS)
			{
					SW_Record();
			}
			
	}
	current_page=Page_Stopwatch;
}

void menu2_4(uint8_t press)//��Ϸ�б�
{
	if(LastIndex!=11)
	{
			//app_menu_anim_Vexit_about_img(true);
			app_menu_anim_Hexit_memo_img(true); //memoͼ���˳�
			// app_about_anim_Vexit(false); //memo�������
			app_memo_anim_Hexit(true);
	}
	current_page=Page_Memo;
}

void menu2_5(uint8_t press)//�����б�
{

    if(LastIndex!=12)
    {
        app_menu_anim_Hexit_light_img(true);
        app_lighting_anim_Hexit_img(true);
        set_bglighting(bglight);
    }
    else
    {
        if(press==KEY2_PRESS)
        {
            bglight+=10;
            if(bglight==370) bglight=360;
        }
        if(press==KEY1_PRESS)
        {
            bglight-=10;
            if(bglight==65526)  bglight=0;
        }
        set_bglighting(bglight);
    }
		current_page=Page_Light;
}

void menu2_6(uint8_t press)//�ƹ��б�
{

}

void menu2_7(uint8_t press)//ʱ���б�
{


}

