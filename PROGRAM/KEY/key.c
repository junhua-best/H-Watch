
#include "key.h"
#include "freertos.h"
#include "stdio.h"


uint8_t button_scan(void)
{
	
	int i=0;
	int key1_times=0;
	int key2_times=0;
	
	if(KEY_2==1  )
	{
			KEY_2 = 0;
				//核心板的key2
		
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11)==0 )
		{
			key2_times=0;
			printf("get in task background 根据按下按键2的时间的长短判断是不是长按操作 ");
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11)==0 )
			{
				printf("%d\r\n",key2_times);
				osDelay(1);
				key2_times++;
				if(key2_times > 400)
				{
					printf("长按2 %d\r\n",key2_times);
					//key_continuous_value = 2;
					//Transient_vibration(); //瞬变振动
					
					return KEY2_LONG_PRESS;
				}
			}
			Transient_vibration(); //瞬变振动
			return KEY2_PRESS;
		} 
	}
	
	if( KEY_1==1 )
		{
			
		
			KEY_1 = 0;
			
		
		
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 1)
		{
			//printf("get in task background 根据按下按键1的时间的长短判断是不是长按操作 ");
			key1_times=0;
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 1)
			{
				printf("%d\r\n",key1_times);
				osDelay(1);
				key1_times++;
				if(key1_times > 400)
				{
					printf("长按1 %d\r\n",key1_times);
					//key_continuous_value = 1;
					//Transient_vibration(); //瞬变振动;
					
					return KEY1_LONG_PRESS;
				}
			}
			Transient_vibration(); //瞬变振动
			return KEY1_PRESS;
		}
		
		
		//核心板不用PA11作为key2 所以暂时注释这 换成PB12 暂时注释
		/*
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11) == 0)
		{
			
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11) == 0)
			{
				printf("%d\r\n",key2_times);
				osDelay(1);
				key2_times++;
				if(key2_times > 400)
				{
					//printf("长按2 %d\r\n",key2_times);
					key_continuous_value = 2;
					//Transient_vibration(); //瞬变振动
					osDelay(50);
					//break;
				}
			}
			
		} 
		*/
	
			
			
			
		}

	
   return 0;		
	
	}


