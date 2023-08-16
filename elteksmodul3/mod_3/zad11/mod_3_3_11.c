#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <mqueue.h>

int main(){

	int num;
	char st[50];
	mqd_t outproc, inproc;
	
	struct mq_attr atr;
	atr.mq_curmsgs=0;
	atr.mq_flags=0;
	atr.mq_maxmsg=10;
	atr.mq_msgsize=50;
	
	pid_t pid;
	int p;
	int prior =256;
	
	if((inproc=mq_open("/mqforzadin", O_CREAT | O_RDONLY, 0600, &atr))==(mqd_t)-1){
		perror("ERROR1");
		exit(EXIT_FAILURE);
	}
	
	if((outproc=mq_open("/mqforzadout", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR, &atr))==(mqd_t)-1){
		perror("ERROR1");
		exit(EXIT_FAILURE);
	}
		
	pid=fork();
	if(pid==-1){
		perror("ERROR");
		exit(EXIT_FAILURE);
	}
	while(1){
	//scanf("\n%s",st);
	if(pid==0){
		while(1){
			if(mq_receive(inproc,st,50,&p)==-1){
				perror("ERROR22");
				exit(EXIT_FAILURE);
			}
			if(p==255){
			if(mq_close(inproc)==-1){
				perror("ERROR33");
				exit(EXIT_FAILURE);
			}
			if(mq_close(outproc)==-1){
				perror("ERROR44");
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
			}else if(p==256){
				printf("\nMessge -> %s\n",st);
			}
		}
	}else{
		fgets(st,24,stdin);
		strcat(st,"\0");
	
		if(st[0]=='0'){prior=255;}else{prior=256;}

	
		if(mq_send(outproc,st,strlen(st),prior)==-1){
			perror("ERROR2");
			exit(EXIT_FAILURE);
		}
		if(st[0]=='0'){
			if(mq_close(inproc)==-1){
				perror("ERROR3");
				exit(EXIT_FAILURE);
			}
			if(mq_close(outproc)==-1){
				perror("ERROR3");
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
			
		}
	}	
	}
}
