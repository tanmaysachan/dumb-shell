#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_prompt(char* pwd);

void reset_last_command();

void tokenize_command(char* cmd);

void get_input();

int command_valid();

int call_function();
