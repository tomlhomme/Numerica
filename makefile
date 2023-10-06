app: main.o array.o
	g++ -g array.o main.o -o app -Wall -lm -lblas -llapack -llapacke

main.o: main.cpp
	g++ -g -c main.cpp -o main.o -Wall -lm -lblas -llapack -llapacke -DNUMERICA_RUNTIME_CHECKS_OFF=0

array.o: array.cpp
	g++ -g -c array.cpp -o array.o -Wall -lm -lblas -llapack -llapacke -DNUMERICA_RUNTIME_CHECKS_OFF=0

clean: 
	rm *.o app