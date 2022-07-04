#ifndef __WATCHPAGE_H
#define __WATCHPAGE_H

#include "taskfun.h"
extern char time_now[6];//小时和分钟
extern char date_now[6] ; //日期
extern char second_now[3];
extern char bat_now[3];
extern char temperature_now[4];
extern char humidity_now[4];
extern char step_now[5];
void get_num(int i);
void app_digital_clock_create(void);


#endif