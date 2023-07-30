#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	printf("Rezult = %s\n", atoi(argv[1])>atoi(argv[2])?argv[1]:argv[2]);
	//exit(EXIT_SUCCESS);
}
