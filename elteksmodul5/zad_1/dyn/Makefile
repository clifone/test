rezdyn : main.o libfsdyn.so
	gcc main.o -L. -lfsdyn -o rezdyn
main.o : main.c
	gcc -c main.c
libfsdyn.so : fmulti.o fdivis.o
	gcc -shared -o libfsdyn.so fmulti.o fdivis.o
fmulti.o : fmulti.c
	gcc -fPIC -c fmulti.c
fdivis.o : fdivis.c
	gcc -fPIC -c fdivis.c
clean :
	rm rezdyn main.o fmulti.o fdivis.o libfsdyn.so
