/*  SpiralPlugin
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

#include "KeyboardPluginGUI.h"
#include <FL/fl_draw.h>
#include <FL/fl_file_chooser.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Scroll.H>

static const int GUI_COLOUR = 179;
static const int GUIBG_COLOUR = 144;
static const int GUIBG2_COLOUR = 145;

static int NKEYS = 30;
static char KEYMAP[30]={'z','s','x','d','c','v','g','b','h','n','j','m','q',
						'2','w','3','e','r','5','t','6','y','7','u','i','9',
						'o','0','p','['};

////////////////////////////////////////////////////////////////////////

KeyboardPluginGUI::KeyboardPluginGUI(int w, int h,KeyboardPlugin *o,ChannelHandler *ch,const HostInfo *Info) :
SpiralPluginGUI(w,h,o,ch),
m_Last(-1),
m_Oct(4)
{		
	Fl_Scroll *Scroll = new Fl_Scroll(2,20,w-4,h-20);
	Fl_Group *Group = new Fl_Group(0,20,500,h-40);
	Group->box(FL_FLAT_BOX);
	Group->user_data(this);
	Scroll->add(Group);

	int KeyWidth=10,Note,Pos=0,Count=0;

	for (int n=0; n<NUM_KEYS; n++)
	{
		m_Num[n]=n;
		
		Note = n%12;
		if (Note!=1 && Note!=3 && Note!=6 && Note!=8 && Note!=10) 
		{
			Count++;
			Pos=Count*KeyWidth;
			m_Key[n] = new Fl_Button(Pos,20,KeyWidth,50,"");
			m_Key[n]->box(FL_THIN_UP_BOX);
			m_Key[n]->labelsize(10);
			m_Key[n]->when(FL_WHEN_CHANGED);

			if (Note==0) 
			{
				int Num=n/12;
				sprintf(m_Label[n],"%d",Num);
				m_Key[n]->label(m_Label[n]);
				m_Key[n]->align(FL_ALIGN_BOTTOM|FL_ALIGN_INSIDE);
			}
			m_Key[n]->color(FL_WHITE);
			m_Key[n]->selection_color(FL_WHITE);
			m_Key[n]->callback((Fl_Callback*)cb_Key, &m_Num[n]);
			Group->add(m_Key[n]);
		}	
	}
	
	Count=0;
	for (int n=0; n<NUM_KEYS; n++)
	{
		Note = n%12;
		if (Note==1 || Note==3 || Note==6 || Note==8 || Note==10) 
		{
			m_Key[n] = new Fl_Button(Pos+5,20,KeyWidth,30,"");
			m_Key[n]->box(FL_THIN_UP_BOX);
			m_Key[n]->labelsize(10);
			m_Key[n]->when(FL_WHEN_CHANGED);
			m_Key[n]->color(FL_BLACK);
			m_Key[n]->selection_color(FL_BLACK);
			m_Key[n]->callback((Fl_Callback*)cb_Key, &m_Num[n]);
			Group->add(m_Key[n]);
		}
		else
		{
			Count++;
			Pos=Count*KeyWidth;
		}
	}
	Group->position(-100,20);
	Group->end();
	Scroll->end();
}
	
void KeyboardPluginGUI::Update()
{	
	int Volume=0,Note=0,EventDevice=0;
	if (Fl::event_key(FL_F+1)) m_Oct=0; 
	if (Fl::event_key(FL_F+2)) m_Oct=1; 
	if (Fl::event_key(FL_F+3)) m_Oct=2; 
	if (Fl::event_key(FL_F+4)) m_Oct=3; 
	if (Fl::event_key(FL_F+5)) m_Oct=4; 
	if (Fl::event_key(FL_F+6)) m_Oct=5; 
	if (Fl::event_key(FL_F+7)) m_Oct=6; 
	if (Fl::event_key(FL_F+8)) m_Oct=7; 
	if (Fl::event_key(FL_F+9)) m_Oct=8; 
	if (Fl::event_key(FL_F+10)) m_Oct=9; 
	if (Fl::event_key(FL_F+11)) m_Oct=10; 

	int  note=0;
	char KeyChar=0;
	bool KeyPressed=false;

	for (int key=0; key<NKEYS; key++)
	{	
		KeyChar=KEYMAP[key];
		
		// check if a key's been pressed
		if (Fl::event_key(KeyChar)) 
		{
			KeyPressed=true;

			Volume = 127;
			Note=(m_Oct*12)+note;
			if (m_Last!=Note)
			{
				if (m_Last!=-1) 
				{	
					// turn off the old one
					m_Key[m_Last]->value(0);
					m_Key[m_Last]->parent()->redraw();
					m_GUICH->SetCommand(KeyboardPlugin::NOTE_OFF); 
					m_GUICH->Wait();
				}
				
				m_Last = Note;
				m_GUICH->Set("Note",Note);
				m_GUICH->SetCommand(KeyboardPlugin::NOTE_ON); 
				m_Key[Note]->value(1);
				m_Key[m_Last]->parent()->redraw();
			}
		}
		else // it's not pressed down 
		{
			//see if the note was pressed down last time			
			Note=(m_Oct*12)+note;
			
			if (m_Last==Note)
			{
				m_Key[m_Last]->value(0);
				m_Key[m_Last]->parent()->redraw();
				m_GUICH->SetCommand(KeyboardPlugin::NOTE_OFF); 
				m_Last=-1;
			} 
		}
		note++;
	}
}

void KeyboardPluginGUI::UpdateValues(SpiralPlugin *o)
{
	KeyboardPlugin *Plugin = (KeyboardPlugin*)o;
}
	
//// Callbacks ////
inline void KeyboardPluginGUI::cb_Key_i(Fl_Button* o, void* v) 
{ 
	int k=*(int*)(v);
	if (o->value()) 
	{
		m_GUICH->Set("Note",k);
		m_GUICH->SetCommand(KeyboardPlugin::NOTE_ON); 
	}
	else
	{
		m_GUICH->SetCommand(KeyboardPlugin::NOTE_OFF); 
	}
	parent()->redraw();
}
void KeyboardPluginGUI::cb_Key(Fl_Button* o, void* v) 
{ ((KeyboardPluginGUI*)(o->parent()->user_data()))->cb_Key_i(o,v);}