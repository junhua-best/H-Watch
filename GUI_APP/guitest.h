#ifndef guitest_h
#define guitest_h

#ifdef __cplusplus
 extern "C" {
#endif
//void task_cb(lv_task_t * task);

void lv_gauge_test_start();

//�򵥱���
void lv_gauge_test_start2();
//void set_tim(uint8_t hour,uint8_t min,uint8_t sec);//����ʱ��
void app_digital_clock_meun_re(bool dir);
	
	//ҳ�����
	void lv_page_test_start3();
	//void event_handler(lv_obj_t * obj,lv_event_t event);
	void key_handler();
	
	//SW���ز���
	
	//extern lv_obj_t * sw1;
	//void lv_sw_test_start_sw();
	//void event_handler_sw(lv_obj_t * obj,lv_event_t event);
	
	
#endif