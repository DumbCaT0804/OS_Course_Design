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
  if (pid == 0) // 子进程
  {
    if (strcmp(command, "history") == 0) {
      show_history();
    } 
    // else if(strstr(command, "cd") != NULL) {
    //   int x = chdir(argv[1]);
    //     if(x == -1) {
    //       printf("bash: cd: %s/: No such file or directory", argv[1]);
    //     }
    //     printf("%s\n",argv[1]);
    // }
    else if (execvp(argv[0], argv) == -1) {
      perror("exec error");
    }
    exit(EXIT_FAILURE);
  } else { // 父进程
    if (flag) {
      insert_pid(pid, &head);
      flag = false;
    } else {
      do {
        pid_t wpid = waitpid(pid, &status, WUNTRACED | WCONTINUED);
        if (wpid == -1) {
          perror("waitpid error");
          exit(EXIT_FAILURE);
        }
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
  }
}
