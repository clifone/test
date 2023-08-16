#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void fucn(char *c, char *cc, int *i, char *n);

int main(int argc, char *argv[]){
	
	pid_t pid;
	int wa;
	int fd[2];
	int fd2[2];
	int oldstdout = dup(1);
	int rv;
	
	while(1==1){
	
	char n[25]="";
	char nn[15]="";
	char nntemp[15]="";
	
	printf("Insert name program and param (For exit insert ESC):\n");
	
	fgets(n,25,stdin);

	int i = 0;
	while(n[i]!=' ' && n[i]!='\0' && n[i]!='\n'){
		nntemp[i]=n[i];
		i++;
	}
	
	char nnn[]="/bin/";
	if(strcmp(nntemp,"sum")!=0 && strcmp(nntemp,"len")!=0 && strcmp(nntemp,"cat")!=0 && strcmp(nntemp,"srav")!=0 && strcmp(nntemp,"ESC")!=0){
		strcat(nn,nnn);
		strcat(nn,nntemp);
	}else{
	printf("%s\n",nn);
	strcat(nn,nntemp);
	}
	
	if(strcmp(nn,"ESC")==0){
		exit(EXIT_SUCCESS);
	}
	
	if(memchr(n,'|',strlen(n))==NULL){
		char a[20]="";
		char b[20]="";
		if(n[i]!='\0') i++;
		int l = 0;
		while(n[i] != ' ' && n[i]!='\0' && n[i]!='\n'){
			a[l]=n[i];
			i++;
			l++;
		}
		if(n[i]!='\0') i++;
		l = 0;
		while(n[i] != ' ' && n[i]!='\0' && n[i]!='\n'){
			b[l]=n[i];
			i++;
			l++;
		}

		
		switch(pid = fork()){
		case -1:
			printf("ERROR\n");
			exit(EXIT_FAILURE);
		case 0:
			if(strlen(a)==0 && strlen(b)==0){
				execl(nn,nn,NULL);
			}else{
			execl(nn,nn,a,b,NULL);
			}
			exit(EXIT_SUCCESS);
		default:
			wait(&wa);
	}
		
	
	}else{
		char a[20]="";
		if(n[i]!='\0') i++;
		int l = 0;
		while(n[i] != '|' && n[i]!='\0' && n[i]!='\n'){
			a[l]=n[i];
			i++;
			l++;
		}
		
		char b[20]="";
		char bb[20]="";

		fucn(b,bb,&i,n);
			
		char c[20]="";
		char cc[20]="";

		fucn(c,cc,&i,n);
		
		if(pipe(fd)<0){
		perror("ERROR");
		exit(EXIT_FAILURE);
	}
	
	pid = fork();
	
	if(pid<0){			
		printf("ERROR\n");
		exit(EXIT_FAILURE);
	}else if (pid == 0){
			dup2(fd[1],1);
			close(fd[0]);
			close(fd[1]);
			if(strlen(a)==0){
				execl(nn,nn,NULL);
			}else{
			execl(nn,nn,a,NULL);
			}
			exit(EXIT_SUCCESS);
		}else{
		
			if(pipe(fd2)<0){
				perror("ERROR");
				exit(EXIT_FAILURE);
			}
		
			pid=fork();
			
			if(pid<0){			
				printf("ERROR\n");
				exit(EXIT_FAILURE);
			}else if (pid == 0){
					dup2(fd[0],0);
					dup2(fd2[1],1);
					printf("\nIN prog tail\n");
					close(fd[0]);
					close(fd[1]);
					close(fd2[0]);
					close(fd2[1]);
					printf("\nBefor execl\n");
					if(strlen(bb)==0){
						execl(strcat(nnn,b),b,NULL);
					}else{
						execl(strcat(nnn,b),b,bb,NULL);
					}
					exit(EXIT_SUCCESS);
			}else{
				pid=fork();
			
				if(pid<0){			
					printf("ERROR\n");
					exit(EXIT_FAILURE);
				}else if (pid == 0){
					dup2(fd2[0],0);
					dup2(oldstdout,1);
					close(fd[0]);
					close(fd[1]);
					close(fd2[0]);
					close(fd2[1]);
					if(strlen(cc)==0){
						execl(strcat(nnn,c),c,NULL);
					}else{
						execl(strcat(nnn,c),c,cc,NULL);
					}
					exit(EXIT_SUCCESS);
				}else{
					close(fd[0]);
					close(fd[1]);
					close(fd2[0]);
					close(fd2[1]);
					waitpid(pid,&wa,0);
				}
			}
		}

	}	
	}
	exit(EXIT_SUCCESS);
}

void fucn(char *c, char *cc, int *i, char *n){
		if(n[*i]=='|')*i+=2;
		int l = 0;
		while(n[*i] != '|' && n[*i]!='\0' && n[*i]!='\n'){
			if(n[*i]==' '){
				*i=*i+1;
				l=0;
				while(n[*i] != '|' && n[*i] != ' ' && n[*i]!='\0' && n[*i]!='\n'){
					cc[l]=n[*i];
					*i=*i+1;
					l++;
				}
			}else{
				c[l]=n[*i];
			}
			*i=*i+1;
			l++;
		}
	}
