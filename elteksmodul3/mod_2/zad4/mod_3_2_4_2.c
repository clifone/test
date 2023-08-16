#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

int main(int argc, char *argv[]){
	
	if(argc != 2){
		perror("ERROR1");
		exit(EXIT_FAILURE);
	}
	
	int u=0;
	DIR *mydir;
	struct dirent *ger; 
	if((mydir=opendir("."))==NULL){
		perror("ERROR1");
		exit(EXIT_FAILURE);
	}
	while((ger=readdir(mydir)) != NULL){
		if(strcmp(argv[1],ger->d_name)==0){
			u=1;
			break;
		}
	}
	if(!u){
		printf("File not found. Creat file? (y/n): ");
		if((getchar())=='y'){
			if(creat(argv[1], S_IRWXU|S_IROTH)==NULL){
				perror("ERROR1");
				exit(EXIT_FAILURE);
			}
		}
		getchar();
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
