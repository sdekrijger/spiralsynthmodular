/*  SpiralSynthModular
 *  Copyleft (C) 2002 David Griffiths <dave@pawfal.org>
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream.h>
#include <stdlib.h>
#include <FL/Fl.H>
#include <FL/Fl_Tooltip.h>
#include <unistd.h>

#include "SpiralSynthModular.h"

pthread_t loopthread;
SynthModular *synth;

int pthread_create_realtime (pthread_t *new_thread,
			 void *(*start)(void *), void *arg,
			 int priority);

void audioloop(void* o)
{
	while(1)
	{
		if (!synth->CallbackMode()) 
		{
			synth->Update();
		}
		else
		{
			usleep(1000000);
		}
	}
}

int main(int argc, char **argv)
{	
	srand(time(NULL));
	SpiralSynthModularInfo::Get()->LoadPrefs();
	
	bool GUI = true;
	bool FIFO = false;
	
	// get args
    string cmd_filename="";
    bool cmd_specd = false;
	
	// parse the args
    if (argc>1) 
	{
		for (int a=1; a<argc; a++)
		{
			if (!strcmp(argv[a],"--NoGUI")) GUI = false;
			else if (!strcmp(argv[a],"--SHED_FIFO")) FIFO = true;
			else 
			{
				cmd_filename = argv[1];
				cmd_specd = true;
			}
		}
    }	
	
	Fl::visual(FL_DOUBLE|FL_RGB);
	
	synth=new SynthModular;
	
	// setup the synth
	Fl_Window* win = synth->CreateWindow();
	synth->LoadPlugins();
	win->xclass("");
	if (GUI) win->show(argc, argv); // prevents stuff happening before the plugins have loaded
	
	// set some fltk defaults
	Fl_Tooltip::size(10);	
	Fl::visible_focus(false);
			
	// spawn the audio thread
	int ret;
	if (FIFO) ret=pthread_create_realtime(&loopthread,(void*(*)(void*))audioloop,NULL,10);
	else ret=pthread_create(&loopthread,NULL,(void*(*)(void*))audioloop,NULL);

	// do we need to load a patch on startup? 
    if (cmd_specd) synth->LoadPatch(cmd_filename.c_str());        
	
	if (!GUI)
	{	
		// if there is no gui needed, there is nothing more to do here
		Fl::check();
		for (;;) sleep(1);
	}
	
    for (;;)
	{
    	if (!Fl::check()) break;    	
		synth->UpdatePluginGUIs(); // deletes any if necc
		usleep(10000);
  	}
	
	//pthread_cancel(loopthread);
	delete synth;	
	return 1;
}

// nicked from Paul Barton-Davis' Ardour code :)
int pthread_create_realtime (pthread_t *new_thread,
			 void *(*start)(void *), void *arg,
			 int priority)

{	
	pthread_attr_t *rt_attributes;
	struct sched_param *rt_param;
	int retval;

	rt_attributes = (pthread_attr_t *) malloc (sizeof (pthread_attr_t));
	rt_param = (struct sched_param *) malloc (sizeof (struct sched_param));

	pthread_attr_init (rt_attributes);

	if (seteuid (0)) {
		cerr << "Cannot obtain root UID for RT scheduling operations"
		      << endl;
		return -1;

	} else {

	    if (pthread_attr_setschedpolicy (rt_attributes, SCHED_FIFO)) {
		cerr << "Cannot set FIFO scheduling attributes for RT thread" << endl;

	    }
	    
	    if (pthread_attr_setscope (rt_attributes, PTHREAD_SCOPE_SYSTEM)) {
		cerr << "Cannot set scheduling scope for RT thread" << endl;

	    }
	    
	    rt_param->sched_priority = priority;
	    if (pthread_attr_setschedparam (rt_attributes, rt_param)) {
		cerr << "Cannot set scheduling priority for RT thread" << endl;

	    }
	}

	retval = pthread_create (new_thread, rt_attributes, start, arg);
	seteuid (getuid());

	return retval;
}
