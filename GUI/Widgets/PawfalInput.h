/*  PawfalInput Fltk Dialog
 *  Copyleft (C) 2001 Dan Bethell <dan@pawfal.org>
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
#ifndef PAWFALINPUT_H
#define PAWFALINPUT_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Pixmap.H>

static const char * tv_xpm[] = {
"30 30 4 1",
" 	c None",
".	c #000000",
"+	c #6E6E6E",
"@	c #FFFFFF",
"..............................",
"..............................",
"..............................",
"..............................",
"..............................",
".....++++++++++++++++++++.....",
".....++++++@@@@@@@@++++++.....",
".....++++++@@@+++@@++++++.....",
".....++++++++++++@@++++++.....",
".....++++++++++++@@++++++.....",
".....+++++++++@@@@@++++++.....",
".....+++++++++@@+++++++++.....",
".....++++++++++++++++++++.....",
".....+++++++++@@+++++++++.....",
".....++++++++++++++++++++.....",
"..............................",
"..............................",
"..............................",
"..............................",
"..............................",
"          ..........          ",
"          ..........          ",
"          ..........          ",
"          ..........          ",
"          ..........          ",
"..............................",
"..............................",
"..............................",
"..............................",
".............................."};

class PawfalInput : public Fl_Window
{
    int handle(int);
    void draw();
        
    static void ok_cb(Fl_Button *, void *);
    static void cancel_cb(Fl_Button *, void *);

  public:
    PawfalInput(int, int, int, int, const char *, const char *);
    PawfalInput(int, int, const char *, const char *);
    ~PawfalInput(); 
    
    bool go();
    const char *getText(){return input->value();}
    
  private:    
    Fl_Pixmap *logo;
    Fl_Box *lbl;
    Fl_Input *input;
    Fl_Button *ok, *cancel;
    bool keyhit;
};

bool Pawfal_Input(const char *a, const char *b, char *out);

#endif
