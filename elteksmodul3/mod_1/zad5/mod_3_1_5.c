#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
	
	pid_t pid;
	int wa;
	
	int rv;
	
	while(1==1){
	
	char n[20]="";
	char nn[15]="";
	char nntemp[15]="";
	
	printf("Insert name program and param (For exit insert ESC):\n");
	
	fgets(n,20,stdin);

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
	
	}
	exit(EXIT_SUCCESS);

}
