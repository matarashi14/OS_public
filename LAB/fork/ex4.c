#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include<sys/wait.h>
int main()
{
	struct timeval start;
	struct timeval finish;
	gettimeofday(&start,NULL);
	pid_t fid = fork();
	if(fid == 0){
		execlp("ls", "ls", (char *)NULL);
	}
	else{
		wait(NULL);
		gettimeofday(&finish,NULL);
		printf("Total time is %ld\n", finish.tv_usec - start.tv_usec);
	}
	
	return 0;
}
