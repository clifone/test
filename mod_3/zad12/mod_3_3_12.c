#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <mqueue.h>

int main(int argc, char *argv[]){
	
	if(argc != 2){
		perror("ERROR1");
		exit(EXIT_FAILURE);
	}
	
	int p;
	int prior;
	pid_t pid;
	char st[50];
	mqd_t outproc, inproc, serproc, frsproc, scnproc, thrproc;

	struct mq_attr atr;
	atr.mq_curmsgs=0;
	atr.mq_flags=0;
	atr.mq_maxmsg=10;
	atr.mq_msgsize=50;
	
	
	if(atoi(argv[1])!=1){
	
	if((inproc=mq_open("/mqser", O_CREAT | O_RDWR, 0600, &atr))==(mqd_t)-1){
		perror("ERROR1");
		exit(EXIT_FAILURE);
	}
	
	switch(atoi(argv[1])){
		case 2:
		if((outproc=mq_open("/mqout1", O_CREAT | O_RDWR, 0600, &atr))==(mqd_t)-1){
		perror("ERROR1");
		exit(EXIT_FAILURE);
		}
		break;
		case 3:
		if((outproc=mq_open("/mqout2", O_CREAT | O_RDWR, 0600, &atr))==(mqd_t)-1){
		perror("ERROR1");
		exit(EXIT_FAILURE);
		}
		break;
		case 4:
		if((outproc=mq_open("/mqout3", O_CREAT | O_RDWR, 0600, &atr))==(mqd_t)-1){
		perror("ERROR1");
		exit(EXIT_FAILURE);
		}
		break;
		default:
		break;
	}
	}else if(atoi(argv[1])==1){
		if((serproc=mq_open("/mqser", O_CREAT | O_RDWR, 0600, &atr))==(mqd_t)-1){
		perror("ERROR1");
		exit(EXIT_FAILURE);
		}
	
		if((frsproc=mq_open("/mqout1", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &atr))==(mqd_t)-1){
		perror("ERROR1");
		exit(EXIT_FAILURE);
		}
	
		if((scnproc=mq_open("/mqout2", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &atr))==(mqd_t)-1){
		perror("ERROR1");
		exit(EXIT_FAILURE);
		}
	
		if((thrproc=mq_open("/mqout3", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &atr))==(mqd_t)-1){
		perror("ERROR1");
		exit(EXIT_FAILURE);
		}
	}

	pid = fork();
	while(1){
	if(pid<0){
		printf("ERROR");
		exit(EXIT_FAILURE);
	}else if(pid==0)
	{

	if(atoi(argv[1])==1){

			memset(st,0,strlen(st));
			if(mq_receive(serproc,st,50,&p)==-1){
				perror("ERROR22");
				exit(EXIT_FAILURE);
			}

			if(p==255){
			prior=255;
			if((mq_send(frsproc,st,strlen(st),p)==-1) || (mq_send(scnproc,st,strlen(st),p)==-1) || (mq_send(thrproc,st,strlen(st),p)==-1)){
				perror("ERROR2");
				exit(EXIT_FAILURE);
			}
			if((mq_close(serproc)==-1) || (mq_close(frsproc)==-1) || (mq_close(scnproc)==-1) || (mq_close(thrproc)==-1)){
				perror("ERROR33");
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
			}else if(p==256){
			char n;
			int y = strlen(st)-1;
			n=st[y];
			st[y]='\0';
			switch(atoi(&n)){
			case 3:
			if((mq_send(frsproc,st,strlen(st),p)==-1) || (mq_send(thrproc,st,strlen(st),p)==-1)){
				perror("ERROR2");
				exit(EXIT_FAILURE);
			}
			break;
			case 4:
			if((mq_send(frsproc,st,strlen(st),p)==-1) || (mq_send(scnproc,st,strlen(st),p)==-1)){
				perror("ERROR2");
				exit(EXIT_FAILURE);
			}
			break;
			case 2:
			if((mq_send(scnproc,st,strlen(st),p)==-1) || (mq_send(thrproc,st,strlen(st),p)==-1)){
				perror("ERROR2");
				exit(EXIT_FAILURE);
			}
			break;
			default:
			break;
			}
			}
			
			
	}else {
		memset(st,0,strlen(st));
		if(mq_receive(outproc,st,50,&p)==-1){
			perror("ERROR22");
			exit(EXIT_FAILURE);
		}
		if(p==255){	
			if((mq_close(inproc)==-1) || (mq_close(outproc)==-1)){
				perror("ERROR33");
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}else if(p==256){
			printf("\nMessge -> %s\n",st);
		}
	}
	}else if(atoi(argv[1])!=1){
		printf("\n");
		fgets(st,49,stdin);
		int y = strlen(st)-1;
		st[y]='\0';
		strcat(st,argv[1]);
		if(st[0]=='0'){prior=255;}else{prior=256;}
		if((mq_send(inproc,st,strlen(st),prior)==-1)){
			perror("ERROR2");
			exit(EXIT_FAILURE);
		}	
		if(st[0]=='0') exit(EXIT_SUCCESS);
	}	
		
	}
	
		
}
