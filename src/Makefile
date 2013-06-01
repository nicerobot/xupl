CFLAGS ?= -g -Wall -I../include
LEX ?= flex

objects: xupl.o test.o main.o xupl.l.o

%.o: %.c ; @cc -c -std=gnu99 -I../include `xml2-config --cflags` -o $@ $^

%.l.c: %.c.l       ; $(LEX) -o $@ $<

clean:
	@rm -f *.o *.l.c
