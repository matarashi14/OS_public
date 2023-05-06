#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int main()
{
	int children[10];
	pid_t fid = 1;
	int i = 0;
	while(fid != 0 && i < 10){
		fid = fork();
		children[i] = fid;
		i = i + 1;
	}
	
	if(fid == 0){
		while(1){
			1 + 1;
		}
	}
	else{
		sleep(60);
		for (i = 0; i < 10; i++){
			kill(children[i], SIGKILL);
		}
		return 0;
	}
	return 0;
}
