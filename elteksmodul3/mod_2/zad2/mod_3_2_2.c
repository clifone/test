#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
	
	if(argc != 2){
		perror("ERROR1");
		exit(EXIT_FAILURE);
	}
	
	FILE *file;
	char string[100]="";
	
	printf("Insert string:\n");
	
	fgets(string,100,stdin);
	
	if((file=fopen(argv[1], "at"))==NULL){
		perror("ERROR2");
		exit(EXIT_FAILURE);
	}
	
	fwrite(string, strlen(string),1,file);
	fclose(file);
	exit(EXIT_FAILURE);
	
	
}
