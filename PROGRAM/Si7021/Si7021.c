#include "SI7021.h"
//#include "delay.h"
#include "usart.h"
#include "i2c.h"
#include "stdio.h"

//�ṹ�嶨��
_si7021_value si7021;//���ݻ������ṹ��
_si7021_filter si7021_filter;//ƽ��ֵ�˲����ṹ��

//�������壬�˲�������ս������ʹ�ô��ڴ�ӡ
float TEMP_buf,Humi_buf;


#define BufferSize 1					//��д��С(ע�ⲻҪ����̫��)
 
#define FLT_TO_UINT32(m, e)           (((uint32_t)(m) & 0x00FFFFFFU) | (uint32_t)((int32_t)(e) << 24))

 
//�������ƣ�Multiple_read_Si7021
//�������ܣ����ֽڶ�ȡ������
//����������
//�� �� ֵ��
void Multiple_read_Si7021(u8 REG_address, u16 *value)
{
	u8 Si7021_BUF[2]={0};
	HAL_StatusTypeDef status;
	
	/* 
	��1������ΪI2C�������
	��2������Ϊ�ӻ��豸��ַ
	��3������Ϊ�ӻ��Ĵ�����ַ
	��4������Ϊ�ӻ��Ĵ�����ַ����
	��5������Ϊ���͵����ݵ���ʼ��ַ
	��6������Ϊ�������ݵĴ�С
	��7������Ϊ������ʱʱ�� 
	*/
	status =	HAL_I2C_Mem_Read(&hi2c2, SLAVE_ADDR<<1, REG_address, I2C_MEMADD_SIZE_8BIT, Si7021_BUF, 2, HAL_MAX_DELAY);

//	printf("%02x %02x\r\n",Si7021_BUF[0],Si7021_BUF[1]);


	if(status != HAL_OK)
	{
		switch(status)
		{
			case 0x00U: printf("IIC���� \r\n"); break;
			case 0x01U: printf("IIC���� \r\n"); break;
			case 0x02U: printf("IICæµ \r\n"); break;
			case 0x03U: printf("IIC��ʱ \r\n"); break;
		}
	}
	
	
	*value=((Si7021_BUF[0]<<8)+Si7021_BUF[1]);
}

//�������ƣ�measure_si7021
//�������ܣ�NO HOLD MASTERģʽ�¶�ȡ��ʪ�� 
//������������
//�� �� ֵ����
void measure_Si7021(void)
{
	//�����������
	u16 TEMP,HUMI;

	//��ȡ�¶�
	Multiple_read_Si7021(TEMP_HOLD_MASTER,&TEMP);
	TEMP_buf=(((((float)TEMP)*175.72f)/65536.0f) - 46.85f);
	
	//��ȡʪ��
	Multiple_read_Si7021(HUMI_HOLD_MASTER,&HUMI);
	Humi_buf=(((((float)HUMI)*125.0f)/65535.0f) - 6.0f);
}




