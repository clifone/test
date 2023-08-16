#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <semaphore.h>
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
	
	sem_t *a;
	sem_t *b;
	sem_t *c;
	
	a=sem_open("/seminer", O_CREAT, 0666, 0);
	b=sem_open("/semouter", O_CREAT, 0666, 1);
	c=sem_open("/semcontr", O_CREAT, 0666, 0);
	
	
	while(1){
	
	sem_wait(b);
	sem_post(a);

	fd_fifof=fopen("/tmp/oldnew_fifo.fifo","r");
	if(fd_fifof==NULL){
		perror("\nERROR1");
		exit(EXIT_FAILURE);
	}
	
	if(!fread(&num,sizeof(int),1,fd_fifof)){
		perror("\nERROR2");
		//fclose(fd_fifof);
		//exit(EXIT_FAILURE);
	}
	
	fclose(fd_fifof);
	sem_wait(c);
	sem_post(b);
	printf("\nRead data = %d\n", num);
	
	}
	sem_close(a);
	sem_close(b);
	sem_close(c);
	exit(EXIT_SUCCESS);
	
}
