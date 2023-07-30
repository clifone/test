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
	if(sig==2) {printf("\nThis SIGINT\n");}
	if(sig==3) printf("\nThis SIGQUIT\n");
	if(u==1) exit(EXIT_SUCCESS);
}

int main(){
	signal(SIGINT,mylistener);
	signal(SIGQUIT,mylistener);
	FILE *file;
	printf("\nPID prog = %d\n",getpid());
	while(1){
		sleep(1);
		u=1;
		if((file=fopen("zad35.txt", "ab"))==NULL){
		perror("ERROR2");
		exit(EXIT_FAILURE);
		}
		sleep(1); //FOR check
		fprintf(file,"%d\n",u);
		fclose(file);
		u=0;
	}	
}
