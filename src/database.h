#ifndef LOCOPDF_DATABASE_H
#define LOCOPDF_DATABASE_H

#define RECORD_STATUS_ERROR -1
#define RECORD_STATUS_OK 0
#define RECORD_STATUS_OUT_OF_DATE 1
#define RECORD_STATUS_ABSENT 2
#define RECORD_STATUS_EXISTS_BUT_UNKNOWN 3

int init_database(char *filename);
void fini_database();


int get_file_record_status(char *filename);
int update_file_mod_time(char *filename);
int set_setting(char *filename,char *settingname,char *value);
int set_setting_INT(char *filename,char *settingname,int value);
int set_setting_DOUBLE(char *filename,char *settingname,double value);
char *get_setting(char *filename,char *settingname);
int get_setting_INT(char *filename,char *settingname);
double get_setting_DOUBLE(char *filename,char *settingname);
#endif
