#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

int sockfd;

void mylistener(int sig){
	close(sockfd);
	raise(SIGCHLD);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
    
    signal(SIGUSR1,mylistener);
    
    
    int n, len;
    char sendline[1000], recvline[1000];
    struct sockaddr_in servaddr, cliaddr; 
    pid_t pid;
    
    
    if(argc != 3){
    	perror("Error2");
        exit(1);
    }
    
    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("Error2");
        exit(1);
    }
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET; 
    cliaddr.sin_port = htons(atoi(argv[2])); 
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
     
    //strcpy(cliaddr.sun_path,"AAAA");/* Локальный адрес 
   //     сокета клиента – AAAA – в текущей директории */
   
    if(bind(sockfd, (struct sockaddr *) &cliaddr, 
    sizeof(cliaddr)) < 0){
        perror("Error1");
        close(sockfd);
        exit(1);
    }

    switch(pid=fork()){
	    case -1:
	    	perror("Error7");
        	exit(1);
	    break;
	    case 0:
	    while(1){
	    	if((n = recvfrom(sockfd, recvline, 1000, 0, 
	    	(struct sockaddr *) NULL, NULL)) <= 0){
			perror("Error4");
			close(sockfd);
			exit(1);
	    	}
	    	recvline[n-1] = '\0';
	    	//printf("\n aut--> %d\n=> ", (int)recvline[n]);
	    	//printf("\n aut--> %d\n=> ", (int)recvline[n-1]);
	    	//printf("\n aut--> %d\n=> ", (int)recvline[n-2]);
	    	if(recvline[0]=='0'){
	    		close(sockfd);
	    		kill(getppid(),10);
	    		exit(EXIT_SUCCESS);
	    	}
	   	printf("\nMeseg from out--> %s\n", recvline);
	   	}
	    break;
	    default:
	    while(1){
	    	bzero(&servaddr, sizeof(servaddr));
	    	servaddr.sin_family = AF_INET;
	    	servaddr.sin_port = htons(atoi(argv[1]));
	    //servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	    if(inet_aton("127.0.0.1" ,&servaddr.sin_addr)==0){
	    	printf("\nBad ip adress\n");
		close(sockfd);
		exit(1);
	    }
	    
	    //printf("\n%c\n",servaddr.sin_addr);
	    //printf("=> ");
	    
	    fgets(sendline, 1000, stdin);
	    sendline[strlen(sendline)-1]='\0';
	    if(sendto(sockfd, sendline, strlen(sendline)+1, 
	    0, (struct sockaddr *) &servaddr, 
	    sizeof(servaddr)) <= 0){
		perror("Error3");
		close(sockfd);
		exit(1);
	    }
	    if(sendline[0]=='0'){
	    	close(sockfd);
	    	kill(pid,9);
	    	exit(EXIT_SUCCESS);
	    }
	    }
	    break;

    }
    return 0;
}

