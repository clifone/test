#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
	
	if(argc != 2){
		perror("ERROR1");
		exit(EXIT_FAILURE);
	}
	
	FILE *file;
	int max = 100*sizeof(char);
	char *string = (char*)malloc(max);

	if((file=fopen(argv[1], "rb"))==NULL){
		perror("ERROR2");
		exit(EXIT_FAILURE);
	}
	
	char temp;
	int i=0;
	while(fread(&temp,sizeof(temp),1,file) != NULL){
		string[i]=temp;
		if(i+1==max){
			string=(char*)realloc(string, max*2);
			max*=2;
		}
		i++;
	}

	printf("%s\n",string);

}
