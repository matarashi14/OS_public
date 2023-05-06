#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFF_SIZE 8

int main(){
	
	int fd;
	char * fifo = "/tmp/lab3fifo";
        mkfifo(fifo,0666);
	pid_t child1, child2;
	child1 = fork();
 
	// Child 1  work
	// print the alphabet 
	if(child1 == 0){
		fd = open(fifo, O_WRONLY);

		char write_msg[BUFF_SIZE];
		srand(time(NULL));

		char current = 'a';
		int msg_length, i;
		while(current <= 'z'){
			//msg_length = rand() % 5 + 3;
			msg_length = 6;
			for (i = 0; i < msg_length && current <= 'z'; i++){
				write_msg[i] = current;
				current++;
			}
			write_msg[i] = '\0';
			write(fd, write_msg, i+1);
			sleep(1);
		}
		
		write(fd,"done\n", 6);
		close(fd);
	}
	else{

		child2 = fork();

		// Child 2 work
		// Capitalize all letters
		if(child2 == 0){
			fd = open(fifo, O_RDONLY);
			char read_msg[BUFF_SIZE]="init";
			
			int i;
			read(fd,read_msg, sizeof(read_msg));
			while(strcmp(read_msg,"done\n")){
				for(i = 0; i < BUFF_SIZE && read_msg[i] != '\0'; i++){
					read_msg[i] = toupper(read_msg[i]);
				}
				printf("%s\n", read_msg);
				read(fd, read_msg, sizeof(read_msg));
			}

			close(fd);
		}

		// Parent work
		// wait for both children to finish
		else{
			int status;
			waitpid(child1, &status, 0);
			waitpid(child2, &status, 0);
			printf("parent is done waiting for %d and %d\n", child1, child2);
		}
	}
}
