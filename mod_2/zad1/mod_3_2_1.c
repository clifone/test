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
	
	int file;
	char string[100]="";
	
	printf("Insert string:\n");
	
	fgets(string,100,stdin);
	
	if((file=open(argv[1], O_CREAT|O_WRONLY|O_APPEND, S_IRWXU|S_IROTH))==-1){
		perror("ERROR2");
		exit(EXIT_FAILURE);
	}
	
	if(write(file,string,strlen(string))==-1){
		perror("ERROR3");
		close(file);
		exit(EXIT_FAILURE);
	}
	
	close(file);
}
