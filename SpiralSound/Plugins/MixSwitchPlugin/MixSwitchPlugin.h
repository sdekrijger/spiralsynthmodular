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

#ifndef MixSwitchPLUGIN
#define MixSwitchPLUGIN

class MixSwitchPlugin : public SpiralPlugin {
  public:
    MixSwitchPlugin ();
    virtual ~MixSwitchPlugin ();
    virtual PluginInfo& Initialise (const HostInfo *Host);
    virtual SpiralGUIType* CreateGUI ();
    virtual void Execute ();
    virtual void Reset();
    virtual void ExecuteCommands();
    virtual void StreamOut (std::ostream &s);
    virtual void StreamIn (std::istream &s);
    int GetSwitch (void) { return m_SwitchPos; }
    int GetChans (void) { return m_PluginInfo.NumInputs - 2; }
    enum GUICommands { NONE, SETCHANS };
    struct GUIArgs {
      int Chans, Switch, Echo;
      bool Auto;
    };
  private:
    GUIArgs m_GUIArgs;
    void SetChans (int n);
    int m_SwitchPos;
    bool m_Triggered;
    void CreatePorts (int n = 2, bool AddPorts = false);
    friend std::istream &operator>> (std::istream &s, MixSwitchPlugin &o);
    friend std::ostream &operator<< (std::ostream &s, MixSwitchPlugin &o);
};

std::istream &operator>> (std::istream &s, MixSwitchPlugin &o);
std::ostream &operator<< (std::ostream &s, MixSwitchPlugin &o);

#endif
