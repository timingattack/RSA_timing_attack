run : main
	./main.exe

main : main.o square_multiply.o
	gcc -Wall -Wextra -std=c89 -pedantic -O2 -lgmp -o main.exe main.o square_multiply.o

main.o : square_multiply.h
	gcc -c main.c

square_multiply.o : square_multiply.h
	gcc -c square_multiply.c

clean :
	rm -vf *.exe
	rm -vf *.o
	clear