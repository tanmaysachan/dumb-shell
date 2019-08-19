#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shell.h>

char*
get_prompt(char* pwd)
{
    if (prompt)
        free(prompt);
    
    prompt = (char *)malloc(1024*sizeof(char));

    // get user name
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

int main()
{
    char pwd[100];
    home = (char *)malloc(1024*sizeof(char));
    home = "here";
    HOME_LEN = 4;
    printf("%s\n", get_prompt(getcwd(pwd, sizeof(pwd))));
}
