#include<stdio.h>
#include<string.h>

int main() {
    char str[] = "apple, orange, banana, grape";
    char *token;
    char *rest = str; // Pointer to the current position in the string

    printf("Original string: %s\n", str);
    token = strsep(&rest, ",");
    // while ((token = strsep(&rest, ",")) != NULL) {
    //     printf("Token: %s\n", token);
    //     printf("str: %s\n",str);
    // }
    printf("\n%s\n",str);
    return 0;
}
