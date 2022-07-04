#ifndef __LCD_INIT_H
#define __LCD_INIT_H


#include "main.h"

#define USE_HORIZONTAL 1  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#define LCD_W 240
#define LCD_H 240



//-----------------LCD端口定义---------------- 

//#define LCD_SCLK_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_12)//SCL=SCLK
//#define LCD_SCLK_Set() GPIO_SetBits(GPIOG,GPIO_Pin_12)

//#define LCD_MOSI_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_5)//SDA=MOSI
//#define LCD_MOSI_Set() GPIO_SetBits(GPIOD,GPIO_Pin_5)

//#define LCD_RES_Clr()  GPIO_ResetBits(GPIOD,GPIO_Pin_4)//RES
//#define LCD_RES_Set()  GPIO_SetBits(GPIOD,GPIO_Pin_4)

//#define LCD_DC_Clr()   GPIO_ResetBits(GPIOD,GPIO_Pin_15)//DC
//#define LCD_DC_Set()   GPIO_SetBits(GPIOD,GPIO_Pin_15)

//#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOD,GPIO_Pin_1)//BLK
//#define LCD_BLK_Set()  GPIO_SetBits(GPIOD,GPIO_Pin_1)


#define LCD_RES_Clr() HAL_GPIO_WritePin(LCD_RES_GPIO_Port,LCD_RES_Pin,GPIO_PIN_RESET)//RES
#define LCD_RES_Set()  HAL_GPIO_WritePin(LCD_RES_GPIO_Port,LCD_RES_Pin,GPIO_PIN_SET)

#define LCD_DC_Clr()  HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_RESET)//DC 
#define LCD_DC_Set()   HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_SET) 
 		     


#define LCD_BLK_Clr()   HAL_GPIO_WritePin(LCD_BLK_GPIO_Port,LCD_BLK_Pin,GPIO_PIN_RESET)//BLK
#define LCD_BLK_Set()    HAL_GPIO_WritePin(LCD_BLK_GPIO_Port,LCD_BLK_Pin,GPIO_PIN_SET)


void LCD_Writ_Bus(uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);//写入一个字节
void LCD_WR_DATA(uint16_t dat);//写入两个字节
void LCD_WR_REG(uint8_t dat);//写入一个指令
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




