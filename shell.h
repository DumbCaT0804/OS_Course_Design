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

#define MaxSize 256



void interface(char*);
void prase_command(char* command, char** argv, int *argc);
void handle_history_command(char*);
void execute_command(char*, char**, int*);
 void show_history();
