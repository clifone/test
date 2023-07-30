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
	char st[25];
	key_t fok;
	int msgoch;
	struct msgbuf{
		long mtype;
		char mtext[25];
	};
	
	struct msgbuf buf;
	int i=0;
	
	fok=ftok("mysectext",13);
	msgoch=msgget(fok,00666|IPC_CREAT);
	
	while(i<4){
	num=rand();
	snprintf(st,25,"%d",num);
	
	if(i==3){buf.mtype=255;}else{buf.mtype=12;}
	strcpy(buf.mtext,st);
	
	msgsnd(msgoch,&buf,25,0);
	
	printf("\n%d\n",num);
	i++;
	}
	
}
