#ifndef _MY_MYSQL_H
#define _MY_MYSQL_H
#include <mysql/mysql.h>

MYSQL accept_mysql(void);
int close_mysql(MYSQL mysql);

#endif
