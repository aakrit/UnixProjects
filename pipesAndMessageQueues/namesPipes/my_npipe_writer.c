#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define CHILD (pid_t) 0

int main(int argc, char * argv[] )
{
	static char message[BUFSIZ];
	char * myfifo = "MYFIFO";//myfifo is a char pointer for string 
	//MYFIFO
	int fifo;
	char *ex = "exit";

	/*fprintf(stderr, "USAGE:  %s >/dev/tty/?\n",argv[0]);*/

	/* create the FIFO, if it already exists, ignore the error */
	if( mkfifo(myfifo, 0666) < 0 ) //create fifo with permissions
	{
		printf("\n Using named pipe: %s\n", myfifo);//print fifo
		//printf("Opening named pipe: %s\n",myfifo);
	} else {
		printf("\n Opening named pipe: %s\n",myfifo);
	}
	
	if( (fifo = open(myfifo, O_WRONLY)) < 0)//open the fifo
	{
		perror("oh oh:\n");
		exit(-1);
	}

	printf(" Enter Input: ");
	while (fgets(message,BUFSIZ,stdin) != NULL )//get message from stdin
	//read from stdin into messge buffer; fgets has a \n char at end
	{
		write(fifo, message, strlen(message));//write to that fifo
		if (strcmp("exit\n", message) == 0) //compare strings for exit
		{
			printf(" Writing buffer to pipe...done\n");
			printf(" Exiting\n");
			exit(0);
		}
		printf(" Writing buffer to pipe...done\n");
		memset(message, '\0', BUFSIZ);
		printf(" Enter Input: ");
	}
	putchar('\n');
	close(fifo);//close fifo
	exit(0);
}

//mutex: exclusive ownership of object to a thread. only one thread
//can use object whicle its locked.
//atomic: objects of atomic type are free form data races, multiple
//thread can access the object but only one at a time
