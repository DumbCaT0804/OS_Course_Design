#include "shell.h"


void interface(char* prompt)
{
  char hostname[MaxSize];
  char curren_path[MaxSize];

  hostname[MaxSize - 1] = '\0';
  curren_path[MaxSize - 1] = '\0';

  gethostname(hostname, MaxSize - 2);
  getcwd(curren_path,MaxSize - 2);
  uid_t uid = getuid();
  struct passwd *pw = getpwuid(uid);
  char *base = basename(curren_path);

  sprintf(prompt,"%s@%s %s $ ", pw->pw_name, hostname, base);
}

