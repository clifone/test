#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int main(){
	DIR *mydir;
	
	struct dirent *ger; 
	
	if((mydir=opendir("."))==NULL){
		perror("ERROR1");
		exit(EXIT_FAILURE);
	}
	
	while((ger=readdir(mydir)) != NULL){
		printf("%s\n", ger->d_name);
	}
	
}
