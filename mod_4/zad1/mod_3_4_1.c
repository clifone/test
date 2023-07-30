#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short * array;
	struct seminfo *__buf;
};

int main(int argc, char *argv[]){
	
	int num;
	
	FILE *fd_fifo;
	DIR *mydir;
	key_t key;
	int semid;
	
	struct sembuf entermy[2] = {{0,0, 0},{0,2,0}};
	struct sembuf exitprog[2] = {{1, 0, 0},{1,2,0}};

	union semun arg;
	
	srand(time(NULL));
	//while(1){
	
	if((mydir=opendir("/tmp"))==NULL){
		perror("ERROR1");
		exit(EXIT_FAILURE);
	}
	chdir("/tmp");
		
	int u=0;
	struct dirent *ger; 
	rewinddir(mydir);
	while((ger=readdir(mydir)) != NULL){
		if(strcmp("new_fifo.fifo",ger->d_name)==0){
			u=1;
			break;
		}
	}
	if(u==0){
		if(mkfifo("/tmp/new_fifo.fifo", S_IRWXO|S_IRWXU)==-1){
			perror("\nERROR1");
			exit(EXIT_FAILURE);
		}
	}

	key=ftok("/etc/mysem",58);

	semid = semget(key,2,0666 | IPC_CREAT);

	arg.val = 0;
	semctl(semid, 0, SETVAL, arg);
	
	arg.val = 0;
	semctl(semid, 1, SETVAL, arg);
	
	

	
	while(1){
	num = rand();

	if(semop(semid, entermy, 2)== -1) {perror("Lock Error");}

	fd_fifo=fopen("/tmp/new_fifo.fifo","w");
	if(fd_fifo==NULL){

	}

	if(!fwrite(&num,sizeof(int),1,fd_fifo)){
		perror("ERROR3");
		//fclose(fd_fifo);
		//exit(EXIT_FAILURE);
	}

	fclose(fd_fifo);
	semop(semid, exitprog, 2);
	//fclose(fd_fifo);
	}
	
}
