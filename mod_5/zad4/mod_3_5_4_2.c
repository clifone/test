#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <fcntl.h>

// Функция обработки ошибок
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{

    int my_sock, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
	FILE *fl;
    	char buff[1024];
    printf("TCP DEMO CLIENT\n");
	
    if (argc < 2) {
       fprintf(stderr,"usage %s port\n", argv[0]);
       exit(0);
    }
    // извлечение порта
	portno = atoi(argv[1]);
    
	// Шаг 1 - создание сокета
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (my_sock < 0) 
        error("ERROR opening socket");

    // заполенние структуры serv_addr
	bzero((char *) &serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
    	if(inet_aton("127.0.0.1" ,&serv_addr.sin_addr)==0){
    		printf("\nBad ip adress\n");
        	close(my_sock);
       	 	exit(1);
   	}
         
    // установка порта
	serv_addr.sin_port = htons(portno);

	// Шаг 2 - установка соединения	
	if (connect(my_sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        	error("ERROR connecting");

	// Шаг 3 - чтение и передача сообщений
	fl=fopen("alice.txt","wb");
	fclose(fl);
	//int oiu = 0;
	while ((n = recv(my_sock, buff, sizeof(buff), 0)) > 0){
		//strcpy(buff,"");
		//oiu++;
		//printf("\n%d\n",oiu);
		fl=fopen("alice.txt","a+b");
		char temp[1024];
		int p;
		memset(temp,0,sizeof(temp));
		for(p = 0; buff[p]!='\0' && p<1024;p++){
			temp[p]=buff[p];
		}
		fwrite(temp, 1,p,fl);
		fclose(fl);
		
	}
        close(my_sock);
     return 0; 
}

