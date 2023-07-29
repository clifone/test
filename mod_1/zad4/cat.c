#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	char a[100];
	strcat(a,argv[1]);
	printf("Rezult = %s\n", strcat(a,argv[2]));
	exit(EXIT_SUCCESS);
}
