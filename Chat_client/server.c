//code for the server
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

#define MAXLEN 80

/*char *word[] = {
    #include "words"
};*///reading a file words and storing that in the char array of pointers

char hostname[MAXLEN];
int WORD_PORT = 1066;

void play_game (int in, int out);

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, clilen, newsockfd, pid;
    struct sockaddr_in serv_addr, cli_addr;

    if( argc == 2 )
    {
        WORD_PORT = atoi(argv[1]);//convert argv to port
        printf("\nUsing Port: %d\n\n",WORD_PORT);
    }
    else if(argc == 1)
        printf("\nUsing Port: %d\n\n",WORD_PORT);

    if (argc >= 3)
        error("\nCannot enter more than 2 arguments!\n");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Unable to create socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(WORD_PORT);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Error binding socket");
    }

    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    for( ; ; )
    {
        if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) < 0)
            error("ERROR could not accept");
        pid = fork();
        if (pid < 0)
        {
            error("ERROR on forking");
        }
        if (pid == 0)//in child process
        {
            close(sockfd);
            play_game(newsockfd, newsockfd);//pass new socket fd to program to execute
            exit(0);
        }
        else close(newsockfd);
    }
    close(newsockfd);
    return 0;
}

void play_game (int in, int out)
{
    int n;
    int check = 0;
    char buffer[256], words[256], outbuf[256];

    gethostname(hostname, MAXLEN);
    sprintf(outbuf, "Playing reading game on host %s:\n\n",hostname);
    write(out, outbuf, strlen(outbuf));
while(n<0)
{
    bzero(buffer,256);
    n = read(in,buffer,256);
    if (n < 0)
        error("ERROR reading from socket");
    printf("The message typed by the client was: %s\n",buffer);

    // FILE *fd;
    // fd = fopen("/usr/share/dict/words", "r");
    // if (fd==NULL)
    //     error("ERROR word file\n");

    // while (!feof(fd))
    // {
    //     fgets(words,sizeof(words),fd);
    //     if (strcmp(buffer, words)==0)
    //     {
    //         if(buffer[strlen(buffer)-1]=='\n')
    //             buffer[strlen(buffer)-1]='\0';
    //         sprintf(outbuf,"The word \" %s \" is spelled CORRECTLY.\n",buffer);
    //         check = 1;
    //         break;
    //     }
    //     /*else
    //     {
    //         //buffer[strlen(buffer)-1]='\0';
    //         sprintf(outbuf,"The word \" %s \" is NOT spelled correctly.\n",buffer);
    //     }*/
    // }
    // if (!check)
    // {
    //     if(buffer[strlen(buffer)-1]=='\n')
    //         buffer[strlen(buffer)-1]='\0';
    //     sprintf(outbuf,"The word \" %s \" is NOT spelled correctly.\n",buffer);
    // }
    sprintf(outbuf,"Message Typed:\" %s \" .\n",buffer);
    n = write(out,outbuf,256);
    if (n < 0)
        error("ERROR writing to socket");
    //fclose(fd);
}
}

