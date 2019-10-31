#include <utils.h>
#include <shell.h>
#include <shell_functions.h>
#include <grp.h>
#include <errno.h>
#include <time.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>

int func_cd()
{
    int old_errno = errno;
    if (last_command_end >= 2)
    {
        printf("%d\n\n", last_command_end);
        return -69;
    }
    if (last_command[1] == NULL || !strcmp(last_command[1], "~"))
        chdir(home);
    else if (last_command[1][0] == '~')
    {
        chdir(home);
        if (strlen(last_command[1]) > 2)
        {
            last_command[1] += 2;
            chdir(last_command[1]);
        }
    }
    else
        chdir(last_command[1]);
    if (errno != old_errno)
        return errno;
    return 0;
}

int func_pwd()
{
    char tmp[STD_BUF];
    int old_errno = errno;
    getcwd(tmp, sizeof(tmp));
    if (errno != old_errno)
        return errno;
    printf("%s\n", tmp);
    return 0;
}

int func_ls()
{
    DIR *top;
    struct dirent *dir_ptr;
    int old_errno;
    if (last_command_end <= 3)
    {
        int show_all = 0;
        int show_long = 0;

        for (int i = 0; last_command[1] && i < strlen(last_command[1]); ++i)
        {
            if (last_command[1][i] == 'a')
                show_all = 1;
            if (last_command[1][i] == 'l')
                show_long = 1;
        }

        for (int i = 1; last_command[i]; ++i)
        {
            if (!strcmp(last_command[i], "-a"))
                show_all = 1;
            if (!strcmp(last_command[i], "-l"))
                show_long = 1;
        }

        old_errno = errno;
        if (last_command_end && last_command[last_command_end][0] != '-')
            top = opendir(last_command[last_command_end]);
        else
            top = opendir(cwd);

        if (errno != old_errno)
            return old_errno;

        old_errno = errno;
        dir_ptr = readdir(top);

        if (errno != old_errno)
            return old_errno;

        old_errno = errno;
        while (dir_ptr)
        {
            struct stat dir_status;
            if (show_all || (!show_all && dir_ptr->d_name[0] != '.' && strcmp(dir_ptr->d_name, "..")))
            {
                if (show_long)
                {

                    stat(dir_ptr->d_name, &dir_status);

                    printf((S_ISDIR(dir_status.st_mode)) ? "d" : "-");
                    printf((dir_status.st_mode & S_IRUSR) ? "r" : "-");
                    printf((dir_status.st_mode & S_IWUSR) ? "w" : "-");
                    printf((dir_status.st_mode & S_IXUSR) ? "x" : "-");
                    printf((dir_status.st_mode & S_IRGRP) ? "r" : "-");
                    printf((dir_status.st_mode & S_IWGRP) ? "w" : "-");
                    printf((dir_status.st_mode & S_IXGRP) ? "x" : "-");
                    printf((dir_status.st_mode & S_IROTH) ? "r" : "-");
                    printf((dir_status.st_mode & S_IWOTH) ? "w" : "-");
                    printf((dir_status.st_mode & S_IXOTH) ? "x" : "-");
                    printf(" %lu", dir_status.st_nlink);

                    struct passwd *psswd = getpwuid(dir_status.st_uid);
                    struct group *grp = getgrgid(dir_status.st_gid);
                    char *date;

                    date = (char *)malloc(STD_BUF * sizeof(char));

                    strftime(date, 20, "%b  %d  %I:%M", gmtime(&(dir_status.st_ctime)));

                    printf(" %s  %s  %ld  %s  %s\n", psswd->pw_name, grp->gr_name,
                           dir_status.st_size, date, dir_ptr->d_name);
                }
                else
                    printf("%s\n", dir_ptr->d_name);
            }
            dir_ptr = readdir(top);
        }

        if (errno != old_errno)
            return old_errno;
    }
    else
        return -69;

    return 0;
}

int func_echo()
{
    for (int i = 1; i < STD_BUF && last_command[i]; ++i)
        printf("%s ", last_command[i]);
    printf("\n");
    return 0;
}

int func_execvp()
{
    int pid = fork();
    if (pid == 0)
    {
        setpgid(0, 0);
        execvp(last_command[0], last_command);
        printf("Command not found\n");
        raise(SIGCHLD);
        _exit(0);
    }
    else
    {
        if (!IS_SUBP)
        {
            int status;
            G_PID = pid;
            waitpid(pid, &status, WUNTRACED);
            if (WIFSTOPPED(status))
            {
                add_proc(last_command, pid);
                for (int i = 0; i < STD_BUF; i++)
                    if (PROCS[i].pid == pid)
                    {
                        PROCS[i].state = 0;
                        break;
                    }
            }
            G_PID = -1;
        }
        else
            add_proc(last_command, pid);
    }
    return 0;
}

int func_bg()
{
    if (last_command_end != 1)
        return -69;
    char *tmp;
    int job_number = strtol(last_command[1], &tmp, 10);
    job_number--;
    if (job_number < 0 || PROCS[job_number].pid == -1)
        return -100;
    kill(PROCS[job_number].pid, SIGCONT);
    PROCS[job_number].state = 1;
    return 0;
}

int func_fg()
{
    if (last_command_end != 1)
        return -69;
    char *tmp;
    int job_number = strtol(last_command[1], &tmp, 10);
    job_number--;
    if (job_number < 0 || PROCS[job_number].pid == -1)
        return -100;
    signal(SIGCHLD, SIG_IGN);
    kill(PROCS[job_number].pid, SIGCONT);
    G_PID = PROCS[job_number].pid;
    struct PROCESS A = PROCS[job_number];
    del_proc(PROCS[job_number].pid);
    int status;
    waitpid(PROCS[job_number].pid, &status, WUNTRACED);
    G_PID = -1;
    if (WIFSTOPPED(status))
        add_proc(A.pname, A.pid);
    return 0;
}

int func_pinfo()
{
    char path_to_proc[STD_BUF] = "/proc/";

    if (last_command[1])
        strcat(path_to_proc, last_command[1]);
    else
        strcat(path_to_proc, "self");

    char prefix_path[STD_BUF];
    strcpy(prefix_path, path_to_proc);

    strcat(path_to_proc, "/stat");

    FILE *proc = fopen(path_to_proc, "r");

    if (!proc)
        return errno;

    int proc_pid;
    char proc_name[STD_BUF];
    char proc_status[2];
    fscanf(proc, "%d %s %s", &proc_pid, proc_name, proc_status);

    char mem[STD_BUF];
    for (int i = 0; i < 20; ++i)
        fscanf(proc, "%s", mem);
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

int func_history()
{
    int items_to_print = 10;
    int old_errno = errno;
    if (last_command[1])
        items_to_print = strtol(last_command[1], (char **)NULL, 10);

    int cur = items_to_print - 1;
    while (cur > -1)
    {
        if (history[cur])
            printf("%s\n", history[cur]);
        cur--;
    }

    if (errno != old_errno)
        return errno;
    return 0;
}

int func_setenv()
{
    if (last_command_end == 0 || last_command_end > 2)
        return -69;
    if (last_command[last_command_end])
        setenv(last_command[last_command_end - 1], last_command[last_command_end], 1);
    else
        setenv(last_command[last_command_end], "", 1);
    return 0;
}

int func_unsetenv()
{
    if (last_command_end == 0)
        return -69;
    unsetenv(last_command[1]);
    return 0;
}

int func_jobs()
{
    if (last_command_end != 0)
        return -69;

    for (int i = 0; i < STD_BUF && PROCS[i].pid != -1; i++)
    {
        printf("[%d] %s ", i + 1, (PROCS[i].state ? "Running" : "Stopped"));
        for (int j = 0; j < STD_BUF && PROCS[i].pname[j]; j++)
            printf("%s ", PROCS[i].pname[j]);
        printf("[%d]\n", PROCS[i].pid);
    }
    return 0;
}

int func_kjob()
{
    if (last_command_end != 2)
        return -69;
    char *tmp, *tmp2;
    int job_number = strtol(last_command[1], &tmp, 10);
    int signal_number = strtol(last_command[2], &tmp2, 10);
    job_number--;
    kill(PROCS[job_number].pid, signal_number);
    return 0;
}

int func_overkill()
{
    for (int i = 0; i < STD_BUF; i++)
        if (PROCS[i].pid != -1)
            kill(PROCS[i].pid, SIGKILL);
    return 0;
}

int func_cronjob()
{
    if (last_command_end != 6)
        return -69;
    char *tmp;
    int interval = strtol(last_command[4], &tmp, 10);
    int till = strtol(last_command[6], &tmp, 10);
    int reps = till / interval;

    char *TEMP[STD_BUF];
    for (int i = 0; i < STD_BUF; i++)
        TEMP[i] = NULL;
    int cnt = 0;
    for (int i = 2; i <= last_command_end - 4; i++)
    {
        TEMP[i - 2] = (char *)malloc(STD_BUF);
        strcpy(TEMP[i - 2], last_command[i]);
        cnt++;
    }
    last_command_end = --cnt;

    for (int i = 0; i < STD_BUF; i++)
    {
        if (TEMP[i])
            strcpy(last_command[i], TEMP[i]);
        else
            last_command[i] = NULL;
    }

    int pid = fork();
    while (pid == 0 && reps--)
    {
        sleep(interval);
        exec_command(last_command[last_command_end]);
    }
    return 0;
}

int func_quit()
{
    func_overkill();
    exit(0);
}
