#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

DIR *mydir;
	
struct dirent *ger; 
struct stat buf; 

int main(){
	
	
	if((mydir=opendir("."))==NULL){
		perror("ERROR1");
		exit(EXIT_FAILURE);
	}
	
	while((ger=readdir(mydir)) != NULL){
		stat(ger->d_name,&buf);
		if(S_ISREG(buf.st_mode)){printf("File ");}
		if(S_ISDIR(buf.st_mode)){printf("Dir ");}
		if(S_ISLNK(buf.st_mode)){printf("Link ");}
		printf("%s\n", ger->d_name);
	}
	
}
