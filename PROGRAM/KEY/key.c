
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
				//���İ��key2
		
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11)==0 )
		{
			key2_times=0;
			printf("get in task background ���ݰ��°���2��ʱ��ĳ����ж��ǲ��ǳ������� ");
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11)==0 )
			{
				printf("%d\r\n",key2_times);
				osDelay(1);
				key2_times++;
				if(key2_times > 400)
				{
					printf("����2 %d\r\n",key2_times);
					//key_continuous_value = 2;
					//Transient_vibration(); //˲����
					
					return KEY2_LONG_PRESS;
				}
			}
			Transient_vibration(); //˲����
			return KEY2_PRESS;
		} 
	}
	
	if( KEY_1==1 )
		{
			
		
			KEY_1 = 0;
			
		
		
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 1)
		{
			//printf("get in task background ���ݰ��°���1��ʱ��ĳ����ж��ǲ��ǳ������� ");
			key1_times=0;
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 1)
			{
				printf("%d\r\n",key1_times);
				osDelay(1);
				key1_times++;
				if(key1_times > 400)
				{
					printf("����1 %d\r\n",key1_times);
					//key_continuous_value = 1;
					//Transient_vibration(); //˲����;
					
					return KEY1_LONG_PRESS;
				}
			}
			Transient_vibration(); //˲����
			return KEY1_PRESS;
		}
		
		
		//���İ岻��PA11��Ϊkey2 ������ʱע���� ����PB12 ��ʱע��
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
					//printf("����2 %d\r\n",key2_times);
					key_continuous_value = 2;
					//Transient_vibration(); //˲����
					osDelay(50);
					//break;
				}
			}
			
		} 
		*/
	
			
			
			
		}

	
   return 0;		
	
	}


