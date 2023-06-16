CC     = gcc
CFLAGS = -std=c11
LFLAGS = -Wall

PROG = rank
OBJ = lista.o grafo.o centralidade.o $(PROG).o


.PHONY: clean purge all

%.o: %.c lista.h grafo.h centralidade.h
	$(CC) -c $(CFLAGS) $<

$(PROG): $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

clean:
	@rm -f *~ *.bak

purge:   clean
	@rm -rf bin obj *.layout *.depend
	@rm -f *.dat *.o core a.out
	@rm -f $(PROG)
