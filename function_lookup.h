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
    if (!strcmp(last_command[0], "pwd")) {
        return func_pwd();
    }
    if (!strcmp(last_command[0], "pinfo")) {
        return func_pinfo();
    }
    if (!strcmp(last_command[0], "history")) {
        return func_history();
    }
    if (!strcmp(last_command[0], "exit")) {
        return func_exit();
    }
    return func_execvp();
}
