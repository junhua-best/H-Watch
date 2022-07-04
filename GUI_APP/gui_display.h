
#ifndef _GUI_DISPLAY_H_
#define _GUI_DISPLAY_H_


#include "key.h"
#include "guitest.h"


typedef struct{
	uint8_t current_index;	//存放当前界面的索引号；
	uint8_t down_index;		//存放按下“down（向下）”键时需要跳转到的索引号；
	uint8_t up_index;		//存放按下“up（向上）”键时需要跳转到的索引号；
	uint8_t enter_index;		//存放按下“enter（进入）”键时需要跳转的索引号；
	uint8_t esc_index;		//存放按下“esc（退出）”键时需要跳转的索引号；
	void (*operate)(uint8_t);	//函数指针变量，存放当前索引号下需要执行的函数的入口地址。
}Key_index_struct;
 

enum Page
{
	Page_Watch=0,
	Page_menu_setting=1,
	Page_menu_about=2,
	Page_menu_stopwatch=3,
	Page_menu_memo=4,
	Page_menu_light=5,
	Page_Setting=6,
	Page_About=7,
	Page_Stopwatch=8,
	Page_Memo=9,
	Page_Light=10,
};

extern int current_page;

void LCD_display(uint8_t press);
void mainWindow(uint8_t press);//主界面
void menu1(uint8_t press);//菜单列表
void menu2(uint8_t press);//菜单列表
void menu3(uint8_t pressd);//菜单列表
void menu4(uint8_t press);//菜单列表
void menu5(uint8_t press);//菜单列表
void menu6(uint8_t press);//菜单列表 
void menu7(uint8_t press);//菜单列表 
void menu2_1(uint8_t press);//菜单列表
void menu2_2(uint8_t press);//菜单列表
void menu2_3(uint8_t press);//菜单列表 
void menu2_4(uint8_t press);//菜单列表 
void menu2_5(uint8_t press);//菜单列表
void menu2_6(uint8_t press);//菜单列表 
void menu2_7(uint8_t press);//菜单列表 


#endif