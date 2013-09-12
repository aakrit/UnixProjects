
/* This program uses read and write with buffer to implement cp function
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define COPYMODE 0777
#define BUFFERSIZE 10000

void oops(char *, char*);
main(int argc, char *argv[])
{
	int 	in_fd, out_fd, nchar;
	char 	buf[BUFFERSIZE];
	
	if (argc != 3 || (strcmp(argv[1], argv[2]) == 0))
	{
		fprintf( stderr, "usage: %s is the source destination\n", *argv);
		exit(1);
	}

	if ((in_fd=open(argv[1], O_RDONLY)) == -1)
		oops("file cannot open", argv[1]);
		
	if ((out_fd=creat(argv[2], COPYMODE)) == -1)
		oops("cannot create file", argv[2]);
	while ((nchar = read(in_fd, buf, BUFFERSIZE)) > 0)
		if (write (out_fd, buf, nchar) != nchar)
			oops("error writing to file", argv[2]);

	if (nchar == -1)
		oops("error reading to file", argv[1]);
	if ((close(in_fd) == -1) || (close(out_fd) == -1))
		oops("error closing","");
}

void oops(char *s1, char *s2)
{
	fprintf(stderr, "error: %s", s1);
	perror(s2);
	exit(1);
}


