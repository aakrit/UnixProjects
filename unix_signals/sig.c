#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	int a;
	if (argc != 2 )
	{
		printf("need to specify the PID for the process to kill in second argument\n");
		exit(0);
	}
	pid_t pid = atoi(argv[1]);//get the pid
	kill(pid, SIGUSR2);

/*	
	void *retval;
	printf("the id of this process is: %d\n", getpid());
	if ((retval = signal(SIGUSR2, handler2)) == SIG_ERR)
	{
		fprintf(stderr, "sorry, failed to establish handler for SIGINT\n");
		exit(1);
	}
	while(1)
	{
		printf("Executing");
		sleep(1);
		fflush(stdout);
	}*/

	return 0;
}
	
