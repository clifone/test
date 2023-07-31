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

    
     if((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0){
        perror("Error1");
        exit(1);
    }
        
    if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror("Error2");
        close(sockfd);
        exit(1);
    }
    
    memset(line,0,999);
    
        clilen = sizeof(cliaddr); 
        if((n = recvfrom(sockfd, line, 999, 0, 
        (struct sockaddr *) &cliaddr, &clilen)) < 0){
            perror("Error3");
            close(sockfd);
            exit(1);
        }
        
        int i = 0;
        printf("\nDAPM MEMORY\n");
       	while (i<n){
       		if(line[i]<0){
       			printf(" %02X ", (256+line[i]));
       		}else{
       		printf(" %02X ",line[i]);
       		}
       		i++;
       	}
       	printf("\n");
       	
       	printf("\nPart whis IP\n");
       	i=0;
       	while (i<20){
       		if(line[i]<0){
       			printf(" %02X ", (256+line[i]));
       		}else{
       		printf(" %02X ",line[i]);
       		}
       		i++;
       	}
       	
       	
       	
       	printf("\nPart whis IP/ version and count octet header: %02X\n", line[0]);
       	printf("\nPart whis IP/ differentiated services code and explicit congestion: %02X\n", line[1]);
       	
       	printf("\nPart whis IP/ ip adress sourse: %02X %02X %02X %02X\n", line[12], line[13], line[14], line[15]);
       	
       	printf("\nPart whis IP/ ip adress destination: %02X %02X %02X %02X\n", line[16], line[17], line[18], line[19]);
       	
       	printf("\nPart whis UDP\n");
       	
       	while (i<28){
       		if(line[i]<0){
       			printf(" %02X ", (256+line[i]));
       		}else{
       		printf(" %02X ",line[i]);
       		}
       		i++;
       	}
       	
       	printf("\nPart whis Data\n");
       	
       	while (i<n){
       		if(line[i]<0){
       			printf(" %02X ", (256+line[i]));
       		}else{
       		printf(" %02X ",line[i]);
       		}
       		i++;
       	}
        
        /*FILE * fl = fopen("datames.txt", "wb");
        write(fl,);
        close(fl);*/

    return 0;
}
