#ifndef LOCOPDF_H_
#define LOCOPDF_H_

#include <Epdf.h>
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
int get_antialias_mode();
void set_antialias_mode(int newantialiasmode);
void set_reader_mode(int newreadermode);
int get_num_pages();
void goto_page(int newpage);
int get_cur_page();
Epdf_Document *get_document();
void update_statusbar();
void set_statusbar_visible(unsigned char visible);
unsigned char get_statusbar_visible();


void render_cur_page();
void prerender_next_page();
void reset_cur_panning();
#endif
