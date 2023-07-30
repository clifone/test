#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <mqueue.h>

int main(){
	
	int i;
	int num;
	unsigned prior;
	char st[25];
	mqd_t dp;
	struct mq_attr atr, oldatr;
	
	if((dp=mq_open("/mqmqfortenzad",O_RDONLY,0600,NULL))==(mqd_t)-1){
		perror("ERROR1");
		exit(EXIT_FAILURE);
	}
	
	i=0;
	while(i<4){
	num=rand();
	snprintf(st,25,"%d",num);
	
	if(mq_receive(dp,st,25,&prior)==-1){
		perror("ERROR2");
		exit(EXIT_FAILURE);
	}
	
	printf("\nMessge -> %s\n",st);
	
	if(prior==255) break;
	i++;
	}
	
	if(mq_close(dp)==-1){
		perror("ERROR3");
		exit(EXIT_FAILURE);
	}
	
}
