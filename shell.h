#include <stdio.h>
#include <unistd.h>
#include <pwd.h>  //得到用户信息
#include <sys/wait.h>
#include <libgen.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <fcntl.h>

#define MaxSize 256

typedef struct pid_list pid_list;
typedef pid_list* pid_head;



void interface(char*);
void prase_command(char* command, char** argv, int *argc);
void handle_history_command(char*);
void execute_command(char*, char**, int*);
void show_history();
pid_head init_pid(pid_head *head);
pid_head insert_pid(pid_t new_pid, pid_head *head);
void kill_child_process(pid_head head);
void handle_signal(int sig);
void child_handle_signal(int sig);