#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

int u = 0;

void mylistener(int sig){
	u++;
	if(u==3) exit(EXIT_SUCCESS);
}

int main(){
	signal(SIGINT,mylistener);
	FILE *file;
	int u = 0;
	printf("\nPID prog = %d\n",getpid());
	while(1){
		sleep(1);
		u++;
		if((file=fopen("zad33.txt", "ab"))==NULL){
		perror("ERROR2");
		exit(EXIT_FAILURE);
		}
		fprintf(file,"%d\n",u);
		fclose(file);
	}	
}
