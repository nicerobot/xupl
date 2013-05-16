.PHONY : clean test check run all enter finish leave dot

test: all
	@cd test; make

all: build xupl

xupl: build/xupl.o build/main.o
	@date +v0.1+%y%j.%H%M | tee VERSION.txt
	@cc -o $@ $^ `xml2-config --libs`

build:
	@mkdir build

build/%.o: src/%.c ; @cc -c -std=gnu99 -Iinclude `xml2-config --cflags` -o $@ $^

check: test

clean: .log
	@rm -vfr build *.c *.dot *.png xupl | tee .log/clean.out

.log:
	@mkdir .log
