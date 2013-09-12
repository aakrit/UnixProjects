//Aakrit Prasad
//Producer File
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
  int running=1;
  int shmid, arg;
  int *shm;
  key_t key = 4;
  int flag = SHM_R | SHM_W;
  char buffer[BUFSIZ];

  if (argc == 2)
  {
    arg = atoi(argv[1]);
    key = arg;
  }

  shmid = shmget(key, 1000, flag | IPC_CREAT);
  //create shared memory
  if (shmid < 0)
  {
    perror("shmget | IPC_CREAT");
  }

  //attached/map sh memory seg to data reference (int *shm)
  shm = (int *)shmat(shmid, /*addr*/0, /*flag*/0);
  //now the int value pointed to by shm is in shared memory
  if (shm == (int *)-1)
  {
    perror("shmat");
    exit(1);
  }
  *shm=5;//set shared memory value to 5
  printf("items count number is:%d\n", *shm); //print count
  while(running)
  {
  	sleep(1);
    printf("Type \"add\" to add one item or type \"exit\" to quit program:\n");
  	fgets(buffer,BUFSIZ,stdin);
  	if (strcmp(buffer,"exit\n")==0)
    {
  	running=0;
  	}
  	if (strcmp(buffer,"add\n")==0)
    {
  		if (*shm<5) //check if shelf is full or not
      {
			*shm=(*shm)+1;//if not then add one to it every second
			printf("one item is added and current shelf count is %d\n",*shm);
		  }
		  else
      {
		  printf("shelf is full, can not add any more untill consumer take one!!!\n");
		  }
    }
  }
  if (shmdt(shm)==-1) //check status of data reference in SM
  {
  	fprintf(stderr,"shmdt failed\n");
    exit(EXIT_FAILURE);
  }
  if (shmctl( shmid, IPC_RMID,0) ==-1)
  //destory the shared memory after use
  {
  	fprintf(stderr,"shmctl(IPC_RMID) failed\n");
    exit(EXIT_FAILURE);
  }
  return 0;
}
