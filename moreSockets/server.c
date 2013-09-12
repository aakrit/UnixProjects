#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}
void *thread_function(void *newsockfd);

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     pthread_t a_thread;
     int res;
     void * retval;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     while(1)
     {
     	clilen = sizeof(cli_addr);
	
	       newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, &clilen);
	       if (newsockfd < 0) 
         	 error("ERROR on accept");
     	res=pthread_create(&a_thread,NULL,thread_function,(void *)&newsockfd);
        if (res!=0) error("ERROR on pthread_create");
        pthread_detach(a_thread);
	}
	while (&a_thread!=NULL)
	{
	pthread_exit(retval);
	}
}


void *thread_function(void  *newsockfd)
{
        char buffer[256], outbuf[256], words[256];
	FILE *fd;
	int n, tempsockfd;
	bzero(buffer,256);
	tempsockfd=*(int *)newsockfd;
	n = read(tempsockfd,buffer,255);
     	if (n < 0) error("ERROR reading from socket");
     	fd=fopen("/usr/share/dict/words","r");
     	if (fd==NULL) error("ERROR word file\n");
        sprintf(outbuf,"The word \" %s \" is spelled not correctly.\n",buffer);
     	while (!feof(fd))
     	{
        	fgets(words,sizeof(words),fd);
		if (strcmp(buffer, words)==0)
		{
			sprintf(outbuf,"The word \" %s \" is spelled correctly.\n",buffer);
			break;	
		}	
     	}
	
      	buffer[strlen(buffer)-1]='\0';
      	n=write(tempsockfd,outbuf,255);
      	if (n<0) error("ERROR writing to socket");
      	fclose(fd);
      	close(tempsockfd);

}
