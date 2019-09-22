#ifndef _MY_FRIENDS_H
#define _MY_FRIENDS_H
#include <mysql/mysql.h>
#include "my_pack.h"

int add_fir(PACK *pack, MYSQL mysql1);
int del_friend(PACK *pack, MYSQL mysql1);
int friends_plz(PACK *pack, MYSQL mysql1);
int black_friend(PACK *pack, MYSQL mysql1);
int white_friend(PACK *pack, MYSQL mysql1);
int care_friend(PACK *pack, MYSQL mysql1);
int discare_friend(PACK *pack, MYSQL mysql1);
FRIEND *look_list(PACK *pack, MYSQL mysql1);

#endif
