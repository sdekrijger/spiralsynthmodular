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

#ifndef OscillatorPLUGIN
#define OscillatorPLUGIN

class MidiPlugin : public SpiralPlugin
{
public:
 	MidiPlugin();
	virtual ~MidiPlugin();

	virtual PluginInfo& Initialise(const HostInfo *Host);
	virtual SpiralGUIType*  CreateGUI();
	virtual void 		Execute();
	virtual void	    StreamOut(ostream &s);
	virtual void	    StreamIn(istream &s);
	
	// has to be defined in the plugin	
	virtual void UpdateGUI() { Fl::check(); }
	
	void SetDeviceNum(int s) { m_DeviceNum=s; }
	int  GetDeviceNum()      { return m_DeviceNum; }
	void SetNoteCut(bool s)  { m_NoteCut=s; }
	bool GetNoteCut()        { return m_NoteCut; }
	void SetContinuousNotes(bool s)  { m_ContinuousNotes=s; }
	bool GetContinuousNotes()        { return m_ContinuousNotes; }
	void AddControl(int s,const string &Name);
	void DeleteControl();
	
private:
	
	int m_DeviceNum;

	float m_NoteLevel;
	float m_TriggerLevel;
	float m_PitchBendLevel;
	float m_ChannelPressureLevel;
	float m_AfterTouchLevel;
	float m_ControlLevel[128];
	bool  m_NoteCut;
	bool  m_ContinuousNotes;
	int	  m_CurrentNote;
	static int m_RefCount;
	
	vector<int> m_ControlList;
};

#endif
