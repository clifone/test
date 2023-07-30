#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
int main(int argc, char *argv[]){
    
    
    
    int sockfd;
    int n, len;
    char sendline[1000], recvline[1000];
    struct sockaddr_in servaddr, cliaddr; 
    
    if(argc != 2){
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
    cliaddr.sin_port = htons(0); 
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
     
    //strcpy(cliaddr.sun_path,"AAAA");/* Локальный адрес 
   //     сокета клиента – AAAA – в текущей директории */
   
    if(bind(sockfd, (struct sockaddr *) &cliaddr, 
    sizeof(cliaddr)) < 0){
        perror("Error1");
        close(sockfd);
        exit(1);
    }
    
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
    printf("String => ");
    
    fgets(sendline, 1000, stdin);
    
    if(sendto(sockfd, sendline, strlen(sendline)+1, 
    0, (struct sockaddr *) &servaddr, 
    sizeof(servaddr)) < 0){
        perror("Error3");
        close(sockfd);
        exit(1);
    }
    if((n = recvfrom(sockfd, recvline, 1000, 0, 
    (struct sockaddr *) NULL, NULL)) < 0){
        perror("Error4");
        close(sockfd);
        exit(1);
    }
    recvline[n] = 0;
    printf("%s\n", recvline);
    close(sockfd);
    return 0;
}

