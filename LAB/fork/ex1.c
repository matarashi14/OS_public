#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
	pid_t fid = fork();
	if(fid == 0){
		return 0;
	}
	else{
		sleep(10);
		return 0;
	}
	return 0;
}
