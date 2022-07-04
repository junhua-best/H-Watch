#include "w25q64.h"


#include "stdio.h"
#include "spi.h"
#include "main.h"



/* ��ID */
uint8_t w25x_read_id = 0x90;					// ��ID
uint8_t check_addr   = 0x05;					// �����·�Ƿ�æ
uint8_t enable_write = 0x06;					// ʹ���˲��ܸı�оƬ����
uint8_t erase_addr   = 0x20;					// �������� ����������4K��
uint8_t write_addr   = 0x02;					// д�������� ҳд��02H��
uint8_t read_addr    = 0x03;					// ����������
uint8_t W25Q64_ID[5] = {0,0,0,0,0};						// ���ջ���

void ReadID(void)
{
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// ʹ��CS
	HAL_SPI_Transmit(&hspi1, &w25x_read_id, 1, 100);		// ��ID����ָ��
	HAL_SPI_Receive(&hspi1, W25Q64_ID, 5, 100);				// ��ȡID
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// ʧ��CS
}

/* ����Ƿ�æ */
void CheckBusy(void)
{
	uint8_t status=1;
	uint32_t timeCount=0;
	do
	{
		timeCount++;
		if(timeCount > 0xEFFFFFFF) //�ȴ���ʱ
		{
			return ;
		}
		
		HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// ʹ��CS	
		HAL_SPI_Transmit(&hspi1, &check_addr, 1, 100);			// ����ָ��
		HAL_SPI_Receive(&hspi1, &status, 1, 100);				// ��ȡ
		HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// ʧ��CS
		
	}while((status&0x01)==0x01);
}


void FLASH_Read_Data(int address,int data_length, unsigned char*buffer)
{
	uint8_t convert_address[3]    = {0,0,0};					// ���Ե�ַ0x000000
	
	convert_address[0] = address>> 16; //���Ե�ַ0x123456 //�õ�12
	convert_address[1] = (address >> 8) - (convert_address[0]<<8); // 0x1234 -0x1200 = 0x34  8388608
	convert_address[2] = address - (convert_address[0]<<16) - (convert_address[1]<<8);//0x123456 - 0x1200 -0x3400 = 0x56
		
	/* ����Ƿ�æ */
	CheckBusy();	
	
	/* ��ʼ������ */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// ʹ��CS
	HAL_SPI_Transmit(&hspi1, &read_addr, 1, 100);			// ������ָ��
	HAL_SPI_Transmit(&hspi1, convert_address, 3, 100);				// ��ַ //��Ҫ����ת���Ժ��16�������ݣ���16�������ݰ���10���Ʒ���
	HAL_SPI_Receive(&hspi1, buffer, data_length, 100);			// �õ�����
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// ʧ��CS
}



uint8_t rdata[4096] = {0};					// �������ݱ����buff
/* ��ȡ���� 

//0x000000// 8 X 1024 X 1024 = 8388608 #10���� 800000#16����s

8388608 / 256 = 32768 10����  8000 16����
8388608 /4096 = 2048  10���� 	 800 16����
								4096  10����  1000 16����
#4096 # 10���� 1000 16����
*/
unsigned char *ReadData_4096(int address)
{
	uint8_t convert_address[3]    = {0,0,0};					// ���Ե�ַ0x000000
	uint8_t *buffer_address;
	buffer_address = rdata;
	convert_address[0] = address>> 16; //���Ե�ַ0x123456 //�õ�12
	convert_address[1] = (address >> 8) - (convert_address[0]<<8); // 0x1234 -0x1200 = 0x34  8388608
	convert_address[2] = address - (convert_address[0]<<16) - (convert_address[1]<<8);//0x123456 - 0x1200 -0x3400 = 0x56
		
	/* ����Ƿ�æ */
	CheckBusy();	
	
	/* ��ʼ������ */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// ʹ��CS
	HAL_SPI_Transmit(&hspi1, &read_addr, 1, 100);			// ������ָ��
	HAL_SPI_Transmit(&hspi1, convert_address, 3, 100);				// ��ַ //��Ҫ����ת���Ժ��16�������ݣ���16�������ݰ���10���Ʒ���
	HAL_SPI_Receive(&hspi1, rdata, 4096, 100);			// �õ�����
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// ʧ��CS
	
	return buffer_address ;
}




void Erase_(unsigned char *address_an)
{
	/* ����Ƿ�æ */
	CheckBusy();
	
	/* дʹ�� */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// ʹ��CS
	HAL_SPI_Transmit(&hspi1, &enable_write, 1, 100);		// ����ָ��
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// ʧ��CS
	
	/* ���� */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// ʹ��CS
	HAL_SPI_Transmit(&hspi1, &erase_addr, 1, 100);			// ����ָ��
	HAL_SPI_Transmit(&hspi1, address_an, 3, 100);				// ���͵�ַ
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// ʧ��CS
}



void Write_data_256(int address_an, unsigned char *data_an)
{	
	uint8_t convert_address[3]    = {0,0,0};
	
	/*
		���ｫint���͵����ݷָ��Ϊ����2�ֽڵ������Ԫ��
	*/
	convert_address[0] = address_an>> 16; //���Ե�ַ0x123456 //�õ�12
	convert_address[1] = (address_an >> 8) - (convert_address[0]<<8); // 0x1234 -0x1200 = 0x34  8388608
	convert_address[2] = address_an - (convert_address[0]<<16) - (convert_address[1]<<8);//0x123456 - 0x1200 -0x3400 = 0x56
	
	/* �ٴμ���Ƿ�æ */
	CheckBusy();
	
	/* дʹ�� */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// ʹ��CS
	HAL_SPI_Transmit(&hspi1, &enable_write, 1, 100);		// ����ָ��
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// ʧ��CS

	/* д���� */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// ʹ��CS
	HAL_SPI_Transmit(&hspi1, &write_addr, 1, 100);			// ����ָ��
	HAL_SPI_Transmit(&hspi1, convert_address, 3, 100);				// ��ַ
	HAL_SPI_Transmit(&hspi1, data_an, 256, 100);			// д������

	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// ʧ��CS
}



void Write_data_(unsigned char *address_an, unsigned char *data_an)
{	
	/* �ٴμ���Ƿ�æ */
	CheckBusy();
	
	/* дʹ�� */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// ʹ��CS
	HAL_SPI_Transmit(&hspi1, &enable_write, 1, 100);		// ����ָ��
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// ʧ��CS

	/* д���� */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// ʹ��CS
	HAL_SPI_Transmit(&hspi1, &write_addr, 1, 100);			// ����ָ��
	HAL_SPI_Transmit(&hspi1, address_an, 3, 100);				// ��ַ
	HAL_SPI_Transmit(&hspi1, data_an, 256, 100);			// д������

	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// ʧ��CS
}





/*
	��ַ��16���Ƶģ�һ��������4096���ֽڣ�Ҳ����0x1000���ֽڣ����Ե�һ���������׵�ַ��0x000000,
	�ڶ����������׵�ַ��0x001000,��������0x002000�������ơ�
*/
void WriteData_4096(int address, unsigned char *data_buffer)
{
	int i=0;
	unsigned int j=0;
	int j_sign=0;
	
	unsigned char data_an[256] = {0};
	uint8_t convert_address[3]    = {0,0,0};
	
	/*
		���ｫint���͵����ݷָ��Ϊ����2�ֽڵ������Ԫ��
	*/
	convert_address[0] = address>> 16; //���Ե�ַ0x123456 //�õ�12
	convert_address[1] = (address >> 8) - (convert_address[0]<<8); // 0x1234 -0x1200 = 0x34  8388608
	convert_address[2] = address - (convert_address[0]<<16) - (convert_address[1]<<8);//0x123456 - 0x1200 -0x3400 = 0x56
	
	Erase_(convert_address);
	
	for( i=0x00; i<0x10; i=i+0x01) //ѭ��16�Σ�ÿ��д��256�ֽ����ݣ���4096���ֽ�����
	{
		//
		
		for(j=0;j<256;j++)//ÿ�δ���256���ֽڵ�����
		{
			data_an[j] = data_buffer[j+j_sign]; 
		}
		j_sign = j_sign + 256; //
		
		//����˳��д��256�ֽڵ�����
		Write_data_(convert_address,data_an);
		
		//ÿ��д��256�ֽڵ����ݣ���ַ���ƶ�һλ
		convert_address[1]++;
	}
}

