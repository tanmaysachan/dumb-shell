#include <string.h>

int call_function_()
{
    if (!strcmp(last_command[0], "cd"))
    {
        return func_cd();
    }
    if (!strcmp(last_command[0], "ls"))
    {
        return func_ls();
    }
    if (!strcmp(last_command[0], "echo"))
    {
        return func_echo();
    }
    if (!strcmp(last_command[0], "pwd"))
    {
        return func_pwd();
    }
    if (!strcmp(last_command[0], "pinfo"))
    {
        return func_pinfo();
    }
    if (!strcmp(last_command[0], "history"))
    {
        return func_history();
    }
    if (!strcmp(last_command[0], "quit"))
    {
        return func_quit();
    }
    if (!strcmp(last_command[0], "setenv"))
    {
        return func_setenv();
    }
    if (!strcmp(last_command[0], "unsetenv"))
    {
        return func_unsetenv();
    }
    if (!strcmp(last_command[0], "jobs"))
    {
        return func_jobs();
    }
    if (!strcmp(last_command[0], "kjob"))
    {
        return func_kjob();
    }
    if (!strcmp(last_command[0], "overkill"))
    {
        return func_overkill();
    }
    if (!strcmp(last_command[0], "fg"))
    {
        return func_fg();
    }
    if (!strcmp(last_command[0], "bg"))
    {
        return func_bg();
    }
    if (!strcmp(last_command[0], "cronjob"))
    {
        return func_cronjob();
    }
    return func_execvp();
}
