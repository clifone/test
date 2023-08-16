#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short * array;
	struct seminfo *__buf;
};

int main(int argc, char *argv[]){
		
	key_t key;
	int semid;
	
	struct sembuf entermy[2] = {{0,0, 0},{0,2,0}};
	struct sembuf exitprog[2] = {{1, 0, 0},{1,2,0}};
	
	struct sembuf waitenter[2] = {{0,-2, 0},{0,1,0}};
	struct sembuf exdo[2] = {{1,-2,0},{0,-1,0}};

	union semun arg;
	
	key=ftok("/etc/semfortwo",69);

	semid = semget(key,2,0666 | IPC_CREAT);

	arg.val = 0;
	semctl(semid, 0, SETVAL, arg);
	
	arg.val = 0;
	semctl(semid, 1, SETVAL, arg);


	int num;
	int num2;
	int wa;
	FILE *file;
	int fd[2];
	int max = 100*sizeof(char);
	char *string = (char*)malloc(max);
	pid_t fok;
	char temp;
	int i=0;
	if(pipe(fd)<0){
		perror("ERROR");
		exit(EXIT_FAILURE);
	}
	
	if(argc!=2){
		perror("ERROR");
		exit(EXIT_FAILURE);
	}
	for(int o=0; o<atoi(argv[1]);o++){
		switch(fok=fork()){
		case -1:
			perror("\nERROR");
			exit(EXIT_FAILURE);
		case 0:
			printf("\nChild\n");
			close(fd[0]);
			srand(time(NULL)-o);
			num = rand();
			if(write(fd[1],&num,sizeof(int))<sizeof(int)){
				perror("\nERROR");
				exit(EXIT_FAILURE);
			}
			printf("\nWrite data %d = %d\n", o, num);
			if(semop(semid, waitenter, 2)== -1) {perror("Lock Error");}
			if((file=fopen("text42.txt", "rt"))==NULL){
				perror("ERROR2");
				exit(EXIT_FAILURE);
			}
			while(fread(&temp,sizeof(temp),1,file) != NULL){
				string[i]=temp;
				if(i+1==max){
					string=(char*)realloc(string, max*2);
					max*=2;
				}
				i++;
			}
			fclose(file);
			semop(semid, exdo, 2);
			printf("\nRead from parent file %s\n",string);
			exit(EXIT_SUCCESS);
		default:
			printf("\nParent\n");
			if(read(fd[0],&num2,sizeof(int))<sizeof(int)){
				perror("\nERROR");
				exit(EXIT_FAILURE);
			}
			printf("\nRead data in p %d = %d\n", o, num2);

			if(semop(semid, entermy, 2)== -1) {perror("Lock Error");}
			if((file=fopen("text42.txt", "at"))==NULL){
				perror("ERROR2");
				exit(EXIT_FAILURE);
			}
			fprintf(file,"%d\n", num2);
			fclose(file);
			semop(semid, exitprog, 2);

		}
	}
	system("read -p 'Press Enter to continue...' var");
}
