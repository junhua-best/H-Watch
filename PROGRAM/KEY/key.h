
#ifndef _KEY_H_
#define _KEY_H_

#include "stdint.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "stm32l4xx_it.h"
#define KEY1_PRESS  1
#define KEY2_PRESS  2
#define KEY1_LONG_PRESS  3
#define KEY2_LONG_PRESS  4


uint8_t button_scan(void);
#endif