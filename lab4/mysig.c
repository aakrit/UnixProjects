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
	psignal(sig, "Thank you for the kill -10 sigusr1 signal, keep working");
}

void handler1(int sig)
{
	if (sigint_count == MAXSTOPS -1)
	{
		exit(0);
	//	kill(getpid(), SIGINT);
	}
	else
	{
		sigint_count++;
		printf("You have typed CTRL+C %d times\n Can't you tell i'm invulnerable? \n",sigint_count);
	}
}


int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		MAXSTOPS = 1;
	}
	else 
	{
		MAXSTOPS = atoi(argv[1]);
	}
	void *retval;
	printf("the id of this process is: %d\n", getpid());
	if ((retval = signal(SIGINT, handler1)) == SIG_ERR)
	{
		fprintf(stderr, "sorry, failed to establish handler for SIGINT\n");
		exit(1);
	}
	if ((retval = signal(SIGUSR1, handler2)) == SIG_ERR)
	{
		fprintf(stderr, "sorry \n");
		exit(1);
	}
	while(sigint_count < MAXSTOPS)
	{
		fprintf(stderr,"Press CTRL+C so i do something\n");
		sleep(3);
		fflush(stdout);
	}
}
	
