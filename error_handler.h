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
    if (!strcmp(cmd, "cd")) {
	if (err == -69) {
	    printf("1 Argument expected\n");
	    return;
	}
    }
    char* error = strerror(err);
    printf("%s\n", error);
    return;
}
