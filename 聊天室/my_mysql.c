#define _MY_MYSQL_C

#include "my_mysql.h"

#include <mysql/mysql.h>
#include <stdio.h>

#include "my_err.h"
#include "my_pack.h"


MYSQL accept_mysql(void) {
    MYSQL               mysql;
	
    if (NULL == mysql_init(&mysql)) {
        my_err("mysqlinit", __LINE__);
	}

    // 初始化数据库
    if (mysql_library_init(0, NULL, NULL) != 0) {
        my_err("mysqllibrary_init", __LINE__);
    }

    //连接数据库
    if (NULL == mysql_real_connect(&mysql, "127.0.0.1", "root", "520520cw...", "happy", 0, NULL, 0)) {
        my_err("mysqlrealconnect", __LINE__);
    }

    //设置中文字符集
    if (mysql_set_character_set(&mysql, "utf8") < 0) {
        my_err("mysqlsetcharacter_set", __LINE__);
    }
    
    return mysql;
}

int close_mysql(MYSQL mysql) {
    mysql_close(&mysql);
    mysql_library_end();
    
    return 0;
}

