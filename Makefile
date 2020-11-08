######################################################################
# $@ es el item que aparece a la izquierda de ':'
# $< es el primer item en la lista de dependencias
# $^ son todos los archivos que se encuentran a la derecha de ':'
########################################################################

CC = gcc -g -ansi -pedantic
##CC = gcc -ansi -O3 -pedantic
CFLAGS = -Wall
LDLIBS = -lodbc

export PGDATABASE :=classicmodels
export PGUSER :=alumnodb
export PGPASSWORD :=alumnodb
export PGCLIENTENCODING :=LATIN9
export PGHOST :=localhost

DBNAME =$(PGDATABASE)
PSQL = psql
CREATEDB = createdb
DROPDB = dropdb --if-exists
PG_DUMP = pg_dump
PG_RESTORE = pg_restore

all: compile

menu.o: menu.c 
	$(CC) -c $< $(CFLAGS)

products.o: products.c odbc.c
	$(CC) -c $^ $(CFLAGS) $(LDLIBS)	

orders.o: orders.c odbc.c
	$(CC) -c $^ $(CFLAGS) $(LDLIBS)	

odbc.o: odbc.c
	$(CC) -c $< $(CFLAGS) 

main.o: main.c
	$(CC) -c $< $(CFLAGS)

compile: main.o menu.o products.o odbc.o orders.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)
	
clean:
	rm -f *.o
	rm -f compile

	