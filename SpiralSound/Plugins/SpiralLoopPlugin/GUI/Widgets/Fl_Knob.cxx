// generated by Fast Light User Interface Designer (fluid) version 2.00

#include "Fl_Knob.H"
#include <FL/fl_draw.H>
#include <math.h>
#include <iostream>

Fl_Knob::Fl_Knob(int xx,int yy,int ww,int hh,const char *l): Fl_Valuator(xx,yy,ww,hh,l) {
  a1 = 35;
	a2 = 325;
	_type = DOTLIN;
	_percent = 0.3;
	_scaleticks = 10;
}

Fl_Knob::~Fl_Knob() {
}

void Fl_Knob::draw() {
  int ox,oy,ww,hh,side;
float sa,ca,a_r;
unsigned char rr,gg,bb;

	ox = x();
	oy = y();
	ww = w();
	hh = h();
	draw_label();
	fl_clip(ox,oy,ww,hh);
	if (ww > hh)
	{
		side = hh;
		ox = ox + (ww - side) / 2; 
	}
	else
	{
		side = ww;
		oy = oy + (hh - side) / 2; 
	}
	side = w() > h () ? hh:ww;
	int dam = damage();
	if (dam & FL_DAMAGE_ALL)
	{
		int col = parent()->color();
		fl_color(col);
		fl_rectf(ox,oy,side,side);
		Fl::get_color((Fl_Color)col,rr,gg,bb);
		shadow(-60,rr,gg,bb);
		fl_pie(ox+9,oy+9,side-12,side-12,0,360);
		if (_type!=NONE) draw_scale(ox,oy,side);
		col = color();
		Fl::get_color((Fl_Color)col,rr,gg,bb);
		shadow(15,rr,gg,bb);
		fl_pie(ox+6,oy+6,side-12,side-12,40,80);
		shadow(30,rr,gg,bb);
		fl_pie(ox+6,oy+6,side-12,side-12,80,220);
		shadow(-15,rr,gg,bb);
		fl_pie(ox+6,oy+6,side-12,side-12,220,260);
		shadow(-30,rr,gg,bb);
		fl_pie(ox+6,oy+6,side-12,side-12,260,400);
		fl_color(FL_BLACK);
		fl_arc(ox+6,oy+6,side-11,side-11,0,360);
		fl_color(col);
		fl_pie(ox+10,oy+10,side-20,side-20,0,360);
	}
	else
	{
		fl_color(color());
		fl_pie(ox+10,oy+10,side-20,side-20,0,360);
	}
	Fl::get_color((Fl_Color)color(),rr,gg,bb);
	shadow(10,rr,gg,bb);
	fl_pie(ox+10,oy+10,side-20,side-20,110,150);
	fl_pie(ox+10,oy+10,side-20,side-20,290,330);
	shadow(17,rr,gg,bb);
	fl_pie(ox+10,oy+10,side-20,side-20,120,140);
	fl_pie(ox+10,oy+10,side-20,side-20,300,320);
	shadow(25,rr,gg,bb);
	fl_pie(ox+10,oy+10,side-20,side-20,127,133);
	fl_pie(ox+10,oy+10,side-20,side-20,307,313);
	draw_cursor(ox,oy,side);
	fl_pop_clip();
}

int Fl_Knob::handle(int  event) {
  int ox,oy,ww,hh;
  
	ox = x() + 10; oy = y() + 10;
	ww = w() - 20;
	hh = h()-20;
	switch (event) 
	{
  		case FL_PUSH:
    			handle_push();
  		case FL_DRAG:
			{
				int mx = Fl::event_x()-ox-ww/2;
				int my = Fl::event_y()-oy-hh/2;
				if (!mx && !my) return 1;
    				double angle = 270-atan2((float)-my, (float)mx)*180/M_PI;
				double oldangle = (a2-a1)*(value()-minimum())/(maximum()-minimum()) + a1;
				while (angle < oldangle-180) angle += 360;
				while (angle > oldangle+180) angle -= 360;
				double val;
				if ((a1<a2) ? (angle <= a1) : (angle >= a1))
				{
					val = minimum();
				} 
				else 
				if ((a1<a2) ? (angle >= a2) : (angle <= a2)) 
				{
      				val = maximum();
    			} 
				else
				{
					val = minimum() + (maximum()-minimum())*(angle-a1)/(a2-a1);
				}
				handle_drag(clamp(round(val)));
			} 
			return 1;
		case FL_RELEASE:
			handle_release();
			return 1;
		default:
			return 0;
  	}
	return 0;
}

void Fl_Knob::type(int ty) {
  _type = ty;
}

void Fl_Knob::shadow(const int offs,const uchar r,uchar g,uchar b) {
  int rr,gg,bb;

	rr = r + offs; 
	rr = rr > 255 ? 255:rr;
	rr = rr < 0 ? 0:rr;
	gg = g + offs; 
	gg = gg > 255 ? 255:gg;
	gg = gg < 0 ? 0:gg;
	bb = b + offs; 
	bb = bb > 255 ? 255:bb;
	bb = bb < 0 ? 0:bb;
	fl_color((uchar)rr,(uchar)gg,(uchar)bb);
}

void Fl_Knob::draw_scale(const int ox,const int oy,const int side) {
  float x1,y1,x2,y2,rds,cx,cy,ca,sa;

	rds = side / 2;
	cx = ox + side / 2;
	cy = oy + side / 2;
	if (!(_type & DOTLOG_3))		
	{
		if (_scaleticks == 0) return;
		double a_step = (10.0*3.14159/6.0) / _scaleticks;
		double a_orig = -(3.14159/3.0);
		for (int a = 0; a <= _scaleticks; a++)
		{
			double na = a_orig + a * a_step;
			ca = cos(na);
			sa = sin(na);
			x1 = cx + rds * ca;
			y1 = cy - rds * sa;
			x2 = cx + (rds-6) * ca;
			y2 = cy - (rds-6) * sa;
			fl_color(FL_BLACK);
			fl_line(x1,y1,x2,y2);
			fl_color(FL_WHITE);
			if (sa*ca >=0)
				fl_line(x1+1,y1+1,x2+1,y2+1);
			else
				fl_line(x1+1,y1-1,x2+1,y2-1);
		}
	}
	else
	{
		int nb_dec = (_type & DOTLOG_3);
		for (int k = 0; k < nb_dec; k++)
		{
			double a_step = (10.0*3.14159/6.0) / nb_dec;
			double a_orig = -(3.14159/3.0) + k * a_step;
			for (int a = (k) ? 2:1; a <= 10; )
			{
				double na = a_orig + log10((double)a) * a_step;
				ca = cos(na);
				sa = sin(na);
				x1 = cx - rds * ca;
				y1 = cy - rds * sa;
				x2 = cx - (rds-6) * ca;
				y2 = cy - (rds-6) * sa;
				fl_color(FL_BLACK);
				fl_line(x1,y1,x2,y2);
				fl_color(FL_WHITE);
				if (sa*ca <0)
					fl_line(x1+1,y1+1,x2+1,y2+1);
				else
					fl_line(x1+1,y1-1,x2+1,y2-1);
				if ((a == 1) || (nb_dec == 1))
					a += 1;
				else
					a += 2;
			}
		}
	}
}

void Fl_Knob::draw_cursor(const int ox,const int oy,const int side) {
  float rds,cur,cx,cy;
double angle;
	
	rds = (side - 20) / 2.0;
	cur = _percent * rds / 2;
	cx = ox + side / 2;
	cy = oy + side / 2;
	angle = (a2-a1)*(value()-minimum())/(maximum()-minimum()) + a1;	
	fl_push_matrix();
	fl_scale(1,1);
	fl_translate(cx,cy);
	fl_rotate(-angle);
	fl_translate(0,rds-cur-2.0);
	if (_type<LINELIN)
	{		
		fl_begin_polygon();
		fl_color(selection_color());
		fl_circle(0.0,0.0,cur);
		fl_end_polygon();
		fl_begin_loop();
		fl_color(FL_BLACK);
		fl_circle(0.0,0.0,cur);
		fl_end_loop();
	}
	else
	{
		fl_begin_polygon();
		fl_color(selection_color());
		fl_vertex(-1.5,-cur);
		fl_vertex(-1.5,cur);
		fl_vertex(1.5,cur);
		fl_vertex(1.5,-cur);
		fl_end_polygon();
		fl_begin_loop();
		fl_color(FL_BLACK);
		fl_vertex(-1.5,-cur);
		fl_vertex(-1.5,cur);
		fl_vertex(1.5,cur);
		fl_vertex(1.5,-cur);
		fl_end_loop();
	}
	fl_pop_matrix();
}

void Fl_Knob::cursor(const int pc) {
  _percent = (float)pc/100.0;

	if (_percent < 0.05) _percent = 0.05;
	if (_percent > 1.0) _percent = 1.0;
	if (visible()) damage(FL_DAMAGE_CHILD);
}

void Fl_Knob::scaleticks(const int tck) {
  _scaleticks = tck;
	if (_scaleticks < 0) _scaleticks = 0;
	if (_scaleticks > 31) _scaleticks = 31;
	if (visible()) damage(FL_DAMAGE_ALL);
}
