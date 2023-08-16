#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	
	pid_t pid;
	int wa;
	
	int rv;
	
	for(int i=1; i<argc;i++){
				if(atoi(argv[i])==0){
					printf("ERROR. Try again. Insert numbers.\n");
					exit(EXIT_FAILURE);
				}
			}
	
	if(argc <= 2){
		printf("ERROR. Try again. Insert more param\n");
		exit(EXIT_FAILURE);
	}
	
	rv = (argc-1)/2;
	
	switch(pid = fork()){
		case -1:
			printf("ERROR");
			exit(EXIT_FAILURE);
		case 0:
			printf("CHILD\n");
			for(int i=rv+1; i<argc;i++){
				printf("%d: S= %d\n", i, atoi(argv[i])*atoi(argv[i]));
			}
			exit(EXIT_SUCCESS);
		default:
			printf("PARENT\n");
			for(int i=1; i<=rv;i++){
				printf("%d: S= %d\n", i, atoi(argv[i])*atoi(argv[i]));
			}
			wait(&wa);
			exit(EXIT_SUCCESS);
		
	}
}
