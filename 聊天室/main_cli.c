#include <stdio.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "my_pack.h"
#include "my_socket.h"
#include "my_err.h"
#include "my_mune.h"

PACK *send_pack;
PACK *recv_pack;
BOX *box;
FRIEND *list;
MESSAGE *message;
GROUP_MESSAGE *group_message;
GROUP *member_list;
GROUP_G *group_list;
FLE *file;
int sing;

/* 用来发送数据的线程 */
void *thread_read(void *sock_fd) {
    int               choose;
    char              password[20];
    char              ch;
    int               i = 0;
    struct termios    old, new;
    
    tcgetattr(0, &old);
    new = old;
    new.c_lflag &= ~(ECHO | ICANON);
    send_pack = (PACK *)malloc(sizeof(PACK));
    /* 1为登录,2为注册,3为退出 */
    while (1) {
        login_mune();
        printf("请选择你需要的功能:\n");
        scanf("%d", &choose);
        switch (choose) {
            case 1:
                   {
                       send_pack->type = LOGIN;
                       printf("请输入账号:\n");
                       scanf("%d", &send_pack->data.send_account);
                       getchar();
                       printf("请输入密码:\n");
                       i = 0;
                       while (1) {
                           tcsetattr(0, TCSANOW, &new);
                           scanf("%c", &ch);
                           tcsetattr(0, TCSANOW, &old);
                           if (ch == '\n') {
                               send_pack->data.read_buff[i] = '\0';
                               break;
                           }
                           send_pack->data.read_buff[i++] = ch;
                           printf("*");
                       }
                       printf("\n");
                      // scanf("%s", send_pack->data.read_buff);
                       memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                       if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                           my_err("send", __LINE__);
                       }
                       pthread_mutex_lock(&mutex_cli);
                       while (sing == 0) {
                            pthread_cond_wait(&cond_cli, &mutex_cli);
                       }
                       pthread_mutex_unlock(&mutex_cli);
                       printf("%s\n", send_pack->data.write_buff);
                       sing = 0;
                       break;
                   }
           case 2:
                   {
                       send_pack->type = REGISTERED;
                       printf("请输入昵称:\n");
                       scanf("%s", send_pack->data.send_user);
                       getchar();
                       printf("请输入密码:\n");
                       i = 0;
                       while (1) {
                           tcsetattr(0, TCSANOW, &new);
                           scanf("%c", &ch);
                           tcsetattr(0, TCSANOW, &old);
                           if (ch == '\n') {
                               send_pack->data.read_buff[i] = '\0';
                               break;
                           }
                           send_pack->data.read_buff[i++] = ch;
                           printf("*");
                       }
                       printf("\n");
                    // scanf("%s", send_pack->data.read_buff);
                       i = 0;
                       printf("请再次输入密码:\n");
                       while (1) {
                           tcsetattr(0, TCSANOW, &new);
                           scanf("%c", &ch);
                           tcsetattr(0, TCSANOW, &old);
                           if (ch == '\n') {
                               password[i] = '\0';
                               break;
                           }
                           password[i++] = ch;
                           printf("*"); 
                       }
                       printf("\n");
                      // scanf("%s", password);
                       if (strcmp(password, send_pack->data.read_buff) == 0) {
                           printf("两次输入一制OK\n");
                           if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                               my_err("send", __LINE__);
                           }
                           pthread_mutex_lock(&mutex_cli);
                           pthread_cond_wait(&cond_cli, &mutex_cli);
                           pthread_mutex_unlock(&mutex_cli);
                       } else {
                           printf("两次密码不一致!!!\n") ;
                           printf("按下回车继续.......");
                           choose = 5;
                           getchar();
                       }
                       break;
                   }
           case 3:
                   {
                       send_pack->type = FIND_PASSWORD;
                       printf("请输入你的账号:\n");
                       scanf("%d", &send_pack->data.send_account);
                       getchar();
                       printf("请输入你的好友数量:\n");
                       scanf("%d", &send_pack->data.recv_account);
                       getchar();
                       if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                           my_err("send", __LINE__);
                       }
                       pthread_mutex_lock(&mutex_cli);
                       pthread_cond_wait(&cond_cli, &mutex_cli);
                       pthread_mutex_unlock(&mutex_cli);
                       printf("%s\n", send_pack->data.write_buff);
                       printf("按下回车键继续.......");
                       getchar();
                       break;
                   }
           case 4:
                   {
                       send_pack->type = EXIT;
                       if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                           my_err("send", __LINE__);
                       }
                       pthread_exit(0);
                   }
           default:
                   {
                       printf("狗东西,好好输入!!!\n");
                       printf("输入回车继续......");
                       getchar();
                       getchar();
                       break;
                   }
        }
        if (choose > 4 || choose < 1) {
            continue;   
        } else if (choose == 1) {
            if (strcmp(send_pack->data.write_buff, "password error") == 0) {
                printf("密码错误或账号错误!!\n按下回车继续.....");
                getchar();
                continue;
            } else {
                printf("登陆成功!!\n");
                printf("按下回车继续......\n");
                getchar();
                break;
            }
        } else if (choose == 2) {
            printf("注册成功!!\n");
            printf("您的账号为:%d\n", send_pack->data.send_account);
            printf("按下回车继续.......");
            getchar();
            continue;
        } else if (choose == 3) {
            continue;
        }
    }
    while (1) {
        mune();
        printf("\n");
        scanf("%d", &choose);
        getchar();
        switch(choose) {
            case 1:
                    {
                        send_pack->type = CHANGE_PASSWORD;
                        printf("请输入原始密码:\n");
                        i = 0;
                        while (1) {
                            tcsetattr(0, TCSANOW, &new);
                            scanf("%c", &ch);
                            tcsetattr(0, TCSANOW, &old);
                            if (ch == '\n') {
                                send_pack->data.read_buff[i] = '\0';
                                break;
                            }
                            send_pack->data.read_buff[i++] = ch;
                            printf("*");
                        }
                        printf("\n");
                        // scanf("%s", send_pack->data.read_buff);
                        printf("请输入修改后的密码:\n");
                        i = 0;
                        while (1) {
                            tcsetattr(0, TCSANOW, &new);
                            scanf("%c", &ch);
                            tcsetattr(0, TCSANOW, &old);
                            if (ch == '\n') {
                                send_pack->data.write_buff[i] = '\0';
                                break;
                            }
                            send_pack->data.write_buff[i++] = ch;
                            printf("*");
                        }
                        printf("\n");
                        // scanf("%s", send_pack->data.write_buff);
                         int ret;
                        if ((ret = send(*(int *)sock_fd, send_pack, sizeof(PACK), 0)) < 0) {
                            my_err("send", __LINE__);
                        }
                        printf("^^%d\n", ret);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        printf("%s\n", recv_pack->data.write_buff);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("修改成功!!!\n");
                            printf("按下回车继续......\n");
                            getchar();
                        } else {
                            printf("修改失败!!!\n");
                            printf("按下回车继续.......\n");
                            getchar();
                        }
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        break;
                    }
            case 2:
                    {
                        send_pack->type = ADD_FRIEND;
                        printf("请输入你想添加好友的账号:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("发送成功等待对方确认!!\n");
                            printf("按下回车键继续.......");
                            getchar();
                        } else {
                            printf("对方已经是你的好友或者对方不存在!!!\n");
                            printf("按下回车键继续.......");
                            getchar();
                        }
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        break;
                    }
            case 3:
                    {
                        send_pack->type = DEL_FRIEND;
                        printf("请输入想要删除好友的账号:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("删除成功!!!\n");
                            printf("按下回车键继续..........");
                            getchar();
                        } else {
                            printf("你没有账号为%d的好友!!!\n", send_pack->data.recv_account);
                            printf("按下回车继续........");
                            getchar();
                        }
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        break;
                    }
            case 4:
                    {
                        pthread_mutex_lock(&mutex_cli);
                        send_pack->type = FRIENDS_PLZ;
                        if (box->friend_number == 0) {
                            printf("消息盒子里没有好友请求!!\n");
                            printf("输入回车继续......");
                            pthread_mutex_unlock(&mutex);
                            getchar();
                        } else {
                            for (int i = 0; i < box->friend_number; ++i) {
                                printf("%s\n", box->write_buff[i]);
                                send_pack->data.recv_account = box->plz_account[i];
                                printf("请选择: 1. 接受 2. 拒绝 3. 忽略\n");
                                scanf("%d", &choose);
                                getchar();
                                if (choose == 3) {
                                    continue;
                                } else if (choose == 1) {
                                    strcpy(send_pack->data.read_buff, "agree");
                                    if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                                        my_err("send", __LINE__);
                                    }
                                } else if (choose == 2) {
                                    strcpy(send_pack->data.read_buff, "disagree");
                                    if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                                        my_err("send", __LINE__);
                                    }
                                }
                            }
                            box->friend_number = 0;
                            printf("处理完毕!!\n");
                            printf("回车键继续.......");
                            pthread_mutex_unlock(&mutex_cli);
                            memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                            memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.read_buff));
                            getchar();
                        }
                        break;
                    }
            case 5:
                    {
                        send_pack->type = BLACK_FRIEND;
                        printf("请输入你想要拉黑的好友:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("账号为%d的好友已经被拉黑!!!\n", send_pack->data.recv_account);
                            printf("按下回车继续.......");
                            getchar();
                        } else {
                            printf("对方不是你的好友或对方不存在!!!\n");
                            printf("按下回车继续...........");
                            getchar();
                        }
                        break;
                    }
            case 6:
                    {
                        send_pack->type = WHITE_FRIEND;
                        printf("请输入你想从黑名单里去除的好友:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("账号为%d的好友从黑名单里剔除!!\n", send_pack->data.recv_account);
                            printf("按下回车继续......");
                            getchar();
                        } else {
                            printf("你没有这个好友或者这个好友不在黑名单!!\n");
                            printf("按下回车键继续.......");
                            getchar();
                        }
                        break;
                    }
            case 7:
                    {
                        send_pack->type = CARE_FRIEND;
                        printf("请输入你想要加入特别关心的好友账号:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("账号为%d的好友加入特别关心\n", send_pack->data.recv_account);
                            printf("按下回车继续......");
                            getchar();
                        } else {
                            printf("你没有这个好友!!\n");
                            printf("按下回车键继续.......");
                            getchar();
                        }
                        break;
                    }
            case 8:
                    {
                        send_pack->type = DISCARE_FRIEND;
                        printf("请输入你想要取关的好友账号:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("账号为%d的好友取消特别关心\n", send_pack->data.recv_account);
                            printf("按下回车继续......");
                            getchar();
                        } else {
                            printf("你没有这个好友!!\n");
                            printf("按下回车键继续.......");
                            getchar();
                        }
                        break;
                    }
            case 9:
                    {
                        send_pack->type = LOOK_LIST;
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        printf("%s\n", send_pack->data.write_buff);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("好友列表:\n");
                            for (int i = 0; i < list->friend_number; i++) {
                                printf("%d\t%-20s\t", list->friend_account[i], list->friend_nickname[i]);
                                if (list->friend_state[i] == 1) {
                                    printf("在线\n");
                                } else {
                                    printf("不在线\n");
                                }
                            }
                        } else {
                            printf("你还没有好友!!\n");
                        }
                        printf("按下回车键继续.......");
                        getchar();
                        break;
                    }
            case 10:
                    {
                        send_pack->type = SEND_FMES;
                        printf("请选择你要聊天的对象:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        printf("开始与账号为%d的好友对话\n", send_pack->data.recv_account);
                        while (1) {
                            scanf("%s", send_pack->data.read_buff);
                            if (strcmp(send_pack->data.read_buff, "#bey") == 0) {
                                printf("与账号为%d的好友的聊天结束\n", send_pack->data.recv_account);
                                break;
                            }
                            if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                                my_err("send", __LINE__);
                            }
                            pthread_mutex_lock(&mutex_cli);
                            pthread_cond_wait(&cond_cli, &mutex_cli);
                            pthread_mutex_unlock(&mutex_cli);
                            if (strcmp(send_pack->data.write_buff, "#fail") == 0) {
                                printf("没有账号为%d的好友\n", send_pack->data.recv_account);
                                break;
                            }
                        }
                        send_pack->data.recv_account = 0;
                        printf("按下回车键个继续...");
                        getchar();
                        getchar();
                        break;
                    }
            case 11:
                    {
                        if (box->talk_number == 0) {
                            printf("你没有未看的好友消息!\n");
                        } else {
                            for (int i = 0; i < box->talk_number; ++i) {
                                printf("账号%d:%s\n", box->send_account[i], box->read_buff[i]);
                            }
                            box->talk_number = 0;
                        }
                        printf("按下回车继续......");
                        getchar();
                        break;
                    }
            case 12:
                    {
                        send_pack->type = SEND_GMES;
                        printf("请输入你要聊天的群号:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        printf("开始发言:\n");
                        while (1) {
                            scanf("%s", send_pack->data.read_buff);
                            getchar();
                            if (strcmp(send_pack->data.read_buff, "#bey") == 0) {
                                printf("在群%d的发言结束\n", send_pack->data.recv_account);
                                break;
                            }
                            if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                                my_err("send", __LINE__);
                            }
                            pthread_mutex_lock(&mutex_cli);
                            pthread_cond_wait(&cond_cli, &mutex_cli);
                            pthread_mutex_unlock(&mutex_cli);
                            if (strcmp(send_pack->data.write_buff, "#fail") == 0) {
                                printf("没有群号为%d的群\n", send_pack->data.recv_account);
                                break;
                            }
                        }
                        send_pack->data.recv_account = 0;
                        printf("按下回车键个继续...");
                        getchar();
                        break;
                    }
            case 13:
                    {
                        send_pack->type = ADD_GROUP;
                        printf("请输入你想加的群的群号:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("加入群%d成功!\n", send_pack->data.recv_account);
                        } else {
                            printf("没有群号为%d的群!或你已加入该群!\n", send_pack->data.recv_account);
                        }
                        printf("按下回车键继续......");
                        getchar();
                        break;
                    }
            case 14:
                    {
                        send_pack->type = CREATE_GROUP;
                        printf("请输入你要创建群的名称:\n");
                        scanf("%s", send_pack->data.recv_user);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("创建群成功!!\n");
                            printf("群号%d\t群名%s\n", send_pack->data.recv_account, send_pack->data.recv_user);
                        } else {
                            printf("创建失败!!\n");
                        }
                        printf("按下回车键继续.....");
                        getchar();
                        break;
                    }
            case 15:
                    {
                        send_pack->type = EXIT_GROUP;
                        printf("请输入你要退出的群的群号:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("已退出群%d!!!\n", send_pack->data.recv_account);
                        } else {
                            printf("你没有加入这个群!!\n");
                        }
                        printf("按下回车键继续......");
                        getchar();
                        break;
                    }
            case 16:
                    {
                        send_pack->type = SET_ADMIN;
                        printf("请输入群号:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        printf("输入要被设置为管理员的群员账号:\n");
                        scanf("%s", send_pack->data.read_buff);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("账号为%s的成员被设置为群%d的管理员!!\n", send_pack->data.read_buff, send_pack->data.recv_account);
                        } else {
                            printf("群%d里面没有%s这个成员!!\n", send_pack->data.recv_account, send_pack->data.read_buff);
                        }
                        printf("按下回车继续.......");
                        getchar();
                        break;
                    }
            case 17:
                    {
                        send_pack->type = DEL_ADMIN;
                        printf("请输入群号:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        printf("输入要被取消管理员的群员账号:\n");
                        scanf("%s", send_pack->data.read_buff);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("账号为%s的成员被取消为群%d的管理员!!\n", send_pack->data.read_buff, send_pack->data.recv_account);
                        } else {
                            printf("群%d里面没有%s这个成员!!\n", send_pack->data.recv_account, send_pack->data.read_buff);
                        }
                        printf("按下回车继续.......");
                        getchar();
                        break;
                    }
            case 18:
                    {
                        send_pack->type = LOOK_MEMBER;
                        printf("输入你想查看的群:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (member_list->group_member_number == 0) {
                            printf("没有群号为%d的群!!!\n", recv_pack->data.recv_account);
                            printf("按下回车键继续.......");
                            getchar();
                            break;
                        }
                        for (int i = 0; i < member_list->group_member_number; ++i) {
                            printf("%d\t%-20s\t", member_list->group_member_account[i], member_list->group_member_nickname[i]);
                            if (member_list->group_member_state[i] == 1) {
                                printf("群主\n");
                            } else if (member_list->group_member_state[i] == 2) {
                                printf("管理员\n");
                            } else {
                                printf("底层群员\n");
                            }
                        }
                        printf("输入回车继续.......");
                        getchar();
                        break;
                    }
            case 19:
                    {
                        struct stat buf;
                        int fd;
                        send_pack->type = SEND_FILE;
                        printf("请输入你要发送文件的对象:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        printf("请输入你想要发送的文件的绝对路径(以'/'开头):\n");
                        scanf("%s", send_pack->data.write_buff);
                        getchar();
                        if (lstat(send_pack->data.write_buff, &buf) < 0) {
                            printf("文件名输入错误\n");
                            printf("按下回车继续......");
                            getchar();
                            break;
                        }
                        if ((fd = open(send_pack->data.write_buff, O_RDONLY)) < 0) {
                            printf("文件打开失败!!\n");
                            printf("按下回车继续.....");
                            getchar();
                            break;
                        }
                        sing = 0;
                        send_pack->data.cont = 0;
                        memset(send_pack->data.read_buff, 0, sizeof(send_pack->data.read_buff));
                        while (read(fd, send_pack->data.read_buff, 1023) > 0) {
                            if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                                my_err("send", __LINE__);
                            }
                            printf("正在发送...%d\n", send_pack->data.cont);
                            memset(send_pack->data.read_buff, 0, sizeof(send_pack->data.read_buff));    
                            pthread_mutex_lock(&mutex_cli);
                            while (sing == 0) {
                                pthread_cond_wait(&cond_cli, &mutex_cli);
                            }
                            pthread_mutex_unlock(&mutex_cli);
                            sing = 0;
                            send_pack->data.cont++;
                        }
                        close(fd);
                        printf("$$$\n");
                        send_pack->type = OK_FILE;
                        sing = 0;
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        printf("&&\n");
                        pthread_mutex_lock(&mutex_cli);
                        while (sing == 0) {   
                            pthread_cond_wait(&cond_cli, &mutex_cli);
                        }
                        pthread_mutex_unlock(&mutex_cli);
                        sing = 0;
                        printf("发送成功!!\n");
                        printf("按下回车键继续.....");
                        getchar();
                        break;
                    }
            case 20:
                    {
                        printf("请输入你要查看的好友:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        send_pack->type = READ_MESSAGE;
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (message->number == 0) {
                            printf("你和他之间还没有聊天记录!!\n");
                        } else {
                           for (int i = 0; i < message->number; ++i) {
                               printf("%d---->%d:\t%s\n", message->send_user[i], message->recv_user[i], message->message[i]);
                            }
                        }
                        printf("按下回车键继续.....");
                        getchar();
                        getchar();
                        break;
                    }
            case 21:
                    {
                        send_pack->type = DEL_MESSAGE;
                        printf("请输入要删除聊天记录的好友:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("删除成功!!\n");
                        } else {
                            printf("没有这个好友或与该好友没有记录!\n");
                        }
                        printf("按下回车键继续.....");
                        getchar();
                        break;
                    }
            case 22:
                    {
                        if (box->number == 0) {
                            printf("消息盒子里面没有群消息!!");
                        } else {
                            for (int i = 0; i < box->number; i++) {
                                printf("群号%d 发送人账号%d 消息内容:\t%s\n", box->group_account[i], box->send_account1[i], box->message[i]);    
                            }
                            box->number = 0;
                        }
                        printf("按下回车键继续......");
                        getchar();
                        break;
                    }
            case 23:
                    {
                        send_pack->type = DEL_MEMBER;
                        printf("输入你的群号:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        printf("请输入你的要删除的成员账号:\n");
                        scanf("%s", send_pack->data.read_buff);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("删除成功!!\n");
                        } else {
                            printf("你没有权限!!!!\n");
                        }
                        printf("按下回车键继续.....");
                        getchar();
                        break;
                    }
            case 24:
                    {
                        send_pack->type = LOOK_GROUP_LIST;
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (group_list->number == 0) {
                            printf("你还没加群!!\n");
                            printf("按下回车键继续........");
                            getchar();
                            break;
                        }
                        for (int i = 0; i < group_list->number; ++i) {
                            printf("%d\t%-20s\t", group_list->group_account[i], group_list->group_name[i]);
                            if (group_list->group_state[i] == 1) {
                                printf("群主\n");
                            } else if (group_list->group_state[i] == 2){
                                printf("管理员\n");
                            } else {
                                printf("底层群员\n");
                            }
                        }
                        printf("按下回车键继续......");
                        getchar();
                        break;
                    }
            case 25:
                    {
                        send_pack->data.cont = 0;
                        sing = 0;
                        if (file->have == 0) {
                            printf("没有人给你发文件!!\n");
                            printf("按下回车键继续.....");
                            getchar();
                            break;
                        } else {
                            printf("账号%d昵称%s的好友发来%s的文件\n", file->send_account, file->send_nickname, file->filename);
                            printf("请选择:\n1. 接收 2. 拒绝\n");
                            scanf("%d", &choose);
                            getchar();
                            if (choose == 1) {
                                while (1) {        
                                    send_pack->type = READ_FILE;
                                    send_pack->data.recv_account = file->send_account;
                                    memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                                    strcpy(send_pack->data.write_buff, file->filename);
                                    printf("%d\n",send_pack->data.cont);
                                    if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                                        my_err("send", __LINE__);
                                    }
                                    printf("开始接收文件.....");
                                    pthread_mutex_lock(&mutex_cli);
                                    while (sing == 0) {
                                        pthread_cond_wait(&cond_cli, &mutex_cli);
                                    }
                                    pthread_mutex_unlock(&mutex_cli);
                                    send_pack->data.cont++;
                                    sing = 0;
                                    if (strcmp(send_pack->data.write_buff, "ok") == 0) {
                                        printf("文件接收完毕!!!\n");
                                        printf("按下回车键继续......");
                                        getchar();
                                        break;
                                    }
                                }
                            } else {
                                printf("你已拒绝接收这个文件!!\n");
                                printf("按下回车继续......\n");
                                getchar();
                            }
                        }
                        break;
                    }
            case 26:
                    {
                            send_pack->type = EXIT;
                            if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                                my_err("send", __LINE__);
                            }
                            pthread_exit(0);
                            break;
                    }
        }
    }
}

void *thread_box(void *sock_fd) {
    if (recv(*(int *)sock_fd, box, sizeof(BOX), MSG_WAITALL) < 0) {
        my_err("recv", __LINE__);
    }
    pthread_exit(0);
}

void *thread_list(void *sock_fd) {
    memset(list, 0, sizeof(FRIEND));
    if (recv(*(int *)sock_fd, list, sizeof(FRIEND), MSG_WAITALL) < 0) {
        my_err("recv", __LINE__);
    }
    pthread_exit(0);
}

void *thread_recv_fmes(void *sock_fd) {
    if (recv_pack->data.send_account == send_pack->data.recv_account) {
        printf("账号为%d昵称为%s的好友说:\t%s\n", recv_pack->data.send_account, recv_pack->data.send_user, recv_pack->data.read_buff);
    } else if(strcmp(recv_pack->data.write_buff, "ohyeah") == 0){
        printf("来自特别关心%d昵称%s的好友说:\t%s\n", recv_pack->data.send_account, recv_pack->data.send_user, recv_pack->data.read_buff);
    } else {
        box->send_account[box->talk_number] = recv_pack->data.send_account;
        strcpy(box->read_buff[box->talk_number++], recv_pack->data.read_buff);
        printf("消息盒子里来了一条好友消息!\n");
    }
    pthread_exit(0);
}

void *thread_recv_gmes(void *sock_fd) {
    if (recv_pack->data.recv_account == send_pack->data.recv_account) {
        printf("群号%d 群名%s 账号%d 昵称%s:\t%s\n", recv_pack->data.recv_account, recv_pack->data.recv_user, recv_pack->data.send_account, recv_pack->data.send_user, recv_pack->data.read_buff);
    } else {
        printf("消息盒子里来了一条群消息!!\n");
        box->group_account[box->number] = recv_pack->data.recv_account;
        box->send_account1[box->number] = recv_pack->data.send_account;
        strcpy(box->message[box->number++], recv_pack->data.read_buff);
    }
}

/*void *thread_send_f(void *sock_fd) {
    int fd = open(recv_pack->data.write_buff, O_RDONLY);
    memset(recv_pack->data.read_buff, 0, sizeof(recv_pack->data.read_buff));
    while (read(fd, recv_pack->data.read_buff, 1023) > 0) {
        recv_pack->type = SEND_F;
        if (send(*(int *)sock_fd, recv_pack, sizeof(PACK), 0) < 0) {
            my_err("send", __LINE__);
        }
        printf("正在发送文件.....\n");
        memset(recv_pack->data.read_buff, 0, sizeof(recv_pack->data.read_buff));
        pthread_mutex_lock(&mutex_cli);
        while(sing ==0) {
            pthread_cond_wait(&cond_cli, &mutex_cli);
        }
        pthread_mutex_unlock(&mutex_cli); 
        sing = 0;
    }
    pthread_exit(0);
}*/

void *thread_recv_file(void *sock_fd) {
    memset(file, 0, sizeof(file));
    file->send_account = recv_pack->data.send_account;
    strcpy(file->send_nickname, recv_pack->data.send_user);
    strcpy(file->filename, recv_pack->data.write_buff);
    file->have = 1;
    printf("账号%d\t昵称%s\t的好友给你发送了一个%s文件快去接收吧\n", file->send_account, file->send_nickname, file->filename);
    pthread_exit(0);
}

void *thread_read_message(void *sock_fd) {
    if (recv(*(int *)sock_fd, message, sizeof(MESSAGE), MSG_WAITALL) < 0) {
        my_err("recv", __LINE__);
    }
    pthread_exit(0);
}

void *thread_member(void *sock_fd) {
    memset(member_list, 0, sizeof(GROUP));
    if (recv(*(int *)sock_fd, member_list, sizeof(GROUP), MSG_WAITALL) < 0) {
        my_err("recv", __LINE__);
    }
    pthread_exit(0);
}

void *thread_group_list(void *sock_fd) {
    memset(group_list, 0, sizeof(GROUP_G));
    if (recv(*(int *)sock_fd, group_list, sizeof(GROUP_G), MSG_WAITALL) < 0) {
        my_err("recv", __LINE__);
    }
    pthread_exit(0);
}

/*void *thread_read_file(void *sock_fd) {
    memset(recv_pack->data.read_buff, 0, sizeof(recv_pack->data.read_buff));
    if (recv(*(int *)sock_fd, recv_pack, sizeof(PACK), MSG_WAITALL) < 0) {
        my_err("recv", __LINE__);
    }
    int fd = open(recv_pack->data.write_buff"3.c", O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IXUSR);
    write(fd, recv_pack->data.read_buff, strlen(recv_pack->data.read_buff));
    close(fd);
    printf("正在接收.......\n");
    recv_pack->type = SEND_F;
    if (send(*(int *)sock_fd, recv_pack, sizeof(PACK), 0) < 0) {
        my_err("send", __LINE__);
    }
    pthread_exit(0);
}*/

void *thread_write(void *sock_fd) {
    pthread_t pid;
    int ret;
    group_list = (GROUP_G *)malloc(sizeof(GROUP_G));
    member_list = (GROUP *)malloc(sizeof(GROUP));
    list = (FRIEND *)malloc(sizeof(FRIEND));
    box = (BOX *)malloc(sizeof(BOX));
    recv_pack = (PACK*)malloc(sizeof(PACK));
    message = (MESSAGE *)malloc(sizeof(MESSAGE));
    group_message = (GROUP_MESSAGE *)malloc(sizeof(GROUP_MESSAGE));
    file = (FLE *)malloc(sizeof(FLE));
    file->have = 0;
    while (1) {
        memset(recv_pack, 0, sizeof(PACK));
        if ((ret = recv(*(int *)sock_fd, recv_pack, sizeof(PACK), MSG_WAITALL)) < 0) {
            my_err("recv", __LINE__);
        }
        switch(recv_pack->type) {
            case EXIT:
                    {
                        printf("end.......\n");
                        pthread_exit(0);
                        break;
                    }
            case EXIT_GROUP:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
        /*    case SEND_F:
                    {
                        pthread_create(&pid, NULL, thread_send_f, sock_fd);
                        pthread_join(pid, NULL);
                        break;
                    }*/
            case FIND_PASSWORD:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case DEL_MEMBER:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case LOOK_GROUP_LIST:
                    {
                        pthread_create(&pid, NULL, thread_group_list, sock_fd);
                        pthread_join(pid, NULL);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
           case SEND_FILE:
                    {
                        pthread_mutex_lock(&mutex_cli);
                        sing = 1;
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
           case RECV_FILE:
                    {
                        memset(file, 0, sizeof(file));
                        file->send_account = recv_pack->data.send_account;
                        strcpy(file->send_nickname, recv_pack->data.send_user);
                        strcpy(file->filename, recv_pack->data.write_buff);
                        file->have = 1;
                        printf("账号%d\t昵称%s\t的好友给你发送了一个%s文件快去接收吧\n", file->send_account, file->send_nickname, file->filename);
                        break; 
                       /* pthread_create(&pid, NULL, thread_recv_file, sock_fd);
                        pthread_join(pid, NULL);
                        break;*/
                    }
            case LOOK_MEMBER:
                    {
                        pthread_create(&pid, NULL, thread_member, sock_fd);
                        pthread_join(pid, NULL);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case OK_FILE:
                    {
                        pthread_mutex_lock(&mutex_cli);
                        sing = 1;
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case READ_FILE:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        int fd = open("3", O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IXUSR);
                        write(fd, recv_pack->data.read_buff, 1023);
                        close(fd);
                        pthread_mutex_lock(&mutex_cli);
                        sing = 1;
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case SET_ADMIN:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case DEL_ADMIN:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case CREATE_GROUP:
                    {
                        strcpy(send_pack->data.recv_user, recv_pack->data.recv_user);
                        send_pack->data.recv_account = recv_pack->data.recv_account;
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break; 
                    }
            case LOGIN:
                    {
                        strcpy(send_pack->data.send_user, recv_pack->data.send_user);
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        send_pack->data.send_fd = recv_pack->data.recv_fd;
                        pthread_create(&pid, NULL, thread_box, sock_fd);
                        pthread_join(pid, NULL);
                        printf("离线期间消息盒子中有%d条消息,%d个好友请求,%d条群消息", box->talk_number, box->friend_number, box->number);
                        pthread_mutex_lock(&mutex_cli);
                        sing = 1;
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break; 
                    }
            case DEL_MESSAGE:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case ADD_GROUP:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case ACCOUNT_ERROR:
                    {  
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        sing = 1;
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;  
                    }
            case REGISTERED:
                    {
                        send_pack->data.send_account = recv_pack->data.send_account;
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case CHANGE_PASSWORD:
                    {                          
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case ADD_FRIEND:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case FRIENDS_PLZ:
                    {
                        pthread_mutex_lock(&mutex_cli);
                        box->plz_account[box->friend_number] = recv_pack->data.send_account; 
                        strcpy(box->write_buff[box->friend_number], recv_pack->data.read_buff);
                        box->friend_number++;
                        printf("消息盒子中来了一条好友请求!!!\n");
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case DEL_FRIEND:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case BLACK_FRIEND:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case WHITE_FRIEND:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case CARE_FRIEND:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case DISCARE_FRIEND:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case LOOK_LIST:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        memset(list, 0, sizeof(FRIEND));
                        pthread_create(&pid, NULL, thread_list, sock_fd);
                        pthread_join(pid, NULL);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case SEND_FMES:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case SEND_GMES:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break; 
                    }
            case RECV_GMES:
                    {
                        pthread_create(&pid, NULL, thread_recv_gmes, sock_fd);
                        pthread_join(pid, NULL);                             
                        break;
                    }
            case RECV_FMES:
                    {
                        pthread_create(&pid, NULL, thread_recv_fmes, sock_fd);
                        pthread_join(pid, NULL);                             
                        break;
                    }
            case READ_MESSAGE:
                    {
                        pthread_mutex_lock(&mutex_cli);
                        pthread_create(&pid, NULL, thread_read_message, sock_fd);
                        pthread_join(pid, NULL);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
        }
    }
}


void mask_ctrl_c()
{
    printf("老哥求求你了别ctrl+c暴力退出了!!!!!!!!!\n");
}

int main() {
    int                 sock_fd;
    pthread_t           pid1;
    pthread_t           pid2;
    struct sockaddr_in  seve;
    
    sing = 0;
    pthread_mutex_init(&mutex_cli, NULL);
    pthread_cond_init(&cond_cli, NULL);
    sock_fd = my_accept_cli();
//    signal(SIGINT,mask_ctrl_c);
    pthread_create(&pid1, NULL, thread_read, (void *)&sock_fd);
    pthread_create(&pid2, NULL, thread_write, (void *)&sock_fd);
    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    
	return 0;
}
