#ifndef CHOICEBOX_H
#define CHOICEBOX_H

#include <Evas.h>

typedef void (*choice_handler)(Evas *e, Evas_Object *obj,int choice, bool lp);
Evas_Object *init_choicebox(Evas *evas,const char *choices[], const char *values[], int numchoices,choice_handler handler, char *header, Evas_Object *parent, bool master = false);
void fini_choicebox(Evas *e, Evas_Object *obj, bool redraw = true);
void update_label(Evas *e, Evas_Object *obj, int number, const char *value);
Evas_Object *choicebox_get_parent(Evas *e, Evas_Object *obj);
void update_choicebox(Evas *e, Evas_Object *obj, const char *choicelist[], const char *values[], int numchoices, bool rewind = false);
char *get_theme_file();

#endif
