
#include "getword.h"





void LCD_Show_Chinese_word_32size(uint16_t x,uint16_t y,uint16_t fc,unsigned char*an)
{
	uint8_t i,j;
	uint16_t x0=x;
	
	//LCD_Address_Set(x,y,x+31,y+31);//����Ҫд�����ݵ�����
	
	for(i=0;i<128;i++) //ÿ��32��С�ĺ���Ϊ128���ֽڣ�ѭ��128�ֽ�
	{
		for(j=0;j<8;j++) //ÿ���ֽ������ݣ��Ƚ�ÿһλ�������Ƿ���1 ����1�����ø����ص����ɫ
		{	
			if(an[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//��һ����
			x++;
			if((x-x0)==32)
			{
				x=x0;
				y++;
				break;
			}
		}
	}
}


void LCD_Show_Chinese_word_24size(uint16_t x,uint16_t y,uint16_t fc,unsigned char*an)
{
	uint8_t i,j;
	uint16_t x0=x;
	
	//LCD_Address_Set(x,y,x+23,y+23);//����Ҫд�����ݵ�����
	
	for(i=0;i<72;i++) //ÿ��32��С�ĺ���Ϊ128���ֽڣ�ѭ��128�ֽ�
	{
		for(j=0;j<8;j++) //ÿ���ֽ������ݣ��Ƚ�ÿһλ�������Ƿ���1 ����1�����ø����ص����ɫ
		{	
			if(an[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//��һ����
			x++;
			if((x-x0)==24)
			{
				x=x0;
				y++;
				break;
			}
		}
	}
}


void LCD_show_one_chinese(uint16_t x,uint16_t y,uint16_t color,uint8_t *string, unsigned char size)
{
	long int word_address = 0x00;
	
	unsigned char word_data[128] = {0};
	
	
	//��W26Q64��Ƭѡ�źţ��ر���Ļ��Ƭѡ�źš�
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
		
	/*
	address=((H-0xA1)��94+L-0XA1)��(size);
	����H��L �ֱ����GB2312 �ĵ�һ���ֽں͵ڶ����ֽ�(Ҳ���Ǹ�λ�͵�λ)��
	���硰�á���
	16����Ϊ0xba(186) 0xc3(195),
	ʹ��16X16���壬��������32���ֽڣ�
	ʹ��32X32���壬��������128���ֽڣ�
	����ʹ��16��С�������㡣
	0xA1��10����Ϊ161
	94�ǹ̶�ֵ
	[(186-161)*94 +195 - 161]* 32 = 76288 �õ�0x12a00 
	λ����0x12a00 76288 
	*/
	if(size == 32)
	{
		word_address = ( (string[0]-0xa1)*94 + string[1] - 0xa1)*128;
		
		FLASH_Read_Data(word_address,128,word_data);
		

		//����Ļ��Ƭѡ�źţ��ر�W26Q64��Ƭѡ�źš�
		HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
		
		LCD_Show_Chinese_word_32size(x,y,color,word_data);
	}
	if(size == 24)
	{
		word_address = GB2312_24X24_heiti + ( (string[0]-0xa1)*94 + string[1] - 0xa1)*72;
		
		FLASH_Read_Data(word_address,72,word_data);
		

		//����Ļ��Ƭѡ�źţ��ر�W26Q64��Ƭѡ�źš�
		HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
		
		LCD_Show_Chinese_word_24size(x,y,color,word_data);
	}
	
	
}






void LCD_show_Chinese(uint16_t x, uint16_t y, uint16_t color, uint8_t *string, uint8_t length ,unsigned char size)
{
	uint8_t an[2] = {0};
	uint16_t i=0;

	for(i=0; i < length*2; i=i+2)
	{
		an[0]= string[i];
		an[1]= string[i+1];
		LCD_show_one_chinese(x, y, color, an, size);
		if(size == 32)
			x = x + 32;
		if(size == 24)
			x = x + 20;
	}
}










