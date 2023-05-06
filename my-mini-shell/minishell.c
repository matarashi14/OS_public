#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "minishell.h"

#define MAX_LENGTH 100

// breaking command up to string of array
int breakUpCMD(char *args[], char cmd[])
{
    char *token = strtok(cmd, " \n");

    // keep tracking number of arguments
    int counter = 0;
    while (token != NULL)
    {
        args[counter] = token;
        counter++;
        token = strtok(NULL, " \n");
    }
    args[counter] = NULL;

    return counter;
}

int main()
{
    char cmd[MAX_LENGTH];
    char *args[MAX_LENGTH];

    while (1)
    {
        bool background = false;
        int pipe_index = -1;

        printf(">> ");
        fgets(cmd, MAX_LENGTH, stdin); // command input

        int counter = breakUpCMD(args, cmd);
        // printf("counter: %d\n", counter);

        if (strcmp(args[0], "exit") == 0) // exit command
        {
            printf("Bye\n");
            break;
        }
        else if (strcmp(args[counter - 1], "&") == 0) // check if it need to do background process
        {
            background = true;
        }

        // finding out if pipe is there
        for (int i = 0; i < counter; i++)
        {
            if (strcmp(args[i], "|") == 0)
            {
                pipe_index = i;
                args[i] = NULL;
            }
        }

        // special case for 'cd' command
        if (strcmp(args[0], "cd") == 0)
        {
            chdir(args[1]);
        }
        else
        {
            pid_t pid;

            if (pipe_index == -1) // single process
            {
                pid = fork();

                if (pid == 0) // child process
                {
                    if (execvp(args[0], args) == -1)
                    {
                        printf("Error: Command not found\n");
                        break;
                    }
                }
                else // parent process
                {
                    if (!background)
                    {
                        int status;
                        waitpid(pid, &status, 0); // if not background process, parents should wait
                    }
                    else
                    {
                        printf("Background process started\n");
                    }
                }
            }
            else // piping
            {
                pid_t pid1, pid2;
                int fd[2];
                pipe(fd);

                pid1 = fork();
                if (pid1 == 0) // first child
                {
                    dup2(fd[1], 1);
                    close(fd[0]);
                    execvp(args[0], args);
                }
                else // parent process
                {
                    pid2 = fork();
                    if (pid2 == 0) // second child
                    {
                        dup2(fd[0], 0);
                        close(fd[1]);
                        execvp(args[pipe_index + 1], &args[pipe_index + 1]); // passed command after | command
                    }
                    else // parent process
                    {
                        close(fd[0]);
                        close(fd[1]);

                        if (!background) // if not background process, parents should wait
                        {
                            int status;
                            waitpid(pid1, &status, 0);
                            waitpid(pid2, &status, 0);
                        }
                        else
                        {
                            printf("Background process started\n");
                        }
                    }
                }
            }
        }
    }

    return 0;
}