//this is the client for message queues

#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/msg.h>

#define BUF 1024

struct msg {
	long int msg_type;
	char my_msg[BUF];
	int value;
};

int main(int argc, char * argv [] )
{
	struct msg mg;
	int msgid;
	int retval = -1;
	mg.value = getpid();//store process id

	msgid = msgget((key_t) 73707564, 0666 | IPC_CREAT);
	//create message Q
	if( msgid == -1 )
	{
		fprintf(stderr, "msgget failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Insert message to send to server: ");
	sleep(1);
	while(fgets(mg.my_msg, BUFSIZ, stdin) != NULL)
	//store user input into my_msg buffer
	{
		if(strcmp(mg.my_msg, "exit\n") == 0)//comp string
		{
			printf("Exiting\n");
			exit(EXIT_FAILURE);
		}
		if( msgsnd(msgid, (void *) &mg, sizeof(struct msg), IPC_NOWAIT) != retval )
		{
			printf("Insert message to send to server: ");
		} else {
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}

/* 5 steps
1. create a message queue structure of include it
2. create message id via a key and permission and maybe flabs
3. take input from stdin or else where and store in the message
4. send the message using msgsnd passing in the id, address of message struct
5. have error checks at each step
*/
