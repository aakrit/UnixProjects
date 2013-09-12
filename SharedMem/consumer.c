//Aakrit Prasad -
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
  int shmid, arg;
  int *shm;
  int running=1;//flag
  char buffer[BUFSIZ];//buffer for storage
  key_t key = 4; //key for shared memory

  if (argc == 2)
  {
    arg = atoi(argv[1]);
    key = arg;
  }
  printf("Trying shared memory %d\n", key);

  // create the shared memory using key in user space
  shmid = shmget(key, 1000, SHM_R | SHM_W | IPC_CREAT);

  if (shmid < 0)
  {
    perror("shmget");
  }
  else
  {
    printf("shared memory: %d\n", shmid);
  }
  //attached/map sh memory seg to data reference (int *shm)
  shm = (int *)shmat(shmid, /*addr*/0, /*flag*/0);
  //now the int value pointed to by shm is in shared memory
  if (*shm == (-1))
  {
    perror("shmat");
    exit(1);
  }
  while(running)
  {
    printf("type \"remove\" to remove one item or type \"exit\" to exit program.\n");
    fgets(buffer,BUFSIZ,stdin);//recieve input from stdin
    if (strcmp(buffer,"exit\n")==0)
    {
		  break;
    }
    else
    {
      if (strcmp(buffer,"remove\n")==0)
      {
        if (*shm>0) //check if value is 0 or not
        {
				  *shm=(*shm)-1;
				  printf("one item is removed,curren item count is %d \n", *shm);
        }
        else
        {
				  printf("The Shelf is empty, wait for production of item.\n");
        }
		  }
    }
  }
  if( shmdt(shm)== -1) //check status of data reference in SM
  {
  	fprintf(stderr, "shmdt failed\n");
    exit(EXIT_FAILURE);
  }
  return 0;
}
