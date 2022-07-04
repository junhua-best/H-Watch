
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
    //当前, 下, 上, 确定, 返回
    {0, 1, 0, 0, 0,(*mainWindow)},//一级界面 表盘
    {1, 2, 0, 8, 1, (*menu1)},//二级界面第一行  设置
    {2, 3, 1, 9, 2,(*menu2)},//二级界面第二行  关于
    {3, 4, 2, 10, 3,(*menu3)},//二级界面第三行  秒表
    {4, 5, 3, 11, 4,(*menu4)},//二级界面第四行  备忘录
    {5, 1, 4, 12, 5,(*menu5)},//二级界面第五行  亮度

    {6, 3, 7, 13, 6,(*menu6)},//
    {7, 2, 0, 14, 7,(*menu7)},//
    {8, 8, 8, 1, 1,(*menu2_1)},//二级界面第四行       设置界面 长按确定和返回都可以返回
    {9, 9, 9, 2, 2,(*menu2_2)},//二级界面第五行       关于界面  长按确定和返回都可以返回
    {10, 10, 10, 3, 3,(*menu2_3)},//二级界面第六行  秒表界面 长按确定和返回都可以返回
    {11, 11, 11, 4, 4,(*menu2_4)},//二级界面第七行  备忘录界面 长按确定和返回都可以返回
    {12, 12, 12, 5, 5,(*menu2_5)},//二级界面第四行  亮度界面    长按确定和返回都可以返回
    {13, 13, 13, 13, 6,(*menu2_6)},//二级界面第五行
    {14, 14, 14, 14, 7,(*menu2_7)},//二级界面第六行

};

/***************************LCD按键选择画面*****************************************/

void LCD_display(uint8_t press)
{
    if(press!=0)
    {

        LastIndex = nowIndex;

        if(press==KEY1_PRESS)       // key1 下
        {
            nowIndex=Key_table[nowIndex].down_index;
            Key_table[nowIndex].operate(press);

        }
        else if(press==KEY2_PRESS)  //key2 上
        {
            nowIndex=Key_table[nowIndex].up_index;
            Key_table[nowIndex].operate(press);

        }
        else if(press==KEY1_LONG_PRESS)  //key1长按 确定
        {
            nowIndex=Key_table[nowIndex].enter_index;
            Key_table[nowIndex].operate(press);


        }
        else if(press==KEY2_LONG_PRESS)  //key2长按 退出
        {
            nowIndex=Key_table[nowIndex].esc_index;
            Key_table[nowIndex].operate(press);

        }
        printf("nowindex=%d\r\n",nowIndex);
    }

}



/**********************************主菜单*****************************************/

void mainWindow(uint8_t press)
{
    if(LastIndex!=0)
    {
        //printf("get in menu 0");
        app_digital_clock_meun_re(false); //表盘恢复
        app_menu_anim_Vexit_setting_img(false); //设置图标退出
        app_tip_anim_Vexit(false); //提示退出
			current_page=Page_Watch;
    }
}




void menu1(uint8_t press)//菜单1列表
{

    if(LastIndex==0 )
    {

        app_digital_clock_meun_re(true);    //表盘散开
        app_menu_anim_Vexit_setting_img(true); //设置时间图标垂直进入
        app_tip_anim_Vexit(true);            //提示进入
    }
    else if(LastIndex==8)  //从设置页面回来
    {
        app_menu_anim_Hexit_setting_img(false); //设置图标水平进入
        app_setting_anim_Vexit(true); //设置时间界面退出
        set_enter(0);
    }
    else if(LastIndex==5)  //从最后一个图标回来
    {
        app_menu_anim_Vexit_light_img(false);  //light退出
        app_menu_anim_Vexit_setting_img(true); //设置时间图标垂直进入
    }
    else if(LastIndex==2)  //从about图标回来
    {
        app_menu_anim_Vexit_about_img(false); //关于图标退出
        app_menu_anim_Vexit_setting_img(true); //设置时间图标垂直进入
    }
		current_page=Page_menu_setting;
}
void menu2(uint8_t press)//菜单2列表
{
    if(LastIndex==1)
    {

        app_menu_anim_Vexit_setting_img(false);  //设置时间图标退出
        app_menu_anim_Vexit_about_img(true); //关于图标进入
    }
    else if(LastIndex==9) //从关于界面回来
    {
        app_menu_anim_Hexit_about_img(false); //关于图标水平进入
        app_about_anim_Hexit(false);
        //app_about_anim_Vexit(true); //关于界面垂直退出
    }
    else if(LastIndex==3)
    {

        app_menu_anim_Vexit_stopwatch_img(false);     //stopwatch退出
        app_menu_anim_Vexit_about_img(true); //关于图标进入
    }
		current_page=Page_menu_about;
}
void menu3(uint8_t press)//菜单3列表
{
    if(LastIndex==2)
    {

        app_menu_anim_Vexit_about_img(false); //关于退出
        app_menu_anim_Vexit_stopwatch_img(true);     //stopwatch进入
    }
    else if(LastIndex==10)
    {
        //
        //app_menu_anim_Vexit_stopwatch_img(true);     //stopwatch进入
			  app_stopwatch_anim_Hexit_img(false);
        app_menu_anim_Hexit_stopwatch_img(false);  //stopwatc进入
    }
    else if(LastIndex==4)
    {

        app_menu_anim_Vexit_memo_img(false);  //memo退出
        app_menu_anim_Vexit_stopwatch_img(true);     //stopwatch进入
    }
		current_page=Page_menu_stopwatch;
}
void menu4(uint8_t press)//菜单4列表
{
    if(LastIndex==3)
    {
        app_menu_anim_Vexit_stopwatch_img(false); //stopwatch退出
        app_menu_anim_Vexit_memo_img(true);  //memo进入
    }
    else if(LastIndex==11)
    {
        app_menu_anim_Hexit_memo_img(false); //memo图标水平进入
        app_memo_anim_Hexit(false);
    }
    else if( LastIndex==5)
    {
        app_menu_anim_Vexit_light_img(false);  //light退出
        app_menu_anim_Vexit_memo_img(true);  //memo进入
    }
		current_page=Page_menu_memo;
}

void menu5(uint8_t press)//菜单5列表
{
    if(LastIndex==4)
    {
        app_menu_anim_Vexit_memo_img(false); //memo退出
        app_menu_anim_Vexit_light_img(true);  //light进入

    }
    if(LastIndex==12)
    {

        app_lighting_anim_Hexit_img(false);
        app_menu_anim_Hexit_light_img(false);  //light进入

    }
		current_page=Page_menu_light;

}

void menu6(uint8_t press)//菜单6列表
{

}

void menu7(uint8_t press)//菜单6列表
{

}

void menu2_1(uint8_t press)//设置界面
{
    if(LastIndex!=8)
    {
        printf("get in setting page");
        app_menu_anim_Hexit_setting_img(true); //设置图标水平移出
        //app_menu_anim_Vexit_setting_img(false);
        app_setting_anim_Vexit(false); //设置时间界面退出  //设置界面垂直移入
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
            app_setting_change(select_roller_index);		                                //设置当前选中roller的项(递减)
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
        app_menu_anim_Hexit_about_img(true); //关于图标退出
        // app_about_anim_Vexit(false); //关于界面进入
        app_about_anim_Hexit(true);
    }
		current_page=Page_About;
}

void menu2_3(uint8_t press)//关于列表
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

void menu2_4(uint8_t press)//游戏列表
{
	if(LastIndex!=11)
	{
			//app_menu_anim_Vexit_about_img(true);
			app_menu_anim_Hexit_memo_img(true); //memo图标退出
			// app_about_anim_Vexit(false); //memo界面进入
			app_memo_anim_Hexit(true);
	}
	current_page=Page_Memo;
}

void menu2_5(uint8_t press)//关于列表
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

void menu2_6(uint8_t press)//灯光列表
{

}

void menu2_7(uint8_t press)//时钟列表
{


}

