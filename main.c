#include "shell.h"

#define MaxSize 256
//let me study git!
// https://www.cnblogs.com/LiuYanYGZ/p/14806139.html

bool flag = false; // extern外部引用，前者必须是全局变量！！！
volatile sig_atomic_t sigint_received = 0;

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
    if (sig == SIGINT) {
        sigint_received = 1;
    } else if (sig == SIGTSTP) {
        const char *message = "\nReceived SIGTSTP (Ctrl+Z). Type 'exit' to quit the shell.\n";
        write(STDOUT_FILENO, message, strlen(message));
    }
}

// history 没有历史命令处理
//& !!

int main() {
  pid_t output_command;
  char *input;
  char *command;
  char *argv[MaxSize];
  int argc = 0;
  char prompt[MaxSize] = {0};


  interface(prompt);

  using_history(); // 使用历史记录！

  signal(SIGINT, handle_signal);
  signal(SIGTSTP, handle_signal);


  // output_command = fork();
  // if(output_command == 0) {
  //   while(1) {
  //     printf("xxx");
  //     sleep(1);
  //     show_history();
  //   }
  // }
    while (1) {
      //
        if (sigint_received) {
            const char *message = "\nReceived SIGINT (Ctrl+C). Type 'exit' to quit the shell.\n";
            write(STDOUT_FILENO, message, strlen(message));
            sigint_received = 0;
        }
      //
      input = readline(prompt);
      
      // 检查输入是否为空
      if (input == NULL) {
        printf("\n");
        break;
      }
      else if (*input == '\0') {
            continue;
      }
      // 如果输入非空，将其添加到历史记录中
      else if (*input && !(strstr(input, "!"))) {
        add_history(input);
      }

      // 使用分号分隔多个命令
      command = strsep(&input, ";");
      while (command != NULL) {
        if (strcmp(command, "history") == 0) {
          HISTORY_STATE *history_state = history_get_history_state();
          if (history_state && history_state->length >= 0)
            show_history();
          else {
            perror("history error: no orders");
          }
        }
        else if (strcmp(command, "exit") == 0) {
          return 0;
        }
        // 如果是外置命令，直接用exec函数族执行
        else {
          prase_command(command, argv, &argc);
          execute_command(command, argv, &argc);
        }

        command = strsep(&input, ";");
      }
    }
    // 释放内存，防止泄漏
    free(input);
    free(command);
    for (int i = 0; i < argc; i++) {
      free(argv[i]);
    }

  return 0;
}