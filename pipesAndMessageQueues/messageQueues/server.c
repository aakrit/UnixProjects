// this is the server for message Queues
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/msg.h>
#include <ctype.h>

#define BUF 1024

struct msg {
	long int msg_type;
	char my_msg[BUF];
	int value;
};
void conv(char *msg, int size) //convert char
{
	int i;
    for (i=0; i<size; ++i)
        if (islower(msg[i]))
            msg[i] =  toupper(msg[i]);
        else if (isupper(msg[i]))
            msg[i] =  tolower(msg[i]);
}

int main(int argc, char * argv [] )
{
	struct msg mg;
	int retval = -1;
	int msgid;
	msgid = msgget((key_t) 73707564, 0666 | IPC_CREAT);
	//returns id to access message queue
	for( ; ; )
	{
		if( msgid == -1 )//check if message was getton
		{
			fprintf(stderr, "msgget failed\n");
			exit(EXIT_FAILURE);
		}
		if (msgrcv(msgid, (void *) &mg, sizeof(struct msg), 0, IPC_NOWAIT) == retval)
		{
			fprintf(stderr, "msgrcv failed\n");
			exit(EXIT_FAILURE);
		} else {
			conv(mg.my_msg, strlen(mg.my_msg));
			printf("Message processed: %s", mg.my_msg);
		}
	}
	return 0;
}

/* Steps
1. create a message queue structure of include it form another file
2. create message id via a key and permission and maybe flags
3. recieve message from client using msgrcv passing in message id and address of mq
4. do necessary computation, id fucntion calling etc
5.		*/
