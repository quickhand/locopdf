#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sqlite3.h>
#include <sys/stat.h>
#include "database.h"


static struct sqlite3 *locopdf_database=NULL;
long get_file_index(char *filename,int create_entry_if_missing);


int init_database(char *filename)
{
    int retval=sqlite3_open(filename,&locopdf_database);
    if(retval!=SQLITE_OK)
    {
        sqlite3_close(locopdf_database);
        return -1;
    }
    //,cur_page INTEGER,zoominc REAL,curzoom REAL,hpaninc REAL,vpaninc REAL,curx INTEGER,cury INTEGER,left_trim INTEGER,right_trim INTEGER,top_trim INTEGER,bottom_trim INTEGER
    sqlite3_exec(locopdf_database,"CREATE TABLE files(fileid INTEGER PRIMARY KEY,filename TEXT UNIQUE,mod_time INTEGER)",NULL,NULL,NULL);
    sqlite3_exec(locopdf_database,"CREATE TABLE settings(fileid INTEGER,settingname TEXT,value TEXT)",NULL,NULL,NULL);
    return 0;
}

int get_file_record_status(char *filename)
{
    

    char **resultp=NULL;
    int rows,cols;
    
    
    char *temp=sqlite3_mprintf("SELECT mod_time FROM files WHERE filename = \'%q\'",filename);
    if(sqlite3_get_table(locopdf_database,temp,&resultp,&rows,&cols,NULL)!=SQLITE_OK)
    {
        sqlite3_free(temp);
        return RECORD_STATUS_ERROR;
    }
    else if(rows==0)
    {
        sqlite3_free(temp);
        sqlite3_free_table(resultp);    
        return RECORD_STATUS_ABSENT;
        
    }
    /*else if(rows>1)
    {
        sqlite_free_table(resultp);    
        return RECORD_STATUS_MULTIPLE_RECORDS;
        
    }*/
    sqlite3_free(temp);
    long table_modtime=strtol(resultp[cols],NULL,10);
    sqlite3_free_table(resultp);
    
    struct stat filestat;
    int fileexists;
    fileexists = stat(filename, &filestat);
    if(fileexists<0)
        return RECORD_STATUS_EXISTS_BUT_UNKNOWN;
    
    
    
    if(table_modtime==filestat.st_mtime)
        return RECORD_STATUS_OK;    
    else
        return RECORD_STATUS_OUT_OF_DATE;
    
    
}

int update_file_mod_time(char *filename)
{
    struct stat filestat;
    int fileexists;
    fileexists = stat(filename, &filestat);
    if(fileexists<0)
    {
        return -1;
    }
    
    char **resultp=NULL;
    int rows,cols;
    
    char *temp=sqlite3_mprintf("SELECT mod_time FROM files WHERE filename = \'%q\'",filename);
    if(sqlite3_get_table(locopdf_database,temp,&resultp,&rows,&cols,NULL)!=SQLITE_OK)
    {
        sqlite3_free(temp);
        sqlite3_free_table(resultp);    
        return -1;
    }
    else
    {
        sqlite3_free(temp);
    }
    
    if(rows==0)
    {
        temp=sqlite3_mprintf("INSERT INTO files (filename,mod_time) VALUES(\'%q\',%d)",filename,filestat.st_mtime);
        sqlite3_exec(locopdf_database,temp,NULL,NULL,NULL);
        sqlite3_free(temp);
        
    }
    else
    {
        temp=sqlite3_mprintf("UPDATE files SET mod_time=%d WHERE filename=\'%q\'",filestat.st_mtime,filename);
        sqlite3_exec(locopdf_database,temp,NULL,NULL,NULL);
        sqlite3_free(temp);
    }
    sqlite3_free_table(resultp);
    return 0;
}
long get_file_index(char *filename,int create_entry_if_missing)
{
    
    char **resultp=NULL;
    int rows,cols;
    char *temp=sqlite3_mprintf("SELECT fileid FROM files WHERE filename = \'%q\'",filename);
    int result= sqlite3_get_table(locopdf_database,temp,&resultp,&rows,&cols,NULL);
    sqlite3_free(temp);
    if(rows<=0 && create_entry_if_missing)
    {
        struct stat filestat;
        int fileexists;
        fileexists = stat(filename, &filestat);
        if(fileexists<0)
        {
            return -1;
        }
        
        sqlite3_free_table(resultp);
        temp=sqlite3_mprintf("INSERT INTO files (filename,mod_time) VALUES(\'%q\',%d)",filename,filestat.st_mtime);
        sqlite3_exec(locopdf_database,temp,NULL,NULL,NULL);
        sqlite3_free(temp);
        char *temp=sqlite3_mprintf("SELECT fileid FROM files WHERE filename = \'%q\'",filename);
        result= sqlite3_get_table(locopdf_database,temp,&resultp,&rows,&cols,NULL);
        sqlite3_free(temp);

    }
    if(rows<=0)
    {
        sqlite3_free_table(resultp);
        return -1;
    }
    
    long retval=strtol(resultp[cols],NULL,10);
    sqlite3_free_table(resultp);
    return retval;
}
int set_setting(char *filename,char *settingname,char *value)
{
    long fileindex=get_file_index(filename,1);
    char **resultp=NULL;
    int rows,cols;
    char *temp=sqlite3_mprintf("SELECT value FROM settings WHERE fileid = %d AND settingname = \'%q\'",fileindex,settingname);
    if(sqlite3_get_table(locopdf_database,temp,&resultp,&rows,&cols,NULL)!=SQLITE_OK)
    {
        sqlite3_free(temp);
        sqlite3_free_table(resultp);    
        return -1;
    }
    else
    {
        sqlite3_free(temp);
    }
    
    if(rows==0)
    {
        temp=sqlite3_mprintf("INSERT INTO settings (fileid,settingname,value) VALUES(%d,\'%q\',\'%q\')",fileindex,settingname,value);
        sqlite3_exec(locopdf_database,temp,NULL,NULL,NULL);
        sqlite3_free(temp);
        
    }
    else
    {
        temp=sqlite3_mprintf("UPDATE settings SET value=\'%q\' WHERE fileid=%d AND settingname=\'%q\'",value,fileindex,settingname);
        sqlite3_exec(locopdf_database,temp,NULL,NULL,NULL);
        sqlite3_free(temp);
    }
    sqlite3_free_table(resultp);
}
int set_setting_INT(char *filename,char *settingname,int value)
{
    char *tempstr;
    asprintf(&tempstr,"%d",value);
    int tempo=set_setting(filename,settingname,tempstr);
    free(tempstr);
    return tempo;
}
int set_setting_DOUBLE(char *filename,char *settingname,double value)
{
    char *tempstr;
    asprintf(&tempstr,"%f",value);
    int tempo=set_setting(filename,settingname,tempstr);
    free(tempstr);
    return tempo;
    
    
}
char *get_setting(char *filename,char *settingname)
{
    long fileindex=get_file_index(filename,0);
    
    if(fileindex==-1)
        return NULL;
    
    char **resultp=NULL;
    int rows,cols;
    char *temp=sqlite3_mprintf("SELECT value FROM settings WHERE fileid = %d AND settingname = \'%q\'",fileindex,settingname);
    int result= sqlite3_get_table(locopdf_database,temp,&resultp,&rows,&cols,NULL);
    sqlite3_free(temp);
    if(rows<=0)
    {
        return NULL;
    }
    
    char *retstr;
    asprintf(&retstr,"%s",resultp[cols]);
    sqlite3_free_table(resultp);
    return retstr;    
}
int get_setting_INT(char *filename,char *settingname)
{
    char *tempstr=get_setting(filename,settingname);
    if(tempstr==NULL)
        return -1;
    long tempo=strtol(tempstr,NULL,10);
    free(tempstr);
    return (int)tempo;    
}
double get_setting_DOUBLE(char *filename,char *settingname)
{
    char *tempstr=get_setting(filename,settingname);
    if(tempstr==NULL)
        return -1;
    double tempo=strtod (tempstr,NULL);
    free(tempstr);
    return tempo;    
}
void fini_database()
{
    sqlite3_close(locopdf_database);
    locopdf_database=NULL;
}

