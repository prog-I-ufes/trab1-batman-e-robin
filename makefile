all: trab

trab: trab1.c lib/calcdistancias.h lib/vetmanip.h lib/filemanip.h lib/classificacao.h calcdistancias.o vetmanip.o filemanip.o classificacao.o
	gcc -o trab1 trab1.c calcdistancias.o filemanip.o vetmanip.o classificacao.o -lm

valgrind: trab1
	valgrind --show-leak-kinds=all --show-leak-kinds=all -v ./trab1

calcdistancias.o: lib/calcdistancias.c lib/calcdistancias.h
	gcc -c lib/calcdistancias.c

vetmanip.o: lib/vetmanip.c lib/vetmanip.h
	gcc -c lib/vetmanip.c

filemanip.o: lib/filemanip.c lib/filemanip.h lib/vetmanip.h
	gcc -c lib/filemanip.c

classificacao.o: lib/classificacao.c lib/classificacao.h lib/vetmanip.h
	gcc -c lib/classificacao.c

clean: 
	rm -rf *.o trab1

cleano: 
	rm -rf *.o

cleantest:
	rm -rf config.txt "dataset/" "predicoes/"

