#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <mqueue.h>
#define MQ_NAME "/mqmqfortenzad"

int main(){
	srand(time(NULL));
	int num, i;
	unsigned prior;
	char st[25];
	mqd_t dp;
	struct mq_attr atr;
	atr.mq_curmsgs=0;
	atr.mq_flags=0;
	atr.mq_maxmsg=10;
	atr.mq_msgsize=25;
	
	if((dp=mq_open(MQ_NAME, O_CREAT | O_WRONLY,S_IRUSR | S_IWUSR, &atr))==(mqd_t)-1){
		perror("ERROR1");
		exit(EXIT_FAILURE);
	}
	
	while(i<4){
	num=rand();
	snprintf(st,25,"%d",num);
	
	if(i==3){prior=255;}else{prior=257;}
	
	if(mq_send(dp,st,strlen(st),prior)==-1){
		perror("ERROR2");
		exit(EXIT_FAILURE);
	}
	
	printf("\n%d\n",num);
	i++;
	}
	
	if(mq_close(dp)==-1){
		perror("ERROR3");
		exit(EXIT_FAILURE);
	}
	
}
