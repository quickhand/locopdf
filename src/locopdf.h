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
void reset_cur_panning();
#endif
