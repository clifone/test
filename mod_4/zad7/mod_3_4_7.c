#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <signal.h>

int count = 0;
sem_t *a;
sem_t *b;
pid_t fok;
int *shmemory;
int shmdata;

void mylistener(int sig){
	if(fok!=0) printf("\nCount of done data -> %d\n", count);
	sem_close(a);
	sem_close(b);
	sem_unlink("/semfora");
	sem_unlink("/semforb");
	shmdt(shmemory);
	shmctl(shmdata, IPC_RMID,0);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
	
	signal(SIGINT,mylistener);
	
	int i;
	key_t key;
	
	sem_unlink("/semfora");
	sem_unlink("/semforb");
	
	a=sem_open("/semfora", O_CREAT, 0666, 1);
	b=sem_open("/semforb", O_CREAT, 0666, 0);
	
	srand(time(NULL));
	
	
	
	while(1){
		
		switch(fok=fork()){
		case -1:
			perror("ERROR");
			//exit(EXIT_FAILURE);
		break;
		case 0:
			int sizemes;
			
			sem_wait(b);
			
			key=ftok("stranmem",89);

			shmdata=shmget(key, 1,0644);
			
			shmemory=(int *)shmat(shmdata,NULL,0);
			
			sizemes=shmemory[0];
			
			shmdt(shmemory);
			
			sem_post(a);
			
			sem_wait(b);
			
			shmctl(shmdata, IPC_RMID,0);
			
			sem_post(a);
			
			sem_wait(b);
			
			key=ftok("stranmem",89);

			shmdata=shmget(key, sizemes,0644);
			
			shmemory=(int *)shmat(shmdata,NULL,0);
			
			int maxdata=0;
			int mindata=shmemory[2];
			
			
			for( i = 2; i<shmemory[0]+2; i++){
				if(shmemory[i]>maxdata) maxdata = shmemory[i];
				if(shmemory[i]<mindata) mindata = shmemory[i];
			}
			
			shmemory[shmemory[1]]=maxdata;
			shmemory[shmemory[1]+1]=mindata;
			
			shmdt(shmemory);
			
			sem_post(a);
			
			exit(EXIT_SUCCESS);
		break;
		default:
			int num = rand() % 15 + 1;
			int max = num*sizeof(int);
			
			int shmdata;
			
			key=ftok("stranmem",89);

			shmdata=shmget(key, 1,IPC_CREAT|S_IRUSR|S_IWUSR);
			
			sem_wait(a);
			
			shmemory=(int *)shmat(shmdata,NULL,0);
			
			shmemory[0]=max+4;
			
			shmdt(shmemory);
			
			sem_post(b);
			
			sem_wait(a);
			
			shmctl(shmdata, IPC_RMID,0);
			
			sem_post(b);
			
			sem_wait(a);
			
			key=ftok("stranmem",89);

			shmdata=shmget(key, max+2+2,IPC_CREAT|S_IRUSR|S_IWUSR);
			
			shmemory=(int *)shmat(shmdata,NULL,0);
			for( i = 2; i<num+2; i++){
				shmemory[i]=rand() % 25 + 2;
			}
			
			shmemory[1]=i;
			shmemory[0]=num;
			
			shmdt(shmemory);
			
			sem_post(b);
			
			sem_wait(a);
			
			shmemory=(int *)shmat(shmdata,NULL,0);
			
			printf("\nChild find max -> %d\n", shmemory[i]);
			
			printf("\nChild find min -> %d\n", shmemory[++i]);
			
			shmdt(shmemory);
			
			shmctl(shmdata, IPC_RMID,0);
			
			sem_post(a);
			
			count++;
		break;
		}
	
	
	}
		
}
