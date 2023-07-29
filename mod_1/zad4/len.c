#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	printf("%s\n",argv[1]);
	printf("Rezult = %ld\n", strlen(argv[1]));
	exit(EXIT_SUCCESS);
}
