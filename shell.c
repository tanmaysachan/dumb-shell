#include <shell.h>
#include <utils.h>
#include <error_handler.h>

void
initialise()
{
    home = getcwd(NULL, 0);
    HOME_LEN = strlen(home);
    IS_SUBP = 0;
    IS_SUBPEXEC = 0;
    char hist_file_loc[STD_BUF];
    sprintf(hist_file_loc, "%s/.history", home);
    cwd = (char *)malloc(STD_BUF * sizeof(char));
    strcpy(cwd, home);

    FILE* hist_file = fopen(hist_file_loc, "r");
    int cur = 0;
    
    if (hist_file) {
	while (1) {
	    char* tmp_buf = (char *)malloc(STD_BUF * sizeof(char));
	    char* tmp_ptr = fgets(tmp_buf, STD_BUF, hist_file);
	    if (tmp_ptr) {
		history[cur] = (char *)malloc(STD_BUF * sizeof(char));
		strcpy(history[cur], tmp_buf);
		cur++;
	    } else {
		break;
	    }
	}
    }

    for (int i = cur; i < 20; i++) {
	history[cur] = NULL;
    }
    
}

void
run_shell()
{
    while (1) {

	char pwd[STD_BUF];
	printf("%s ", get_prompt(getcwd(pwd, sizeof(pwd))));
	strcpy(cwd, pwd);

	int empty_inp = get_input();
	if (empty_inp)
	    continue;

	add_to_history();

	int cur = 0;

	while (one_shot[cur]) {
	    getcwd(pwd, sizeof(pwd));
	    strcpy(cwd, pwd);
	    tokenize_command(one_shot[cur++]);
	    int ret_val;
	    if (check_is_bg(last_command[last_command_end])) {
		ret_val = call_function_bg();
	    } else {
		ret_val = call_function();
	    }
	    if (ret_val) {
		handle_error(last_command[0], ret_val);
	    }
	}

	if (IS_SUBP) {
	    printf("\r[-1] command: %s, pid: %d                \n", last_command[0], getpid());
	    fflush(stdin);
            fputc('\n', stdout);
	    exit(0);
	} else if (IS_SUBPEXEC) {
	    fflush(stdin);
	    exit(0);
	}
    }
}

int main()
{
    initialise();
    run_shell();
}
