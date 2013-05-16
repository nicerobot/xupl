.PHONY : clean test check run all enter finish leave dot

test: all
	@cd test; make

all: build xupl

xupl: build/xupl.o build/main.o
	cc `xml2-config --cflags --libs` -o $@ $^

build:
	mkdir build

build/%.o: src/%.c       ; cc -c -Iinclude `xml2-config --cflags --libs` -o $@ $^

check: test
clean:             ; rm -f *.c *.dot *.png xupl
