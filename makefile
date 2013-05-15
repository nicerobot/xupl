.PHONY : clean test check run all enter finish leave dot

test: all
	@./xupl test/note.xupl
	@./xupl test/html.xupl
	@./xupl test/attr1.xupl
	@./xupl test/meta1.xupl

all: xupl

xupl: src/xupl.c

%: src/%.c             ; cc -I../include `xml2-config --cflags --libs` -o $@ $^

check: test
clean:             ; rm -f *.c *.dot *.png xupl
