#ifndef _MY_TALK_H
#define _MY_TALK_H
#include "my_pack.h"
#include <mysql/mysql.h>

int send_fmes(PACK *pack, MYSQL mysql1);
int read_message(PACK *pack, MYSQL mysql1);
int del_message(PACK *pack, MYSQL msyql1);
int send_gmes(PACK *pack, MYSQL mysql1);
// int send_file(PACK *pack, MYSQL mysql1);
// int read_file(PACK *pack, MYSQL mysql1);
int ok_file(PACK *pack, MYSQL msyql1); 

#endif
