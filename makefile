all: trab

trab: trab1.c calcdistancias.h vetmanip.h filemanip.h classificacao.h calcdistancias.o vetmanip.o filemanip.o  classificacao.o
	gcc -o trab1 trab1.c calcdistancias.o filemanip.o vetmanip.o classificacao.o -lm

calcdistancias.o: calcdistancias.c calcdistancias.h
	gcc -c calcdistancias.c

vetmanip.o: vetmanip.c vetmanip.h
	gcc -c vetmanip.c

filemanip.o: filemanip.c filemanip.h vetmanip.h
	gcc -c filemanip.c

classificacao.o: classificacao.c classificacao.h vetmanip.h
	gcc -c classificacao.c

clean: rm -rf *.o trab1
