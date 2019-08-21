#include <pwd.h>
#include <unistd.h>

#define STD_BUF 1024

char* home;

int HOME_LEN;

char* prompt;

char* last_command[STD_BUF];
