#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_prompt(char* pwd);

void add_to_history();

void reset(char* arr[]);

void tokenize_command(char* cmd);

int check_is_bg(char* str);

int get_input();

int command_valid();

int call_function();

int call_function_bg();
