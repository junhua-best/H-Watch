#ifndef ymodel_h
#define ymodel_h


#include "main.h"
#include "usart.h"
#include "w25q64.h"
#include "stdio.h"

#define	SOH	0x01
#define STX	0x02
#define ACK	0x06
#define NAK	0x15
#define EOT	0x04
#define C1	0x43

#define GB2312_32X32_songti 0x000000 
#define GB2312_24X24_heiti  0x200000

//主界面上显示时间的数字图片的地址
#define Pictures_address_NUM0  0x300000
#define Pictures_address_NUM1  0x301000
#define Pictures_address_NUM2  0x302000
#define Pictures_address_NUM3  0x303000
#define Pictures_address_NUM4  0x304000
#define Pictures_address_NUM5  0x305000
#define Pictures_address_NUM6  0x306000
#define Pictures_address_NUM7  0x307000
#define Pictures_address_NUM8  0x308000
#define Pictures_address_NUM9  0x309000

//保存其他数据的地址 包括番茄钟的时间数据
//包括设置数据
#define Data_address 0x400000//


//保存太空人的第1张图片
#define Space_Picture_addressA1 0x500000
#define Space_Picture_addressA2 0x501000
#define Space_Picture_addressA3 0x502000
#define Space_Picture_addressA4 0x503000
#define Space_Picture_addressA5 0x504000
//保存太空人的第2张图片
#define Space_Picture_addressB1 0x505000
#define Space_Picture_addressB2 0x506000
#define Space_Picture_addressB3 0x507000
#define Space_Picture_addressB4 0x508000
#define Space_Picture_addressB5 0x509000
//保存太空人的第3张图片
#define Space_Picture_addressC1 0x50a000
#define Space_Picture_addressC2 0x50b000
#define Space_Picture_addressC3 0x50c000
#define Space_Picture_addressC4 0x50d000
#define Space_Picture_addressC5 0x50e000
//保存太空人的第4张图片
#define Space_Picture_addressD1 0x510000
#define Space_Picture_addressD2 0x511000
#define Space_Picture_addressD3 0x512000
#define Space_Picture_addressD4 0x513000
#define Space_Picture_addressD5 0x514000
//保存太空人的第5张图片
#define Space_Picture_addressE1 0x515000
#define Space_Picture_addressE2 0x516000
#define Space_Picture_addressE3 0x517000
#define Space_Picture_addressE4 0x518000
#define Space_Picture_addressE5 0x519000
//保存太空人的第6张图片
#define Space_Picture_addressF1 0x51a000
#define Space_Picture_addressF2 0x51b000
#define Space_Picture_addressF3 0x51c000
#define Space_Picture_addressF4 0x51d000
#define Space_Picture_addressF5 0x51e000
//保存太空人的第7张图片
#define Space_Picture_addressG1 0x520000
#define Space_Picture_addressG2 0x521000
#define Space_Picture_addressG3 0x522000
#define Space_Picture_addressG4 0x523000
#define Space_Picture_addressG5 0x524000
//保存太空人的第8张图片
#define Space_Picture_addressH1 0x525000
#define Space_Picture_addressH2 0x526000
#define Space_Picture_addressH3 0x527000
#define Space_Picture_addressH4 0x528000
#define Space_Picture_addressH5 0x529000
//保存太空人的第9张图片
#define Space_Picture_addressI1 0x52a000
#define Space_Picture_addressI2 0x52b000
#define Space_Picture_addressI3 0x52c000
#define Space_Picture_addressI4 0x52d000
#define Space_Picture_addressI5 0x52e000
//保存太空人的第10张图片
#define Space_Picture_addressJ1 0x530000
#define Space_Picture_addressJ2 0x531000
#define Space_Picture_addressJ3 0x532000
#define Space_Picture_addressJ4 0x533000
#define Space_Picture_addressJ5 0x534000
//保存太空人的第11张图片
#define Space_Picture_addressK1 0x535000
#define Space_Picture_addressK2 0x536000
#define Space_Picture_addressK3 0x537000
#define Space_Picture_addressK4 0x538000
#define Space_Picture_addressK5 0x539000
//保存太空人的第12张图片
#define Space_Picture_addressL1 0x53a000
#define Space_Picture_addressL2 0x53b000
#define Space_Picture_addressL3 0x53c000
#define Space_Picture_addressL4 0x53d000
#define Space_Picture_addressL5 0x53e000

//一个扇区4k --》0x1000
//flowers图片大小 115200 112k  需要28个扇区
#define flowers_address 0x540000
#define flowers_address_end 0x55c000



//最后一个区域的地址
#define other_address 0x80000000
/*******************************************
* @function: 	执行Ymodel协议的运行主要流程 ，同时进行数据处理，边接收数据边处理数据
* @input:			无
* @return:		无
* @author:		田小花
* @time:			2021.4.30
*******************************************/
void Ymodel_process_fun(void);


#endif


