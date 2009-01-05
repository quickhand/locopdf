/***************************************************************************
 *   Copyright (C) 2008 by Marc Lajoie                                     *
 *   quickhand@openinkpot.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include <cstring>


#include <Evas.h>
#include <Ecore_Evas.h>
#include <Edje.h>
#include "keyhandler.h"
#include "entrybox.h"
#include "locopdf.h"




typedef struct _entry_info_struct {
	char *entry;
    int curchar;
    int maxchar;
	entry_handler handler;
	Evas_Object *parent;
} entry_info_struct;



void entrybox_ok(Evas *e, Evas_Object *obj)
{
    entry_info_struct *infostruct=(entry_info_struct *) evas_object_data_get(obj,"entry_info");
    evas_object_focus_set(infostruct->parent,1);
	fini_entrybox(e,obj);
    (infostruct->handler)(e,obj,infostruct->entry);
}

void entrybox_esc(Evas *e, Evas_Object *obj)
{
    entry_info_struct *infostruct=(entry_info_struct *) evas_object_data_get(obj,"entry_info");
    if(infostruct->curchar>0)
    {
        infostruct->curchar--;
        infostruct->entry[infostruct->curchar]='\0';
        edje_object_part_text_set(obj,"dlg_entrybox/entrytext",infostruct->entry);
    }
    else
    {
        evas_object_focus_set(infostruct->parent,1);
        free(infostruct->entry);
        fini_entrybox(e,obj);
        (infostruct->handler)(e,obj,NULL);
    }
}

void entrybox_item(Evas *e, Evas_Object *obj,int index, bool lp)
{

	entry_info_struct *infostruct=(entry_info_struct *) evas_object_data_get(obj,"entry_info");
    if(infostruct->curchar>=infostruct->maxchar)
        return;
    else if(index>=0 && index<=9)
    {
        (infostruct->entry)[infostruct->curchar]=((char)index)+'0';    
        (infostruct->entry)[infostruct->curchar+1]='\0';
        infostruct->curchar++;
        edje_object_part_text_set(obj,"dlg_entrybox/entrytext",infostruct->entry);
    }
}





static key_handler_info_t entrybox_handlers = {
	entrybox_ok,
	entrybox_esc,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	entrybox_item
};

Evas_Object *init_entrybox(Evas *evas,const char* title,const char *defaultentry,int maxlength,entry_handler handler, Evas_Object *parent)
{

	entry_info_struct *info =
		(entry_info_struct *) malloc(sizeof(entry_info_struct));
    int usedefault=1;
    if(strlen(defaultentry)>maxlength)
        usedefault=0;
    
    info->entry=(char*)malloc(sizeof(char)*(maxlength+1));
    (info->entry)[0]='\0';
	if(usedefault)
    {
        info->curchar = strlen(defaultentry);
        strcat(info->entry,defaultentry);
    }
    else
        info->curchar = 0;
    
    
	info->maxchar = maxlength;
	info->handler = handler;
	info->parent = parent;

    Evas_Object *win=edje_object_add(evas);
    char *themefile=get_theme_file();

    edje_object_file_set(win,themefile, "dlg_entrybox");
    evas_object_data_set(win,"entry_info",(void *)info);
    set_key_handler(win, &entrybox_handlers);
    int edje_w,edje_h;
    edje_object_size_min_get(win, &edje_w, &edje_h);
    evas_object_resize(win,edje_w,edje_h);
    evas_object_move (win,0,0);
    if(title)
        edje_object_part_text_set(win,"dlg_entrybox/titletext",title);
    edje_object_part_text_set(win,"dlg_entrybox/entrytext",info->entry);
    evas_object_show(win);
    evas_object_focus_set(win,1);
    
    
    
    
    free(themefile);
	return win;
}


void fini_entrybox(Evas *e, Evas_Object *obj)
{
	entry_info_struct *infostruct =(entry_info_struct *) evas_object_data_get(obj,"entry_info");
	//free(infostruct->entry);
    evas_object_del(obj);
	
}



Evas_Object *entrybox_get_parent(Evas *e, Evas_Object *obj)
{
	entry_info_struct *infostruct;
	infostruct =
		(entry_info_struct *) evas_object_data_get(obj,"entry_info");

	return infostruct->parent;
}

