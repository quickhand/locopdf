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


extern void redraw_text();

const int noptions = 8;

/*#define HIDE_LABELS \
	for (int i = 1; i <= 8; i++) \
		ewl_widget_hide(EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(vbox), i)))

#define SHOW_LABELS \
	for (int i = 1; i <= 8; i++) \
		ewl_widget_show(EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(vbox), i)))*/

#define REL_THEME "themes/themes_oitheme.edj"
typedef struct _choice_info_struct {
	char **choices;
	char **values;
	int numchoices;
	int curindex;
	int navsel;
	bool master;
	choice_handler handler;
	Evas_Object *parent;
} choice_info_struct;

/*
 * Returns edje theme file name.
 */
char *get_theme_file()
{
//	char *cwd = get_current_dir_name();
	char *rel_theme;
	asprintf(&rel_theme, "%s/%s", "/usr/share/locopdf", REL_THEME);
	//free(cwd);
	return rel_theme;
}

/*static void choicebox_reveal_cb(Ewl_Widget *w, void *ev, void *data) {
	ewl_window_move(EWL_WINDOW(w), (600 - CURRENT_W(w)) / 2, (800 - CURRENT_H(w)) / 2);
	ewl_window_keyboard_grab_set(EWL_WINDOW(w), 1);
}*/

/*static void choicebox_realize_cb(Ewl_Widget *w, void *ev, void *data) {
	Ewl_Widget *win;
	if(data)
		win = (Ewl_Widget *)data;
	else
		win = ewl_widget_name_find("main_win");
	if(win)
		ewl_window_keyboard_grab_set(EWL_WINDOW(win), 0);
}*/

/*static void choicebox_unrealize_cb(Ewl_Widget *w, void *ev, void *data) {
	Ewl_Widget *win;


	if(data)
		win = (Ewl_Widget *)data;
	else
		win = ewl_widget_name_find("main_win");
	if(win)
		ewl_window_keyboard_grab_set(EWL_WINDOW(win), 1);
}*/

void choicebox_change_selection(Evas *e, Evas_Object *obj, int new_navsel)
{
	choice_info_struct *infostruct =
		(choice_info_struct *) evas_object_data_get(obj,"choice_info");
	//Ewl_Widget *vbox = ewl_container_child_get(EWL_CONTAINER(widget), 0);
	//Ewl_Widget *oldselected =
	//	ewl_container_child_get(EWL_CONTAINER(vbox), infostruct->navsel + 1);
	//Ewl_Widget *newselected =
	//	ewl_container_child_get(EWL_CONTAINER(vbox), new_navsel + 1);
    char *tempstr;
    if(get_nav_mode()==1) 
    {
        asprintf(&tempstr,"choicelabel_%d",infostruct->navsel);
        Evas_Object *oldselected_choice=evas_object_name_find(e,tempstr);
        free(tempstr);
        asprintf(&tempstr,"valuelabel_%d",infostruct->navsel);
        Evas_Object *oldselected_value=evas_object_name_find(e,tempstr);
        free(tempstr);
        asprintf(&tempstr,"choicelabel_%d",new_navsel);
        Evas_Object *newselected_choice=evas_object_name_find(e,tempstr);
        free(tempstr);
        asprintf(&tempstr,"valuelabel_%d",new_navsel);
        Evas_Object *newselected_value=evas_object_name_find(e,tempstr);
        free(tempstr);
        
        edje_object_signal_emit(oldselected_choice,"unselect","EVAS");
        edje_object_signal_emit(oldselected_value,"unselect","EVAS");
        edje_object_signal_emit(newselected_choice,"select","EVAS");
        edje_object_signal_emit(newselected_choice,"select","EVAS");
    }
    
	/*if (get_nav_mode() == 1) {
		ewl_widget_state_set(oldselected, "unselect",EWL_STATE_PERSISTENT);
		ewl_widget_state_set(ewl_container_child_get(EWL_CONTAINER(oldselected), 0), "unselect",EWL_STATE_PERSISTENT);
		ewl_widget_state_set(newselected,"select",EWL_STATE_PERSISTENT);
		ewl_widget_state_set(ewl_container_child_get
				(EWL_CONTAINER(newselected), 0), "select",
				EWL_STATE_PERSISTENT);
	}*/
	infostruct->navsel = new_navsel;
}

void choicebox_next_page(Evas *e, Evas_Object *obj)
{
	choice_info_struct *infostruct;
	infostruct =
		(choice_info_struct *) evas_object_data_get(obj,"choice_info");
	//Ewl_Widget *vbox = ewl_container_child_get(EWL_CONTAINER(widget), 0);
	//Ewl_Widget *tempw1, *v1, *v2;
	if (infostruct->numchoices < noptions
			|| (infostruct->curindex + noptions) >= infostruct->numchoices)
		return;
	infostruct->curindex += noptions;

	int shownum =
		((infostruct->numchoices - infostruct->curindex) >
		 noptions) ? noptions : (infostruct->numchoices -
			 infostruct->curindex);

	//HIDE_LABELS;
    char *tempstr;
    for(int i=0;i<noptions;i++)
    {
        asprintf(&tempstr,"choicelabel_%d",i);
        Evas_Object *choicelabel=evas_object_name_find(e,tempstr);
        free(tempstr);
        asprintf(&tempstr,"valuelabel_%d",i);
        Evas_Object *valuelabel=evas_object_name_find(e,tempstr);
        free(tempstr);
        edje_object_part_text_set(choicelabel,"dlg_optionlabel/text",infostruct->choices[infostruct->curindex+i]);
        edje_object_part_text_set(choicelabel,"dlg_valuelabel/text",infostruct->values[infostruct->curindex+i]);
        
        
        
    }
    
    
	/*for (int i = 0; i < noptions; i++) {
		tempw1 =
			EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(vbox), i + 1));

		v1 = EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(tempw1), 0));
		v2 = EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(tempw1), 1));
		if (i < shownum) {
			ewl_label_text_set(EWL_LABEL
					(ewl_container_child_get
					 (EWL_CONTAINER(v1), 0)),
					infostruct->choices[infostruct->curindex +
					i]);

			ewl_label_text_set(EWL_LABEL
					(ewl_container_child_get
					 (EWL_CONTAINER(v2), 0)),
					infostruct->values[infostruct->curindex +
					i]);

			if(!strlen(infostruct->values[infostruct->curindex + i]))
				ewl_widget_hide(v2);
			else
				ewl_widget_show(v2);

		} else {
			ewl_label_text_set(EWL_LABEL
					(ewl_container_child_get
					 (EWL_CONTAINER(v1), 0)), "");

			ewl_label_text_set(EWL_LABEL
					(ewl_container_child_get
					 (EWL_CONTAINER(v2), 0)), "");
		}
	}

	char *p;
	asprintf(&p, "Page %d of %d", 1 + infostruct->curindex / 8, (7 + infostruct->numchoices) / 8);

	tempw1 =
		EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(vbox), noptions + 1));

	v1 = EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(tempw1), 0));
	ewl_label_text_set(EWL_LABEL
			(ewl_container_child_get
			 (EWL_CONTAINER(v1), 0)),
			p);
	free(p);*/

	choicebox_change_selection(e,obj, 0);
	//SHOW_LABELS;
}

void choicebox_previous_page(Evas *e, Evas_Object *obj)
{
	choice_info_struct *infostruct;
	infostruct =
		(choice_info_struct *) evas_object_data_get(obj,"choice_info");
	//Ewl_Widget *vbox = ewl_container_child_get(EWL_CONTAINER(widget), 0);
	//Ewl_Widget *tempw1, *v1, *v2;
	if (infostruct->numchoices < noptions || infostruct->curindex == 0)
		return;
	infostruct->curindex -= noptions;
	//HIDE_LABELS;
    
    char *tempstr;
    for(int i=0;i<noptions;i++)
    {
        asprintf(&tempstr,"choicelabel_%d",i);
        Evas_Object *choicelabel=evas_object_name_find(e,tempstr);
        free(tempstr);
        asprintf(&tempstr,"valuelabel_%d",i);
        Evas_Object *valuelabel=evas_object_name_find(e,tempstr);
        free(tempstr);
        edje_object_part_text_set(choicelabel,"dlg_optionlabel/text",infostruct->choices[infostruct->curindex+i]);
        edje_object_part_text_set(choicelabel,"dlg_valuelabel/text",infostruct->values[infostruct->curindex+i]);
        
        
        
    }
    
	/*for (int i = 0; i < noptions; i++) {
		tempw1 =
			EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(vbox), i + 1));
		v1 = EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(tempw1), 0));
		v2 = EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(tempw1), 1));

		ewl_label_text_set(EWL_LABEL
				(ewl_container_child_get
				 (EWL_CONTAINER(v1), 0)),
				infostruct->choices[infostruct->curindex + i]);

		ewl_label_text_set(EWL_LABEL
				(ewl_container_child_get
				 (EWL_CONTAINER(v2), 0)),
				infostruct->values[infostruct->curindex + i]);

		if(!strlen(infostruct->values[infostruct->curindex + i]))
			ewl_widget_hide(v2);
		else
			ewl_widget_show(v2);
	}

	char *p;
	asprintf(&p, "Page %d of %d", 1 + infostruct->curindex / 8, (7 + infostruct->numchoices) / 8);

	tempw1 =
		EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(vbox), noptions + 1));

	v1 = EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(tempw1), 0));
	ewl_label_text_set(EWL_LABEL
			(ewl_container_child_get
			 (EWL_CONTAINER(v1), 0)),
			p);
	free(p);*/

	choicebox_change_selection(e,obj, 0);
	//SHOW_LABELS;
}

void choicebox_esc(Evas *e, Evas_Object *obj)
{
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

/*static key_handler_info_t choicebox_handlers = {
	.nav_up_handler = &choicebox_nav_up,
	.nav_down_handler = &choicebox_nav_down,
	.nav_left_handler = &choicebox_nav_left,
	.nav_right_handler = &choicebox_nav_right,
	.nav_sel_handler = &choicebox_nav_sel,
	.esc_handler = &choicebox_esc,
	.item_handler = &choicebox_item,
};
*/

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
	choicebox_item
};

/*void choicebox_destroy_cb(Ewl_Widget * w, void *event, void *data)
{
	fini_choicebox(w);
	Ewl_Widget *win;
	win = ewl_widget_name_find("main_win");
	if(win == data)
		redraw_text();
}*/

Evas_Object *init_choicebox(Evas *evas,const char *choicelist[], const char *values[], int numchoices,
		choice_handler handler, char *header, Evas_Object *parent, bool master)
{
	//Ewl_Widget *win, *vbox, *tempw1, *tempw2, *w, *v1, *v2;

	//w = ewl_widget_name_find("main_win");

	choice_info_struct *info =
		(choice_info_struct *) malloc(sizeof(choice_info_struct));

	info->numchoices = numchoices;
	info->curindex = 0;
	info->navsel = 0;
	info->handler = handler;
	info->master = master;
	info->parent = parent;

	info->choices = (char **) malloc(sizeof(char *) * numchoices);
	info->values = (char **) malloc(sizeof(char *) * numchoices);
	for (int i = 0; i < numchoices; i++) {
		info->choices[i] =
			(char *) malloc(sizeof(char) * (strlen(choicelist[i]) + 1));
		asprintf(&(info->choices[i]), "%s", choicelist[i]);

		info->values[i] = (char *)malloc(sizeof(char) * (strlen(values[i]) + 1));
		asprintf(&(info->values[i]), "%s", values[i]);
	}

	/*win = ewl_window_new();
	ewl_window_title_set(EWL_WINDOW(win), "EWL_WINDOW");
	ewl_window_name_set(EWL_WINDOW(win), "EWL_WINDOW");
	ewl_window_class_set(EWL_WINDOW(win), "ChoiceBox");
	set_key_handler(EWL_WIDGET(win), &choicebox_handlers);
	ewl_callback_append(win, EWL_CALLBACK_DELETE_WINDOW, choicebox_destroy_cb, (void *)parent);
	ewl_callback_append(win, EWL_CALLBACK_REVEAL, choicebox_reveal_cb, (void *)parent);
	ewl_callback_append(win, EWL_CALLBACK_REALIZE, choicebox_realize_cb, (void *)parent);
	ewl_callback_append(win, EWL_CALLBACK_UNREALIZE, choicebox_unrealize_cb, (void *)parent);
    ewl_theme_data_str_set(win, "/window/group","ewl/window/dlg_choicebox");
	EWL_EMBED(win)->x = 600;
	EWL_EMBED(win)->y = 0;
	ewl_widget_data_set(EWL_WIDGET(win), (void *)"choice_info",
			(void *) info);
	ewl_widget_show(win);

	ewl_widget_focus_send(win);*/
    Evas_Object *win=edje_object_add(evas);
    char *themefile=get_theme_file();

    edje_object_file_set(win,themefile, "dlg_choicebox");
    fprintf(stderr,edje_load_error_str(edje_object_load_error_get(win)));
    int torf=edje_object_part_exists(win,"dlg_choicebox/border");
    if(!torf)
        fprintf(stderr,"\nfalse\n");
    else
        fprintf(stderr,"\ntrue\n");
    evas_object_data_set(win,"choice_info",(void *)info);
    set_key_handler(win, &choicebox_handlers);
    int edje_w,edje_h;
    edje_object_size_min_get(win, &edje_w, &edje_h);
    evas_object_resize(win, edje_w, edje_h);

    evas_object_move (win,0,0);
    evas_object_show(win);
    evas_object_focus_set(win,1);
    
    
	/*vbox = ewl_vbox_new();
	ewl_container_child_append(EWL_CONTAINER(win), vbox);
	ewl_object_fill_policy_set(EWL_OBJECT(vbox), EWL_FLAG_FILL_FILL);
	ewl_widget_show(vbox);*/
    
    Evas_Object *maintable=evas_object_table_add_to(win);//(evas);
    
    //evas_object_table_homogeneous_set(maintable,EVAS_OBJECT_TABLE_HOMOGENEOUS_TABLE);
    edje_object_part_swallow(win,"dlg_choicebox/swallow",maintable);
    evas_object_show(maintable);
//	int shownum = (numchoices <= noptions) ? numchoices : noptions;
	int shownum = noptions;
    char *tempstr;
    for(int i=0;i<shownum;i++)
    {
        Evas_Object *choicelabel=edje_object_add(evas);
        edje_object_file_set(choicelabel,themefile, "dlg_optionlabel");
        evas_object_table_pack(maintable,choicelabel,0,i,1,1);
        
        asprintf(&tempstr,"choicelabel_%d",i);
        evas_object_name_set(choicelabel,tempstr);
        free(tempstr);
        evas_object_show(choicelabel);
        
        Evas_Object *valuelabel=edje_object_add(evas);
        edje_object_file_set(valuelabel,themefile, "dlg_valuelabel");
        evas_object_table_pack(maintable,valuelabel,1,i,1,1);
        asprintf(&tempstr,"valuelabel_%d",i);
        evas_object_name_set(valuelabel,tempstr);
        free(tempstr);
        evas_object_show(valuelabel);
    }
    
    
    
    /*
	tempw1 = ewl_hbox_new();
	ewl_container_child_append(EWL_CONTAINER(vbox), tempw1);
	ewl_theme_data_str_set(EWL_WIDGET(tempw1), "/hbox/group",
			"ewl/box/dlg_optionbox");
	ewl_object_fill_policy_set(EWL_OBJECT(tempw1), EWL_FLAG_FILL_FILL);
	ewl_widget_show(tempw1);

	v1 = ewl_vbox_new();
	ewl_container_child_append(EWL_CONTAINER(tempw1), v1);
	ewl_theme_data_str_set(EWL_WIDGET(v1), "/hbox/group",
			"ewl/box/dlg_optionbox");
	ewl_object_fill_policy_set(EWL_OBJECT(v1), EWL_FLAG_FILL_HFILL);
	ewl_widget_show(v1);

	v2 = ewl_vbox_new();
	ewl_container_child_append(EWL_CONTAINER(tempw1), v2);
	ewl_theme_data_str_set(EWL_WIDGET(v2), "/hbox/group",
			"ewl/box/dlg_optionbox");
	ewl_object_fill_policy_set(EWL_OBJECT(v2), EWL_FLAG_FILL_HFILL);
	ewl_widget_show(v2);

	tempw2 = ewl_label_new();
	ewl_container_child_append(EWL_CONTAINER(v1), tempw2);
	ewl_theme_data_str_set(EWL_WIDGET(tempw2), "/label/group",
			"ewl/label/dlg_header");
	ewl_theme_data_str_set(EWL_WIDGET(tempw2), "/label/textpart",
		"ewl/label/dlg_header/text");
	ewl_object_fill_policy_set(EWL_OBJECT(tempw2), EWL_FLAG_FILL_HFILL);
	ewl_label_text_set(EWL_LABEL(tempw2), header);
	ewl_widget_show(tempw2);

	for(int i = 0; i < 9; i++) {

		tempw1 = ewl_hbox_new();
		ewl_container_child_append(EWL_CONTAINER(vbox), tempw1);
		ewl_theme_data_str_set(EWL_WIDGET(tempw1), "/hbox/group",
				"ewl/box/dlg_optionbox");
		ewl_object_fill_policy_set(EWL_OBJECT(tempw1), EWL_FLAG_FILL_FILL);
		if (get_nav_mode() == 1 && i == 0)
			ewl_widget_state_set(tempw1, "select", EWL_STATE_PERSISTENT);

		ewl_widget_show(tempw1);

		v1 = ewl_vbox_new();
		ewl_container_child_append(EWL_CONTAINER(tempw1), v1);
		ewl_theme_data_str_set(EWL_WIDGET(v1), "/hbox/group",
				"ewl/box/dlg_optionbox");
		ewl_object_fill_policy_set(EWL_OBJECT(v1), EWL_FLAG_FILL_HFILL);
		ewl_widget_show(v1);

		tempw2 = ewl_label_new();
		ewl_container_child_append(EWL_CONTAINER(v1), tempw2);
		ewl_object_fill_policy_set(EWL_OBJECT(tempw2), EWL_FLAG_FILL_HFILL);
		if(i < shownum) {
			ewl_theme_data_str_set(EWL_WIDGET(tempw2), "/label/group",
					"ewl/label/dlg_optionlabel");
			ewl_theme_data_str_set(EWL_WIDGET(tempw2), "/label/textpart",
					"ewl/label/dlg_optionlabel/text");
			if (get_nav_mode() == 1 && i == 0)
				ewl_widget_state_set(tempw2, "select", EWL_STATE_PERSISTENT);
			if(numchoices > i)
				ewl_label_text_set(EWL_LABEL(tempw2), info->choices[i]);
		} else if(i == 8) {
			ewl_theme_data_str_set(EWL_WIDGET(tempw2), "/label/group",
					"ewl/label/dlg_footer");
			ewl_theme_data_str_set(EWL_WIDGET(tempw2), "/label/textpart",
					"ewl/label/dlg_footer/text");
			char *p;
			asprintf(&p, "Page %d of %d", 1 + info->curindex / 8, (7 + info->numchoices) / 8);
			ewl_label_text_set(EWL_LABEL(tempw2), p);
			free(p);
		}
		ewl_widget_show(tempw2);

		if(i < shownum) {
			v2 = ewl_vbox_new();
			ewl_container_child_append(EWL_CONTAINER(tempw1), v2);
			ewl_theme_data_str_set(EWL_WIDGET(v2), "/hbox/group",
					"ewl/box/dlg_optionbox");
			ewl_object_fill_policy_set(EWL_OBJECT(v2), EWL_FLAG_FILL_HFILL);
			if((numchoices > i) && strlen(info->values[i]))
				ewl_widget_show(v2);

			tempw2 = ewl_label_new();
            ewl_container_child_append(EWL_CONTAINER(v2), tempw2);
            ewl_object_fill_policy_set(EWL_OBJECT(tempw2), EWL_FLAG_FILL_HFILL);

			ewl_theme_data_str_set(EWL_WIDGET(tempw2), "/label/group",
					"ewl/label/dlg_valuelabel");
			ewl_theme_data_str_set(EWL_WIDGET(tempw2), "/label/textpart",
					"ewl/label/dlg_valuelabel/text");
			if(numchoices > i)
				ewl_label_text_set(EWL_LABEL(tempw2), info->values[i]);
			ewl_widget_show(tempw2);
		}
	}*/
    free(themefile);
	return win;
}

void free_choices(choice_info_struct *infostruct)
{
	for (int i = 0; i < infostruct->numchoices; i++) {
		free(infostruct->choices[i]);
		free(infostruct->values[i]);
	}
	free(infostruct->choices);
	free(infostruct->values);
}

void fini_choicebox(Evas *e, Evas_Object *obj, bool redraw)
{
	bool master = false;
	//ewl_widget_hide(win);
	choice_info_struct *infostruct =
		(choice_info_struct *) evas_object_data_get(obj,"choice_info");
	if(infostruct->master)
		master = true;
	free_choices(infostruct);
	free(infostruct);
	//ewl_widget_destroy(win);
	//if(master && redraw)
	//	redraw_text();
}

void update_choicebox(Evas *e, Evas_Object *obj, const char *choicelist[], const char *values[], int numchoices, bool rewind)
{
	choice_info_struct *info =
		(choice_info_struct *) evas_object_data_get(obj, "choice_info");

	free_choices(info);

	info->numchoices = numchoices;
	if(rewind || (info->curindex >= numchoices))
		info->curindex = 0;

	info->choices = (char **) malloc(sizeof(char *) * numchoices);
	info->values = (char **) malloc(sizeof(char *) * numchoices);
	for (int i = 0; i < numchoices; i++) {
		info->choices[i] =
			(char *) malloc(sizeof(char) * (strlen(choicelist[i]) + 1));
		asprintf(&(info->choices[i]), "%s", choicelist[i]);

		info->values[i] = (char *)malloc(sizeof(char) * (strlen(values[i]) + 1));
		asprintf(&(info->values[i]), "%s", values[i]);
	}

	//Ewl_Widget *vbox = ewl_container_child_get(EWL_CONTAINER(w), 0);
	//Ewl_Widget *tempw1, *v1, *v2;

	int shownum =
		((info->numchoices - info->curindex) >
		 noptions) ? noptions : (info->numchoices -
			 info->curindex);

	//HIDE_LABELS;
    
    char *tempstr;
    for(int i=0;i<noptions;i++)
    {
        asprintf(&tempstr,"choicelabel_%d",i);
        Evas_Object *choicelabel=evas_object_name_find(e,tempstr);
        free(tempstr);
        asprintf(&tempstr,"valuelabel_%d",i);
        Evas_Object *valuelabel=evas_object_name_find(e,tempstr);
        free(tempstr);
        edje_object_part_text_set(choicelabel,"dlg_optionlabel/text",info->choices[info->curindex+i]);
        edje_object_part_text_set(choicelabel,"dlg_valuelabel/text",info->values[info->curindex+i]);
        
        
        
    }
    
	/*for (int i = 0; i < noptions; i++) {
		tempw1 =
			EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(vbox), i + 1));

		v1 = EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(tempw1), 0));
		v2 = EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(tempw1), 1));
		if (i < shownum) {
			ewl_label_text_set(EWL_LABEL
					(ewl_container_child_get
					 (EWL_CONTAINER(v1), 0)),
					info->choices[info->curindex +
					i]);

			ewl_label_text_set(EWL_LABEL
					(ewl_container_child_get
					 (EWL_CONTAINER(v2), 0)),
					info->values[info->curindex +
					i]);

			if(!strlen(info->values[info->curindex + i]))
				ewl_widget_hide(v2);
			else
				ewl_widget_show(v2);

		} else {
			ewl_label_text_set(EWL_LABEL
					(ewl_container_child_get
					 (EWL_CONTAINER(v1), 0)), "");

			ewl_label_text_set(EWL_LABEL
					(ewl_container_child_get
					 (EWL_CONTAINER(v2), 0)), "");
		}
	}

	char *p;
	asprintf(&p, "Page %d of %d", 1 + info->curindex / 8, (7 + info->numchoices) / 8);

	tempw1 =
		EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(vbox), noptions + 1));

	v1 = EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(tempw1), 0));
	ewl_label_text_set(EWL_LABEL
			(ewl_container_child_get
			 (EWL_CONTAINER(v1), 0)),
			p);
	free(p);*/

	choicebox_change_selection(e,obj, 0);
	//SHOW_LABELS;
}


Evas_Object *choicebox_get_parent(Evas *e, Evas_Object *obj)
{
	choice_info_struct *infostruct;
	infostruct =
		(choice_info_struct *) evas_object_data_get(obj,"choice_info");

	return infostruct->parent;
}


void update_label(Evas *e, Evas_Object *obj, int number, const char *value)
{
    /* FIX ME */
    
	/*Ewl_Widget *vbox = ewl_container_child_get(EWL_CONTAINER(w), 0);
	Ewl_Widget *tempw1, *v1, *v2;

	tempw1 =
		EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(vbox), number + 1));
	v2 = EWL_WIDGET(ewl_container_child_get(EWL_CONTAINER(tempw1), 1));

	ewl_label_text_set(EWL_LABEL
			(ewl_container_child_get
			 (EWL_CONTAINER(v2), 0)),
			value);*/
}
