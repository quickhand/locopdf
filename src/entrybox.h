#ifndef ENTRYBOX_H
#define ENTRYBOX_H

#include <Evas.h>

typedef void (*entry_handler)(Evas *e, Evas_Object *obj,char *value);
Evas_Object *init_entrybox(Evas *evas,const char* title,const char *defaultentry,int maxlength,entry_handler handler, Evas_Object *parent);
void fini_entrybox(Evas *e, Evas_Object *obj);
Evas_Object *entrybox_get_parent(Evas *e, Evas_Object *obj);
char *get_theme_file();

#endif
