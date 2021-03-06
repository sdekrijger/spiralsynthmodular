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

// for lrintf()
#define	_ISOC9X_SOURCE	1
#define _ISOC99_SOURCE	1
#include  <math.h>
#define lrintf(flt)             ((int) (flt))

#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#if defined (__FreeBSD__)
	#include <sys/soundcard.h>
#else
#if defined (__NetBSD__) || defined (__OpenBSD__)
	#include <soundcard.h>            /* OSS emulation */
#undef ioctl
#else 	                              /* BSDI, Linux, Solaris */
	#include <sys/soundcard.h>
#endif                                /* __NetBSD__ or __OpenBSD__ */
#endif                                /* __FreeBSD__ */
#include <sys/ioctl.h>
#include <limits.h>

#include "OutputPlugin.h"
#include "OutputPluginGUI.h"
#include <FL/Fl_File_Chooser.H>
#include "SpiralIcon.xpm"

using namespace std;

static const int IN_FREQ  = 0;
static const int IN_PW    = 1;
static const int IN_SHLEN = 2;

static const int OUT_MAIN = 0;

static const HostInfo* host;
OSSClient* OSSClient::m_Singleton = NULL;
int OutputPlugin::m_RefCount=0;
int OutputPlugin::m_NoExecuted=0;
OutputPlugin::Mode OutputPlugin::m_Mode=NO_MODE;

#define CHECK_AND_REPORT_ERROR	if (result<0)         \
								{                     \
									perror("Sound device did not accept settings"); \
									m_OutputOk=false; \
									return false;     \
								}

extern "C" {
SpiralPlugin* SpiralPlugin_CreateInstance()
{
	return new OutputPlugin;
}

const char** SpiralPlugin_GetIcon()
{	
	return SpiralIcon_xpm;
}

int SpiralPlugin_GetID()
{
	return 0x0000;
}

const char * SpiralPlugin_GetGroupName()
{
	return "InputOutput";
}
} /* extern "C" */

///////////////////////////////////////////////////////

OutputPlugin::OutputPlugin() :
m_Volume(1.0f)
{
	m_RefCount++;
	// we are an output.
	m_IsTerminal=true;
        m_NotifyOpenOut=false;
	m_PluginInfo.Name="OSS";
	m_PluginInfo.Width=100;
	m_PluginInfo.Height=100;
	m_PluginInfo.NumInputs=2;
	m_PluginInfo.NumOutputs=2;
	m_PluginInfo.PortTips.push_back("Left Out");
	m_PluginInfo.PortTips.push_back("Right Out");
	//m_PluginInfo.PortTips.push_back("Record Controller");
	m_PluginInfo.PortTips.push_back("Left In");
	m_PluginInfo.PortTips.push_back("Right In");
	m_AudioCH->Register ("Volume", &m_Volume);
        m_AudioCH->Register ("OpenOut", &m_NotifyOpenOut, ChannelHandler::OUTPUT);
}

OutputPlugin::~OutputPlugin()
{
	m_RefCount--;
	if (m_RefCount==0)
	{
		cb_Blocking(m_Parent,false);
		OSSClient::PackUpAndGoHome();
		m_Mode=NO_MODE;
	}
}

PluginInfo &OutputPlugin::Initialise(const HostInfo *Host)
{
	PluginInfo& Info= SpiralPlugin::Initialise(Host);
	host=Host;
	OSSClient::Get()->AllocateBuffer();

	return Info;
}

SpiralGUIType *OutputPlugin::CreateGUI()
{
	return new OutputPluginGUI(m_PluginInfo.Width, m_PluginInfo.Height, this, m_AudioCH, m_HostInfo);
}


bool OutputPlugin::Kill()
{
	m_IsDead=true;
	OSSClient::Get()->Kill();
	m_Mode=CLOSED;
	cb_Blocking(m_Parent,false);
	return true;
}

void OutputPlugin::Reset()
{
	if (m_IsDead) return;
	m_IsDead=true;
	OSSClient::Get()->Kill();
	cb_Blocking(m_Parent,false);
	ResetPorts();
	OSSClient::Get()->AllocateBuffer();
	
	switch (m_Mode)
	{
		case INPUT :
			OSSClient::Get()->OpenRead();
			cb_Blocking(m_Parent,true);
		break;
		case OUTPUT :
			OSSClient::Get()->OpenWrite();
			cb_Blocking(m_Parent,true);
		break;
		case DUPLEX :
			OSSClient::Get()->OpenReadWrite();
			cb_Blocking(m_Parent,true);
		break;
		
		default:{}
	}
	
	m_IsDead=false;
}

void OutputPlugin::Execute()
{
	if (m_IsDead) 
		return;
	
        if (m_Mode==NO_MODE && m_RefCount==1)
	{
		if (OSSClient::Get()->OpenWrite())
		{
			cb_Blocking(m_Parent,true);
			m_Mode=OUTPUT;
                        m_NotifyOpenOut=true;
		}
	}

	if (m_Mode==OUTPUT || m_Mode==DUPLEX)
	{
		OSSClient::Get()->SendStereo(GetInput(0),GetInput(1));
	}

	if (m_Mode==INPUT || m_Mode==DUPLEX)
           OSSClient::Get()->GetStereo(GetOutputBuf(0),GetOutputBuf(1));
}

void OutputPlugin::ExecuteCommands()
{
	if (m_IsDead) 
		return;

	if (m_AudioCH->IsCommandWaiting())
	{
		switch(m_AudioCH->GetCommand())
		{
			case OPENREAD :
				if (OSSClient::Get()->OpenRead())
				{
					m_Mode=INPUT;
					//cb_Blocking(m_Parent,true);
				}
			break;
			case OPENWRITE :
				if (OSSClient::Get()->OpenWrite())
				{
					m_Mode=OUTPUT;
					cb_Blocking(m_Parent,true);
				}
			break;
			case OPENDUPLEX :
				if (OSSClient::Get()->OpenReadWrite())
				{
					m_Mode=DUPLEX;
					cb_Blocking(m_Parent,true);
				}
			break;
			case CLOSE :
				m_Mode=CLOSED;
				cb_Blocking(m_Parent,false);
				OSSClient::Get()->Close();
			break;
			case SET_VOLUME :
                                OSSClient::Get()->SetVolume(m_Volume);
                                break;
                        case CLEAR_NOTIFY:
                                m_NotifyOpenOut=false;
                                break;
                        default: break;
		}
        }
}

void OutputPlugin::ProcessAudio()
{
	if (m_IsDead) 
		return;

	// Only Play() once per set of plugins
	m_NoExecuted--;
	if (m_NoExecuted<=0)
	{
		if (m_Mode==INPUT || m_Mode==DUPLEX) OSSClient::Get()->Read();
		if (m_Mode==OUTPUT || m_Mode==DUPLEX) OSSClient::Get()->Play();
		m_NoExecuted=m_RefCount;
	}
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

OSSClient::OSSClient() :
m_Amp(0.5),
m_Channels(2),
m_ReadBufferNum(0),
m_WriteBufferNum(0),
m_OutputOk(false),
m_IsDead(false)

{
	m_Buffer[0]=NULL;
	m_Buffer[1]=NULL;
	m_InBuffer[0]=NULL;
	m_InBuffer[1]=NULL;
}

//////////////////////////////////////////////////////////////////////

OSSClient::~OSSClient()
{
	Close();
	DeallocateBuffer();
}

//////////////////////////////////////////////////////////////////////

void OSSClient::AllocateBuffer()
{
	if (m_Buffer[0]==NULL)
	{
		m_BufSizeBytes=host->BUFSIZE*m_Channels*2;
		
		// initialise for stereo
		m_Buffer[0] = (short*) calloc(m_BufSizeBytes/2,m_BufSizeBytes);
		m_Buffer[1] = (short*) calloc(m_BufSizeBytes/2,m_BufSizeBytes);
		m_InBuffer[0] = (short*) calloc(m_BufSizeBytes/2,m_BufSizeBytes);
		m_InBuffer[1] = (short*) calloc(m_BufSizeBytes/2,m_BufSizeBytes);
	}
}

void OSSClient::DeallocateBuffer()
{
	if (m_Buffer[0]!=NULL)
	{
		m_BufSizeBytes=0;
		
		// initialise for stereo
		free(m_Buffer[0]);
		free(m_Buffer[1]);
		free(m_InBuffer[0]);
		free(m_InBuffer[1]);
	}
}

//////////////////////////////////////////////////////////////////////

void OSSClient::SendStereo(const Sample *ldata,const Sample *rdata) 
{
	if (m_Channels!=2) return;
	
	int on=0;
	float t;
	for (int n=0; n<host->BUFSIZE; n++)
	{
		if (m_IsDead) return;

		// stereo channels - interleave	
		if (ldata) 
		{
			t=(*ldata)[n]*m_Amp;
			if (t>1) t=1;
			if (t<-1) t=-1;
			m_Buffer[m_WriteBufferNum][on]+=lrintf(t*SHRT_MAX);
		}
		on++;
		
		if (rdata) 
		{
			t=(*rdata)[n]*m_Amp;
			if (t>1) t=1;
			if (t<-1) t=-1;
			m_Buffer[m_WriteBufferNum][on]+=lrintf(t*SHRT_MAX);
		}		
		on++;
	}
}

//////////////////////////////////////////////////////////////////////

void OSSClient::Play()
{
    int BufferToSend=!m_WriteBufferNum;
	
    #if __BYTE_ORDER == BIG_ENDIAN
    for (int n=0; n<host->BUFSIZE*m_Channels; n++)
    {		
		m_Buffer[BufferToSend][n]=(short)((m_Buffer[BufferToSend][n]<<8)&0xff00)|
		                                 ((m_Buffer[BufferToSend][n]>>8)&0xff);
    }
    #endif	
	if (m_OutputOk)
	{
		write(m_Dspfd,m_Buffer[BufferToSend],m_BufSizeBytes);
	}
		
	memset(m_Buffer[BufferToSend],0,m_BufSizeBytes);
	m_WriteBufferNum=BufferToSend;
}

//////////////////////////////////////////////////////////////////////

void OSSClient::GetStereo(Sample *ldata,Sample *rdata) 
{
	if (m_Channels!=2) return;
	
	int on=0;
	for (int n=0; n<host->BUFSIZE; n++)
	{
		if (m_IsDead) return;

		// stereo channels - interleave	
		if (ldata) ldata->Set(n,(m_InBuffer[m_ReadBufferNum][on]*m_Amp)/(float)SHRT_MAX);
		on++;
		if (rdata) rdata->Set(n,(m_InBuffer[m_ReadBufferNum][on]*m_Amp)/(float)SHRT_MAX);
		on++;
	}
}

//////////////////////////////////////////////////////////////////////

void OSSClient::Read()
{
    int BufferToRead=!m_ReadBufferNum;

	if (m_OutputOk)
		read(m_Dspfd,m_InBuffer[BufferToRead],m_BufSizeBytes);
	
    #if __BYTE_ORDER == BIG_ENDIAN
    for (int n=0; n<host->BUFSIZE*m_Channels; n++)
    {		
		m_InBuffer[BufferToRead][n]=(short)((m_InBuffer[BufferToRead][n]<<8)&0xff00)|
		                                   ((m_InBuffer[BufferToRead][n]>>8)&0xff);
    }
    #endif	
    
	m_ReadBufferNum=BufferToRead;
}

//////////////////////////////////////////////////////////////////////

bool OSSClient::Close()
{ 	
	cerr<<"Closing dsp output"<<endl;
	close(m_Dspfd);
	
	return true;
}

//////////////////////////////////////////////////////////////////////

bool OSSClient::OpenWrite()
{ 	
	int result,val;
	cerr<<"Opening dsp output"<<endl;
  
	m_Dspfd = open(host->OUTPUTFILE.c_str(),O_WRONLY);  
	if(m_Dspfd<0) 
	{
		fprintf(stderr,"Can't open audio driver for writing.\n");
		m_OutputOk=false;
		return false;
	}
   
	result = ioctl(m_Dspfd,SNDCTL_DSP_RESET,NULL);

	CHECK_AND_REPORT_ERROR;
			 
	short fgmtsize=0;
	int numfgmts=host->FRAGCOUNT;	 	 
	if (host->FRAGCOUNT==-1) numfgmts=0x7fff;
	
	for (int i=0; i<32; i++)
	{	 	
		if ((host->FRAGSIZE)==(1<<i)) 
		{
			fgmtsize=i; 
			break;
		}
	}

	if (fgmtsize==0)
	{
		cerr<<"Fragment size ["<<host->FRAGSIZE<<"] must be power of two!"<<endl;
		fgmtsize=256;
	}
 	 	
	numfgmts=numfgmts<<16;
	val=numfgmts|(int)fgmtsize;
	result=ioctl(m_Dspfd, SNDCTL_DSP_SETFRAGMENT, &val);
	CHECK_AND_REPORT_ERROR;
	
	val = 1;
	result = ioctl(m_Dspfd, SOUND_PCM_WRITE_CHANNELS, &val);
	CHECK_AND_REPORT_ERROR;
	
	val = AFMT_S16_LE;
	result = ioctl(m_Dspfd,SNDCTL_DSP_SETFMT,&val);	 
	CHECK_AND_REPORT_ERROR;

	if (m_Channels==2) val=1;
	else val=0;
	result = ioctl(m_Dspfd,SNDCTL_DSP_STEREO,&val);
	CHECK_AND_REPORT_ERROR;
		
	val = host->SAMPLERATE;
	result = ioctl(m_Dspfd,SNDCTL_DSP_SPEED,&val);
	CHECK_AND_REPORT_ERROR;
	
	m_OutputOk=true;
	return true;
}

//////////////////////////////////////////////////////////////////////

bool OSSClient::OpenRead()
{ 	
	int result,val;
  
	cerr<<"Opening dsp input"<<endl;

	m_Dspfd = open(host->OUTPUTFILE.c_str(),O_RDONLY);  
	if(m_Dspfd<0) 
	{
		fprintf(stderr,"Can't open audio driver for reading.\n");
		m_OutputOk=false;
		return false;
	}
   
	result = ioctl(m_Dspfd,SNDCTL_DSP_RESET,NULL);
	CHECK_AND_REPORT_ERROR;
	
	val = 1;
	result = ioctl(m_Dspfd, SOUND_PCM_READ_CHANNELS, &val);
	CHECK_AND_REPORT_ERROR;
	
	val = AFMT_S16_LE;
	result = ioctl(m_Dspfd,SNDCTL_DSP_SETFMT,&val);	 
	CHECK_AND_REPORT_ERROR;
	
	val = host->SAMPLERATE;
	result = ioctl(m_Dspfd,SNDCTL_DSP_SPEED,&val);
	CHECK_AND_REPORT_ERROR;
	m_OutputOk=true;
	
	return true;
}

//////////////////////////////////////////////////////////////////////

bool OSSClient::OpenReadWrite()
{ 	
	int result,val;
	cerr<<"Opening dsp output (duplex)"<<endl;
  
	m_Dspfd = open(host->OUTPUTFILE.c_str(),O_RDWR);  
	if(m_Dspfd<0) 
	{
		fprintf(stderr,"Can't open audio driver for writing.\n");
		m_OutputOk=false;
		return false;
	}
   
	result = ioctl(m_Dspfd,SNDCTL_DSP_RESET,NULL);

	CHECK_AND_REPORT_ERROR;
			 
	short fgmtsize=0;
	int numfgmts=host->FRAGCOUNT;	 	 
	if (host->FRAGCOUNT==-1) numfgmts=0x7fff;
	
	for (int i=0; i<32; i++)
	{	 	
		if ((host->FRAGSIZE)==(1<<i)) 
		{
			fgmtsize=i; 
			break;
		}
	}

	if (fgmtsize==0)
	{
		cerr<<"Fragment size ["<<host->FRAGSIZE<<"] must be power of two!"<<endl;
		fgmtsize=256;
	}
 	 	
	numfgmts=numfgmts<<16;
	val=numfgmts|(int)fgmtsize;
	result=ioctl(m_Dspfd, SNDCTL_DSP_SETFRAGMENT, &val);
	CHECK_AND_REPORT_ERROR;
	
	val = 1;
	result = ioctl(m_Dspfd, SOUND_PCM_WRITE_CHANNELS, &val);
	CHECK_AND_REPORT_ERROR;
	
	val = AFMT_S16_LE;
	result = ioctl(m_Dspfd,SNDCTL_DSP_SETFMT,&val);	 
	CHECK_AND_REPORT_ERROR;

	if (m_Channels==2) val=1;
	else val=0;
	result = ioctl(m_Dspfd,SNDCTL_DSP_STEREO,&val);
	CHECK_AND_REPORT_ERROR;
		
	val = host->SAMPLERATE;
	result = ioctl(m_Dspfd,SNDCTL_DSP_SPEED,&val);
	CHECK_AND_REPORT_ERROR;
	m_OutputOk=true;
	
    return true;
}
