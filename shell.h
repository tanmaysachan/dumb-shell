#include <pwd.h>
#include <unistd.h>

#define STD_BUF 1024

char* home;

int HOME_LEN;

char* prompt;

char* one_shot[STD_BUF];

char* last_command[STD_BUF];

char last_input[STD_BUF];

char* history[20];

int last_command_end;

int IS_SUBP;

int IS_SUBPEXEC;
