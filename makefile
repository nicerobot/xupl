.PHONY : clean test check run all enter finish leave dot

test: all
	@echo 'html{head{title{"XUPL"}}body{"Supple"}}' | ./xupl

all: xupl

xupl: src/xupl.c

%: src/%.c             ; cc -I../include `xml2-config --cflags --libs` -o $@ $^

check: test
clean:             ; rm -f *.c *.dot *.png xupl
