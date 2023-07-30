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
	shmdt(tempshmemory);
	shmctl(yshmdatay, IPC_RMID,0);
	shmdt(shmemory);
	shmctl(shmdata, IPC_RMID,0);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
	
	signal(SIGINT,mylistener);
	
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

				key=ftok("cammonhelpme",106);

				shmdata=shmget(key, sizeof(tempdata),0644);

				tempshmemory=(tempdata *)shmat(shmdata,NULL,0);

				work=tempshmemory[0].oper;
					
				sizemes=tempshmemory[0].countnum;

				shmdt(tempshmemory);

				sem_post(a);

				sem_wait(b);

				shmctl(shmdata, IPC_RMID,0);
				
				
				key=ftok("workplese",3333);
				
				if((shmdata=shmget(key, sizemes,0666))==-1){
					perror("ERROR1");
					raise(2);
					//exit(EXIT_FAILURE);
				}
				if((shmemory=(data *)shmat(shmdata,NULL,0))==-1){
					perror("ERROR2");
					raise(2);
					//exit(EXIT_FAILURE);
				}
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
				
				shmdt(shmemory);

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
			key=ftok("workplese",3333);
	
			shmdata=shmget(key, (max+6)*sizeof(data),IPC_CREAT|S_IRUSR|S_IWUSR);

			shmemory=(data *)shmat(shmdata,NULL,0);
			if(l==0){
				for( i = 2; i<numm+2; i++){
					shmemory[i].number=rand() % 25 + 2;
				}
			
				shmemory[1].number=i;
				shmemory[0].number=numm;
			}
			shmdt(shmemory);

			//for(int l=0; l<4; l++){
				//if(l>0) sem_post(a);
				sem_wait(a);

				key=ftok("cammonhelpme",106);

				yshmdatay=shmget(key, sizeof(tempdata),IPC_CREAT|S_IRUSR|S_IWUSR);

				tempshmemory=(tempdata *)shmat(yshmdatay,NULL,0);

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
				
				shmdt(tempshmemory);

				sem_post(b);

				sem_wait(a);
				
				shmctl(yshmdatay, IPC_RMID,0);

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
			
			shmemory=(data *)shmat(shmdata,NULL,0);
			
			printf("\nChild find max . %d\n", shmemory[i].number);
			
			printf("\nChild find min . %d\n", shmemory[++i].number);
			
			printf("\nChild find sum . %d\n", shmemory[++i].number);
			
			printf("\nChild find mean . %d\n", shmemory[++i].number);
			
			shmdt(shmemory);
			
			shmctl(shmdata, IPC_RMID,0);
			
			sem_post(a);
			
			count++;
		break;
		}
	
	
	}
		
}
