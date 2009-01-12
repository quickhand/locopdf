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
#include "choicebox.h"
#include "locopdf.h"


const int noptions = 8;



typedef struct _choice_info_struct {
	char **choices;
	char **values;
	int numchoices;
	int curindex;
	int navsel;
	bool master;
	choice_handler handler;
	Evas_Object *parent;
    Evas_Object **choicelabels;
    Evas_Object **valuelabels;
    Evas_Object *maintable;
    void *userdata;
} choice_info_struct;


void choicebox_change_selection(Evas *e, Evas_Object *obj, int new_navsel)
{
	choice_info_struct *infostruct =
		(choice_info_struct *) evas_object_data_get(obj,"choice_info");
    char *tempstr;
    if(get_nav_mode()==1) 
    {
        //asprintf(&tempstr,"choicelabel_%d",infostruct->navsel);
        Evas_Object *oldselected_choice=infostruct->choicelabels[infostruct->navsel];//evas_object_name_find(e,tempstr);
        //free(tempstr);
        //asprintf(&tempstr,"valuelabel_%d",infostruct->navsel);
        Evas_Object *oldselected_value;
        if(infostruct->values)
            oldselected_value=infostruct->valuelabels[infostruct->navsel];//evas_object_name_find(e,tempstr);
        //free(tempstr);
        //asprintf(&tempstr,"choicelabel_%d",new_navsel);
        Evas_Object *newselected_choice=infostruct->choicelabels[new_navsel];//evas_object_name_find(e,tempstr);
        //free(tempstr);
        //asprintf(&tempstr,"valuelabel_%d",new_navsel);
        Evas_Object *newselected_value;
        if(infostruct->values)
            newselected_value=infostruct->valuelabels[new_navsel];//evas_object_name_find(e,tempstr);
        //free(tempstr);
        
        edje_object_signal_emit(oldselected_choice,"unselect","EVAS");
        if(infostruct->values)
            edje_object_signal_emit(oldselected_value,"unselect","EVAS");
        edje_object_signal_emit(newselected_choice,"select","EVAS");
        if(infostruct->values)
            edje_object_signal_emit(newselected_value,"select","EVAS");
    }
    
    infostruct->navsel = new_navsel;
}

void choicebox_next_page(Evas *e, Evas_Object *obj)
{
	choice_info_struct *infostruct;
	infostruct =
		(choice_info_struct *) evas_object_data_get(obj,"choice_info");

	if (infostruct->numchoices < noptions
			|| (infostruct->curindex + noptions) >= infostruct->numchoices)
		return;
	infostruct->curindex += noptions;

	int shownum =
		((infostruct->numchoices - infostruct->curindex) >
		 noptions) ? noptions : (infostruct->numchoices -
			 infostruct->curindex);

    char *tempstr;
    for(int i=0;i<noptions;i++)
    {
        //asprintf(&tempstr,"choicelabel_%d",i);
        Evas_Object *choicelabel=infostruct->choicelabels[i];//evas_object_name_find(e,tempstr);
        //free(tempstr);
        //asprintf(&tempstr,"valuelabel_%d",i);
        Evas_Object *valuelabel;
        if(infostruct->values)
            valuelabel=infostruct->valuelabels[i];//evas_object_name_find(e,tempstr);
        //free(tempstr);
        if(i<shownum)
        {
            edje_object_part_text_set(choicelabel,"dlg_optionlabel/text",infostruct->choices[infostruct->curindex+i]);
            if(infostruct->values)
                edje_object_part_text_set(valuelabel,"dlg_valuelabel/text",infostruct->values[infostruct->curindex+i]);
        }
        else
        {
            edje_object_part_text_set(choicelabel,"dlg_optionlabel/text","");
            if(infostruct->values)
                edje_object_part_text_set(valuelabel,"dlg_valuelabel/text","");
            
        }
        
        
        
    }
    
    if(infostruct->numchoices>noptions)
    {
        char *p;
        asprintf(&p, "Page %d of %d", 1 + infostruct->curindex / 8, (7 + infostruct->numchoices) / 8);
        edje_object_part_text_set(obj,"dlg_choicebox/footertext",p);
        free(p);
    }
	choicebox_change_selection(e,obj, 0);

}

void choicebox_previous_page(Evas *e, Evas_Object *obj)
{
	choice_info_struct *infostruct;
	infostruct =
		(choice_info_struct *) evas_object_data_get(obj,"choice_info");
	if (infostruct->numchoices < noptions || infostruct->curindex == 0)
		return;
	infostruct->curindex -= noptions;
    int shownum =
		((infostruct->numchoices - infostruct->curindex) >
		 noptions) ? noptions : (infostruct->numchoices -
			 infostruct->curindex);
         
    char *tempstr;
    for(int i=0;i<noptions;i++)
    {
        //asprintf(&tempstr,"choicelabel_%d",i);
        Evas_Object *choicelabel=infostruct->choicelabels[i];//evas_object_name_find(e,tempstr);
        //free(tempstr);
        //asprintf(&tempstr,"valuelabel_%d",i);
        Evas_Object *valuelabel;
        if(infostruct->values)
            valuelabel=infostruct->valuelabels[i];//evas_object_name_find(e,tempstr);
        //free(tempstr);
        if(i<shownum)
        {
            edje_object_part_text_set(choicelabel,"dlg_optionlabel/text",infostruct->choices[infostruct->curindex+i]);
            if(infostruct->values)
                edje_object_part_text_set(valuelabel,"dlg_valuelabel/text",infostruct->values[infostruct->curindex+i]);
        }
        else
        {
            edje_object_part_text_set(choicelabel,"dlg_optionlabel/text","");
            if(infostruct->values)
                edje_object_part_text_set(valuelabel,"dlg_valuelabel/text","");
        }
        
        
    }
    if(infostruct->numchoices>noptions)
    {
        char *p;
        asprintf(&p, "Page %d of %d", 1 + infostruct->curindex / 8, (7 + infostruct->numchoices) / 8);
        edje_object_part_text_set(obj,"dlg_choicebox/footertext",p);
        free(p);
    }

	choicebox_change_selection(e,obj, 0);
}

void choicebox_esc(Evas *e, Evas_Object *obj)
{
    choice_info_struct *infostruct=(choice_info_struct *) evas_object_data_get(obj,"choice_info");
    evas_object_focus_set(infostruct->parent,1);
	fini_choicebox(e,obj);
}

void choicebox_item(Evas *e, Evas_Object *obj,int index, bool lp)
{
	choice_info_struct *infostruct;
	if (index >= 1 && index <= 8) {
		int curchoice;
		infostruct =
			(choice_info_struct *) evas_object_data_get(obj,"choice_info");
		curchoice = infostruct->curindex + (index - 1);
		if (curchoice < infostruct->numchoices) {
			choicebox_change_selection(e,obj, index);
			(infostruct->handler) (e,obj,curchoice, lp);
		}			
	} else if (index == 9)
		choicebox_previous_page(e,obj);
	else if (index == 0)
		choicebox_next_page(e,obj);
}


void choicebox_nav_up(Evas *e, Evas_Object *obj)
{
	choice_info_struct *infostruct =
		(choice_info_struct *) evas_object_data_get(obj,"choice_info");
	if (infostruct->navsel == 0)
		return;

	choicebox_change_selection(e,obj, infostruct->navsel - 1);
}

void choicebox_nav_down(Evas *e, Evas_Object *obj)
{
	choice_info_struct *infostruct =
		(choice_info_struct *) evas_object_data_get(obj,"choice_info");
	if (infostruct->navsel == (noptions - 1)
			|| ((infostruct->curindex + infostruct->navsel + 1) >=
				infostruct->numchoices))
		return;
	choicebox_change_selection(e,obj, infostruct->navsel + 1);
}

void choicebox_nav_left(Evas *e, Evas_Object *obj)
{
	choicebox_previous_page(e,obj);
}

void choicebox_nav_right(Evas *e, Evas_Object *obj)
{
	choicebox_next_page(e,obj);
}

void choicebox_nav_sel(Evas *e, Evas_Object *obj)
{
	choice_info_struct *infostruct =
		(choice_info_struct *) evas_object_data_get(obj,"choice_info");
	(infostruct->handler) (e,obj,infostruct->curindex * noptions +
			infostruct->navsel,  false);

}


static key_handler_info_t choicebox_handlers = {
	choicebox_esc,
	choicebox_esc,
	choicebox_nav_up,
	choicebox_nav_down,
	choicebox_nav_left,
	choicebox_nav_right,
	choicebox_nav_sel,
	NULL,
	NULL,
    NULL,
	choicebox_item
};

Evas_Object *init_choicebox(Evas *evas,const char *choicelist[], const char *values[], int numchoices,
		choice_handler handler, char *header, Evas_Object *parent, void *userdata, bool master)
{

	choice_info_struct *info =
		(choice_info_struct *) malloc(sizeof(choice_info_struct));

	info->numchoices = numchoices;
	info->curindex = 0;
	info->navsel = 0;
	info->handler = handler;
	info->master = master;
	info->parent = parent;
    info->userdata=userdata;
	info->choices = (char **) malloc(sizeof(char *) * numchoices);
    if(values)
        info->values = (char **) malloc(sizeof(char *) * numchoices);
    else
        info->values=NULL;
	for (int i = 0; i < numchoices; i++) {
		info->choices[i] =
			(char *) malloc(sizeof(char) * (strlen(choicelist[i]) + 1));
		asprintf(&(info->choices[i]), "%s", choicelist[i]);
        if(values)
        {
            info->values[i] = (char *)malloc(sizeof(char) * (strlen(values[i]) + 1));
            asprintf(&(info->values[i]), "%s", values[i]);
        }
	}

    Evas_Object *win=edje_object_add(evas);
    char *themefile=get_theme_file();

    edje_object_file_set(win,themefile, "dlg_choicebox");
    evas_object_data_set(win,"choice_info",(void *)info);
    set_key_handler(win, &choicebox_handlers);
    int edje_w,edje_h;
    edje_object_size_min_get(win, &edje_w, &edje_h);
    evas_object_resize(win,edje_w,edje_h);

    evas_object_move (win,0,0);
    if(header)
        edje_object_part_text_set(win,"dlg_choicebox/titletext",header);
    evas_object_show(win);
    evas_object_focus_set(win,1);
    
    
    Evas_Object *maintable=evas_object_table_add(evas);
    info->maintable=maintable;
    evas_object_table_homogeneous_set(maintable,EVAS_OBJECT_TABLE_HOMOGENEOUS_TABLE);
    edje_object_part_swallow(win,"dlg_choicebox/swallow",maintable);

    evas_object_show(maintable);
	//int shownum = (numchoices <= noptions) ? numchoices : noptions;
    //evas_object_name_set(maintable,"maintable");
    info->choicelabels=(Evas_Object**)malloc(sizeof(Evas_Object*)*noptions);
    if(values)
        info->valuelabels=(Evas_Object**)malloc(sizeof(Evas_Object*)*noptions);
    else
        info->valuelabels=NULL;
	int shownum = noptions;
    char *tempstr;
    for(int i=0;i<shownum;i++)
    {
        
        Evas_Object *choicelabel=edje_object_add(evas);
        info->choicelabels[i]=choicelabel;
        edje_object_file_set(choicelabel,themefile, "dlg_optionlabel");
        evas_object_table_pack(maintable,choicelabel,0,i,1,1);
        
        //asprintf(&tempstr,"choicelabel_%d",i);
        //evas_object_name_set(choicelabel,tempstr);
        //free(tempstr);
        
        if(i<numchoices)
            edje_object_part_text_set(choicelabel,"dlg_optionlabel/text",info->choices[i]);
        else
            edje_object_part_text_set(choicelabel,"dlg_optionlabel/text","");

        evas_object_show(choicelabel);
        if(values)
        {
            Evas_Object *valuelabel=edje_object_add(evas);
            info->valuelabels[i]=valuelabel;
            edje_object_file_set(valuelabel,themefile, "dlg_valuelabel");
            evas_object_table_pack(maintable,valuelabel,1,i,1,1);
            //asprintf(&tempstr,"valuelabel_%d",i);
            //evas_object_name_set(valuelabel,tempstr);
            //free(tempstr);
        
        
            
            if(i<numchoices)
                edje_object_part_text_set(valuelabel,"dlg_valuelabel/text",info->values[i]);
            else
                edje_object_part_text_set(valuelabel,"dlg_valuelabel/text","");
            
            evas_object_show(valuelabel);
        }
    }
    
    if(numchoices>noptions)
    {
     
        char *p;
        asprintf(&p,"Page 1 of %d",(7 + numchoices) / 8);
        edje_object_part_text_set(win,"dlg_choicebox/footertext",p);
        free(p);
        
    }
    
    free(themefile);
	return win;
}

void free_choices(choice_info_struct *infostruct)
{
	for (int i = 0; i < infostruct->numchoices; i++) {
		free(infostruct->choices[i]);
        if(infostruct->values)
            free(infostruct->values[i]);
	}
	free(infostruct->choices);
    if(infostruct->values)
    {
        free(infostruct->values);
    }
    
    
}

void fini_choicebox(Evas *e, Evas_Object *obj, bool redraw)
{
	bool master = false;
	choice_info_struct *infostruct =(choice_info_struct *) evas_object_data_get(obj,"choice_info");
	if(infostruct->master)
		master = true;
	
    char *tempstr;
    Evas_Object *tempobj;
    for(int i=0;i<noptions;i++)
    {
        //asprintf(&tempstr,"optionlabel_%d",i);
        tempobj=infostruct->choicelabels[i];//evas_object_name_find(e,tempstr);
        evas_object_del(tempobj);
        //free(tempstr);
    }
    for(int i=0;i<noptions && infostruct->values;i++)
    {
        //asprintf(&tempstr,"valuelabel_%d",i);
        tempobj=infostruct->valuelabels[i];//evas_object_name_find(e,tempstr);
        evas_object_del(tempobj);
        //free(tempstr);
    }
    if(infostruct->values)
    {
        free(infostruct->valuelabels);
    }
    free(infostruct->choicelabels);
    
    tempobj=infostruct->maintable;//evas_object_name_find(e,"maintable");
    evas_object_del(tempobj);
    evas_object_del(obj);
	
    free_choices(infostruct);
	free(infostruct);
	//if(master && redraw)
	//	redraw_text();
}

void update_choicebox(Evas *e, Evas_Object *obj, const char *choicelist[], const char *values[], int numchoices, bool rewind)
{
	choice_info_struct *info =
		(choice_info_struct *) evas_object_data_get(obj, "choice_info");
        
    if(info->values && !values)
    {
        Evas_Object *tempobj;
        for(int i=0;i<noptions;i++)
        {
            tempobj=info->valuelabels[i];
            evas_object_del(tempobj);
        }
        free(info->valuelabels);
    }
    else if(!info->values && values)
    {
        info->valuelabels=(Evas_Object**)malloc(sizeof(Evas_Object*)*noptions);
        char *themefile=get_theme_file();
        for(int i=0;i<noptions;i++)
        {
        
            
       
            Evas_Object *valuelabel=edje_object_add(e);
            info->valuelabels[i]=valuelabel;
            edje_object_file_set(valuelabel,themefile, "dlg_valuelabel");
            evas_object_table_pack(info->maintable,valuelabel,1,i,1,1);       
            
            if(i<numchoices)
                edje_object_part_text_set(valuelabel,"dlg_valuelabel/text",info->values[i]);
            else
                edje_object_part_text_set(valuelabel,"dlg_valuelabel/text","");
            
            evas_object_show(valuelabel);

        }
        free(themefile);
    }
    free_choices(info);
    
    
    
    
	info->numchoices = numchoices;
	if(rewind || (info->curindex >= numchoices))
		info->curindex = 0;

	info->choices = (char **) malloc(sizeof(char *) * numchoices);
    if(values)
        info->values = (char **) malloc(sizeof(char *) * numchoices);
    else
        info->values=NULL;
	for (int i = 0; i < numchoices; i++) {
		info->choices[i] =
			(char *) malloc(sizeof(char) * (strlen(choicelist[i]) + 1));
		asprintf(&(info->choices[i]), "%s", choicelist[i]);
        if(values)
        {
            info->values[i] = (char *)malloc(sizeof(char) * (strlen(values[i]) + 1));
            asprintf(&(info->values[i]), "%s", values[i]);
        }
	}

	
	int shownum =
		((info->numchoices - info->curindex) >
		 noptions) ? noptions : (info->numchoices -
			 info->curindex);

    char *tempstr;
    for(int i=0;i<noptions;i++)
    {
        //asprintf(&tempstr,"choicelabel_%d",i);
        Evas_Object *choicelabel=info->choicelabels[i];//evas_object_name_find(e,tempstr);
        //free(tempstr);
        //asprintf(&tempstr,"valuelabel_%d",i);
        /* FIX ME! */
        Evas_Object *valuelabel=info->valuelabels[i];//evas_object_name_find(e,tempstr);
        //free(tempstr);
        if(i<shownum)
        {
            if(values)
                edje_object_part_text_set(choicelabel,"dlg_optionlabel/text",info->choices[info->curindex+i]);
            edje_object_part_text_set(valuelabel,"dlg_valuelabel/text",info->values[info->curindex+i]);
        }
        else
        {
            
            edje_object_part_text_set(choicelabel,"dlg_optionlabel/text","");
            if(values)
                edje_object_part_text_set(valuelabel,"dlg_valuelabel/text","");
        }
        
        
    }
    
	choicebox_change_selection(e,obj, 0);
}


Evas_Object *choicebox_get_parent(Evas *e, Evas_Object *obj)
{
	choice_info_struct *infostruct;
	infostruct =
		(choice_info_struct *) evas_object_data_get(obj,"choice_info");
    if(infostruct)
        return infostruct->parent;
    else
        return NULL;
}
void *choicebox_get_userdata(Evas *e,Evas_Object *obj)
{
    choice_info_struct *infostruct;
	infostruct = (choice_info_struct *) evas_object_data_get(obj,"choice_info");
    return infostruct->userdata;
    
}
void update_label(Evas *e, Evas_Object *obj, int number, const char *value)
{
    choice_info_struct *infostruct;
	infostruct = (choice_info_struct *) evas_object_data_get(obj,"choice_info");
    
    edje_object_part_text_set(infostruct->valuelabels[number],"dlg_valuelabel/text",value);
    
}
