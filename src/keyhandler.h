#ifndef KEYHANDLER_H
#define KEYHANDLER_H

typedef void (*key_handler_t)(Evas *e, Evas_Object *obj);
typedef void (*item_handler_t)(Evas *e, Evas_Object *obj,int index, bool lp);

typedef struct {
    key_handler_t ok_handler;
    key_handler_t esc_handler;
    key_handler_t nav_up_handler;
    key_handler_t nav_down_handler;
    key_handler_t nav_left_handler;
    key_handler_t nav_right_handler;
    key_handler_t nav_sel_handler;
    key_handler_t nav_menubtn_handler;
    key_handler_t plus_handler;
    key_handler_t minus_handler;
    item_handler_t item_handler;
} key_handler_info_t;

/* FIXME: HACK */
static void _key_handler(void *data, Evas *e, Evas_Object *obj, void *event_info);
void set_key_handler(Evas_Object* obj, key_handler_info_t* handler_info);
int get_nav_mode();
void set_nav_mode(int mode);

#endif
