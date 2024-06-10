
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

#define MaxSize 256
// let me study git!
//  https://www.cnblogs.com/LiuYanYGZ/p/14806139.html

bool flag = false; // extern外部引用，前者必须是全局变量！！！

typedef struct pid_list {
  pid_t pid;
  struct pid_list *next;
} pid_list, *pid_head;

pid_head head = NULL; // 提前定义，是为了能够extern让其他文件引用

pid_head init_pid(pid_head *head) {
  *head = (pid_list *)malloc(sizeof(pid_list));
  if (head == NULL) {
    perror("memory allocation error");
    exit(EXIT_FAILURE);
  }
  (*head)->next = NULL;

  return *head;
}

pid_head insert_pid(pid_t new_pid, pid_head *head) {
  pid_list *new_node = (pid_list *)malloc(sizeof(pid_list));

  if (new_node == NULL) {
    perror("memory allocation error");
    exit(EXIT_FAILURE);
  }

  new_node->pid = new_pid;
  new_node->next = (*head)->next;
  (*head)->next = new_node;
  printf("插入成功\n");
  printf("%d\n", (*head)->next->pid);
  return *head;
}

void kill_child_process(pid_head head) {
  pid_list *current = head->next;
  pid_list *temp;
  printf("kill 子进程\n");
  while (current != NULL) {
    temp = current;
    printf("子进程：%d\n", temp->pid);
    kill(temp->pid, SIGKILL);
    current = current->next;
    free(temp);
  }

  free(head);
}

void show_history() {
  int i = 0;
  HIST_ENTRY **his;
  his = history_list();
  while (his[i] != NULL) {
    printf("%d %s\n", i + 1, his[i]->line);
    i++;
  }
}

void handle_signal(int sig) {
  if (sig == SIGINT)
    printf("\nReceived SIGINT (Ctrl+C). Type 'exit' to quit the shell.\n");
  else if (sig == SIGTSTP)
    printf("\nReceived SIGTSTP (Ctrl+Z). Type 'exit' to quit the shell.\n");
}

void child_handle_signal(int sig) {
  // 让子进程对于ctrl + c 等不做处理
}

int main() {
  pid_t monitoring_process;
  int p[2];
  int count = 1;
  char *input;
  char *command;
  char pipe_write[MaxSize];
  char pipe_read[MaxSize];
  char pipe_list[MaxSize * 10] = {0};
  char *argv[MaxSize];
  int argc = 0;
  char prompt[MaxSize] = {0};

  init_pid(&head);
  interface(prompt);

  using_history(); // 使用历史记录！

  signal(SIGINT, handle_signal);
  signal(SIGTSTP, handle_signal);

  if (-1 == pipe(p)) {
    printf("pipe error!\n");
    return -1;
  } // 保证父子进程都有pipe;

  monitoring_process = fork();

  // 预处理 父子进程
  if (monitoring_process == 0) {
    signal(SIGINT, child_handle_signal);
    signal(SIGTSTP, child_handle_signal);

    close(p[1]); // 关闭子进程写功能
  } else {
    insert_pid(monitoring_process, &head); // 插入子进程pid
    close(p[0]);                           // 关闭父进程读功能
  }
  // 实际操作 父子进程
  if (monitoring_process == 0) {
    pipe_list[0] = '\n';
    sleep(5);
    while (1) {
      // fcntl(p[0],F_SETFL, O_NONBLOCK);  //取消阻塞
      int bytes_read = read(p[0], pipe_read, MaxSize);
      if (bytes_read > 0) { // 目前是输入命令后，监督进程在等待后输出已输入命令
        strncat(pipe_list, pipe_read, MaxSize * 10 - strlen(pipe_list) - 1);
        printf("%s", pipe_list);
        sleep(3); // 把这两条移出来就变成固定时间里
      }
    }
  } else {
    while (1) {
      input = readline(prompt);
      // 检查输入是否为空
      if (input == NULL) {
        printf("\n");
        break;
      } else if (*input == '\0') {
        continue;
      }
      // 如果输入非空，将其添加到历史记录中
      else if (*input && !(strstr(input, "!"))) {
        add_history(input); // 不加！这个的，因为它不是一条命令
      }
      sprintf(pipe_write, "%d %s\n", count, input);
      write(p[1], pipe_write, strlen(pipe_write));
      // 使用分号分隔多个命令
      command = strsep(&input, ";");
      while (command != NULL) {
        if (strcmp(command, "history") == 0) {
          HISTORY_STATE *history_state = history_get_history_state();
          if (history_state && history_state->length >= 0)
            show_history(); // history命令展示
          else {            // 如果没有命令的话~ !!展示错误！
            // 我放到了prase.c进行处理
          }
        } else if (strcmp(command, "exit") == 0) {

          //!!!!!子
          kill_child_process(head);
          // kill(monitoring_process, SIGKILL);

          free(input);
          free(command);

          return 0;
        }
        // 如果是外置命令，直接用exec函数族执行
        else {
          prase_command(command, argv, &argc);
          execute_command(command, argv, &argc);
        }

        command = strsep(&input, ";");
      }
      count++;
    }
  }

  // 释放内存，防止泄漏

  return 0;
}