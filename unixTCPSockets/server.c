//code for the server
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

#define NUM 10
#define MAXLEN 80

/*char *word[] = {
    #include "words"
};*///reading a file words and storing that in the char array of pointers
int th_count = 0;
int status;
pthread_t thread[NUM];//[NUM];//initialize thread array to 10
pthread_attr_t thread_attr[NUM];

char hostname[MAXLEN];
int WORD_PORT = 1066;

void *word_game (void *);

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, clilen, newsockfd, pid; //initialize vars
    struct sockaddr_in serv_addr, cli_addr; //create structs
    void * retval;
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
    
    while(1)
    {
        clilen = sizeof(cli_addr); //store the len of cli_addr
        if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) < 0)
            error("ERROR could not accept");
        //pid = fork();
        
        //pthread_attr_init(&thread_attr[th_count]);//initiaize attr
        //pthread_attr_setscope(&thread_attr[th_count], PTHREAD_SCOPE_SYSTEM);
        if( (status = pthread_create(&thread[th_count], NULL, word_game, (void *)&newsockfd)) < 0 )//pthread create returns an int while running a program thread_routine
        {
            printf("P: VERY BAD NEWS:  pthread_create failed!\n");
            exit(-1);
        }
        //status = pthread_join(thread, &retval);
        pthread_detach(thread[th_count]);
        th_count++;
        //close(newsockfd);
       // pthread_exit(retval);
    }
    
    while (&thread[th_count] != NULL)
    {
        pthread_exit(retval);
    }
    
    //close(sockfd);
    return 0;
}

void *word_game (void *in)
{
    int n, so;
    so = *(int *) in;
    int check = 0;
    char buffer[256], words[256], outbuf[256];

    gethostname(hostname, MAXLEN);
    sprintf(outbuf, "Playing reading game on host %s:\n\n",hostname);
    write(so, outbuf, strlen(outbuf));

    bzero(buffer,256);
    n = read(so,buffer,256);
    if (n < 0) 
        error("ERROR reading from socket");
    printf("The word typed by the client was: %s\n",buffer);

    FILE *fd;
    fd = fopen("/usr/share/dict/words", "r");
    if (fd==NULL) 
        error("ERROR word file\n");

    while (!feof(fd))
    {
        fgets(words,sizeof(words),fd);
        if (strcmp(buffer, words)==0)
        {
            if(buffer[strlen(buffer)-1]=='\n')
                buffer[strlen(buffer)-1]='\0';
            sprintf(outbuf,"The word \" %s \" is spelled CORRECTLY.\n",buffer);
            check = 1;
            break;  
        } 
        /*else
        {
            //buffer[strlen(buffer)-1]='\0';
            sprintf(outbuf,"The word \" %s \" is NOT spelled correctly.\n",buffer);
        }*/
    }  
    if (!check)
    {
        if(buffer[strlen(buffer)-1]=='\n')
            buffer[strlen(buffer)-1]='\0';
        sprintf(outbuf,"The word \" %s \" is NOT spelled correctly.\n",buffer);
    }
    
    n = write(so, outbuf, 256);
    if (n < 0) 
        error("ERROR writing to socket");
    fclose(fd);
    close(so);
    //return(in);
}

