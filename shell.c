#include <shell.h>
#include <utils.h>

void
initialise()
{
    home = getcwd(home, sizeof(home));
    HOME_LEN = strlen(home);
}

void
run_shell()
{
    while (true) {
        get_input();
        call_function();
    }
}

int main()
{
    initialise();
    run_shell();
}
