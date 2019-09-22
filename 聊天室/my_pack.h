#ifndef _MY_PACK_H
#define _MY_PACK_H
#include <pthread.h>

#define MAX_FRIEND 500
// 登录
#define LOGIN 1
// 注册
#define REGISTERED 2
// 找回密码
#define FIND_PASSWORD 3
// 修改密码
#define CHANGE_PASSWORD 4
// 添加好友
#define ADD_FRIEND 5
// 删除好友
#define DEL_FRIEND 6
// 查看好友列表
#define LOOK_LIST 7
// 发消息
#define SEND_FMES 8
// 创建群
#define CREATE_GROUP 9
// 加入群
#define ADD_GROUP 10
// 退出群
#define EXIT_GROUP 11
// 删除群成员
#define DEL_MEMBER 12
// 设置管理员
#define SET_ADMIN 13
// 删除管理员
#define DEL_ADMIN 14
// 发送群消息
#define SEND_GMES 15
// 查看加入的群
#define LOOK_GROUP 16
// 群主删除群
#define DIS_GROUP 17
// 发送文件
#define SEND_FILE 18
// 好友请求
#define FRIENDS_PLZ 19
// 黑名单
#define BLACK_FRIEND 20
// 取消黑名单
#define WHITE_FRIEND 21
// 特别关心
#define CARE_FRIEND 22
// 取消特关
#define DISCARE_FRIEND 23
#define MAXIN 1024
#define EXIT -1
#define ACCOUNT_ERROR -2
#define CARE 1
#define OK 0
#define RECV_FMES 24
#define BLACK -1
#define READ_MESSAGE 25
#define READ_GMES 26
#define RECV_GMES 27
#define DEL_MESSAGE 28
#define LOOK_MEMBER 29
#define LOOK_GROUP_LIST 30
#define RECV_FILE 31
#define READ_FILE 32
#define OK_FILE 33
#define SEND_F 34

pthread_mutex_t mutex;
pthread_mutex_t mutex_cli;
pthread_cond_t cond_cli;
pthread_cond_t cond;

typedef struct {
    int               cont;
    int               send_fd;
    int               recv_fd;
    int               send_account;
    int               recv_account;
    char              send_user[50];
    char              recv_user[50];
    char              read_buff[MAXIN];
    char              write_buff[MAXIN];
} DATA;

typedef struct {
    int               friend_number;
    int               friend_account[500];
    char              friend_nickname[500][20];
    int               friend_state[500];
} FRIEND;

typedef struct {
    int               number;
    char              message[500][MAXIN];
    int               send_user[500];
    int               recv_user[500];
} MESSAGE;

typedef struct {
    int               group_account;
    int               message_number;
    int               send_account[500];
    char              message[500][MAXIN];
} GROUP_MESSAGE;

typedef struct {
    int               group_account;
    int               group_member_number;
    int               group_member_account[500];	
    char              group_member_nickname[500][20];
    int               group_member_state[500];
} GROUP;

typedef struct {
    int               type;
    DATA              data;
} PACK;

typedef struct {
    int               group_account[100];
    char              group_name[100][20];
    int               number;
    int               group_state[100];
} GROUP_G;

typedef struct {
    char              filename[50];
    int               send_account; 	
    char              send_nickname[50];
    int               have;
} FLE;

// 消息盒子
typedef struct BOX {
    // 接受消息的人的账号
    int               recv_account;
    // 发送消息人的账号
    int               send_account[500];
    // 发送好友请求人的账号
    int               plz_account[500];
    // 发送的消息
    char              read_buff[500][MAXIN];
    // 发送的请求
    char              write_buff[500][100];
    // 消息数量
    int               talk_number;
    // 请求数量
    int               friend_number;
    // 群里发送消息的人
    int               send_account1[500];
    // 消息内容
    char              message[500][MAXIN];
    // 群消息数量
    int               number;
    // 群号
    int               group_account[500];
    struct BOX        *next;
} BOX;


BOX *box_head;
BOX *box_tail;

#endif
