/*  SpiralSound
 *  Copyleft (C) 2001 David Griffiths <dave@pawfal.org>
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

#include "../SpiralPlugin.h"
#include <FL/Fl_Pixmap.H>

#ifndef AnotherFilterPLUGIN
#define AnotherFilterPLUGIN

static const int NUM_CHANNELS = 4;

// A State Variable Filter
class AnotherFilterPlugin : public SpiralPlugin
{
public:
 	AnotherFilterPlugin();
	virtual ~AnotherFilterPlugin();
	
	virtual PluginInfo &Initialise(const HostInfo *Host);
	virtual SpiralGUIType *CreateGUI();
	virtual void Execute();
	virtual void StreamOut(ostream &s);
	virtual void StreamIn(istream &s);
	
	float GetCutoff()    { return Cutoff; }
	float GetResonance() { return Resonance;  } 
	
	void Randomise();
	void Clear();

private:
	float Cutoff, Resonance;
		
	// Calculation
	double w,q,r,c,vibrapos,vibraspeed;
	 	
	friend istream &operator>>(istream &s, AnotherFilterPlugin &o);
	friend ostream &operator<<(ostream &s, AnotherFilterPlugin &o);
};
istream &operator>>(istream &s, AnotherFilterPlugin &o);
ostream &operator<<(ostream &s, AnotherFilterPlugin &o);

#endif
