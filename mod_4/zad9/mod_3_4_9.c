#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
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
	munmap(shmemory, sizeof(int)+1);
	shm_unlink("/tmpshmemory");
	shm_unlink("/tmpdataforshmemory");
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
	
	signal(SIGINT,mylistener);
	
	int shm;
	
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
			perror("ERROR1");
			//exit(EXIT_FAILURE);
		break;
		case 0:
			int sizemes;
			
			sem_wait(b);
			
			if((shm=shm_open("/tmpshmemory",O_CREAT|O_RDWR,0777))==-1){
				perror("ERROR2");
				//exit(EXIT_FAILURE);
			}
			
			if(ftruncate(shm,sizeof(int)+1)==-1){
				perror("ERROR3");
				//exit(EXIT_FAILURE);
			}
			
			shmemory=mmap(0,sizeof(int)+1,PROT_WRITE|PROT_READ,MAP_SHARED,shm,0);
			
			sizemes=shmemory[0];
			
			munmap(shmemory, sizeof(int)+1);
			
			sem_post(a);
			
			sem_wait(b);
			
			shm_unlink("/tmpshmemory");
			
			sem_post(a);
			
			sem_wait(b);
			
			if((shm=shm_open("/tmpdataforshmemory",O_CREAT|O_RDWR,0777))==-1){
				perror("ERROR4");
				//exit(EXIT_FAILURE);
			}
			
			
			if(ftruncate(shm,sizemes)==-1){
				perror("ERROR5");
				//exit(EXIT_FAILURE);
			}
			
			shmemory=mmap(0,sizemes,PROT_WRITE|PROT_READ,MAP_SHARED,shm,0);
			
			int maxdata=0;
			int mindata=shmemory[2];
			
			
			for( i = 2; i<shmemory[0]+2; i++){
				if(shmemory[i]>maxdata) maxdata = shmemory[i];
				if(shmemory[i]<mindata) mindata = shmemory[i];
			}
			
			shmemory[shmemory[1]]=maxdata;
			shmemory[shmemory[1]+1]=mindata;
			
			munmap(shmemory, sizemes);
			
			sem_post(a);
			
			sem_wait(b);
			
			shm_unlink("/tmpdataforshmemory");
			
			exit(EXIT_SUCCESS);
		break;
		default:
			int num = rand() % 15 + 1;
			int max = num*sizeof(int);
			
			int shmdata;						
			
			if((shm=shm_open("/tmpshmemory",O_CREAT|O_RDWR,0777))==-1){
				perror("ERROR6");
				//exit(EXIT_FAILURE);
			}
			
			
			if(ftruncate(shm,sizeof(int)+1)==-1){
				perror("ERROR7");
				//exit(EXIT_FAILURE);
			}
			
			sem_wait(a);
						
			shmemory=mmap(0,sizeof(int)+1,PROT_WRITE|PROT_READ,MAP_SHARED,shm,0);
			
			shmemory[0]=(num+4)*sizeof(int);
			
			munmap(shmemory, sizeof(int)+1);
			
			sem_post(b);
			
			sem_wait(a);
			
			shm_unlink("/tmpshmemory");
			
			sem_post(b);
			
			sem_wait(a);
			
			if((shm=shm_open("/tmpdataforshmemory",O_CREAT|O_RDWR,0777))==-1){
				perror("ERROR8");
				//exit(EXIT_FAILURE);
			}
			
			
			if(ftruncate(shm,(num+4)*sizeof(int))==-1){
				perror("ERROR9");
				//exit(EXIT_FAILURE);
			}
			
			shmemory=mmap(0,(num+4)*sizeof(int),PROT_WRITE|PROT_READ,MAP_SHARED,shm,0);
			
			for( i = 2; i<num+2; i++){
				shmemory[i]=rand() % 25 + 2;
			}
			
			shmemory[1]=i;
			shmemory[0]=num;
			
			munmap(shmemory, (num+4)*sizeof(int));
			
			sem_post(b);
			
			sem_wait(a);
			
			shmemory=mmap(0,(num+4)*sizeof(int),PROT_WRITE|PROT_READ,MAP_SHARED,shm,0);
			
			printf("\nChild find max -> %d\n", shmemory[i]);
			
			printf("\nChild find min -> %d\n", shmemory[++i]);
			
			munmap(shmemory, (num+4)*sizeof(int));
			
			shm_unlink("/tmpdataforshmemory");
			
			sem_post(b);
			
			sem_post(a);
			
			count++;
		break;
		}
	
	
	}
		
}
