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
	
	while(1){
	
	char n[20]="";
	char nn[8]="";
	
	printf("Insert name program and param (For exit insert ESC):\n");
	
	fgets(n,20,stdin);
	int i = 0;
	while(n[i]!=' ' && n[i]!='\0' && n[i]!='\n'){
		nn[i]=n[i];
		i++;
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
			printf("ERROR");
			exit(EXIT_FAILURE);
		case 0:
			execl(nn," ",a,b,NULL);
		default:
			wait(&wa);
		
	}
	
	}
	exit(EXIT_SUCCESS);

}
