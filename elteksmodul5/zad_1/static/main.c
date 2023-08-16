#include <stdio.h>

extern int fm(int a, int b);

extern double fd(int a, int b);

int main(){

	
	int n = fm(10, 5);
	int u = fd(40, 4);
	
	printf("\n%d  %d\n", n, u);
	
	return 0;
	
}
