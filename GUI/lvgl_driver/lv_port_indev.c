/**
 * @file lv_port_indev.c
 *
 */

 /*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev.h"
#include "stm32l4xx_it.h"
#include "taskfun.h"
//#include "touch.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void button_init(void);
static bool button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static int8_t button_get_pressed_id(void);
static bool button_is_pressed(uint8_t id);

//static bool touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
lv_indev_t * indev_button;


/**********************
 *  STATIC VARIABLES
 **********************/



/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*******************************************
* @function: 	按键的LVGL indev驱动
* @input:			
* @return:		
* @author:		jovan
* @time:			2021.12.9
*******************************************/
void lv_port_indev_init(void)
{
    lv_indev_drv_t indev_drv;

	  /*------------------
     * Button
     * -----------------*/
    /*Initialize your button if you have*/
    //button_init();

    /*Register a button input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_BUTTON;
    indev_drv.read_cb = button_read;
    indev_button = lv_indev_drv_register(&indev_drv);

    /*Assign buttons to points on the screen*/
    static const lv_point_t btn_points[2] = {
            {0, 210},   /*Button 1 -> x:10; y:10*/
            {210, 210},  /*Button 2 -> x:40; y:100*/
    };
    lv_indev_set_button_points(indev_button, btn_points);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/* Will be called by the library to read the touchpad */
static bool touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
	/*
		static uint16_t last_x = 0;
		static uint16_t last_y = 0;
		if(tp_dev.sta&TP_PRES_DOWN)//����������
		{
			last_x = tp_dev.x[0];
			last_y = tp_dev.y[0];
			data->point.x = last_x;
			data->point.y = last_y;
			data->state = LV_INDEV_STATE_PR;
		}else{
			data->point.x = last_x;
			data->point.y = last_y;
			data->state = LV_INDEV_STATE_REL;
		}  */
    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}

/* Initialize your buttons */
static void button_init(void)
{
    /*Your code comes here*/
}

/* Will be called by the library to read the button */
static bool button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{

    static uint8_t last_btn = 0;

    /*Get the pressed button's ID*/
    int8_t btn_act = button_get_pressed_id();

    if(btn_act >= 0) {
        data->state = LV_INDEV_STATE_PR;
        last_btn = btn_act;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Save the last pressed button's ID*/
    data->btn_id = last_btn;

    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}

/*Get ID  (0, 1, 2 ..) of the pressed button*/
static int8_t button_get_pressed_id(void)
{
    uint8_t i;

    /*Check to buttons see which is being pressed (assume there are 2 buttons)*/
    for(i = 1; i < 3; i++) {
        /*Return the pressed button's ID*/
        if(button_is_pressed(i)) {
            return i;
        }
    }

    /*No button pressed*/
    return -1;
}

/*Test if `id` button is pressed or not*/
static bool button_is_pressed(uint8_t id)
{

    /*判断按键1、2是否按下，并且按键事件已经处理完成*/
		if(KEY_1==1 && id==1 &&key_value_handle_flag==DONE)
			return true;
		else if(KEY_2==1 && id==2 &&key_value_handle_flag==DONE)
			return true;
		else
    return false;
}



#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
