#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

void mylistus1(int sig){
	printf("\nStoppppppp\n");
	int us2sig;
	sigset_t mm;
	sigemptyset(&mm);
	sigaddset(&mm, SIGUSR2);
	sigwait(&mm,&us2sig);
	printf("\nGoooooooooo\n");
}

int main(int argc, char *argv[]){
		
	struct sigaction act1;
	act1.sa_handler = mylistus1;
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	act1.sa_mask=set;
	
	sigaction(SIGUSR1,&act1,0);

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
			if((file=fopen("text36.txt", "rt"))==NULL){
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
			printf("\nRead from parent file %s\n",string);
			exit(EXIT_SUCCESS);
		default:
			printf("\nParent\n");
			if(read(fd[0],&num2,sizeof(int))<sizeof(int)){
				perror("\nERROR");
				exit(EXIT_FAILURE);
			}
			printf("\nRead data in p %d = %d\n", o, num2);

			kill(fok,10);
			if((file=fopen("text36.txt", "at"))==NULL){
				perror("ERROR2");
				exit(EXIT_FAILURE);
			}
			fprintf(file,"%d\n", num2);
			fclose(file);
			kill(fok,12);

		}
	}
	system("read -p 'Press Enter to continue...' var");
}
