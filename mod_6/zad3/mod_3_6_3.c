#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    int sockfd;
    int clilen, n;
    char line[1000];
    struct sockaddr_in servaddr, cliaddr; 
    
    if(argc != 2){
    	perror("Error2");
        exit(1);
    }
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(atoi(argv[1])); 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 

    
     if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        perror("Error1");
        exit(1);
    }
        
    if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror("Error2");
        close(sockfd);
        exit(1);
    }
    
    while(1) {
        clilen = sizeof(cliaddr); 
        if((n = recvfrom(sockfd, line, 999, 0, 
        (struct sockaddr *) &cliaddr, &clilen)) < 0){
            perror("Error3");
            close(sockfd);
            exit(1);
        }
        printf("\n--> %s\n",line);
    }
    return 0;
}
