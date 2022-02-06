run : main
	./main.exe

main : main.o square_multiply.o chiffrement.o dechiffrement.o creation_des_cles.o miller_rabin.o
	gcc -Wall -Wextra -std=c89 -pedantic -O2 -lgmp -o main.exe main.o square_multiply.o chiffrement.o dechiffrement.o creation_des_cles.o miller_rabin.o

main.o : chiffrement.h dechiffrement.h creation_des_cles.h
	gcc -c main.c

square_multiply.o : square_multiply.h
	gcc -c square_multiply.c

miller_rabin.o : miller_rabin.h square_multiply.h
	gcc -c miller_rabin.c

chiffrement.o : chiffrement.h square_multiply.h
	gcc -c chiffrement.c

dechiffrement.o : dechiffrement.h square_multiply.h
	gcc -c dechiffrement.c

creation_des_cles.o : creation_des_cles.h square_multiply.h miller_rabin.h
	gcc -c creation_des_cles.c

clean :
	@clear
	@rm -vf *.exe
	@rm -vf *.o
	