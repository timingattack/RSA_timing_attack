run : main
	./main.exe

main : main.o square_multiply.o Dechiffrement.o Creation_des_cles.o montgomery.o
	gcc -Wall -Wextra -std=c89 -pedantic -O2 -lgmp -o main.exe main.o square_multiply.o Dechiffrement.o Creation_des_cles.o montgomery.o

main.o : square_multiply.h Dechiffrement.h Creation_des_cles.h montgomery.h
	gcc -c main.c

square_multiply.o : square_multiply.h
	gcc -c square_multiply.c

Dechiffrement.o : Dechiffrement.h
	gcc -c Dechiffrement.c

Creation_des_cles.o : Creation_des_cles.h
	gcc -c Creation_des_cles.c

montgomery.o : montgomery.h
	gcc -c montgomery.c

clean :
	rm -vf *.exe
	rm -vf *.o
	clear