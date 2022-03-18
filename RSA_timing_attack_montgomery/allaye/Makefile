DEBUG=no
CC=gcc
ifeq ($(DEBUG),yes)
	CFLAGS=-W -Wall -ansi -pedantic -g -std=c99
	LDFLAGS=-lgmp
else
	CFLAGS=-W -Wall -ansi -pedantic -std=c99
	LDFLAGS=-lgmp
endif
EXEC=rsa
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)
ifeq ($(DEBUG),yes)
	@echo "Génération en mode debug"
else
	@echo "Génération en mode release"
endif

rsa: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

main.o: rsa.h

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

propre: clean
	@rm -rf $(EXEC)
	@rm -rf id_rsa*