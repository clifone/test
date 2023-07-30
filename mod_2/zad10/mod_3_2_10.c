#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

int main(int argc, char *argv[]){
	
	int num;
	FILE *fd_fifo;
	DIR *mydir;
	srand(time(NULL));
	//while(1){
	
	if((mydir=opendir("/tmp"))==NULL){
		perror("ERROR1");
		exit(EXIT_FAILURE);
	}
	chdir("/tmp");
		
	int u=0;
	struct dirent *ger; 
	rewinddir(mydir);
	while((ger=readdir(mydir)) != NULL){
		if(strcmp("my_new_fifo.fifo",ger->d_name)==0){
			u=1;
			break;
		}
	}
	if(u==0){
		if(mkfifo("/tmp/my_new_fifo.fifo", S_IRWXO|S_IRWXU)==-1){
			perror("\nERROR1");
			exit(EXIT_FAILURE);
		}
	}
	
	fd_fifo=fopen("/tmp/my_new_fifo.fifo","w");
	if(fd_fifo==NULL){
		printf("\nNULL\n");
	}
	while(1){
	num = rand();
	
	if(!fwrite(&num,sizeof(int),1,fd_fifo)){
		perror("ERROR3");
		//fclose(fd_fifo);
		//exit(EXIT_FAILURE);
	}
	//fclose(fd_fifo);
	}
	
}
