rezdynsec : main.o libfsdynsec.so
	gcc main.o -o rezdynsec -ldl
main.o : main.c
	gcc -c main.c
libfsdynsec.so : fmulti.o fdivis.o
	gcc -shared -o libfsdynsec.so fmulti.o fdivis.o
fmulti.o : fmulti.c
	gcc -fPIC -c fmulti.c
fdivis.o : fdivis.c
	gcc -fPIC -c fdivis.c
clean :
	rm rezdynsec main.o fmulti.o fdivis.o libfsdynsec.so
