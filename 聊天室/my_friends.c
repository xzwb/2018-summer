#define _MY_FRIENDS_C

#include "my_friends.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "my_mysql.h"
#include "my_pack.h"
#include "my_err.h"

int add_fir(PACK *pack, MYSQL mysql1) {
    PACK            *recv_pack = pack;
    MYSQL           mysql = mysql1;
    MYSQL_RES       *result;
    MYSQL_ROW       row, row1;
    BOX             *tmp = box_head;
    int             ret;
    char            need[100];
    
    sprintf(need, "select *from user_data where account = %d", recv_pack->data.recv_account);
    pthread_mutex_lock(&mutex);
    ret = mysql_query(&mysql, need);
    if (!ret) {
        result = mysql_store_result(&mysql);
        row = mysql_fetch_row(result);
        if (row == NULL) {
            pthread_mutex_unlock(&mutex);
            return -1;
        }
        memset(need, 0, sizeof(need));
        sprintf(need, "select *from friends where user = %d and friend_user = %d", recv_pack->data.send_account, recv_pack->data.recv_account);
        ret = mysql_query(&mysql, need);
        if (!ret) {
            // 如果可以查到数据说明对方已经是好友了 
            result = mysql_store_result(&mysql);
            row1 = mysql_fetch_row(result);
            if (row1 != NULL) {
                pthread_mutex_unlock(&mutex);
                return -1;
            }
            memset(need, 0, sizeof(need));
            sprintf(need, "账号为%d,昵称为%s的用户发来好友请求\n", recv_pack->data.send_account, recv_pack->data.send_user);
            if (atoi(row[3]) == 0) {
                while (tmp) {
                    if (tmp->recv_account == recv_pack->data.recv_account) {
                        break;
                    }
                    tmp = tmp->next;
                }
                if (tmp != NULL) {
                    tmp->plz_account[tmp->friend_number] = recv_pack->data.send_account;
                    strcpy(tmp->write_buff[tmp->friend_number], need);
                    tmp->friend_number++;
                } else {
                    tmp = (BOX *)malloc(sizeof(BOX));
                    tmp->recv_account = recv_pack->data.recv_account;
                    tmp->friend_number = 0;
                    tmp->talk_number = 0;
                    strcpy(tmp->write_buff[tmp->friend_number], need);
                    tmp->plz_account[tmp->friend_number++] = recv_pack->data.send_account;
                    if (box_head == NULL) {
                        box_head = box_tail = tmp;
                        box_tail->next = NULL;
                    } else {
                        box_tail->next = tmp;
                        box_tail = tmp;
                        box_tail->next = NULL;
                    }
                }
                pthread_mutex_unlock(&mutex);
                return 0;
            } else {
                recv_pack->data.send_fd = atoi(row[4]);
                strcpy(recv_pack->data.recv_user, row[1]);
                strcpy(recv_pack->data.read_buff, need);
                recv_pack->type = FRIENDS_PLZ;
                if (send(recv_pack->data.send_fd, recv_pack, sizeof(PACK), 0) < 0) {
                    my_err("send", __LINE__);
                } 
                pthread_mutex_unlock(&mutex);
                
                return 0;
            }
        }
    }
}

int friends_plz(PACK *pack, MYSQL mysql1) {
    char            need[100]; 
    MYSQL           mysql = mysql1;
    PACK            *recv_pack = pack;
    
    pthread_mutex_lock(&mutex);
    sprintf(need, "insert into friends values(%d,%d,0)", recv_pack->data.send_account, recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    memset(need, 0, sizeof(need));
    sprintf(need, "insert into friends values(%d,%d,0)", recv_pack->data.recv_account, recv_pack->data.send_account);
    mysql_query(&mysql, need);
    pthread_mutex_unlock(&mutex);
    
    return 0;
}   
     
int del_friend(PACK *pack, MYSQL mysql1) {
    PACK            *recv_pack = pack;
    MYSQL           mysql = mysql1;
    char            need[100];
    MYSQL_ROW       row;
    MYSQL_RES       *result;

    pthread_mutex_lock(&mutex);
    sprintf(need, "select *from friends where user = %d and friend_user = %d", recv_pack->data.send_account, recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if (row == NULL) {
        pthread_mutex_unlock(&mutex);    
        return -1;
    }
    memset(need, 0, sizeof(need));
    sprintf(need, "delete from friends where user = %d and friend_user = %d", recv_pack->data.send_account, recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    memset(need, 0, sizeof(need));
    sprintf(need, "delete from friends where user = %d and friend_user = %d", recv_pack->data.recv_account, recv_pack->data.send_account);
    mysql_query(&mysql, need);
    pthread_mutex_unlock(&mutex);
    
    return 0;
}

int black_friend(PACK *pack, MYSQL mysql1) {
    PACK            *recv_pack = pack;
    MYSQL           mysql = mysql1;
    char            need[100];
    MYSQL_ROW       row;
    MYSQL_RES       *result;
    
    pthread_mutex_lock(&mutex);
    sprintf(need, "select *from friends where user = %d and friend_user = %d", recv_pack->data.send_account, recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if (row == NULL) {
        pthread_mutex_unlock(&mutex);    
        return -1;
    }
    memset(need, 0, sizeof(need));
    sprintf(need, "update friends set realtion = -1 where user = %d and friend_user = %d", recv_pack->data.send_account, recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    pthread_mutex_unlock(&mutex);

    return 0;
}

int white_friend(PACK *pack, MYSQL mysql1) {
    PACK            *recv_pack = pack;
    MYSQL           mysql = mysql1;
    char            need[100];
    MYSQL_ROW       row;
    MYSQL_RES       *result;

    pthread_mutex_lock(&mutex);
    sprintf(need, "select *from friends where user = %d and friend_user = %d", recv_pack->data.send_account, recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if (row == NULL) {
        pthread_mutex_unlock(&mutex);    
        return -1;
    }
    if (atoi(row[2]) == OK) {
        pthread_mutex_unlock(&mutex);
        return 0;
    }
    memset(need, 0, sizeof(need));
    sprintf(need, "update friends set realtion = 0 where user = %d and friend_user = %d", recv_pack->data.send_account, recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    pthread_mutex_unlock(&mutex);

    return 0;
}

int care_friend(PACK *pack, MYSQL mysql1) {
    PACK            *recv_pack = pack;
    MYSQL           mysql = mysql1;
    char            need[100];
    MYSQL_RES       *result;
    MYSQL_ROW       row;

    pthread_mutex_lock(&mutex);
    sprintf(need, "select *from friends where user = %d and friend_user = %d", recv_pack->data.send_account, recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if (row == NULL) {
        pthread_mutex_unlock(&mutex);    
        return -1;
    }
    memset(need, 0, sizeof(need));
    sprintf(need, "update friends set realtion = 1 where user = %d and friend_user = %d", recv_pack->data.send_account, recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    pthread_mutex_unlock(&mutex);
}

int discare_friend(PACK *pack, MYSQL mysql1) {
    PACK            *recv_pack = pack;
    MYSQL           mysql = mysql1;
    char            need[100];
    MYSQL_RES       *result;
    MYSQL_ROW       row;

    pthread_mutex_lock(&mutex);
    sprintf(need, "select *from friends where user = %d and friend_user = %d", recv_pack->data.send_account, recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if (row == NULL) {
        pthread_mutex_unlock(&mutex);    
        return -1;
    }
    memset(need, 0, sizeof(need));
    sprintf(need, "update friends set realtion = 0 where user = %d and friend_user = %d", recv_pack->data.send_account, recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    pthread_mutex_unlock(&mutex);
}

FRIEND *look_list(PACK *pack, MYSQL mysql1) {
    PACK            *recv_pack = pack;
    MYSQL           mysql = mysql1;
    char            need[100];
    MYSQL_RES       *result, *result1;
    MYSQL_ROW       row, row1;
    FRIEND          *list;

    list = (FRIEND *)malloc(sizeof(FRIEND));
    list->friend_number = 0;
    pthread_mutex_lock(&mutex);
    sprintf(need, "select *from friends where user = %d", recv_pack->data.send_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    while (row = mysql_fetch_row(result)) {
        list->friend_account[list->friend_number] = atoi(row[1]);
        memset(need, 0, sizeof(need));
        sprintf(need, "select *from user_data where account = %d", atoi(row[1]));
        mysql_query(&mysql, need);
        result1 = mysql_store_result(&mysql);
        row1 = mysql_fetch_row(result1);
        strcpy(list->friend_nickname[list->friend_number], row1[1]);
        list->friend_state[list->friend_number++] = atoi(row1[3]);
    }
    if (list->friend_number == 0) {
        pthread_mutex_unlock(&mutex);
        return list;
    } else {
        pthread_mutex_unlock(&mutex);
        return list;
    }
}
