#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <string.h>

#define BUFF_SIZE 8

int fd[2];

void child1_work(){
	close(fd[0]);
	char write_msg[BUFF_SIZE];
	srand(time(NULL));
	
	char current = 'a';
	int msg_length, i;
	while(current <= 'z'){
		msg_length = rand() % 5 + 3;
		for (i = 0; i < msg_length && current <= 'z'; i++){
			write_msg[i] = current;
			current++;
		}
		write_msg[i] = '\0';
		write(fd[1], write_msg, i+1);
		sleep(1);
	}
	write(fd[1],"done\n",6);
	close(fd[1]);
}

void child2_work(){
	close(fd[1]);
	char read_msg[BUFF_SIZE];
                        
	int i;
	read(fd[0],read_msg, sizeof(read_msg));
	while(strcmp(read_msg,"done\n")){
		for(i = 0; i < BUFF_SIZE && read_msg[i] != '\0'; i++){
			read_msg[i] = toupper(read_msg[i]);
		}
		//printf("%s\n", read_msg);
		read(fd[0],read_msg, sizeof(read_msg));
	}
	close(fd[0]);
}

int main(){
	pipe(fd);
	pid_t child1, child2;

	// fork first child
	child1 = fork();
 
	// send child 1 to do work
	if(child1 == 0){
		child1_work();
	}
	else{
		// fork second child
		child2 = fork();

		// send child 2 to do work
		if(child2 == 0){
			child2_work();
		}

		// parent waits for both children to finish
		else{
			int status;
			waitpid(child1, &status, 0);
			waitpid(child2, &status, 0);
			printf("parent is done waiting for %d and %d\n", child1, child2);
		}
	}
}
