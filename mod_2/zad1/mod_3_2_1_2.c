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
	int max = 100*sizeof(char);
	char *string = (char*)malloc(max);

	if((file=open(argv[1], O_RDONLY))==-1){
		perror("ERROR2");
		exit(EXIT_FAILURE);
	}
	
	
	while(1){
		if(read(file,string,max)<max){
			printf("%s\n",string);
			break;
		}
		if(read(file,string,max)==max){
			string=(char*)realloc(string, max*2);
			max*=2;
		}
		
	}
	close(file);
	
	
}
