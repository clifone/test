#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <semaphore.h>
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
	
	sem_t *a;
	sem_t *b;
	sem_t *c;
	
	a=sem_open("/seminer", O_CREAT, 0666, 0);
	b=sem_open("/semouter", O_CREAT, 0666, 1);
	c=sem_open("/semcontr", O_CREAT, 0666, 0);
	
		
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
		if(strcmp("oldnew_fifo.fifo",ger->d_name)==0){
			u=1;
			break;
		}
	}
	if(u==0){
		if(mkfifo("/tmp/oldnew_fifo.fifo", S_IRWXO|S_IRWXU)==-1){
			perror("\nERROR1");
			exit(EXIT_FAILURE);
		}
	}

	
	while(1){
	num = rand();
	
	sem_wait(a);
	fd_fifo=fopen("/tmp/oldnew_fifo.fifo","w");
	if(fd_fifo==NULL){

	}

	if(!fwrite(&num,sizeof(int),1,fd_fifo)){
		perror("ERROR3");
		//fclose(fd_fifo);
		//exit(EXIT_FAILURE);
	}

	fclose(fd_fifo);
	sem_post(c);
	//fclose(fd_fifo);
	}
	sem_close(a);
	sem_close(b);
	sem_close(c);
	sem_unlink("/seminer");
	sem_unlink("/semouter");
	sem_unlink("/semcontr");
	
}
