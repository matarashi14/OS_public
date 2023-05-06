#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
	pid_t fid = fork();
	if(fid == 0){
		sleep(10);
		return 0;
	}
	else{
		return 0;
	}
	return 0;
}
