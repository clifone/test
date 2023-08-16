#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

int main(int argc, char *argv[]){
		
	sem_t *a;
	sem_t *b;
	sem_t *c;
	
	sem_unlink("/semfora");
	sem_unlink("/semforb");
	sem_unlink("/semforc");
	
	a=sem_open("/semfora", O_CREAT, 0666, 0);
	b=sem_open("/semforb", O_CREAT, 0666, 1);
	c=sem_open("/semforc", O_CREAT, 0666, 0);

	int num;
	int num2;
	int wa;
	FILE *file;
	int fd[2];
	int max = 100*sizeof(char);
	char *string = (char*)malloc(max);
	pid_t fok, fokk, fokkk;
	char temp;
	int i=0;
	if(pipe(fd)<0){
		perror("ERROR");
		exit(EXIT_FAILURE);
	}
	
	if(argc!=2){
		perror("ERROR");
		exit(EXIT_FAILURE);
	}
	for(int o=0; o<atoi(argv[1]);o++){
		switch(fok=fork()){
		case -1:
			perror("\nERROR");
			exit(EXIT_FAILURE);
		case 0:
			printf("\nChild1\n");
			close(fd[0]);
			srand(time(NULL)-o-1);
			num = rand();
			if(write(fd[1],&num,sizeof(int))<sizeof(int)){
				perror("\nERROR");
				sem_wait(c);
				sem_post(b);
				exit(EXIT_FAILURE);
			}
			printf("\n(child1)Write data %d = %d\n", o, num);
			
			sem_wait(b);
			sem_post(a);
			
			if((file=fopen("text46.txt", "rt"))==NULL){
				perror("ERROR2");
				sem_wait(c);
				sem_post(b);
				exit(EXIT_FAILURE);
			}
			while(fread(&temp,sizeof(temp),1,file) != NULL){
				string[i]=temp;
				if(i+1==max){
					string=(char*)realloc(string, max*2);
					max*=2;
				}
				i++;
			}
			fclose(file);
			
			sem_wait(c);
			sem_post(b);
			
			printf("\n(child1)Read from parent file %s\n",string);
			exit(EXIT_SUCCESS);
		default:
			switch(fokk=fork()){
				case -1:
					perror("\nERROR");
					exit(EXIT_FAILURE);
				case 0:
					printf("\nChild2\n");
					close(fd[0]);
					srand(time(NULL)-o-122);
					num = rand();
					if(write(fd[1],&num,sizeof(int))<sizeof(int)){
						perror("\nERROR");
						sem_wait(c);
						sem_post(b);
						exit(EXIT_FAILURE);
					}
					printf("\n(child2)Write data %d = %d\n", o, num);
					
					sem_wait(b);
					sem_post(a);
					
					if((file=fopen("text46.txt", "rt"))==NULL){
						perror("ERROR2");
						sem_wait(c);
						sem_post(b);
						exit(EXIT_FAILURE);
					}
					while(fread(&temp,sizeof(temp),1,file) != NULL){
						string[i]=temp;
						if(i+1==max){
							string=(char*)realloc(string, max*2);
							max*=2;
						}
						i++;
					}
					fclose(file);
					
					sem_wait(c);
					sem_post(b);
					
					printf("\n(child2)Read from parent file %s\n",string);
					exit(EXIT_SUCCESS);
				default:
					switch(fokkk=fork()){
						case -1:
							perror("\nERROR");
							exit(EXIT_FAILURE);
						case 0:
							printf("\nChild3\n");
							close(fd[0]);
							srand(time(NULL)-o+99);
							num = rand();
							if(write(fd[1],&num,sizeof(int))<sizeof(int)){
								perror("\nERROR");
								sem_wait(c);
								sem_post(b);
								exit(EXIT_FAILURE);
							}
							printf("\n(child3)Write data %d = %d\n", o, num);
							
							sem_wait(b);
							sem_post(a);
							
							if((file=fopen("text46.txt", "rt"))==NULL){
								perror("ERROR2");
								sem_wait(c);
								sem_post(b);
								exit(EXIT_FAILURE);
							}
							while(fread(&temp,sizeof(temp),1,file) != NULL){
								string[i]=temp;
								if(i+1==max){
									string=(char*)realloc(string, max*2);
									max*=2;
								}
								i++;
							}
							fclose(file);
							
							sem_wait(c);
							sem_post(b);
							
							printf("\n(child3)Read from parent file %s\n",string);
							exit(EXIT_SUCCESS);
						default:
						for(int h = 0; h<3; h++){
							printf("\nParent\n");
							if(read(fd[0],&num2,sizeof(int))<sizeof(int)){
								perror("\nERROR");
								exit(EXIT_FAILURE);
							}
							printf("\nRead data in p %d = %d\n", o, num2);

							sem_wait(a);
							
							if((file=fopen("text46.txt", "at"))==NULL){
								perror("ERROR2");
								exit(EXIT_FAILURE);
							}
							fprintf(file,"%d\n", num2);
							fclose(file);
							
							sem_post(c);
						}

					}

			}
			
			
			
		}
	}
	sem_close(a);
	sem_close(b);
	sem_close(c);
	sem_unlink("/semfora");
	sem_unlink("/semforb");
	sem_unlink("/semforc");
	system("read -p 'Press Enter to continue...' var");
}
