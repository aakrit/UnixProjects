//Aakrit Prasad work
// Consumer
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/sem.h>
#ifdef _SEM_SEMUN_UNDEFINED
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
	void *__pad;
};
#endif
union semun sem_union;

int main(int argc, char *argv[])
{
  int shmid;
  int *shm;
  int running=1;
  char buffer[BUFSIZ];
  key_t key = 4;
  int sem_id;

  //creat semaphore
  sem_id=semget(ftok(".",1),1,0666|IPC_CREAT);
  //create semaphore with just 1 byte size
  if(sem_id==-1)//check if create worked
  {
  	perror("semget fail\n");
    exit(EXIT_FAILURE);
  }
  //initialize semaphore
  sem_union.val=1;//set value to one (unlocked)
  //now set this value to the value 1 in the semaphore struct
  //we get from the sem_id
  if(semctl(sem_id,0,SETVAL,sem_union)==-1)
  {
  	perror("semctl failed on initialization\n");
    exit(EXIT_FAILURE);
  }
  printf("Trying shared memory %d\n", key);
  shmid = shmget(key, 1000, SHM_R | SHM_W | IPC_CREAT);
  if (shmid < 0)
  {
    perror("shmget");
  }
  else {
    printf("shared memory: %d\n", shmid);
  }

  shm = (int *)shmat(shmid, /*addr*/0, /*flag*/0);
  if (*shm == (-1)) {
    perror("shmat");
    exit(1);
  }
  while(running)
  {
    	printf("type \"take\" to remove one item or  type \"exit\" to exit program.\n");
      fgets(buffer,BUFSIZ,stdin);
  	if (strcmp(buffer,"exit\n")==0)
    {
  		break;
  	}
  	else
    {
  		if (strcmp(buffer,"take\n")==0)
      {
  			if (*shm>0)
        {
  				//start of critical section
  				struct sembuf sem_b;
  				sem_b.sem_num=0;
  				sem_b.sem_op=-1;
  				sem_b.sem_flg=SEM_UNDO;
          //time to perform operation on a semaphore set
  				if (semop(sem_id,&sem_b,1)==-1)
          {
  					perror("semop fail.\n");
  					exit(EXIT_FAILURE);
  				}
  				//critical section locked

  				*shm=(*shm)-1;
  				printf("one item is removed,curren item count is %d \n", *shm);
  				//end of critical section
  				sem_b.sem_num=0;
  				sem_b.sem_op=1;
  				sem_b.sem_flg=SEM_UNDO;
  				if (semop(sem_id,&sem_b,1)==-1)
          {
            perror("semop failed\n");
  		      exit(EXIT_FAILURE);
  		    }
  				//critical section unlocked
  			}
  			else
        {
  			printf("Blasted!!! I knew I should have gone to T...t. Current item number is 0.But I probably can wait to producer stock item.\n");
  			}
  		}
    }
  }
  if( shmdt(shm)== -1)
  {
  	fprintf(stderr, "shmdt failed\n");
    exit(EXIT_FAILURE);
  }
  if(shmctl(shmid,IPC_RMID,0)==-1)
  {
  	fprintf(stderr,"shmctl ipc_rmid failed\n");
    exit(EXIT_FAILURE);
  }
  //delete semaphore
  if( semctl(sem_id,0, IPC_RMID,sem_union)==-1)
  {
  	perror("semctl ipc_rmid failed:\n");
    exit(EXIT_FAILURE);
  }
  return 0;
  }
