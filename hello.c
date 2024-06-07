#include <stdio.h>
#include <unistd.h>
#include <pwd.h>  //得到用户信息
#include <libgen.h>
#include <readline/readline.h>
#include <readline/history.h>

int main()
{
    while(1) {
        printf("hello\n");
        sleep(10);
    }
    return 0;
}