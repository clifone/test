#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
	FILE *file;
	int u = 0;
	printf("\nPID prog = %d",getpid());
	while(1){
		sleep(1);
		u++;
		if((file=fopen("zad31.txt", "ab"))==NULL){
		perror("ERROR2");
		exit(EXIT_FAILURE);
		}
		fprintf(file,"%d\n",u);
		fclose(file);
	}	
}
