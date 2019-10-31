#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_prompt(char *pwd);

void add_to_history();

void reset(char *arr[]);

void tokenize_command(char *cmd);

void exec_command(char *str);

int check_is_bg(char *str);

int get_input(char *pre_inp);

int command_valid();

int call_function();

void add_proc(char **pname, int pid);

void del_proc(int pid);

void find_and_delete(int sig);

void handle_sigs(int sig);

int call_function_bg();
