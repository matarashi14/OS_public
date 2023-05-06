#ifndef MINISHELL_H
#define MINISHELL_H

int breakUpCMD(char *args[], char cmd[]);
int findPipeIndex(char *args[], int counter);

#endif
