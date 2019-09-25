#include <shell.h>
#include <utils.h>
#include <shell_functions.h>
#include <function_lookup.h>
#include <error_handler.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

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
add_to_history()
{
    if (history[19]) {
	free(history[19]);
    }

    for (int i = 19; i >= 1; --i) {
	history[i] = history[i-1];
    }

    history[0] = (char *)malloc(STD_BUF * sizeof(char));
    strcpy(history[0], last_input);

    char* hist_file_loc = (char *)malloc(STD_BUF*sizeof(char));
    sprintf(hist_file_loc, "%s/.history", home);

    FILE* hist_file = fopen(hist_file_loc, "w");
    for (int i = 0; history[i] && i < 20; ++i) {
	fputs(history[i], hist_file);
	fflush(hist_file);
    }
}

void
reset(char* arr[])
{
    for (int i = 0; i < STD_BUF; ++i) {
	arr[i] = NULL;
    }
}

void
tokenize_command(char* cmd)
{
    reset(last_command_full);
    int cur = 0;
    last_command_full[cur] = strtok(cmd, " \r\t\n");
    while (last_command_full[cur]) {
	last_command_full[++cur] = strtok(NULL, " \r\t\n");
    }
    last_command_full_end = --cur;
}

void
exec_command(char* str) {
    int ret_val;

    int savestdin = dup(0);
    int savestdout = dup(1);
    if (last_command_end-3 >= 0 && last_command[last_command_end-3][0] == '<' && last_command_end-1 >= 0 && (last_command[last_command_end-1][0] == '>' || !strcmp(last_command[last_command_end-1], ">>"))) {
        int fdin = open(last_command[last_command_end-2], O_RDONLY | O_CREAT, S_IRWXU);
        int fdout;
        if (!strcmp(last_command[last_command_end-1], ">>")) fdout = open(last_command[last_command_end], O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
        else if (last_command[last_command_end-1][0] == '>') fdout = open(last_command[last_command_end], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        dup2(fdin, 0);
        dup2(fdout, 1);
        int tmp = last_command_end;
        for (int i = last_command_end-3; i <= tmp; i++) {
            last_command[i] = NULL;
            last_command_end--;
        }
        ret_val = call_function();
    } else if (last_command_end-1 >= 0 && last_command[last_command_end-1][0] == '<') {
        int fdin = open(last_command[last_command_end], O_RDONLY | O_CREAT, S_IRWXU);
        dup2(fdin, 0);
        int tmp = last_command_end;
        for (int i = last_command_end-1; i <= tmp; i++) {
            last_command[i] = NULL;
            last_command_end--;
        }
        ret_val = call_function();
    } else if (last_command_end-1 >= 0 && (last_command[last_command_end-1][0] == '>' || !strcmp(last_command[last_command_end-1], ">>"))) {
        int fdout;
        if (!strcmp(last_command[last_command_end-1], ">>")) fdout = open(last_command[last_command_end], O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
        else if (last_command[last_command_end-1][0] == '>') fdout = open(last_command[last_command_end], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        dup2(fdout, 1);
        int tmp = last_command_end;
        for (int i = last_command_end-1; i <= tmp; i++) {
            last_command[i] = NULL;
            last_command_end--;
        }
        ret_val = call_function();
    } else {
        if (check_is_bg(last_command[last_command_end])) {
            ret_val = call_function_bg();
        } else {
            ret_val = call_function();
        }
    }
    dup2(savestdin, 0);
    dup2(savestdout, 1);
    if (ret_val) {
        handle_error(last_command[0], ret_val);
    }
}

int
check_is_bg(char* str)
{
    if (str[strlen(str)-1] == '&' && strlen(str) > 1) {
	str[strlen(str)-1] = '\0';
	return 1;
    } else if (!strcmp(last_command[last_command_end], "&")) {
	last_command[last_command_end] = NULL;
	last_command_end--;
	return 1;
    }
    return 0;
}

int
get_input(char* pre_inp)
{
    char input[STD_BUF];
    if (pre_inp) {
        strcpy(input, pre_inp);
    } else {
        fgets(input, STD_BUF, stdin);
        strcpy(last_input, input);
    }

    if (!strcmp(input, "\n")) {
	return -11;
    }
    
    int cur = 0;
    int ret = 0;
    while (1) {
	if (cur + 2 >= strlen(input)-1) {
	    if (ret >= 1) {
		return ret;
	    }
	    break;
	}
	if (input[cur] == 27 && input[cur+1] == 91 &&
	    input[cur+2] == 65) {
	    ret++;
	    cur += 3;
	} else {
	    break;
	}
    }

    cur = 0;
    reset(one_shot);
    one_shot[cur] = strtok(input, ";");
    while (one_shot[cur]) {
	one_shot[++cur] = strtok(NULL, ";");
    }
    return 0;
}

int
call_function()
{
    return call_function_();
}

int call_function_bg()
{
    int pid = fork();
    if (pid == 0) {
	IS_SUBP = 1;
	return call_function();
    } else {
	printf("[+1] pid: %d\n", pid);
	return 0;
    }
}
