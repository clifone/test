#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(){
	srand(time(NULL));
	int num;
	char st[50];
	key_t fok;
	int msgoch;
	struct msgbuf{
		long mtype;
		char mtext[50];
	};
	
	struct msgbuf buf;
	int pid;
	
	fok=ftok("mythretext",13);
	msgoch=msgget(fok,00666|IPC_CREAT);
	pid=fork();
	if(pid==-1){
		perror("ERROR");
		exit(EXIT_FAILURE);
	}
	while(1){
	//scanf("\n%s",st);
	if(pid==0){
		if(msgrcv(msgoch,&buf,50,255,IPC_NOWAIT)>0){
			//printf("\nMes from out +>%s\n", buf.mtext);
			struct msqid_ds bb;
			msgctl(msgoch,IPC_RMID,&bb);
			exit(EXIT_SUCCESS);
		}
		while(msgrcv(msgoch,&buf,50,2,IPC_NOWAIT)>0){

		if(buf.mtype==2) printf("\nMes from out +>%s\n", buf.mtext);


		}
	}else{
		fgets(st,50,stdin);
	
		if(st[0]=='0'){buf.mtype=255;}else{buf.mtype=1;}
		strcpy(buf.mtext,st);
	
		msgsnd(msgoch,&buf,50,0);
	
		if(st[0]=='0') exit(EXIT_SUCCESS);
	}	
	}
}
