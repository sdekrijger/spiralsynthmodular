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

#include "FlipflopPluginGUI.h"
#include <FL/fl_draw.h>
#include <FL/fl_draw.H>

static const int GUI_COLOUR = 179;
static const int GUIBG_COLOUR = 144;
static const int GUIBG2_COLOUR = 145;

////////////////////////////////////////////

FlipflopPluginGUI::FlipflopPluginGUI(int w, int h,FlipflopPlugin *o,ChannelHandler *ch,const HostInfo *Info) :
SpiralPluginGUI(w,h,o,ch)
{	
	end();
}



void FlipflopPluginGUI::UpdateValues(SpiralPlugin *o)
{
}

//// Callbacks ////
const string FlipflopPluginGUI::GetHelpText(const string &loc){
    return string("")
    + "Converts a momentary pulse into a sustained one.";
}