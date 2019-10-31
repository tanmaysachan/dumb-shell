#include <stdio.h>
#include <string.h>
#include <errno.h>

void handle_error(char *cmd, int err)
{
    if (err == -105)
    {
        printf("execvp failed\n");
        return;
    }
    if (err == -69)
    {
        printf("Invalid arguments\n");
        return;
    }
    if (err == -100)
    {
        printf("Process does not exist\n");
        return;
    }

    char *error = strerror(err);
    printf("%s\n", error);
    return;
}
