rez : main.o fmulti.o fdivis.o
	gcc -o rez main.o fmulti.o fdivis.o
main.o : main.c
	gcc -c main.c
fmulti.o : fmulti.c
	gcc -c fmulti.c
fdivis.o : fdivis.c
	gcc -c fdivis.c
clean :
	rm rez main.o fmulti.o fdivis.o
