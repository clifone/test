#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	int num;
	int num2;
	int wa;
	unsigned int nnumm, nnumm2;
	FILE *file;
	int fd[2];
	if(pipe(fd)<0){
		perror("ERROR");
		exit(EXIT_FAILURE);
	}
	
	if(argc!=2){
		perror("ERROR");
		exit(EXIT_FAILURE);
	}
	for(int o=0; o<atoi(argv[1]);o++){
		switch(fork()){
		case -1:
			perror("\nERROR");
			exit(EXIT_FAILURE);
		case 0:
			//close(fd[0]);
			srand(time(NULL)-o);
			num = rand();
			if(write(fd[1],&num,sizeof(int))<sizeof(int)){
				perror("\nERROR");
				exit(EXIT_FAILURE);
			}
			printf("\nWrite data %d = %d\n", o, num);
			if(read(fd[0],&nnumm,sizeof(unsigned int))<sizeof(unsigned int)){
				perror("\nERROR");
				exit(EXIT_FAILURE);
			}
			printf("\nBack data %d = %d\n", o, nnumm);
			exit(EXIT_SUCCESS);
		default:
			//close(fd[1]);
			//wait(&wa);
			
			if(read(fd[0],&num2,sizeof(int))<sizeof(int)){
				perror("\nERROR");
				exit(EXIT_FAILURE);
			}
			printf("\nRead data %d = %d\n", o, num2);
			nnumm2=num2*2;
			if(write(fd[1],&nnumm2,sizeof(unsigned int))<sizeof(unsigned int)){
				perror("\nERROR");
				exit(EXIT_FAILURE);
			}
			
			if((file=fopen("text8.txt", "at"))==NULL){
				perror("ERROR2");
				exit(EXIT_FAILURE);
			}
			fprintf(file,"%d\n", num2);
			fclose(file);
			wait(&wa);
		}
	}
	
}
