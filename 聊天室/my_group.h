#ifndef _MY_GROUP_H
#define _MY_GROUP_H

#include "my_pack.h"

#include <mysql/mysql.h>

// 创建群
int create_group(PACK *pack, MYSQL mysql1);
// 加群
int add_group(PACK *pack, MYSQL mysql1);
// 退群
int exit_group(PACK *pack, MYSQL mysql1);
// 设置管理员
int set_admin(PACK *pack, MYSQL mysql1);
// 取消管理员
int del_admin(PACK *pack, MYSQL mysql1);
// 查看群成员
int look_member(PACK *pack, MYSQL mysql1);
// 查看已经加入的群
int look_group_list(PACK *pack, MYSQL mysql1);
// 群t人
int del_member(PACK *pack, MYSQL mysql1);

#endif
