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
    char line[990];
    char head[1000];
    struct sockaddr_in servaddr, cliaddr; 
    
    if(argc != 2){
    	perror("Error2");
        exit(1);
    }
    
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET; 
    cliaddr.sin_port = htons(63003); 
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(atoi(argv[1])); 
    if(inet_aton("127.0.0.1" ,&servaddr.sin_addr)==0){
    	printf("\nBad ip adress\n");
        close(sockfd);
        exit(1);
    }

    
     if((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0){
        perror("Error1");
        exit(1);
    }
        
    if(bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0){
        perror("Error1");
        close(sockfd);
        exit(1);
    }
    
     printf("String => ");
    
    fgets(line, 990, stdin);
    
    memset(head,1,8);
    head[8]=0;
    strcat(head,line);
    
    head[0]= 63003/256;
    head[1]= (63003 -((63003/256)*256));
    head[2]=(atoi(argv[1])/256);
    head[3]=atoi(argv[1])-((atoi(argv[1])/256)*256);
    int lendatagram = 8 + strlen(line) + 1;
    head[4]=lendatagram/256;
    head[5]=lendatagram-(lendatagram/256);
    head[6]= 0;
    head[7]= 0;
    
    
    if(sendto(sockfd, head, lendatagram, 
    0, (struct sockaddr *) &servaddr, 
    sizeof(servaddr)) < 0){
        perror("Error3");
        close(sockfd);
        exit(1);
    }
    
    //strcat(head,line);
    
    /*printf("\n%c-",head[0]);
    printf("\n%c-",head[1]);
    printf("\n%c-",head[2]);
    printf("\n%c-",head[3]);
    printf("\n%c-",head[4]);
    printf("\n%c-",head[5]);
    printf("\n%c-",head[6]);
    printf("\n%c-",head[7]);
    printf("\n%c-",head[8]);
    printf("\n%c-",head[9]);
    printf("\n%c-",head[10]);
    printf("\n%c-",head[11]);
    */
       

    return 0;
}
