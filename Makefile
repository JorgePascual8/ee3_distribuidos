CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lnsl -lkeyvalue

all: servidor cliente libclaves.so

libclaves.so: claves.o
    $(CC) -shared -o libclaves.so claves.o

claves.o: claves.c
    $(CC) $(CFLAGS) -fPIC -c claves.c

servidor: servidor.o keyvalue_svc.o keyvalue_xdr.o
    $(CC) $(CFLAGS) -o servidor servidor.o keyvalue_svc.o keyvalue_xdr.o $(LDFLAGS)

cliente: cliente.o
    $(CC) $(CFLAGS) -o cliente cliente.o -L. -lclaves $(LDFLAGS)

clean:
    rm -f *.o servidor cliente libclaves.so
