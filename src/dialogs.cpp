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


#include "dialogs.h"
#include "choicebox.h"

#include <iostream>
#include <cstdlib>
#include <cstring>



// Options dialogs



void preferences_choicehandler(Evas *e, Evas_Object *parent,int choice, bool lp)
{
	
}

void PreferencesDialog(Evas *e, Evas_Object *obj)
{
	const char *initchoices[] = { 		
		"1. Hor. Panning",
		"2. Ver. Panning",
		"3. Trimming",
		"4. Zoom Factor",
	};

	const char *values[] = {
		"bla",	
		"bla",
		"bla",
		"bla",
	};

	Evas_Object * preferenceschoicebox=init_choicebox(e,initchoices, values, 4, preferences_choicehandler, "Settings",obj, true);
    //evas_object_stack_above(preferenceschoicebox, evas_object_top_get(e));
    
}

