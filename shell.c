#include <shell.h>
#include <utils.h>
#include <error_handler.h>

void
initialise()
{
    home = (char *)malloc(STD_BUF*sizeof(char));
    getcwd(home, sizeof(home));
    HOME_LEN = strlen(home);
}

void
run_shell()
{
    while (1) {
        char pwd[STD_BUF];
        printf("%s ", get_prompt(getcwd(pwd, sizeof(pwd))));
        get_input();
        int ret_val = call_function();
        if (ret_val) {
            handle_error(last_command[0], ret_val);
        }
    }
}

int main()
{
    initialise();
    run_shell();
}
