CFLAGS=-Wall -Ofast 
LINKS=-lm -lblas -llapack -llapacke -fopenmp -lgomp
NUMERICA_FLAGS=-DNUMERICA_RUNTIME_CHECKS_OFF=0

app: main.o array.o
	g++ -g main.o array.o -o app $(CFLAGS) $(LINKS)

main.o: main.cpp
	g++ -g -c main.cpp -o main.o $(CFLAGS) $(LINKS) $(NUMERICA_FLAGS)

objects: array.o

array.o: array.cpp
	g++ -g -c array.cpp -o array.o $(CFLAGS) $(LINKS) $(NUMERICA_FLAGS)

clean: 
	rm *.o