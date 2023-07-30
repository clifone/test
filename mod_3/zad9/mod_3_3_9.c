#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc, char *argv[]){
	
	if(argc != 2){
		perror("ERROR1");
		exit(EXIT_FAILURE);
	}
	
	int a,b,c;
	pid_t pid;
	char st[50];
	key_t fok1;
	key_t fok2;
	key_t fok3;
	int msgoch1;
	int msgoch2;
	int msgoch3;
	
	struct msgbuf{
		long mtype;
		char mtext[50];
	};
	
	struct msgbuf buf,bufa,bufb,bufc;

	
	fok1=ftok("forfirstandans1",10);
	fok2=ftok("forsecond1",11);
	fok3=ftok("forthird1",12);
	
	msgoch1=msgget(fok1,00666|IPC_CREAT);
	msgoch2=msgget(fok2,00666|IPC_CREAT);
	msgoch3=msgget(fok3,00666|IPC_CREAT);
	//scanf("\n%s",st);
	pid = fork();
	while(1){
	if(pid<0){
		printf("ERROR");
		exit(EXIT_FAILURE);
	}else if(pid==0)
	{
	a=0;
	a = msgrcv(msgoch1,&bufa,50,255,IPC_NOWAIT);
	if(a>0) exit(EXIT_SUCCESS);
	a=0;
	b=0;
	c=0;
	a = msgrcv(msgoch1,&bufa,50,atoi(argv[1]),IPC_NOWAIT);
	b = msgrcv(msgoch2,&bufb,50,atoi(argv[1]),IPC_NOWAIT);
	c = msgrcv(msgoch3,&bufc,50,atoi(argv[1]),IPC_NOWAIT);
	
	if(atoi(argv[1])==1){
			if(a>0){
				struct msgbuf buf2;
				strcpy(buf2.mtext,bufa.mtext);
				buf2.mtype=3;
				msgsnd(msgoch1,&buf2,50,0);
				struct msgbuf buf3;
				strcpy(buf3.mtext,bufa.mtext);
				buf3.mtype=4;
				msgsnd(msgoch1,&buf3,50,0);	
			}
			if(b>0){
				struct msgbuf buf2;
				strcpy(buf2.mtext,bufb.mtext);
				buf2.mtype=2;
				msgsnd(msgoch1,&buf2,50,0);
				struct msgbuf buf3;
				strcpy(buf3.mtext,bufb.mtext);
				buf3.mtype=4;
				msgsnd(msgoch1,&buf3,50,0);
			}
			if(c>0){
				struct msgbuf buf2;
				strcpy(buf2.mtext,bufc.mtext);
				buf2.mtype=2;
				msgsnd(msgoch1,&buf2,50,0);
				struct msgbuf buf3;
				strcpy(buf3.mtext,bufc.mtext);
				buf3.mtype=3;
				msgsnd(msgoch1,&buf3,50,0);
			}
	}else if(a>0){
		printf("\nMes from out +>%s\n", bufa.mtext);
	}
	}else if(atoi(argv[1])!=1){
		fgets(st,50,stdin);
		strcpy(buf.mtext,st);
	
		if(st[0]=='0'){
			buf.mtype=255;
			msgsnd(msgoch1,&buf,50,0);
		}else{
			buf.mtype=1;
			switch (atoi(argv[1])){
				case 2:
				msgsnd(msgoch1,&buf,50,0);
				break;
				case 3:
				msgsnd(msgoch2,&buf,50,0);
				break;
				case 4:
				msgsnd(msgoch3,&buf,50,0);
				break;
				default:
				break;
			}
			
		}
		
		if(st[0]=='0') exit(EXIT_SUCCESS);
	}	
		
	}
	
		
}
