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
#include "../../RiffWav.h"
#include <FL/Fl_Pixmap.H>

#ifndef OutputPLUGIN
#define OutputPLUGIN

class OSSClient
{
public:
	static OSSClient *Get()       { if(!m_Singleton) m_Singleton=new OSSClient; return m_Singleton; }
	static void PackUpAndGoHome() { if(m_Singleton)  { delete m_Singleton; m_Singleton=NULL; } }
	~OSSClient();

	void    AllocateBuffer();
	void    DeallocateBuffer();
	void    SendStereo(const Sample *ldata,const Sample *rdata);
	void    GetStereo(Sample *ldata,Sample *rdata);
	void    SetVolume(float s) {m_Amp=s;}
	void    SetNumChannels(int s) {m_Channels=s;}
	float   GetVolume() {return m_Amp;}
	void    Play();
	void    Read();

	short  *GetBuffer() {return m_Buffer[m_WriteBufferNum];}

	bool 	OpenReadWrite();
	bool    OpenWrite();
	bool    OpenRead();
	bool    Close();
	void    Kill() { m_IsDead = true; m_OutputOk=false; PackUpAndGoHome(); }
private:
	static OSSClient* m_Singleton;

 	OSSClient();

	short  *m_Buffer[2];
	short  *m_InBuffer[2];
	int     m_BufSizeBytes;
	int     m_Dspfd;
	float   m_Amp;
	int     m_Channels;

	int    	m_ReadBufferNum;
	int     m_WriteBufferNum;
	bool    m_OutputOk;
	bool	m_IsDead;
};


class OutputPlugin : public AudioDriver
{
public:
	enum Mode {NO_MODE,INPUT,OUTPUT,DUPLEX,CLOSED};

 	OutputPlugin();
	virtual ~OutputPlugin();

	virtual PluginInfo& Initialise(const HostInfo *Host);
	virtual SpiralGUIType*  CreateGUI();

	/* General Plugin Function */
	virtual	void	Execute();
	virtual void	ExecuteCommands();

	virtual bool	Kill();
	virtual void	Reset();
	
	/* Audio Driver Specific Functions */
	virtual bool			IsAudioDriver() { return true; }
	virtual AudioProcessType	ProcessType() { return AudioDriver::ALWAYS; }		
	virtual void			ProcessAudio();

	/* OSS Plugin Specific Functions */
	enum GUICommands {NONE, OPENREAD, OPENWRITE, OPENDUPLEX, CLOSE, SET_VOLUME, CLEAR_NOTIFY};
	float m_Volume;

	Mode GetMode() { return m_Mode; }

	/* OSS Plugin Streaming - soon to be obsolete and for backward compatibility only*/
	virtual void	    StreamOut(std::ostream &s) {}
	virtual void	    StreamIn(std::istream &s)  {}
private:
	static int m_RefCount;
	static int m_NoExecuted;
	static Mode m_Mode;
        bool m_NotifyOpenOut;
	bool m_CheckedAlready;
};

#endif
