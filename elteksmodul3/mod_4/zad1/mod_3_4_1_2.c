#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short * array;
	struct seminfo *__buf;
};

int main(){
	srand(time(NULL));
	int num;
	FILE *fd_fifof;
	
	key_t key;
	int semid;
	
	struct sembuf waitenter[2] = {{0,-2, 0},{0,1,0}};
	struct sembuf exdo[2] = {{1,-2,0},{0,-1,0}};
	union semun arg;
	
	//while(1){
	
	key=ftok("/etc/mysem",58);

	semid = semget(key,2,0666);

	//arg.val = 1;
	//semctl(semid,0,SETVAL, arg);
	
	
	while(1){

	if(semop(semid, waitenter, 2)== -1) {perror("Lock Error");}

	fd_fifof=fopen("/tmp/new_fifo.fifo","r");
	if(fd_fifof==NULL){
		perror("\nERROR1");
		exit(EXIT_FAILURE);
	}
	
	if(!fread(&num,sizeof(int),1,fd_fifof)){
		perror("\nERROR2");
		//fclose(fd_fifof);
		//exit(EXIT_FAILURE);
	}
	
	fclose(fd_fifof);
	printf("\nRead data = %d\n", num);
	
	semop(semid, exdo, 2);
	}
	exit(EXIT_SUCCESS);
	
}
