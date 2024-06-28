#include "shell.h"
#include <stdio.h>
//123
extern bool flag;
extern pid_head head;

void execute_command(char *command, char **argv, int *argc) {
  pid_t pid;
  int status;
  if ((pid = fork()) == -1) {
    perror("fork error");
    exit(EXIT_FAILURE);
  }
  if (pid == 0) // 子进程exec代替，来执行命令
  {
    if (strcmp(command, "history") == 0) {
      show_history();
    } 
    else if (execvp(argv[0], argv) == -1) {
      perror("exec error");
    }
    exit(EXIT_FAILURE);
  } else { 
  // 父进程
  //flag == true 代表后台进行，插入pid 以便后续程序结束释放子进程
    if (flag) {
      insert_pid(pid, &head);
      flag = false;
    } else {
        waitpid(pid, &status, 0);
  }
}
}

