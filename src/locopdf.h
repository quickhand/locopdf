#ifndef LOCOPDF_H_
#define LOCOPDF_H_

char *get_theme_file();
int get_win_width();
int get_win_height();
double get_zoom_inc();
void set_zoom_inc(double newzoominc);
double get_hpan_inc();
void set_hpan_inc(double newhpaninc);
double get_vpan_inc();
void set_vpan_inc(double newvpaninc);
int get_lefttrim();
void set_lefttrim(int newlefttrim);
int get_righttrim();
void set_righttrim(int newrighttrim);
int get_toptrim();
void set_toptrim(int newtoptrim);
int get_bottomtrim();
void set_bottomtrim(int newbottomtrim);
void render_cur_page();
void prerender_next_page();
void reset_cur_panning();
#endif
