#ifndef getword_h
#define getword_h

#include "main.h"
#include "spi.h"
#include "lcd_init.h"
#include "lcd.h"
#include "w25q64.h"
#include "ymodel.h"

void LCD_Show_Chinese_word_32size(uint16_t x,uint16_t y,uint16_t fc,unsigned char*an);

void LCD_Show_Chinese_word_24size(uint16_t x,uint16_t y,uint16_t fc,unsigned char*an);

void LCD_show_one_chinese(uint16_t x,uint16_t y,uint16_t color,uint8_t *string, unsigned char size);

void LCD_show_Chinese(uint16_t x, uint16_t y, uint16_t color, uint8_t *string, uint8_t length, unsigned char size);

#endif



