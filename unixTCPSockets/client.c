//code for the client
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) 
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]); //convert argv 2 into port and store it
    sockfd = socket(AF_INET, SOCK_STREAM, 0);//create socket
    if (sockfd < 0) 
        error("ERROR opening socket");//check if socket created
    server = gethostbyname(argv[1]);//get the hostname from argv1 
    if (server == NULL) 
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting"); //check if you made connection to socket

    //while(1)
   // {
        n = read(sockfd,buffer,256);//read from socket into buffer
        if (n < 0) 
             error("ERROR reading from socket");//check if read sucessful
        printf("%s\n",buffer);
        printf("Please enter word to see if it exist: ");
        
        bzero(buffer,256); //empty the buffer
        fgets(buffer,256,stdin); //take user input and store in buffer

        n = write(sockfd,buffer,strlen(buffer)); //write from buffer to socket
        if (n < 0) 
             error("ERROR writing to socket"); //check if write sucessful
        bzero(buffer,256);//empty the buffer
        n = 1;
        while (n>0)
        {
            printf("%s\n",buffer);
            bzero(buffer, 256);
            n = read(sockfd, buffer, 256);
        }
        close(sockfd);
   // }   
    return 0;
}
