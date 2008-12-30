/***************************************************************************
 *   Copyright (C) 2008 by Marc Lajoie   *
 *   marc@gatherer   *
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
#include <epdf/Epdf.h>

using namespace std;

int main(int argc, char *argv[])
{
    Ecore_Evas *ee;
    Evas *evas;
    Evas_Object *bg,*o;

    /* initialize our libraries */
    evas_init();
    ecore_init();
    ecore_evas_init();

    /* create our Ecore_Evas and show it */
    ee = ecore_evas_software_x11_new(0, 0, 0, 0, 600, 800);
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
    evas_object_show(bg);



    Epdf_Document *document;
    Epdf_Page     *page;
    char          *filename;

    document = epdf_document_new (argv[1]);
    if (!document) {
    // manage error here
        fprintf(stderr,"Error Opening Document");

    }
    page = epdf_page_new (document);
    if (!page) {
    // manage error here
        fprintf(stderr,"Error Processing Document");
    }
    o = evas_object_image_add (evas);
    evas_object_move (o, 0, 0);
    epdf_page_render (page, o);
    evas_object_show (o);

    /* start the main event loop */
    ecore_main_loop_begin();

    /* when the main event loop exits, shutdown our libraries */
    evas_object_del (o);
    epdf_page_delete (page);
    epdf_document_delete (document);


    ecore_evas_shutdown();
    ecore_shutdown();
    evas_shutdown();


}
