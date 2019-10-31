#include <shell.h>
#include <utils.h>

void initialise()
{
    home = getcwd(NULL, 0);
    HOME_LEN = strlen(home);
    IS_SUBP = 0;
    IS_SUBPEXEC = 0;
    char hist_file_loc[STD_BUF];
    sprintf(hist_file_loc, "%s/.history", home);
    cwd = (char *)malloc(STD_BUF * sizeof(char));
    strcpy(cwd, home);

    FILE *hist_file = fopen(hist_file_loc, "r");
    int cur = 0;

    if (hist_file)
        while (1)
        {
            char *tmp_buf = (char *)malloc(STD_BUF * sizeof(char));
            char *tmp_ptr = fgets(tmp_buf, STD_BUF, hist_file);
            if (tmp_ptr)
            {
                history[cur] = (char *)malloc(STD_BUF * sizeof(char));
                strcpy(history[cur], tmp_buf);
                cur++;
            }
            else
                break;
        }

    for (int i = cur; i < 20; i++)
        history[cur] = NULL;

    for (int i = 0; i < STD_BUF; i++)
    {
        for (int j = 0; j < STD_BUF; j++)
            PROCS[i].pname[j] = NULL;
        PROCS[i].pid = -1;
        PROCS[i].state = -1;
    }

    G_PID = -1;
}

void run_shell()
{
    while (1)
    {
        signal(SIGCHLD, find_and_delete);
        signal(SIGINT, handle_sigs);
        signal(SIGTSTP, handle_sigs);

        char pwd[STD_BUF];
        printf("%s ", get_prompt(getcwd(pwd, sizeof(pwd))));
        strcpy(cwd, pwd);

        int inp = get_input(NULL);
        if (inp == -11)
            continue;
        else if (inp)
        {
            int prev = inp;
            prev--;
            if (history[prev])
            {
                get_input(history[prev]);
                printf("%s ", get_prompt(getcwd(pwd, sizeof(pwd))));
                printf("%s\n", history[prev]);
                getchar();
            }
            else
            {
                printf("Command not found\n");
                continue;
            }
        }

        add_to_history();

        int cur = 0;

        while (one_shot[cur])
        {
            getcwd(pwd, sizeof(pwd));
            strcpy(cwd, pwd);
            tokenize_command(one_shot[cur++]);
            int pipe_cnt = 0;
            int pipe_locations[STD_BUF];
            int pl_cur = 0;

            for (int i = 0; i <= last_command_full_end; i++)
                if (last_command_full[i][0] == '|')
                {
                    pipe_cnt++;
                    pipe_locations[pl_cur] = i;
                    pl_cur++;
                }

            int cur_pipe = 0;
            int last_pipe = -1;

            int savestdin = dup(0);
            int savestdout = dup(1);
            char TEMP[STD_BUF][STD_BUF];
            for (int i = 0; i < STD_BUF; i++)
            {
                if (last_command_full[i])
                    strcpy(TEMP[i], last_command_full[i]);
                else
                    break;
            }
            while (1)
            {
                reset(last_command);
                last_command_end = 0;

                if (cur_pipe == pl_cur)
                    for (int i = last_pipe + 1, j = 0; i <= last_command_full_end; i++, j++)
                    {
                        if (last_command[j])
                            free(last_command[j]);
                        last_command[j] = (char *)malloc(STD_BUF);
                        strcpy(last_command[j], TEMP[i]);
                        last_command_end++;
                    }
                else
                    for (int i = last_pipe + 1, j = 0; i < pipe_locations[cur_pipe]; i++, j++)
                    {
                        if (last_command[j])
                            free(last_command[j]);
                        last_command[j] = (char *)malloc(STD_BUF);
                        strcpy(last_command[j], TEMP[i]);
                        last_command_end++;
                    }

                last_command_end--;
                last_pipe = pipe_locations[cur_pipe];
                cur_pipe++;

                if (cur_pipe <= pl_cur)
                {
                    int status;
                    int fds[2];
                    pipe(fds);

                    int pid = fork();

                    if (pid == 0)
                    {
                        dup2(fds[1], 1);
                        exec_command(last_command[last_command_end]);
                        abort();
                    }
                    else
                    {
                        dup2(fds[0], 0);
                        close(fds[1]);
                        waitpid(pid, &status, 0);
                    }

                    close(fds[0]);
                    close(fds[1]);
                }
                else
                {
                    dup2(savestdout, 1);
                    exec_command(last_command[last_command_end]);
                    break;
                }
            }
            dup2(savestdin, 0);
            dup2(savestdout, 1);
        }
    }
}

int main()
{
    initialise();
    run_shell();
}
