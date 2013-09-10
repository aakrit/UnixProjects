#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

static int sigint_count = 0;
static int MAXSTOPS;

void handler2(int sig)
{
	psignal(sig, "SIGUSR2 recieved");
}


int main(int argc, char *argv[])
{
	void *retval;
	printf("the id of this process is: %d\n", getpid());
	if ((retval = signal(SIGUSR2, handler2)) == SIG_ERR)
	{
		fprintf(stderr, "sorry, failed to establish handler for SIGINT\n");
		exit(1);
	}
	while(1)
	{
		printf("Executing\n");
		sleep(1);
		fflush(stdout);
	}
}
	
