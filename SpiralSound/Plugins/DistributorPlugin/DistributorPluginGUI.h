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

#include <FL/Fl.H>
#include <FL/Fl_Counter.H>
#include "DistributorPlugin.h"
#include "../SpiralPluginGUI.h"

#ifndef DistributorGUI
#define DistributorGUI

class DistributorPluginGUI : public SpiralPluginGUI
{
public:
	DistributorPluginGUI(int w, int h, DistributorPlugin *o,ChannelHandler *ch,const HostInfo *Info);
	
	virtual void UpdateValues(SpiralPlugin *o);
	virtual void Update ();

protected:
	const std::string GetHelpText(const std::string &loc);	

private:
	Fl_Counter *m_Chans;

	inline void cb_Chans_i (Fl_Counter* o);
	static void cb_Chans(Fl_Counter* o, DistributorPluginGUI* v)  {v->cb_Chans_i(o);}
};

#endif

