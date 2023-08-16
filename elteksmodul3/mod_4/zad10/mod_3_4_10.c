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
#include <string.h>
#include <signal.h>

typedef struct{
	int number;
} data;

typedef struct{
	int countnum;
	char oper;
} tempdata;

int count = 0;
sem_t *a;
sem_t *b;
pid_t fok, fokk;
data *shmemory;
tempdata *tempshmemory;
int shmdata, yshmdatay;

void mylistener(int sig){
	if(fok!=0) printf("\nCount of done data . %d\n", count);
	sem_close(a);
	sem_close(b);
	sem_unlink("/semfora");
	sem_unlink("/semforb");
	munmap(shmemory, sizeof(tempdata)+1);
	shm_unlink("/secondtmpshmemory");
	shm_unlink("/workdata");
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
	
	signal(SIGINT,mylistener);
	
	int shm, shmur;
	
	int i;
	key_t key;
		
	sem_close(a);
	sem_close(b);
	
	sem_unlink("/semfora");
	sem_unlink("/semforb");

	a=sem_open("/semfora", O_CREAT, 0666, 1);
	b=sem_open("/semforb", O_CREAT, 0666, 0);
	
	srand(time(NULL));
	
	int l = 0;
	
	while(1){
		fok=fork();
		switch(fok){
		case -1:
			perror("ERROR");
			raise(2);
			//exit(EXIT_FAILURE);
		break;
		case 0:
			//for(int jam = 0; jam<4; jam++){
				int sizemes;
				char num[12];
				char work;
				int shmdata;

				sem_wait(b);			

				if((shm=shm_open("/secondtmpshmemory",O_CREAT|O_RDWR,0777))==-1){
				perror("ERROR2");
				//exit(EXIT_FAILURE);
				}
				
				if(ftruncate(shm,sizeof(tempdata)+1)==-1){
					perror("ERROR3");
					//exit(EXIT_FAILURE);
				}
				
				tempshmemory=mmap(0,sizeof(tempdata)+1,PROT_WRITE|PROT_READ,MAP_SHARED,shm,0);

				work=tempshmemory[0].oper;
					
				sizemes=tempshmemory[0].countnum;

				munmap(tempshmemory, sizeof(tempdata)+1);

				sem_post(a);

				sem_wait(b);

				shm_unlink("/secondtmpshmemory");
				
				
				if((shm=shm_open("/workdata",O_CREAT|O_RDWR,0777))==-1){
					perror("ERROR2");
					//exit(EXIT_FAILURE);
				}
				
				if(ftruncate(shm,sizemes)==-1){
					perror("ERROR3");
					//exit(EXIT_FAILURE);
				}
				
				shmemory=mmap(0,sizemes,PROT_WRITE|PROT_READ,MAP_SHARED,shm,0);
				
				if(work=='a'){
					int maxdata=0;
					int mindata=shmemory[2].number;
					for( i = 2; i<(shmemory[0].number)+2; i++){
						if(shmemory[i].number>maxdata) maxdata = shmemory[i].number;
						if(shmemory[i].number<mindata) mindata = shmemory[i].number;
					}
					shmemory[shmemory[1].number].number=maxdata;
					shmemory[shmemory[1].number+1].number=mindata;
				}else if(work=='i'){
					int maxdata=0;
					int mindata=shmemory[2].number;
					for( i = 2; i<(shmemory[0].number)+2; i++){
						if(shmemory[i].number>maxdata) maxdata = shmemory[i].number;
						if(shmemory[i].number<mindata) mindata = shmemory[i].number;
					}
					shmemory[shmemory[1].number].number=maxdata;
					shmemory[shmemory[1].number+1].number=mindata;
				}else if(work=='+'){
					int sum=0;
					for( i = 2; i<(shmemory[0].number)+2; i++){
						sum+=shmemory[i].number;
					}
				
					shmemory[shmemory[1].number+2].number=sum;
				}else if(work=='s'){
					shmemory[shmemory[1].number+3].number=shmemory[shmemory[1].number+2].number/(shmemory[0].number);
				}
				
				munmap(shmemory, sizemes);

				sem_post(a);
			//}
			exit(EXIT_SUCCESS);
		break;
		default:
			int numm;
			int max;
			if(l==0){
				numm = rand() % 15 + 5;
				max = numm*sizeof(int);
			}
			if((shmur=shm_open("/workdata",O_CREAT|O_RDWR,0777))==-1){
				perror("ERROR2");
				//exit(EXIT_FAILURE);
			}
				
			if(ftruncate(shmur,(max+6)*sizeof(data))==-1){
				perror("ERROR3");
				//exit(EXIT_FAILURE);
			}
			
			shmemory=mmap(0,(max+6)*sizeof(data),PROT_WRITE|PROT_READ,MAP_SHARED,shmur,0);
			
			if(l==0){
				for( i = 2; i<numm+2; i++){
					shmemory[i].number=rand() % 25 + 2;
				}
			
				shmemory[1].number=i;
				shmemory[0].number=numm;
			}
			
			munmap(shmemory, (max+6)*sizeof(data));

			//for(int l=0; l<4; l++){
				//if(l>0) sem_post(a);
				sem_wait(a);
				
				if((shm=shm_open("/secondtmpshmemory",O_CREAT|O_RDWR,0777))==-1){
					perror("ERROR2");
					//exit(EXIT_FAILURE);
				}
				
				if(ftruncate(shm,sizeof(tempdata)+1)==-1){
					perror("ERROR3");
					//exit(EXIT_FAILURE);
				}
				
				tempshmemory=mmap(0,sizeof(tempdata)+1,PROT_WRITE|PROT_READ,MAP_SHARED,shm,0);

				switch(l){
					case 0:
					tempshmemory[0].oper='a';
					break;
					case 1:
					tempshmemory[0].oper='i';
					break;
					case 2:
					tempshmemory[0].oper='+';
					break;
					case 3:
					tempshmemory[0].oper='s';
					break;
					default:
					break;
				}				
				tempshmemory[0].countnum=(max+6)*sizeof(data);
				
				munmap(tempshmemory,sizeof(tempdata)+1);

				sem_post(b);

				sem_wait(a);
				
				shm_unlink("/secondtmpshmemory");

				sem_post(b);
				
				if(l!=4){
					l++;
					break;
				}else{
					l=0;
				}
				//sem_wait(a);
				//fokk=fork();
			
			//}
			
			//kill(fokk, 2);
			sem_wait(a);
			
			shmemory=mmap(0,(max+6)*sizeof(data),PROT_WRITE|PROT_READ,MAP_SHARED,shmur,0);
			
			printf("\nChild find max . %d\n", shmemory[i].number);
			
			printf("\nChild find min . %d\n", shmemory[++i].number);
			
			printf("\nChild find sum . %d\n", shmemory[++i].number);
			
			printf("\nChild find mean . %d\n", shmemory[++i].number);
			
			munmap(tempshmemory,(max+6)*sizeof(data));
				
			shm_unlink("/workdata");
			
			sem_post(a);
			
			count++;
		break;
		}
	
	
	}
		
}
