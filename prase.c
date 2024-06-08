#include "shell.h"
//let us study git!
extern bool flag;

void prase_command(char *command, char **argv, int *argc) {
  argv[0] = strtok(command, " ");
  char *history_command = strstr(argv[0], "!");
  if (history_command)
    handle_history_command(history_command);
  for (*argc = 1; argv[*argc - 1] != NULL; (*argc)++) {
    argv[*argc] = strtok(NULL, " ");
    if (argv[*argc] == NULL)
      break;
    char *replace = strstr(argv[*argc], "&");
    if (replace) {
      flag = true;
      memset(replace, '\0', strlen("&")); // ret是首地址
    }
    history_command = strstr(argv[*argc], "!");
    if (history_command)
      handle_history_command(history_command);

  } // 对命令进一步分割： 采用" "分割
}

void handle_history_command(char *history_command) {
  HIST_ENTRY **his;
  his = history_list();
  int i;
  if (history_command[0] == '!' &&
      (history_command[1] >= '1' && history_command[1] <= '9')) {
    i = (int)history_command[1] - 48;
  }
  else if (history_command[0] == '!' && history_command[1] == '!' ) {
    HISTORY_STATE *history_state = history_get_history_state();
    i = history_state->length;
  }
  HIST_ENTRY *entry = history_get(i);
  if (entry) {
    strcpy(history_command, entry->line);
  } else {
    // Handle the case where the history entry does not exist
    if (history_command[1] == '!') {
      fprintf(stderr, "history error: no orders\n");
    }
    else
      fprintf(stderr, "Error: History entry %c does not exist.\n",
            history_command[1]);
  }
  //   while (his[i] != NULL)
  //   {
  //       printf("%d %s\n", i + 1, his[i]->line);
  //       i++;
  //  }
}