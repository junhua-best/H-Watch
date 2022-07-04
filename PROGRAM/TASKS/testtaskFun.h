#ifndef testtaskFun_h
#define testtaskFun_h



#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stdio.h"


#include "lcd_init.h"
#include "lcd.h"
#include "getword.h"
//#include "pic.h"
//#include "picmain.h"


#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

#include "hmc5883l.h"

#include "SI7021.h"

#include "w25q64.h"


#include "taskfun.h"


#include "string.h"
#include "ymodel.h"




//≤‚ ‘–¥»Î◊÷ø‚
void fun_1(void);
void fun_2(void);
void test_init_task(void);


void Interface_main_write(void);
#endif


