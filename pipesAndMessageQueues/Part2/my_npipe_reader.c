#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define CHILD (pid_t) 0

int main(int argc, char * argv[] )
{
	static char message[BUFSIZ];
	char * myfifo = "MYFIFO";
	//char *ex = "exit";
	int fifo;
	int n;

	if( mkfifo(myfifo, 0666) < 0 )//create fifo with permissions
	{
		printf("\n Using named pipe: %s\n", myfifo);//print fifo
		//printf("Creating named pipe: %s\n",myfifo);
	} else {
		printf("\n Creating named pipe: %s\n",myfifo);
	}

	if( (fifo = open(myfifo, O_RDONLY)) < 0)//open fifo file
	{
		perror("oh oh:\n");
		exit(-1);
	}

	fprintf(stderr,"\n Waiting for input...");

	while( (n = read(fifo, message, sizeof(message))) > 0 )//read from fifo
	{
		//message[BUFSIZ] = message[BUFSIZ-1];
		fprintf(stderr,"Got it: %s ",message);
		if (strcmp(message, "exit\n") == 0) 
			{
				printf("Exiting\n");
				exit(0);
			}
		fprintf(stderr,"Waiting for input...");
		memset(message, '\0', BUFSIZ); //set everything in buffer to 2nd arg char up to len of bufsiz
	}
	putchar('\n');
	close(fifo);//close fifo
	exit(0);
}

