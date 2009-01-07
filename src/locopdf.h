#ifndef LOCOPDF_H_
#define LOCOPDF_H_

#define FIT_WIDTH 0
#define FIT_HEIGHT 1
#define FIT_BEST 2
#define FIT_STRETCH 3
#define FIT_NO 4


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
int get_fit_mode();
void set_fit_mode(int newfitmode);
int get_reader_mode();
void set_reader_mode(int newreadermode);
void render_cur_page();
void prerender_next_page();
void reset_cur_panning();
#endif
