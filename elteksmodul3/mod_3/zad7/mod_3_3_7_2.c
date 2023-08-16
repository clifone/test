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

	int num;
	char st[25];
	key_t fok;
	int msgoch;
	struct msgbuf{
		long mtype;
		char mtext[25];
	};
	
	struct msgbuf buf;
	
	fok=ftok("mysectext",13);
	
	
	msgoch=msgget(fok,00666|IPC_CREAT);
	while(1){
	msgrcv(msgoch,&buf,25,0,0);
	if(buf.mtype==255){
		printf("\n%s\n", buf.mtext);
		struct msqid_ds bb;
		msgctl(msgoch,IPC_RMID,&bb);
		break;
	}else{
		printf("\n%s\n", buf.mtext);
	}
	}
		
}
