/*  SpiralLoops
 *  Copyleft (C) 2000 David Griffiths <dave@pawfal.org>
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
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include "../../Widgets/Fl_Knob.H"

#include "GUIBase.h"
#include "../SpiralSound/Delay.h"

#ifndef DELAYGUI
#define DELAYGUI

class DelayGUI : public GUIBase
{
public:
	DelayGUI(Delay *o);
	~DelayGUI();
	
	virtual void CreateGUI(int xoff, int yoff, char *name);
	virtual void UpdateValues();
	Fl_Group* GetGroup() { return GUIDelayGroup; }
	
	Delay *m_delay;	
	
private:

	Fl_Group 		*GUIDelayGroup;
	Fl_Knob         *DelayA;
	Fl_Knob 		*Feedback;
	Fl_Button       *Bypass;
	
	//// Callbacks ////
	
	inline void cb_Delay_i(Fl_Knob* o, void* v);
    static void cb_Delay(Fl_Knob*, void*);
	inline void cb_Feedback_i(Fl_Knob* o, void* v);
	static void cb_Feedback(Fl_Knob* o, void* v);
	inline void cb_Bypass_i(Fl_Button* o, void* v);
	static void cb_Bypass(Fl_Button* o, void* v);
	 
};

#endif
