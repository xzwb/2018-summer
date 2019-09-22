#define _MY_LOGIN_C

#include "my_login.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "my_err.h"
#include "my_pack.h"
#include "my_mysql.h"


int login(PACK *pack, MYSQL mysql1) {
    int                i;
    int                ret;
    int                cont = 0;
    char               inedx[100];	
    PACK               *recv_pack = pack;
    MYSQL              mysql = mysql1;
    MYSQL_RES          *result = NULL;
    MYSQL_ROW          row;

    sprintf(inedx, "select *from user_data where account = %d", recv_pack->data.send_account);
    pthread_mutex_lock(&mutex);	
    ret = mysql_query(&mysql, inedx);

    if (!ret) {
        result = mysql_store_result(&mysql);
        row = mysql_fetch_row(result);
        if (row == NULL) {
            pthread_mutex_unlock(&mutex);
            return -1;
        }
        if (strcmp(row[2], recv_pack->data.read_buff) == 0) {
            strcpy(recv_pack->data.send_user, row[1]);
            memset(inedx, 0, sizeof(inedx));
            pack->data.recv_fd = atoi(row[4]);
            sprintf(inedx, "update user_data set user_state = 1 where account = %d", pack->data.send_account);
            mysql_query(&mysql, inedx);
            mysql_free_result(result);
            pthread_mutex_unlock(&mutex);
                
            return 0;
        } else {      
            pack->data.recv_fd = atoi(row[4]);
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    } else {
        printf("query fail\n");
        pthread_mutex_unlock(&mutex);
        
        return -1;
    }    
}

int registered(PACK *pack, MYSQL mysql1) {
    FILE               *fp;
    MYSQL              mysql = mysql1;	
    char               need[100];
    PACK               *recv_pack = pack;
    int                user_number;
	
    pthread_mutex_lock(&mutex);	
    /* 在user_number.txt文件中存放着用户的个数,数量从1000000开始 */
    if ((fp = fopen("user_number.txt", "r")) == NULL) {
        printf("打开文件失败\n");
        exit(-1);
    }
    fread(&user_number, sizeof(int), 1, fp);
	
    sprintf(need, "insert into user_data values(%d,\"%s\",\"%s\",%d,%d)", user_number++, recv_pack->data.send_user, recv_pack->data.read_buff, 0, recv_pack->data.recv_fd);
    recv_pack->data.send_account = user_number-1;
    mysql_query(&mysql, need);
    fclose(fp);
    if ((fp = fopen("user_number.txt", "w")) == NULL) {
        printf("打开文件失败\n");
        exit(-1);
    }
    fwrite(&user_number, sizeof(int), 1, fp);
    fclose(fp);
    pthread_mutex_unlock(&mutex);

    return 0;
}

int change_password(PACK *pack, MYSQL mysql1) {
    MYSQL_RES          *result = NULL;
    PACK               *recv_pack = pack;
    MYSQL              mysql = mysql1;
    char               need[100];
    MYSQL_ROW          row;

    sprintf(need, "select *from user_data where account = %d", recv_pack->data.send_account);
    pthread_mutex_lock(&mutex);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if (row) {
        if (strcmp(recv_pack->data.read_buff, row[2]) == 0) {
            recv_pack->data.recv_fd = atoi(row[4]);
            memset(need, 0, sizeof(need));
            sprintf(need, "update user_data set password = \"%s\" where account = %d", recv_pack->data.write_buff, recv_pack->data.send_account);
            mysql_query(&mysql, need);
            pthread_mutex_unlock(&mutex);
            
            return 0;
        } else {
            pthread_mutex_unlock(&mutex);
            
            return -1;
        }
    } else {
        pthread_mutex_unlock(&mutex);

        return -1;
    }

    return -1;
}

int find_password(PACK *pack, MYSQL mysql1) {
    MYSQL           mysql = mysql1;
    MYSQL_RES       *result;
    MYSQL_ROW       row;
    char            need[150];
    PACK            *recv_pack = pack;
    int             cont = 0;

    pthread_mutex_lock(&mutex);
    sprintf(need, "select *from friends where user = %d", recv_pack->data.send_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    while (row = mysql_fetch_row(result)) {
        cont++;
    }
    if (cont != recv_pack->data.recv_account) {
        strcpy(recv_pack->data.write_buff, "你找不回来你的密码了！！");
        memset(need, 0, sizeof(need));
        sprintf(need, "select *from user_data where account = %d", recv_pack->data.send_account);
        mysql_query(&mysql, need);
        result = mysql_store_result(&mysql);
        row = mysql_fetch_row(result);
        if (!row) {
            strcpy(recv_pack->data.write_buff, "你的账号都是错的哦!!");
        }
        if (send(recv_pack->data.recv_fd, recv_pack, sizeof(PACK), 0) < 0) {
            my_err("send", __LINE__);
        }
    } else {
        memset(need, 0, sizeof(need));
        sprintf(need, "select *from user_data where account = %d", recv_pack->data.send_account);
        mysql_query(&mysql, need);
        result = mysql_store_result(&mysql);
        row = mysql_fetch_row(result);
        if (!row) {
            strcpy(recv_pack->data.write_buff, "你的账号都是错的哦!!");
        } else {
            sprintf(recv_pack->data.write_buff, "你的密码是%s", row[2]);
        }
        if (send(recv_pack->data.recv_fd, recv_pack, sizeof(PACK), 0) < 0) {
            my_err("send", __LINE__);
        }
    }
    
    pthread_mutex_unlock(&mutex);
    return 0;
}

