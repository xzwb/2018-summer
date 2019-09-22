#define _MY_TALK_C

#include "my_talk.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <mysql/mysql.h>

#include "my_pack.h"
#include "my_err.h"
#include "my_mysql.h"
#include "my_friends.h"
#include "my_login.h"

int send_fmes(PACK *pack, MYSQL mysql1) {
    char            need[100];
    MYSQL           mysql = mysql1;
    PACK            *recv_pack = pack;
    MYSQL_RES       *result;
    MYSQL_ROW       row;
    BOX             *tmp;

    pthread_mutex_lock(&mutex);
    sprintf(need, "select *from user_data where account = %d", recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if (!row) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    strcpy(recv_pack->data.recv_user, row[1]);
    if ((atoi(row[3]) == 1)) {
        recv_pack->type = RECV_FMES;
        recv_pack->data.recv_fd = atoi(row[4]);
        memset(need, 0, sizeof(need));
        sprintf(need, "select *from friends where user = %d", recv_pack->data.recv_account);
        mysql_query(&mysql, need);
        result = mysql_store_result(&mysql);
        while (row = mysql_fetch_row(result)) {
            if (atoi(row[1]) != recv_pack->data.send_account) {
                continue;
            }
            if (atoi(row[2]) == BLACK) {
               recv_pack->type = SEND_FMES;
               pthread_mutex_unlock(&mutex);
               return -1;
            } else if (atoi(row[2]) == CARE) {
                memset(recv_pack->data.write_buff, 0, sizeof(recv_pack->data.read_buff));
                strcpy(recv_pack->data.write_buff, "ohyeah");
                if (send(recv_pack->data.recv_fd, recv_pack, sizeof(PACK), 0) < 0) {
                    my_err("send", __LINE__);
                }
                memset(recv_pack->data.write_buff, 0, sizeof(recv_pack->data.write_buff));
                memset(need, 0, sizeof(need));
                sprintf(need, "insert into chat_messages values(%d,%d,\"%s\",1,1)", recv_pack->data.send_account, recv_pack->data.recv_account,recv_pack->data.read_buff);
                mysql_query(&mysql, need);
                break;
            } else if(atoi(row[2]) == OK){
                if (send(recv_pack->data.recv_fd, recv_pack, sizeof(PACK), 0) < 0) {
                    my_err("send", __LINE__);
                }
                memset(need, 0, sizeof(need));
                sprintf(need, "insert into chat_messages values(%d,%d,\"%s\",1,1)", recv_pack->data.send_account, recv_pack->data.recv_account, recv_pack->data.read_buff);
                mysql_query(&mysql, need);
                break;
            }
        }
        if (!row) {
            pthread_mutex_unlock(&mutex);
            recv_pack->type = SEND_FMES;
            return -1;
        }
    } else {
        tmp = box_head;
        while (tmp != NULL) {
            if (tmp->recv_account = recv_pack->data.recv_account) {
                tmp->send_account[tmp->talk_number] = recv_pack->data.send_account;
                strcpy(tmp->read_buff[tmp->talk_number++], recv_pack->data.read_buff);
                memset(need, 0, sizeof(need));
                sprintf(need, "insert into chat_messages values(%d,%d,\"%s\",1,1)", recv_pack->data.send_account, recv_pack->data.recv_account,recv_pack->data.read_buff);
                mysql_query(&mysql, need);
                break;
            }
            tmp = tmp->next;
        }
    }
    pthread_mutex_unlock(&mutex);
    recv_pack->type = SEND_FMES;

    return 0;
}

int read_message(PACK *pack, MYSQL mysql1) {
    PACK            *recv_pack = pack;
    MYSQL           mysql =  mysql1;
    MYSQL_RES       *result;
    MYSQL_ROW       row;
    char            need[200];
    MESSAGE         *message;

    pthread_mutex_lock(&mutex);
    message = (MESSAGE *)malloc(sizeof(MESSAGE));
    message->number = 0;
    sprintf(need, "select *from chat_messages where send_user = %d and recv_user = %d and send_can_look = 1 union select *from chat_messages where send_user = %d and recv_user = %d and recv_can_look = 1", recv_pack->data.send_account, recv_pack->data.recv_account, recv_pack->data.recv_account, recv_pack->data.send_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    while (row = mysql_fetch_row(result)) {
       message->send_user[message->number] = atoi(row[0]);
       message->recv_user[message->number] = atoi(row[1]);
       strcpy(message->message[message->number++], row[2]);
    }
    if (send(recv_pack->data.send_fd, recv_pack, sizeof(PACK), 0) < 0) {
        my_err("send", __LINE__);
    }
    if (send(recv_pack->data.send_fd, message, sizeof(MESSAGE), 0) < 0) {
        my_err("send", __LINE__);
    }
    
    pthread_mutex_unlock(&mutex);
    return 0;
}

int del_message(PACK *pack, MYSQL mysql1) {
    MYSQL           mysql = mysql1;
    PACK            *recv_pack = pack;
    MYSQL_RES       *result;
    MYSQL_ROW       row;
    char            need[100];

    pthread_mutex_lock(&mutex);
    sprintf(need, "update chat_messages set send_can_look = 0 where send_user = %d and recv_user = %d", recv_pack->data.send_account, recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    memset(need, 0, sizeof(need));
    sprintf(need, "update chat_messages set recv_can_look = 0 where recv_user = %d and send_user = %d", recv_pack->data.send_account, recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    memset(need, 0, sizeof(need));
    sprintf(need, "delete from chat_messages where send_can_look = 0 and recv_can_look = 0");
    mysql_query(&mysql, need);

    return 0;
}

int send_gmes(PACK *pack, MYSQL mysql1) {
    MYSQL           mysql = mysql1;
    PACK            *recv_pack = pack;
    MYSQL_RES       *result, *result1;
    MYSQL_ROW       row, row1;
    char            need[100];
    BOX             *tmp = box_head;

    pthread_mutex_lock(&mutex);
    recv_pack->type = RECV_GMES;
    sprintf(need, "select *from groups where group_account = %d", recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if (!row) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    memset(need, 0, sizeof(need));
    sprintf(need, "select *from group_members where group_account = %d", recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    while (row = mysql_fetch_row(result)) {
        if (atoi(row[2]) == recv_pack->data.send_account) {
            continue;
        }
        memset(need, 0, sizeof(need));
        sprintf(need, "select *from user_data where account = %d", atoi(row[2]));
        mysql_query(&mysql, need);
        result1 = mysql_store_result(&mysql);
        row1 = mysql_fetch_row(result1);
        if (atoi(row1[3]) == 1) {
            if (send(atoi(row1[4]), recv_pack, sizeof(PACK), 0) < 0) {
                my_err("send", __LINE__);
            }
        } else {
            while (tmp != NULL) {
                if (tmp->recv_account == atoi(row1[0])) {
                    break;
                }
                tmp = tmp->next;
            }
           /* tmp->send_account1[tmp->number] = recv_pack->data.send_account;
            tmp->group_account[tmp->number] = recv_pack->data.send_account;
            strcpy(tmp->message[tmp->number++], recv_pack->data.read_buff);
            if (send(atoi(row1[4]), recv_pack, sizeof(PACK), 0) < 0) {
                my_err("send", __LINE__);
            }*/
            if (tmp == NULL) {
                tmp = (BOX *)malloc(sizeof(BOX));
                tmp->number = 0;
                tmp->recv_account = atoi(row1[0]);
                tmp->send_account1[tmp->number] = recv_pack->data.send_account;
                tmp->group_account[tmp->number] = recv_pack->data.recv_account;
                strcpy(tmp->message[tmp->number++], recv_pack->data.read_buff);
                if (box_head == NULL) {
                    box_head = box_tail = tmp;
                    box_tail->next = NULL;
                } else {
                    box_tail->next = tmp;
                    box_tail = tmp;
                    box_tail->next = NULL;
                }
            } else {
                tmp->send_account1[tmp->number] = recv_pack->data.send_account;
                tmp->group_account[tmp->number] = recv_pack->data.send_account;
                strcpy(tmp->message[tmp->number++], recv_pack->data.read_buff);
            }
        }
    }
    recv_pack->type = SEND_GMES;
    pthread_mutex_unlock(&mutex);

    return 0;
}

/*int send_file(PACK *pack, MYSQL mysql1) {
    MYSQL           mysql = mysql1;
    PACK            *recv_pack = pack;
    MYSQL_RES       *result;
    MYSQL_ROW       row;
    char            need[100];

    pthread_mutex_lock(&mutex);
    sprintf(need, "select *from friends where user = %d and friend_user = %d", recv_pack->data.send_account, recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if (!row) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    memset(need, 0, sizeof(need));
    sprintf(need, "select *from user_data where account = %d", recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if (atoi(row[3]) == 0) {
        pthread_mutex_unlock(&mutex);
        return -1;
    } else {
        recv_pack->data.recv_fd = atoi(row[4]);
        recv_pack->type = RECV_FILE;
        if (send(recv_pack->data.recv_fd, recv_pack, sizeof(PACK), 0) < 0) {
            my_err("send", __LINE__);
        }
        recv_pack->type = SEND_FILE;
        pthread_mutex_unlock(&mutex);
        return 0;
    }
}


int read_file(PACK *pack, MYSQL mysql1) {
    PACK            *recv_pack = pack;
    MYSQL           mysql = mysql1;
    MYSQL_RES       *result;
    MYSQL_ROW       row;
    char            need[100];

    pthread_mutex_lock(&mutex);
    sprintf(need, "select *from user_data where account = %d", recv_pack->data.recv_account);
    printf("^^%s\n", need);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    recv_pack->data.recv_fd = atoi(row[4]);
    recv_pack->type = SEND_F;
    if (send(recv_pack->data.recv_fd, recv_pack, sizeof(PACK), 0) < 0) {
        my_err("send", __LINE__);
    }
    recv_pack->type = OK_FILE;
    if (send(recv_pack->data.send_fd, recv_pack, sizeof(PACK), 0) < 0) {
        my_err("send", __LINE__);
    }
    pthread_mutex_unlock(&mutex);
    
    return 0;
}*/

int ok_file(PACK *pack, MYSQL mysql1) {
    MYSQL           mysql = mysql1;
    PACK            *recv_pack = pack;
    MYSQL_RES       *result;
    MYSQL_ROW       row;
    char            need[100];
    
    pthread_mutex_lock(&mutex);
    sprintf(need, "select *from user_data where account = %d", recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if (!row) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    if (atoi(row[3]) == 0) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    recv_pack->data.recv_fd = atoi(row[4]);
    recv_pack->type = RECV_FILE; 
    if (send(recv_pack->data.recv_fd, recv_pack, sizeof(PACK), 0) < 0) {
        my_err("send", __LINE__);
    }
    recv_pack->type = OK_FILE;
    if (send(recv_pack->data.send_fd, recv_pack, sizeof(PACK), 0) < 0) {
        my_err("send", __LINE__);
    }
    pthread_mutex_unlock(&mutex);
    
    return 0;
}

