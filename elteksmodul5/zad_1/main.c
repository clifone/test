#include <stdio.h>
#include <dlfcn.h>

//extern int fm(int a, int b);

//extern double fd(int a, int b);

int main(){

	void *ext_library;
	double (*divisfunc)(int a, int b);
	int (*multifunc)(int a, int b);
	
	ext_library = dlopen("/home/student/modul3rep/elteksmodul5/zad_1/dynsec/libfsdynsec.so", RTLD_LAZY);
	if(!ext_library){
		fprintf(stderr, "dlopen() error: %s\n", dlerror());
		return 1;
	}
	
	divisfunc = dlsym(ext_library, "fd");
	multifunc = dlsym(ext_library, "fm");
	
	int n = (*multifunc)(10, 5);
	int u = (*divisfunc)(40, 4);
	
	printf("\n%d  %d\n", n, u);
	
	dlclose(ext_library);
	
	return 0;
	
}
