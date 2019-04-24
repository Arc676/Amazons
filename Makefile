CC=gcc
FLAGS=-std=c11

LIBOUT=libamazons.a
EXECOUT=amazons

ifdef DEBUG
FLAGS+=-g -O0
endif

INCLUDE=-I . -L .
CFLAGS=$(FLAGS) $(INCLUDE)
LIB=-l amazons

ifdef FPIC
FLAGS+=-fPIC
endif

amazons: lib
	$(CC) $(CFLAGS) amazons.c $(LIB) -o $(EXECOUT)

lib: libamazons.o
	ar rcs $(LIBOUT) libamazons.o

libamazons.o:
	$(CC) -c $(FLAGS) libamazons.c

clean:
	rm -f $(LIBOUT) $(EXECOUT) *.o
