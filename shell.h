#include <pwd.h>
#include <unistd.h>
#include <sys/wait.h>

#define STD_BUF 1024

char* home;

char* cwd;

int HOME_LEN;

char* prompt;

char* one_shot[STD_BUF];

char* last_command[STD_BUF];

char* last_command_full[STD_BUF];

char last_input[STD_BUF];

char* history[20];

int last_command_end;

int G_PID;

int last_command_full_end;

struct {char* pname[STD_BUF]; int pid; int state;} PROCS[STD_BUF];

int IS_SUBP;

int IS_SUBPEXEC;
