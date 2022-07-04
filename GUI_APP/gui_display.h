
#ifndef _GUI_DISPLAY_H_
#define _GUI_DISPLAY_H_


#include "key.h"
#include "guitest.h"


typedef struct{
	uint8_t current_index;	//��ŵ�ǰ����������ţ�
	uint8_t down_index;		//��Ű��¡�down�����£�����ʱ��Ҫ��ת���������ţ�
	uint8_t up_index;		//��Ű��¡�up�����ϣ�����ʱ��Ҫ��ת���������ţ�
	uint8_t enter_index;		//��Ű��¡�enter�����룩����ʱ��Ҫ��ת�������ţ�
	uint8_t esc_index;		//��Ű��¡�esc���˳�������ʱ��Ҫ��ת�������ţ�
	void (*operate)(uint8_t);	//����ָ���������ŵ�ǰ����������Ҫִ�еĺ�������ڵ�ַ��
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
void mainWindow(uint8_t press);//������
void menu1(uint8_t press);//�˵��б�
void menu2(uint8_t press);//�˵��б�
void menu3(uint8_t pressd);//�˵��б�
void menu4(uint8_t press);//�˵��б�
void menu5(uint8_t press);//�˵��б�
void menu6(uint8_t press);//�˵��б� 
void menu7(uint8_t press);//�˵��б� 
void menu2_1(uint8_t press);//�˵��б�
void menu2_2(uint8_t press);//�˵��б�
void menu2_3(uint8_t press);//�˵��б� 
void menu2_4(uint8_t press);//�˵��б� 
void menu2_5(uint8_t press);//�˵��б�
void menu2_6(uint8_t press);//�˵��б� 
void menu2_7(uint8_t press);//�˵��б� 


#endif