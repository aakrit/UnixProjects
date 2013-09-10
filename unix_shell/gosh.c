//A C Shell by AAKRIT PRASAD
//DATE: 2/17/2013

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#include "gosh.h"

#define BUF 1024

extern int errno;
void handler1(int sig);
void handler2(int sig);
void handler3(int sig);
int main(int argc, char *argv[])
{
  	int retval;
	int h;
  	struct command_t command_a;
	signal(SIGCHLD, handler1);
	signal(SIGINT, handler2);
	while(1)
	{
    		retval = init_command(&command_a);
    		printf("Aakrit_Shell$ ");
		retval = simple_accept_input(&command_a);
    		print_command(&command_a, "c1");

		if (retval == 0)
		{
	 		simple_fork_command(&command_a);
    }
		else if (retval == 1)
		{
        printf("unExpected ERROR:\n");
      	exit(0);
    }
  }
	for(h = 0; h < command_a.num_args; h++)
	{
		free(command_a.args[h]);
	}
  	exit(0);
}
/* Helper Function.  Initialized a command_t struct */
int init_command(struct command_t *cmd) {
  cmd->num_args = 0;
  cmd->args[0] = NULL;
  cmd->outfile[0] = '\0';
  cmd->infile[0] = '\0';
  return(0);
}
/* Helper Function.  Print out relevent info stored in a command_t struct */
int print_command(struct command_t *cmd, const char *tag) {
  int i;
  for (i=0; i<cmd->num_args; i++) {
    printf("%s %d: %s\n", tag, i, cmd->args[i]);
  }
  if (cmd->outfile[0] != '\0') {
    printf("%s outfile: %s\n", tag, cmd->outfile);
  }
  if (cmd->infile[0] != '\0') {
    printf("%s infile: %s\n", tag, cmd->infile);
  }
  return(0);
}
/* read input from stdin and split it up by " " characters.
   Store the pieces in the passed in cmd_a->args[] array.  If the user
   inputs 'exit', return a 1.  If the user inputs nothing (\n), return
   a value > 1.  If the user inputs somthing else, return a 0. */
int simple_accept_input(struct command_t *cmd_a)
{
	char ar[BUF]; //declare pointer to an array of char
	char *stoken, *fg; //char array to store stdin as tokens
	int j = 0; //argv counter set to 0
	char delimiter[] = " ";
  fg = fgets(ar, BUF, stdin); //get input from stdin and store in array
	fg[strlen(ar)-1]='\0';
	stoken = strtok(fg, delimiter);
	while(stoken!=NULL)
	{
		cmd_a->args[j]=strdup(stoken); //store total argv by duplicating string stoken
		cmd_a->num_args=++j; //store total argc
		if(strcmp(stoken, "\n") == 0)
			return 2;
		else if(strcmp(stoken, "exit") == 0)
			return 1;
		stoken=strtok(NULL,delimiter);

	}
	return(0);
}

int simple_fork_command(struct command_t *cmd)
{
 	int x=0;
	char test;
	while(x<strlen(cmd->args[0]))
	{
		if ('='==cmd->args[0][x])
		{
			test='=';
			break;
		}
		x++;
	}
	if (test=='=')
	{
		char *token2;
		char *var1;
		char *var2;
		token2=strtok(cmd->args[0],"=");
		if (token2==NULL)
			return 10;
		var1=(char *)malloc(sizeof(char) * strlen(token2));
		strcpy(var1,token2);
 		token2=strtok(NULL,"=");
		if (token2==NULL)
			return 10;
		var2=(char *)malloc(sizeof(char) * strlen(token2));
        	strcpy(var2,token2);
		setenv(var1,var2,1);
		printf("%s is : %s\n",var1,getenv(var1));
	}
	else
	{
		int j;
		pid_t child;
		int status = 1;
		int option = 0;
		int arc = cmd->num_args;
		char *newar[arc + 1];
		newar[arc] = NULL;
		for (j = 0; j < arc; j++)
		{
			newar[j] = cmd->args[j];
		}
		if (strcmp(newar[0], "exit") == 0) {exit(0);}
		if (strcmp(newar[0], "cd") == 0)
		{
			if (newar[1] != NULL)
				chdir(newar[1]);
			else
				printf("Please specify dir name\n");
		}
		if (arc == 0)
		{ printf("\n"); }
		else
		{
			if ((child = fork()) == 0)
			{
//			printf("C: In the child process, child pid is %d\n", getpid());
//			printf("C: My parent process id is: %d\n", getppid());
			sleep(1);
//			printf("C: going to run with execvp\n");
			execvp(newar[0], newar); //call execvp on parent
			}
			else
			{
//			printf("P: Still in the parent process with pid: %d\n", getpid());
			waitpid(child, &status, option);
//			printf("P: My child has ended with status: %d \n", status);
			}
		}
		return(0);
	}
}

/* Problem 3: set up all of your signal actions here */
void handler1(int sig)
{
//	printf("The child has exited\n");
}
void handler2(int sig)
{
	printf("Please type 'exit' to leave this shell\ngosh$ ");
}
void handler3(int sig)
{
	printf("Type something\ngosh$ ");
}
