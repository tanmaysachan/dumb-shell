#include <stdio.h>
#include <string.h>
#include <errno.h>

void
handle_error(char* cmd, int err)
{
    if (err == -105) {
	printf("execvp failed\n");
	return;
    }
    if (!strcmp(cmd, "cd") || !strcmp(cmd, "ls")) {
	if (err == -69) {
	    printf("Invalid arguments\n");
	    return;
	}
    }
    char* error = strerror(err);
    printf("%s\n", error);
    return;
}
