main.exe: main.o 
	g++ main.o -o main.exe

main.o: main.cpp graph.h
	g++ -c main.cpp -o main.o

.PHONY: clean
clean: 
	rm -r *.o *.exe *.txt