#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t pid;

void Normalback();

int main(int argc, char *argv[]){
	
	
	int rv;
	
	atexit(Normalback);
	
	switch(pid = fork()){
		case -1:
			printf("ERROR");
			exit(EXIT_FAILURE);
		case 0:
			printf("CHILD\n");
			for(int i=0; i<argc;i++){
				printf("%d: %s\n", i, argv[i]);
			}
			exit(EXIT_SUCCESS);
		default:
			printf("PARENT\n");
			for(int i=0; i<argc;i++){
				printf("%d: %s\n", i, argv[i]);
			}
			wait(&rv);
			exit(EXIT_SUCCESS);
		
	}
}

void Normalback(){
	printf("NORMAL RETURN. constant pid = %d\n", pid);
}
