#include <utils.h>
#include <shell.h>
#include <shell_functions.h>
#include <function_lookup.h>

char*
get_prompt(char* pwd)
{
    if (prompt)
        free(prompt);
    
    prompt = (char *)malloc(1024*sizeof(char));

    int uid = getuid();
    struct passwd *user = getpwuid(uid);
    char *username = user->pw_name;
    
    char buf[1000];

    sprintf(buf, "<%s@UBUNTU:", username);
    strcpy(prompt, buf);

    memset(buf, 0, sizeof(buf));
    
    if (strcmp(pwd, home) == 0){
            strcat(prompt, "~");
    } else {
        if (!strncmp(home, pwd, strlen(home))) {
            char* tmp = pwd + HOME_LEN;
            sprintf(buf, "~%s", tmp);
            strcat(prompt, buf);
            
        } else {
            sprintf(buf, "%s", pwd);
            strcat(prompt, buf);
        }
    }
    strcat(prompt, ">");
    return prompt;
}

void
reset_last_command()
{
    for (int i = 0; i < STD_BUF; ++i) {
        last_command[i] = NULL;
    }
}

void
tokenize_command(char* cmd)
{
    reset_last_command();
    int cur = 0;
    last_command[cur] = strtok(cmd, " \r\t\n");
    while (last_command[cur]) {
        last_command[++cur] = strtok(NULL, " \r\t\n");
    }
}

void
get_input()
{
    char input[STD_BUF];
    fgets(input, STD_BUF, stdin);
    tokenize_command(input);
}

int
command_valid()
{
    return 1;
}

int
call_function()
{
    if (!command_valid()) {
        return 1;
    }
    return call_function_();
}
