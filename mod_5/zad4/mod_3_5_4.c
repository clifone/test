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

    	char buff[1024];    // Буфер для различных нужд
	printf("TCP enter file\n");
	
	int sockfd, newsockfd; // дескрипторы сокетов
	int portno; // номер порта
	FILE *fl;
    	socklen_t clilen; // размер адреса клиента типа socklen_t
    	struct sockaddr_in serv_addr, cli_addr; // структура сокета сервера и клиента
	
	// ошибка в случае если мы не указали порт
    	if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    // Шаг 1 - создание сокета
	// AF_INET     - сокет Интернета
    // SOCK_STREAM  - потоковый сокет (с
    //      установкой соединения)
    // 0 - по умолчанию выбирается TCP протокол
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // ошибка при создании сокета
	if (sockfd < 0) 
       error("ERROR opening socket");
	 
	// Шаг 2 - связывание сокета с локальным адресом
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // сервер принимает подключения на все IP-адреса
    serv_addr.sin_port = htons(portno);
    // вызываем bind для связывания
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
    // Шаг 3 - ожидание подключений, размер очереди - 2
	listen(sockfd,2);
    clilen = sizeof(cli_addr);
	
	// Шаг 4 - извлекаем сообщение из очереди
	// цикл извлечения запросов на подключение из очереди
	  
	//while (1) 
	//{
         newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) error("ERROR on accept");
                  	
         	int i = 0;
         	
         	fl=fopen("../alice.txt","rb");
         	
		while(feof(fl)==0){
		
			memset(buff,'\0',sizeof(buff));

			fseek(fl,i*1024,SEEK_SET);
			
		    	fread(buff, 1,sizeof(buff),fl);
		    	// отправляем клиенту сообщение
	   	 	write(newsockfd, buff, sizeof(buff));
	   	 	i++;	
		}

	     fclose(fl);
             close(newsockfd);
             close(sockfd);
             exit(0);  

     return 0; 
}
