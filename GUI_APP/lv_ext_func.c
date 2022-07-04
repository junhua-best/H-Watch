#include "DisplayPrivate.h"



/**
  * @brief  ��ȫɾ�����󣬵�����ΪNULL��ʱ��ִ��ɾ��
  * @param  obj:���������ַ
  * @retval true �ɹ�; false ʧ��
  */
bool lv_obj_del_safe(lv_obj_t** obj)
{
    bool ret = false;
    if(*obj != NULL)
    {
        lv_obj_del(*obj);
        *obj = NULL;
        ret = true;
    }
    return ret;
}

/**
  * @brief  ��label��׷���ַ���
  * @param  label:��׷�ӵĶ���
  * @param  text:׷�ӵ��ַ���
  * @retval ��
  */
void lv_label_set_text_add(lv_obj_t * label, const char * text)
{
    if(!label)
        return;
    
    lv_label_ins_text(label, strlen(lv_label_get_text(label)), text);
}

/**
  * @brief  ��ȡ�����X��������
  * @param  obj:�����ַ
  * @retval X��������
  */
lv_coord_t lv_obj_get_x_center(lv_obj_t * obj)
{
    return (obj->coords.x2 + obj->coords.x1) / 2; 
    //return (lv_obj_get_x(obj) + lv_obj_get_width(obj) / 2);
}

/**
  * @brief  ��ȡ�����Y��������
  * @param  obj:�����ַ
  * @retval Y��������
  */
lv_coord_t lv_obj_get_y_center(lv_obj_t * obj)
{
    return (obj->coords.y2 + obj->coords.y1) / 2; 
    //return (lv_obj_get_y(obj) + lv_obj_get_height(obj) / 2);
}

/**
  * @brief  ���ö������ɫ(�������ã����ܻ�Ӱ�쵽Ĭ�ϵ�style)
  * @param  obj:�����ַ
  * @param  color:��ɫ
  * @retval ��
  */
void lv_obj_set_color(lv_obj_t * obj, lv_color_t color)
{
    lv_style_t * style = (lv_style_t*)lv_obj_get_style(obj);
    style->body.grad_color = style->body.main_color = color;
    lv_obj_set_style(obj, style);
}

/**
  * @brief  ����table��������г�Ա�Ķ��뷽ʽ
  * @param  table:�����ַ
  * @param  align:���䷽ʽ
  * @retval ��
  */
void lv_table_set_align(lv_obj_t * table, lv_label_align_t align)
{
    uint16_t col = lv_table_get_col_cnt(table);
    uint16_t row = lv_table_get_row_cnt(table);
    for(uint16_t i = 0; i < col; i++)
    {
        for(uint16_t j = 0; j < row; j++)
        {
            lv_table_set_cell_align(table, j, i, align);
        }
    }
}

/**
  * @brief  ��ȡwin����ı���label
  * @param  win:�����ַ
  * @retval label��ַ
  */
lv_obj_t * lv_win_get_label(lv_obj_t * win)
{
    lv_win_ext_t * ext = (lv_win_ext_t*)lv_obj_get_ext_attr(win);
    return ext->title;
}

/**
  * @brief  Ϊ������Ӷ���
  * @param  obj:�����ַ
  * @param  a:������������ַ
  * @param  exec_cb:���ƶ������Եĺ�����ַ
  * @param  start:�����Ŀ�ʼֵ
  * @param  end:�����Ľ���ֵ
  * @param  time:������ִ��ʱ��
  * @param  ready_cb:���������¼��ص�
  * @param  path_cb:��������
  * @retval ��
  */
void lv_obj_add_anim(
    lv_obj_t * obj, lv_anim_t * a,
    lv_anim_exec_xcb_t exec_cb, 
    int32_t start, int32_t end,    
    uint16_t time,
    lv_anim_ready_cb_t ready_cb,
    lv_anim_path_cb_t path_cb
)
{
    lv_anim_t a_tmp;
    if(a == NULL)
    {
        lv_anim_init(&a_tmp);
        a = &a_tmp;
    }

    a->var = obj;
    a->start = start;
    a->end = end;
    a->exec_cb = exec_cb;
    a->path_cb = lv_anim_path_ease_out;
    a->ready_cb = NULL;
    a->time = 200;
    lv_anim_create(a);
}


void obj_add_anim(lv_obj_t *obj, lv_anim_exec_xcb_t exec_cb, uint16_t time, lv_coord_t start, lv_coord_t end, lv_anim_path_cb_t path_cb)
{
	lv_anim_t a;

	lv_anim_init(&a);
	//lv_anim_set_var(&a, obj); //��������
  //a.var     = obj;
	lv_anim_set_exec_cb(&a,obj, (lv_anim_exec_xcb_t)exec_cb); //��������
	lv_anim_set_time(&a, time,NULL);

	a.start = start;		  //��ʼֵ
	a.end = end; //����ֵ
	a.path_cb = path_cb;	  //�������㷽��
	a.playback = 0;				  //�ط�ʱ����Ϊ0��ִ�ж����ط�

	lv_anim_create(&a); //��������
}

/*
uint16_t time =200,
    lv_anim_ready_cb_t ready_cb = NULL,
    lv_anim_path_cb_t path_cb = lv_anim_path_ease_out*/