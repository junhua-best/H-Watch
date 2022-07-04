#ifndef w25q64_h
#define w25q64_h

#include "main.h"


extern uint8_t w25x_read_id ;					// ��ID
extern uint8_t m_addr[4]    ;					// ���Ե�ַ0x000000
extern uint8_t check_addr   ;					// �����·�Ƿ�æ
extern uint8_t enable_write ;					// ʹ���˲��ܸı�оƬ����
extern uint8_t erase_addr   ;					// �������� ����������4K��
extern uint8_t write_addr   ;					// д��������
extern uint8_t read_addr    ;					// ����������


/* ����Ƿ�æ */
void CheckBusy(void);

/* ��ID */

extern uint8_t W25Q64_ID[5];						// ���ջ���

void ReadID(void);

void FLASH_Read_Data(int address,int data_length, unsigned char*buffer);
	
extern uint8_t rdata[4096];					// �������ݱ����buff
/* д������ */
unsigned char* ReadData_4096(int address);


void Write_data_256(int address_an, unsigned char *data_an);

/* ��ȡ���� */
void WriteData_4096(int address, unsigned char *data_buffer);

#endif

