#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(){
	srand(time(NULL));
	int num;
	FILE *fd_fifof;
	
	//while(1){
	
	fd_fifof=fopen("/tmp/my_new_fifo.fifo","r");
	if(fd_fifof==NULL){
		perror("\nERROR1");
		exit(EXIT_FAILURE);
	}
	while(1){
	if(!fread(&num,sizeof(int),1,fd_fifof)){
		perror("\nERROR2");
		//fclose(fd_fifof);
		//exit(EXIT_FAILURE);
	}
	
	//fclose(fd_fifof);
	printf("\nRead data = %d\n", num);
	}
	exit(EXIT_SUCCESS);
	
}
