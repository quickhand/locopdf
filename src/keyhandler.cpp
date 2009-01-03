#include <Evas.h>
#include <Ecore_Evas.h>
#include "keyhandler.h"
#include <iostream>
#include <cstring>

int nav_mode=0;

int get_nav_mode()
{
    return nav_mode;    
}

void set_nav_mode(int mode)
{
    nav_mode=mode;    
}

/* FIXME: HACK */
static void _key_handler(void *data, Evas *evas, Evas_Object *obj, void *event_info)
{
    Evas_Event_Key_Up* e = (Evas_Event_Key_Up*)event_info;
    key_handler_info_t* handler_info = (key_handler_info_t*)data;

    const char* k = e->keyname;

#define HANDLE_ITEM(h, params, lp) { if(handler_info->h) (*handler_info->h)(evas,obj,params, lp);}
#define HANDLE_KEY(h) {if(handler_info->h) (*handler_info->h)(evas,obj);}
    if(!strcmp(k, "Return")) {
        if(nav_mode == 1)            HANDLE_KEY(nav_sel_handler)
        else                         HANDLE_KEY(ok_handler)
    }
    else if(!strcmp(k, "Escape"))    HANDLE_KEY(esc_handler)
    else if (isdigit(k[0]) && !k[1]) {
		bool lp = false;
		//if(e->modifiers & EWL_KEY_MODIFIER_ALT)
		//	lp = true;
		HANDLE_ITEM(item_handler, k[0]-'0', lp)
	}
    else if (!strcmp(k,"Up")) {
        if(nav_mode == 1)            HANDLE_KEY(nav_up_handler)
        else                         HANDLE_KEY(nav_right_handler)
    }
    else if (!strcmp(k, "Down")) {
        if(nav_mode == 1)            HANDLE_KEY(nav_down_handler)
        else                         HANDLE_KEY(nav_left_handler)
    }
    else if (!strcmp(k, "Left"))     HANDLE_KEY(nav_left_handler)
    else if (!strcmp(k, "Right"))    HANDLE_KEY(nav_right_handler)
    else if (!strcmp(k, "F2"))       HANDLE_KEY(nav_menubtn_handler)
    else if (!strcmp(k, "+"))        HANDLE_KEY(shift_handler)
}

void set_key_handler(Evas_Object* obj, key_handler_info_t* handler_info)
{
    evas_object_event_callback_add(obj,EVAS_CALLBACK_KEY_UP,_key_handler,handler_info);
}

