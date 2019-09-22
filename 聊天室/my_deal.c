#define _MY_DEAL_C

#include "my_deal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <mysql/mysql.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "my_pack.h"
#include "my_err.h"
#include "my_mysql.h"
#include "my_friends.h"
#include "my_login.h"
#include "my_group.h"
#include "my_talk.h"

void *deal(void *recv_pack) {
    pthread_detach(pthread_self());
	PACK               *pack;
	int                i;
    BOX                *tmp = box_head;
	MYSQL              mysql;
	mysql = accept_mysql();
	pack = (PACK*)recv_pack;
	switch(pack->type)
	{
		case LOGIN:
			{
				if (login(pack, mysql) != 0) {
                    pack->type = ACCOUNT_ERROR;
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
					strcpy(pack->data.write_buff, "password error");
                    if (send(pack->data.recv_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
				} else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "good");
                    int ret;
                    if ((ret = send(pack->data.recv_fd, pack, sizeof(PACK), 0)) < 0) {
                        my_err("send", __LINE__);
                    }
                    while (tmp != NULL) {
                        if (tmp->recv_account == pack->data.send_account) {
                            break;
                        }
                        tmp = tmp->next;
                    }
                    if (tmp == NULL) {
                        tmp = (BOX *)malloc(sizeof(BOX));
                        tmp->recv_account = pack->data.send_account;
                        tmp->talk_number = tmp->friend_number = 0;
                        tmp->number = 0;
                        tmp->next = NULL;
                        if (box_head == NULL) {
                            box_head = box_tail = tmp;
                        } else {
                            box_tail->next = tmp;
                            box_tail = tmp;
                        }
                        if (send(pack->data.recv_fd, tmp, sizeof(BOX), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                    } else {
                        if (send(pack->data.recv_fd, tmp, sizeof(BOX), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        tmp->friend_number = 0;
                        tmp->talk_number = 0;
                    }
                }
                break;
			}
        case REGISTERED:
            {
                registered(pack, mysql);
                memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                strcpy(pack->data.write_buff, "registered success!!");
                if (send(pack->data.recv_fd, pack, sizeof(PACK), 0) < 0) {
                    my_err("send", __LINE__);
                }
                
                break;
            }
        case CHANGE_PASSWORD:
            {
                if (change_password(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.recv_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.recv_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);        
                    }
                }
                break;
            }
        case ADD_FRIEND:
            {
                if (add_fir(pack, mysql) == 0) {
                    pack->type = ADD_FRIEND;
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.recv_fd, pack, sizeof(PACK) , 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.recv_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);        
                    } 
                }
                break;
            }
        case FRIENDS_PLZ:
            {
                friends_plz(pack, mysql);
                break;
            }
        case DEL_FRIEND:
            {
                if (del_friend(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                }
                break;
            }
        case BLACK_FRIEND:
            {
                if (black_friend(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    } 
                }
                break;
            }
        case WHITE_FRIEND:
            {
                if (white_friend(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    } 
                }
                break;
            }
        case CARE_FRIEND:
            {
                if (care_friend(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    } 
                }
                break;
            }
        case DISCARE_FRIEND:
            {
                if (discare_friend(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    } 
                }
                break;
            }
        case LOOK_LIST:
            {
                FRIEND *list = look_list(pack, mysql);
                if (list->friend_number != 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                    if (send(pack->data.send_fd, list, sizeof(FRIEND), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                    if (send(pack->data.send_fd, list, sizeof(FRIEND), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                }
                break;       
            }
        case SEND_FMES:
            {
                if (send_fmes(pack, mysql) == -1) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "#fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "#succss");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                }
                break;
            }
        case SEND_GMES:
            {
                if (send_gmes(pack, mysql) == -1) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "#fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "#succss");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                }
                break;
            }
        case READ_MESSAGE:
            {
                read_message(pack, mysql);
                break;
            }
        case DEL_MESSAGE:
            {
                if (del_message(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    } 
                }
                break;    
            }
        case CREATE_GROUP:
            {
                if (create_group(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    printf("%s\n", pack->data.write_buff);
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    } 
                }
                break; 
            }
        case ADD_GROUP:
            {
               if (add_group(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    printf("%s\n", pack->data.write_buff);
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    } 
                }
                break;  
            }
        case EXIT_GROUP:
            {
                if (exit_group(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    printf("%s\n", pack->data.write_buff);
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    } 
                }
                break; 
            }
        case SET_ADMIN:
            {
                if (set_admin(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    printf("%s\n", pack->data.write_buff);
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    } 
                }
                break; 
            }
        case DEL_ADMIN:
            {
                if (del_admin(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    printf("%s\n", pack->data.write_buff);
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    } 
                }
                break; 
            }
        case LOOK_MEMBER:
            {
                look_member(pack, mysql);
                break;
            }
        case LOOK_GROUP_LIST:
            {
                look_group_list(pack, mysql);
                break;
            }
        case FIND_PASSWORD:
            {
                find_password(pack, mysql);
                break;
            }
        case DEL_MEMBER:
            {
                 if (del_member(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    } 
                }
                break;   
            }
        case SEND_FILE:
            {
                pthread_mutex_lock(&mutex);
                int fd = open("2", O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IXUSR);
                write(fd, pack->data.read_buff, 1023);
                close(fd);
                printf("%d\n", pack->data.cont);
                if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                    my_err("send", __LINE__);
                }
                pthread_mutex_unlock(&mutex);
                break;
            }
        case READ_FILE:
            {
               pthread_mutex_lock(&mutex);
               int fd = open("2", O_RDONLY);
               lseek(fd, 1023*pack->data.cont, SEEK_SET);
               memset(pack->data.read_buff, 0, sizeof(pack->data.read_buff));
               if (read(fd, pack->data.read_buff, 1023) == 0) {
                   strcpy(pack->data.write_buff, "ok");
               }
               if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                    my_err("send", __LINE__);
               }
               close(fd);
               pthread_mutex_unlock(&mutex);
               break;
            }
        case OK_FILE:
            {
                    printf("sssssss^^\n");
                ok_file(pack, mysql);
                break;
            }
       
	}
	close_mysql(mysql);
}

