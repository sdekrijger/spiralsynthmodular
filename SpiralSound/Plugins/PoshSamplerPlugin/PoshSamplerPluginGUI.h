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

#ifndef POSH_SAMP_GUI_H
#define POSH_SAMP_GUI_H

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Counter.H>
#include "PoshSamplerPlugin.h"
#include "../SpiralPluginGUI.h"
#include "../Widgets/Fl_Knob.H"

class Fl_WaveDisplay : public Fl_Widget {
   public:
      Fl_WaveDisplay (int x,int y,int w,int h, const char* Name);
      ~Fl_WaveDisplay();
      virtual void draw();
      virtual int handle (int event);
      void SetSample (const float* s, long len);
      void ClearSample (void);
      long GetRangeStart()      { return m_StartPos; }
      long GetRangeEnd()        { return m_EndPos; }
      long GetViewStart()       { return m_ViewStart; }
      void SetViewStart(long s) { m_ViewStart=s; }
      long GetViewEnd()         { return m_ViewEnd; }
      void SetViewEnd(long s)   { m_ViewEnd=s; }
      void SetPlayPos(long s)   { m_PlayPos=s; if(m_PosMarker) redraw(); }
      void SetPlayStart(long s) { m_PlayStart=s; }
      long GetPlayStart()       { return m_PlayStart; }
      void SetLoopStart(long s) { m_LoopStart=s; }
      long GetLoopStart()       { return m_LoopStart; }
      void SetLoopEnd(long s)   { m_LoopEnd=s; }
      long GetLoopEnd()         { return m_LoopEnd; }
      void SetPosMarker(bool s) { m_PosMarker=s; }
      void ZoomIn();
      void ZoomOut();
      void SetColours (unsigned b, unsigned f, unsigned s, unsigned i, unsigned m) {
           m_BGColour=(Fl_Color)b;  m_FGColour=(Fl_Color)f;  m_SelColour=(Fl_Color)s;
           m_IndColour=(Fl_Color)i; m_MrkColour=(Fl_Color)m;
      }
   private:
      Fl_Color m_BGColour, m_FGColour, m_SelColour, m_IndColour, m_MrkColour;
      Sample *m_Sample;
      long m_StartPos, m_EndPos, m_ViewStart, m_ViewEnd;
      long m_PlayPos, m_PlayStart, m_LoopStart, m_LoopEnd;
      bool m_PosMarker;
};


class PoshSamplerPluginGUI : public SpiralPluginGUI {
   public:
      PoshSamplerPluginGUI (int w, int h, PoshSamplerPlugin *o, ChannelHandler *ch, const HostInfo *Info);
      virtual void UpdateValues (SpiralPlugin *o);
      virtual void Update();
      void SetPlayPos (long s) { m_Display->SetPlayPos(s); }
      int GetCurrentSample() { return (int)m_SampleNum->value(); }
   protected:
      const std::string GetHelpText(const std::string &loc);
   private:
      void UpdateSampleDisplay(int num);
      char m_TextBuf[256];
      int Numbers[NUM_SAMPLES];
      bool m_UpdateMe;
      Fl_Knob *m_Volume, *m_Pitch, *m_Octave;
      Fl_Button *m_Load, *m_Save, *m_Record, *m_Loop, *m_PingPong, *m_PosMarker, *m_Retrig;
      Fl_Button *m_ZoomIn, *m_ZoomOut, *m_Cut, *m_Copy, *m_Paste, *m_Mix, *m_Crop, *m_Reverse, *m_Amp;
      Fl_Counter *m_Note, *m_SampleNum;
      Fl_WaveDisplay *m_Display;
      //// Callbacks ////
      inline void cb_Load_i (Fl_Button *o, void *v);
      static void cb_Load (Fl_Button *o, void *v);
      inline void cb_Save_i (Fl_Button *o, void *v);
      static void cb_Save (Fl_Button *o, void *v);
      inline void cb_Record_i (Fl_Button *o, void *v);
      static void cb_Record (Fl_Button *o, void *v);
      inline void cb_Loop_i (Fl_Button *o, void *v);
      static void cb_Loop (Fl_Button *o, void *v);
      inline void cb_PingPong_i (Fl_Button *o, void *v);
      static void cb_PingPong (Fl_Button *o, void *v);
      inline void cb_PosMarker_i (Fl_Button *o, void *v);
      static void cb_PosMarker (Fl_Button *o, void *v);
      inline void cb_Retrig_i (Fl_Button *o, void *v);
      static void cb_Retrig (Fl_Button *o, void *v);
      inline void cb_Volume_i (Fl_Knob *o, void *v);
      static void cb_Volume (Fl_Knob *o, void *v);
      inline void cb_Pitch_i (Fl_Knob *o, void *v);
      static void cb_Pitch (Fl_Knob *o, void *v);
      inline void cb_Octave_i (Fl_Knob *o, void *v);
      static void cb_Octave (Fl_Knob *o, void *v);
      inline void cb_Note_i (Fl_Counter *o, void *v);
      static void cb_Note (Fl_Counter *o, void *v);
      inline void cb_SampleNum_i (Fl_Counter *o, void *v);
      static void cb_SampleNum (Fl_Counter *o, void *v);
      inline void cb_Cut_i (Fl_Button *o, void *v);
      static void cb_Cut (Fl_Button *o, void *v);
      inline void cb_Copy_i (Fl_Button *o, void *v);
      static void cb_Copy (Fl_Button *o, void *v);
      inline void cb_Paste_i (Fl_Button *o, void *v);
      static void cb_Paste (Fl_Button *o, void *v);
      inline void cb_Mix_i (Fl_Button *o, void *v);
      static void cb_Mix (Fl_Button *o, void *v);
      inline void cb_Crop_i (Fl_Button *o, void *v);
      static void cb_Crop (Fl_Button *o, void *v);
      inline void cb_Reverse_i (Fl_Button *o, void *v);
      static void cb_Reverse (Fl_Button *o, void *v);
      inline void cb_Amp_i (Fl_Button *o, void *v);
      static void cb_Amp (Fl_Button *o, void *v);
      inline void cb_ZoomIn_i (Fl_Button *o, void *v);
      static void cb_ZoomIn (Fl_Button *o, void *v);
      inline void cb_ZoomOut_i (Fl_Button *o, void *v);
      static void cb_ZoomOut (Fl_Button *o, void *v);
      inline void cb_WaveDisplay_i  (Fl_WaveDisplay *o, void *v);
      static void cb_WaveDisplay  (Fl_WaveDisplay *o, void *v);
};

#endif
