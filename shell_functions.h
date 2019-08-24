#include <utils.h>
#include <shell.h>
#include <errno.h>
#include <sys/wait.h>

int
func_cd()
{
    int old_errno = errno;
    if (last_command[2] != NULL) {
        return -69;
    }
    if (last_command[1] == NULL || !strcmp(last_command[1], "~")) {
        chdir(home);
    } else if (last_command[1][0] == '~') {
        chdir(home);
        if (strlen(last_command[1]) > 2) {
            last_command[1] += 2;
            chdir(last_command[1]);
        }
    } else {
        chdir(last_command[1]);
    }
    if (errno != old_errno) {
        return errno;
    }
    return 0;
}

int
func_pwd()
{
    char tmp[STD_BUF];
    int old_errno = errno;
    getcwd(tmp, sizeof(tmp));
    if (errno != old_errno) {
        return errno;
    }
    printf("%s\n", tmp);
    return 0;
}

int
func_ls()
{

    return 0;
}

int
func_echo()
{
    for (int i = 1; i < STD_BUF && last_command[i]; ++i) {
        printf("%s ", last_command[i]);
    }
    printf("\n");
    return 0;
}

int
func_execvp()
{
    int pid = fork();
    if (pid == 0) {
        IS_SUBPEXEC = 1;
        execvp(last_command[0], last_command);
        printf("Command not found\n");
        _exit(-1);
    } else {
        waitpid(pid, NULL, 0);
    }
    return 0;
}

int
func_pinfo()
{
    char path_to_proc[STD_BUF] = "/proc/";

    if (last_command[1]) {
        strcat(path_to_proc, last_command[1]);
    } else {
        strcat(path_to_proc, "self");
    }

    char prefix_path[STD_BUF];
    strcpy(prefix_path, path_to_proc);

    strcat(path_to_proc, "/stat");

    FILE* proc = fopen(path_to_proc, "r");
    
    if (!proc) {
        return errno;
    }

    int proc_pid;
    char proc_name[STD_BUF];
    char proc_status[2];
    fscanf(proc, "%d %s %s", &proc_pid, proc_name, proc_status);

    char mem[STD_BUF];
    for (int i = 0; i < 20; ++i) {
        fscanf(proc, "%s", mem);
    }
    fclose(proc);

    printf("pid -- %d\n", proc_pid);
    printf("Process status -- %s\n", proc_status);
    printf("memory -- %s\n", mem);

    strcpy(path_to_proc, prefix_path);
    strcat(path_to_proc, "/exe");

    char buf[STD_BUF];

    readlink(path_to_proc, buf, sizeof(buf));
    
    printf("Executable path -- %s\n", buf);

    return 0;
}

int
func_history()
{
    int items_to_print = 10;
    int old_errno = errno;
    if (last_command[1]) {
        items_to_print = strtol(last_command[1], (char **)NULL, 10);
    }

    int cur = items_to_print - 1;
    while (cur > -1) {
        if (history[cur]) {
            printf("%s\n", history[cur]);
        }
        cur--;
    }

    if (errno != old_errno) {
        return errno;
    }
    return 0;
}

int
func_exit()
{
    exit(0);
}
