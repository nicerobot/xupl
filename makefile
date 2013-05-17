.PHONY : clean test check run all enter finish leave dot

all: build .log xupl

xupl: build/xupl.o build/main.o
	@date +v0.1+%y%j.%H%M | tee VERSION.txt
	@cc -o $@ $^ `xml2-config --libs` 2>&1 | tee .log/ld-xupl.out

test: all
	@cd test; make

build:
	@mkdir build

build/%.o: src/%.c ; @cc -c -std=gnu99 -Iinclude `xml2-config --cflags` -o $@ $^ 2>&1 | tee -a .log/cc-xupl.out

check: test

clean: .log
	@rm -vfr build .log *.c *.dot *.png xupl | tee .log/clean.out

.log:
	@mkdir .log
