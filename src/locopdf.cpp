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
#include <Evas.h>
#include <Ecore.h>
#include <Ecore_Evas.h>
#include <Edje.h>
#include <epdf/Epdf.h>
#include "keyhandler.h"
#include "dialogs.h"
#include "locopdf.h"
#define REL_THEME "themes/themes_oitheme.edj"

using namespace std;


pthread_t thread;

Evas *evas;
Epdf_Document *document;
Epdf_Page     *page;
char          *filename;

int numpages;
int curpage=0;
int curpdfobj=1;
int prerendering=0;
int fitmode=0;  //0=none; 1=fit width; 2=fit height
double zoom=1.0;
double zoominc=0.1;
double hpaninc=0.5;
double vpaninc=0.5;

int lefttrim=0;
int righttrim=0;
int toptrim=0;
int bottomtrim=0;



int winwidth=600;
int winheight=800;
/*
 * Returns edje theme file name.
 */
 
char *get_theme_file()
{
 	//char *cwd = get_current_dir_name();
	char *rel_theme;
	asprintf(&rel_theme, "%s/%s", "/usr/share/locopdf", REL_THEME);
    //asprintf(&rel_theme, "%s/%s",cwd, REL_THEME);
	//free(cwd);
	return rel_theme;
}

int get_win_width()
{
    return winwidth;    
}
int get_win_height()
{
    return winheight;    
}
double get_zoom_inc()
{
    return zoominc;    
}
void set_zoom_inc(double newzoominc)
{
    zoominc=newzoominc;    
}
double get_hpan_inc()
{
    return hpaninc;    
}
void set_hpan_inc(double newhpaninc)
{
    hpaninc=newhpaninc;    
}
double get_vpan_inc()
{
    return vpaninc;    
}
void set_vpan_inc(double newvpaninc)
{
    vpaninc=newvpaninc;    
}
int get_lefttrim()
{
    return lefttrim;    
}
void set_lefttrim(int newlefttrim)
{
    lefttrim=newlefttrim;    
}
int get_righttrim()
{
    return righttrim;    
}
void set_righttrim(int newrighttrim)
{
    righttrim=newrighttrim;    
}
int get_toptrim()
{
    return toptrim;    
}
void set_toptrim(int newtoptrim)
{
    toptrim=newtoptrim;    
}
int get_bottomtrim()
{
    return bottomtrim;    
}
void set_bottomtrim(int newbottomtrim)
{
    bottomtrim=newbottomtrim;    
}
void render_cur_page()
{
    char pdfobjstr[20];
    sprintf(pdfobjstr,"pdfobj%d",curpdfobj);    
    Evas_Object *pdfobj=evas_object_name_find(evas,pdfobjstr);
    epdf_page_page_set(page,curpage);
    int width,height;
    epdf_page_size_get (page, &width, &height);
    double fitwidthzoom=((double)get_win_width())/((double)(width-lefttrim-righttrim))*zoom;
    double fitheightzoom=((double)get_win_height())/((double)(height-toptrim-bottomtrim))*zoom;
    
    epdf_page_scale_set (page,fitwidthzoom,fitwidthzoom);
    //epdf_page_scale_set (page,1.0,1.0);
    //epdf_page_scale_set(page,zoom,zoom);
    if(!lefttrim && !righttrim && !toptrim && !bottomtrim)
    {
        epdf_page_render (page,pdfobj);
    }
    else
    {
        epdf_page_render_slice (page,pdfobj,(int)(((double)lefttrim)*fitwidthzoom),(int)(((double)toptrim)*fitwidthzoom),(int)(((double)(width-lefttrim-righttrim))*fitwidthzoom),(int)(((double)(height-toptrim-bottomtrim))*fitwidthzoom));
                             
        
    }
    //fprintf(stderr,"\nwidth=%d,height=%d,ltrim=%d,rtrim=%d,ttrim=%d,btrim=%d,fwzoom=%f,fhzoom=%f\n",width,height,lefttrim,righttrim,toptrim,bottomtrim,fitwidthzoom,fitheightzoom);
}
void *thread_func(void *vptr_args)
{
    if(curpage>=(numpages-1))
        return NULL;

    Evas_Object *pdfobj;
    if(curpdfobj==1)
        pdfobj=evas_object_name_find(evas,"pdfobj2");
    else
        pdfobj=evas_object_name_find(evas,"pdfobj1");
    epdf_page_page_set(page,curpage+1);
    int width,height;
    epdf_page_size_get (page, &width, &height);
    //epdf_page_scale_set (page,((double)get_win_width())/((double)width)*zoom,((double)get_win_height())/((double)height)*zoom);
    double fitwidthzoom=((double)get_win_width())/((double)(width-lefttrim-righttrim))*zoom;
    double fitheightzoom=((double)get_win_height())/((double)(height-toptrim-bottomtrim))*zoom;
    
    epdf_page_scale_set (page,fitwidthzoom,fitwidthzoom);
    //epdf_page_scale_set(page,zoom,zoom);
    if(!lefttrim && !righttrim && !toptrim && !bottomtrim)
    {
        epdf_page_render (page,pdfobj);
    }
    else
    {
        epdf_page_render_slice (page,pdfobj,(int)(((double)lefttrim)*fitwidthzoom),(int)(((double)toptrim)*fitwidthzoom),(int)(((double)(width-lefttrim-righttrim))*fitwidthzoom),(int)(((double)(height-toptrim-bottomtrim))*fitwidthzoom));
                             
        
    }
    prerendering=0;
    return NULL;

}
void pan_cur_page(int panx,int pany)
{
    Evas_Object *pdfobj;
    if(curpdfobj==1)
        pdfobj=evas_object_name_find(evas,"pdfobj1");
    else
        pdfobj=evas_object_name_find(evas,"pdfobj2"); 
    int x,y,w,h;
    evas_object_geometry_get(pdfobj,&x,&y,&w,&h);
    evas_object_move (pdfobj,x+panx,y+pany);
    
    
}
void reset_cur_panning()
{
    Evas_Object *pdfobj;
    if(curpdfobj==1)
        pdfobj=evas_object_name_find(evas,"pdfobj1");
    else
        pdfobj=evas_object_name_find(evas,"pdfobj2"); 
    evas_object_move (pdfobj,0,0);    
}
void reset_next_panning()
{
    Evas_Object *pdfobj;
    if(curpdfobj==1)
        pdfobj=evas_object_name_find(evas,"pdfobj2");
    else
        pdfobj=evas_object_name_find(evas,"pdfobj1"); 
    evas_object_move (pdfobj,0,0);    
}
void ensure_thread_dead()
{
    if(prerendering)
        pthread_join(thread, NULL);
    prerendering=0;

}

void prerender_next_page()
{
    ensure_thread_dead();
    prerendering=1;
    pthread_create(&thread, NULL, thread_func, NULL);
}


void flip_pages()
{
    Evas_Object *active,*inactive;
    if(curpdfobj==1)
    {
        active=evas_object_name_find(evas,"pdfobj1");
        inactive=evas_object_name_find(evas,"pdfobj2");
        curpdfobj=2;
    }
    else
    {
        active=evas_object_name_find(evas,"pdfobj2");
        inactive=evas_object_name_find(evas,"pdfobj1");
        curpdfobj=1;
    }
    evas_object_hide(active);
    evas_object_show(inactive);
}
void next_page()
{
    if(curpage>=(numpages-1))
        return;
    curpage++;
    //pthread_join(thread, NULL);
    ensure_thread_dead();
    reset_next_panning();
    flip_pages();
    prerender_next_page();


}
void prev_page()
{
    if(curpage<=0)
        return;
    curpage--;
    reset_cur_panning();
    render_cur_page();
    
    prerender_next_page();
}

/* GUI */

/* Main key handler */

void main_esc(Evas *e, Evas_Object *obj)
{
    ecore_main_loop_quit();
}

void main_ok(Evas *e, Evas_Object *obj)
{
    Evas_Object *bgobj=evas_object_name_find(evas,"background");
    PreferencesDialog(evas,bgobj);
    
}

void main_shift(Evas *e, Evas_Object *obj)
{
    
}

void main_nav_up(Evas *e, Evas_Object *obj)
{
    
}

void main_nav_down(Evas *e, Evas_Object *obj)
{
    
}

void main_nav_left(Evas *e, Evas_Object *obj)
{
    
    prev_page();
}

void main_nav_right(Evas *e, Evas_Object *obj)
{
    next_page();
}

void main_nav_sel(Evas *e, Evas_Object *obj)
{
    
    
}
void main_nav_menubtn(Evas *e, Evas_Object *obj)
{
    
    
    
}
void main_item(Evas *e, Evas_Object *obj,int index, bool lp)
{
    //int paninc=5;
    if(index==1)
    {
        pan_cur_page((int)((-1)*((double)get_win_width())*hpaninc),0);
    }
    else if(index==2)
    {
        pan_cur_page((int)(((double)get_win_width())*hpaninc),0);
        
    }
    else if(index==3)
    {
        pan_cur_page(0,(int)(((double)get_win_height())*vpaninc));
        
    }
    else if(index==4)
    {
        pan_cur_page(0,(int)((-1)*((double)get_win_height())*vpaninc));
        
    }
    else if(index==5)
    {
        reset_cur_panning();
        
    }
    else if(index==7)
    {
        if((zoom-zoominc)>0)
        {
            zoom-=zoominc;
            render_cur_page();
            prerender_next_page();

        }
        
    }
    else if(index==8)
    {
        zoom+=zoominc;
        render_cur_page();
        prerender_next_page();

        
    }
    else if(index==9)
    {
        prev_page();    
        
    }
    else if(index==0)
    {
        next_page();    
    }
}

static key_handler_info_t main_info =
{
    main_ok,
    main_esc,
    main_nav_up,
    main_nav_down,
    main_nav_left,
    main_nav_right,
    main_nav_sel,
    main_nav_menubtn,
    main_shift,
    main_item
};




int main(int argc, char *argv[])
{
    Ecore_Evas *ee;
    
    Evas_Object *bg,*o1,*o2;

    /* initialize our libraries */
    evas_init();
    ecore_init();
    ecore_evas_init();
    edje_init();
    /* create our Ecore_Evas and show it */
    ee = ecore_evas_software_x11_new(0, 0, 0, 0, 600, 800);
    
    
    ecore_evas_borderless_set(ee, 0);
    ecore_evas_shaped_set(ee, 0);
    ecore_evas_title_set(ee, "LoCoPDF");
    ecore_evas_show(ee);

    /* get a pointer our new Evas canvas */
    evas = ecore_evas_get(ee);

    /* create our white background */
    bg = evas_object_rectangle_add(evas);
    evas_object_color_set(bg, 255, 255, 255, 255);
    evas_object_move(bg, 0, 0);
    evas_object_resize(bg, 600, 800);
    evas_object_name_set(bg, "background");
    evas_object_focus_set(bg, 1);
    set_key_handler(bg,&main_info);
    evas_object_show(bg);
    


    
    filename=argv[1];
    document = epdf_document_new (argv[1]);
    if (!document) {
    // manage error here
        fprintf(stderr,"Error Opening Document");

    }
    numpages=epdf_document_page_count_get(document);
    page = epdf_page_new (document);
    if (!page) {
    // manage error here
        fprintf(stderr,"Error Processing Document");
    }
    curpage=0;
    curpdfobj=1;

    o2 = evas_object_image_add (evas);
    evas_object_move (o2, 0, 0);
    evas_object_name_set(o2, "pdfobj2");
    //evas_object_show (o2);

    o1 = evas_object_image_add (evas);
    evas_object_move (o1, 0, 0);
    evas_object_name_set(o1, "pdfobj1");
    evas_object_show (o1);

    render_cur_page();
    prerender_next_page();


    /* start the main event loop */
    ecore_main_loop_begin();

    /* when the main event loop exits, shutdown our libraries */
    evas_object_del (o1);
    evas_object_del (o2);
    evas_object_del (bg);
    epdf_page_delete (page);
    epdf_document_delete (document);

    edje_shutdown();
    ecore_evas_shutdown();
    ecore_shutdown();
    evas_shutdown();


}
