/*  SpiralLoops
 *  Copyright (C) 2000 David Griffiths <dave@blueammonite.f9.co.uk>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/ 

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include "Widgets/Fl_Knob.H"
#include <FL/Fl_Button.H>
#include <FL/Fl_Slider.H>

#include "GUIBase.h"
#include "../SpiralSound/Filter.h"

#ifndef FILTERGUI
#define FILTERGUI

class FilterGUI : public GUIBase
{
public:
	FilterGUI(Filter *o);
	
	virtual void CreateGUI(int xoff, int yoff, char *name);	
	virtual void UpdateValues();

	Filter *m_filter;	
	
private:

	Fl_Group 		*GUIFilterGroup;
	Fl_Slider 		*Cutoff;
	Fl_Knob 		*Resonance;
	Fl_Button       *RevCutoff;
	Fl_Button		*RevResonance;
	
	//// Callbacks ////
	
	inline void cb_Cutoff_i(Fl_Slider* o, void* v);
    static void cb_Cutoff(Fl_Slider*, void*);
	inline void cb_Resonance_i(Fl_Knob* o, void* v);
	static void cb_Resonance(Fl_Knob* o, void* v); 
	inline void cb_RevCutoff_i(Fl_Button* o, void* v);
    static void cb_RevCutoff(Fl_Button*, void*);
	inline void cb_RevResonance_i(Fl_Button* o, void* v);
	static void cb_RevResonance(Fl_Button* o, void* v); 
};

#endif
