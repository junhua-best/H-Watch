#ifndef w25q64_h
#define w25q64_h

#include "main.h"


extern uint8_t w25x_read_id ;					// 读ID
extern uint8_t m_addr[4]    ;					// 测试地址0x000000
extern uint8_t check_addr   ;					// 检查线路是否繁忙
extern uint8_t enable_write ;					// 使能了才能改变芯片数据
extern uint8_t erase_addr   ;					// 擦除命令 扇区擦除（4K）
extern uint8_t write_addr   ;					// 写数据命令
extern uint8_t read_addr    ;					// 读数据命令


/* 检查是否繁忙 */
void CheckBusy(void);

/* 读ID */

extern uint8_t W25Q64_ID[5];						// 接收缓存

void ReadID(void);

void FLASH_Read_Data(int address,int data_length, unsigned char*buffer);
	
extern uint8_t rdata[4096];					// 读出数据保存的buff
/* 写入数据 */
unsigned char* ReadData_4096(int address);


void Write_data_256(int address_an, unsigned char *data_an);

/* 读取数据 */
void WriteData_4096(int address, unsigned char *data_buffer);

#endif

