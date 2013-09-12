//Aakrit Prasad
// Producer
#include <math.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#ifdef _SEM_SEMUN_UNDEFINED
//semaphore union
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *_buf;
	void *_pad;
};
#endif

union semun sem_union;//create semphore struct
int main(int argc, char *argv[])
{
  int running=1;
  int shmid;
  int *shm;
  key_t key = 4;
  int flag = SHM_R | SHM_W;
  char buffer[BUFSIZ];
  struct sembuf sem_b;//create buffer for shared memory
  int sem_id;
 // union semun sem_union;

  sem_id=semget(ftok(".",1),1,0666|IPC_CREAT);//create semaphore
  if( sem_id==-1)//check if created worked
  {
  	perror("semget failed:\n");
    exit(EXIT_FAILURE);
  }
  sem_union.val = 1 ;//set value to one (unlocked)
  if(semctl(sem_id,0,SETVAL,sem_union)==-1)//semaphore control
  {
  	perror("semget failed on initialize: \n");
    exit(EXIT_FAILURE);
  }
  //create shared memory
  shmid = shmget(key, 1000, flag | IPC_CREAT);
  if (shmid < 0)
  {
    perror("shmget | IPC_CREAT");
  }
  //attach value to shared memory
  shm = (int *)shmat(shmid, /*addr*/0, /*flag*/0);
  if (shm == (int *)-1) {
    perror("shmat");
    exit(1);
  }
  *shm=5;//set value in shared memory
  printf("items count number is:%d\n", *shm);
  while(running)
  {
  	sleep(1);
	printf("Type \"add\" to add one item or type \"exit\" to quit program:\n");
  	fgets(buffer,BUFSIZ,stdin);
  	if (strcmp(buffer,"exit\n")==0) {
  	running=0;
  	}
  	if (strcmp(buffer,"add\n")==0) {
  	//START of CRITICAL SECTION
		sem_b.sem_num=0;
		sem_b.sem_op=-1;
		sem_b.sem_flg=SEM_UNDO;
    //perform operation on a semaphore set
		if (semop(sem_id,&sem_b,1)==-1)
		{
			perror("semop p failed\n");
			exit(EXIT_FAILURE);
		}
		//CRITICAL LOCKED
		if (*shm<5) {
			*shm=(*shm)+1;

			printf("one item is added and current shelf count is %d\n",*shm);
		//end of critical section
		sem_b.sem_num=0;
		sem_b.sem_op=1;
		sem_b.sem_flg=SEM_UNDO;
		if(semop(sem_id,&sem_b,1)==-1)
		{
			perror("semop P failed \n");
			exit(EXIT_FAILURE);
		}
		//critical unlocked


		}
		else {
		printf("shelf is full, can not add any more untill consumer take one!!!\n");
		}
	}
  }
  if (shmdt(shm)==-1) //check if shared mem was made
  {
  	fprintf(stderr,"shmdt failed\n");
    exit(EXIT_FAILURE);
  }
  if (shmctl( shmid, IPC_RMID,0) ==-1) //remove shmem
  {
  	fprintf(stderr,"shmctl(IPC_RMID) failed\n");
    exit(EXIT_FAILURE);
  }
//  #ifdef DONTDO

  //delete semphore
  if(semctl(sem_id,0,IPC_RMID, sem_union)==-1)
  {
  	perror("semctl IPC_RMID failed:\n");
    exit(EXIT_FAILURE);
  };
 // #endif
  return 0;
}
