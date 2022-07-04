#include "w25q64.h"


#include "stdio.h"
#include "spi.h"
#include "main.h"



/* 读ID */
uint8_t w25x_read_id = 0x90;					// 读ID
uint8_t check_addr   = 0x05;					// 检查线路是否繁忙
uint8_t enable_write = 0x06;					// 使能了才能改变芯片数据
uint8_t erase_addr   = 0x20;					// 擦除命令 扇区擦除（4K）
uint8_t write_addr   = 0x02;					// 写数据命令 页写（02H）
uint8_t read_addr    = 0x03;					// 读数据命令
uint8_t W25Q64_ID[5] = {0,0,0,0,0};						// 接收缓存

void ReadID(void)
{
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// 使能CS
	HAL_SPI_Transmit(&hspi1, &w25x_read_id, 1, 100);		// 读ID发送指令
	HAL_SPI_Receive(&hspi1, W25Q64_ID, 5, 100);				// 读取ID
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// 失能CS
}

/* 检查是否繁忙 */
void CheckBusy(void)
{
	uint8_t status=1;
	uint32_t timeCount=0;
	do
	{
		timeCount++;
		if(timeCount > 0xEFFFFFFF) //等待超时
		{
			return ;
		}
		
		HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// 使能CS	
		HAL_SPI_Transmit(&hspi1, &check_addr, 1, 100);			// 发送指令
		HAL_SPI_Receive(&hspi1, &status, 1, 100);				// 读取
		HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// 失能CS
		
	}while((status&0x01)==0x01);
}


void FLASH_Read_Data(int address,int data_length, unsigned char*buffer)
{
	uint8_t convert_address[3]    = {0,0,0};					// 测试地址0x000000
	
	convert_address[0] = address>> 16; //测试地址0x123456 //得到12
	convert_address[1] = (address >> 8) - (convert_address[0]<<8); // 0x1234 -0x1200 = 0x34  8388608
	convert_address[2] = address - (convert_address[0]<<16) - (convert_address[1]<<8);//0x123456 - 0x1200 -0x3400 = 0x56
		
	/* 检查是否繁忙 */
	CheckBusy();	
	
	/* 开始读数据 */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// 使能CS
	HAL_SPI_Transmit(&hspi1, &read_addr, 1, 100);			// 读发送指令
	HAL_SPI_Transmit(&hspi1, convert_address, 3, 100);				// 地址 //需要发送转换以后的16进制数据，把16进制数据按照10进制发送
	HAL_SPI_Receive(&hspi1, buffer, data_length, 100);			// 拿到数据
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// 失能CS
}



uint8_t rdata[4096] = {0};					// 读出数据保存的buff
/* 读取数据 

//0x000000// 8 X 1024 X 1024 = 8388608 #10进制 800000#16进制s

8388608 / 256 = 32768 10进制  8000 16进制
8388608 /4096 = 2048  10进制 	 800 16进制
								4096  10进制  1000 16进制
#4096 # 10进制 1000 16进制
*/
unsigned char *ReadData_4096(int address)
{
	uint8_t convert_address[3]    = {0,0,0};					// 测试地址0x000000
	uint8_t *buffer_address;
	buffer_address = rdata;
	convert_address[0] = address>> 16; //测试地址0x123456 //得到12
	convert_address[1] = (address >> 8) - (convert_address[0]<<8); // 0x1234 -0x1200 = 0x34  8388608
	convert_address[2] = address - (convert_address[0]<<16) - (convert_address[1]<<8);//0x123456 - 0x1200 -0x3400 = 0x56
		
	/* 检查是否繁忙 */
	CheckBusy();	
	
	/* 开始读数据 */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// 使能CS
	HAL_SPI_Transmit(&hspi1, &read_addr, 1, 100);			// 读发送指令
	HAL_SPI_Transmit(&hspi1, convert_address, 3, 100);				// 地址 //需要发送转换以后的16进制数据，把16进制数据按照10进制发送
	HAL_SPI_Receive(&hspi1, rdata, 4096, 100);			// 拿到数据
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// 失能CS
	
	return buffer_address ;
}




void Erase_(unsigned char *address_an)
{
	/* 检查是否繁忙 */
	CheckBusy();
	
	/* 写使能 */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// 使能CS
	HAL_SPI_Transmit(&hspi1, &enable_write, 1, 100);		// 发送指令
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// 失能CS
	
	/* 擦除 */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// 使能CS
	HAL_SPI_Transmit(&hspi1, &erase_addr, 1, 100);			// 发送指令
	HAL_SPI_Transmit(&hspi1, address_an, 3, 100);				// 发送地址
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// 失能CS
}



void Write_data_256(int address_an, unsigned char *data_an)
{	
	uint8_t convert_address[3]    = {0,0,0};
	
	/*
		这里将int类型的数据分割成为三个2字节的数组的元素
	*/
	convert_address[0] = address_an>> 16; //测试地址0x123456 //得到12
	convert_address[1] = (address_an >> 8) - (convert_address[0]<<8); // 0x1234 -0x1200 = 0x34  8388608
	convert_address[2] = address_an - (convert_address[0]<<16) - (convert_address[1]<<8);//0x123456 - 0x1200 -0x3400 = 0x56
	
	/* 再次检查是否繁忙 */
	CheckBusy();
	
	/* 写使能 */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// 使能CS
	HAL_SPI_Transmit(&hspi1, &enable_write, 1, 100);		// 发送指令
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// 失能CS

	/* 写数据 */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// 使能CS
	HAL_SPI_Transmit(&hspi1, &write_addr, 1, 100);			// 发送指令
	HAL_SPI_Transmit(&hspi1, convert_address, 3, 100);				// 地址
	HAL_SPI_Transmit(&hspi1, data_an, 256, 100);			// 写入数据

	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// 失能CS
}



void Write_data_(unsigned char *address_an, unsigned char *data_an)
{	
	/* 再次检查是否繁忙 */
	CheckBusy();
	
	/* 写使能 */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// 使能CS
	HAL_SPI_Transmit(&hspi1, &enable_write, 1, 100);		// 发送指令
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// 失能CS

	/* 写数据 */
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);	// 使能CS
	HAL_SPI_Transmit(&hspi1, &write_addr, 1, 100);			// 发送指令
	HAL_SPI_Transmit(&hspi1, address_an, 3, 100);				// 地址
	HAL_SPI_Transmit(&hspi1, data_an, 256, 100);			// 写入数据

	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);	// 失能CS
}





/*
	地址是16进制的，一个扇区是4096个字节，也就是0x1000个字节，所以第一个扇区的首地址是0x000000,
	第二个扇区的首地址是0x001000,第三个是0x002000依次类推。
*/
void WriteData_4096(int address, unsigned char *data_buffer)
{
	int i=0;
	unsigned int j=0;
	int j_sign=0;
	
	unsigned char data_an[256] = {0};
	uint8_t convert_address[3]    = {0,0,0};
	
	/*
		这里将int类型的数据分割成为三个2字节的数组的元素
	*/
	convert_address[0] = address>> 16; //测试地址0x123456 //得到12
	convert_address[1] = (address >> 8) - (convert_address[0]<<8); // 0x1234 -0x1200 = 0x34  8388608
	convert_address[2] = address - (convert_address[0]<<16) - (convert_address[1]<<8);//0x123456 - 0x1200 -0x3400 = 0x56
	
	Erase_(convert_address);
	
	for( i=0x00; i<0x10; i=i+0x01) //循环16次，每次写入256字节数据，共4096个字节数据
	{
		//
		
		for(j=0;j<256;j++)//每次处理256个字节的数据
		{
			data_an[j] = data_buffer[j+j_sign]; 
		}
		j_sign = j_sign + 256; //
		
		//按照顺序写入256字节的数据
		Write_data_(convert_address,data_an);
		
		//每次写入256字节的数据，地址都移动一位
		convert_address[1]++;
	}
}

