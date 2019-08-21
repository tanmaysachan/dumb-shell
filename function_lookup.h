#include <string.h>

int
call_function_()
{
    if (!strcmp(last_command[0], "cd")) {
        return func_cd();
    }
    if (!strcmp(last_command[0], "ls")) {
        return func_ls();
    }
    if (!strcmp(last_command[0], "echo")) {
        return func_echo();
    }
}
