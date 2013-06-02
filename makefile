# Copyright 2013 Nice Robot Corporation
#
# This file is part of Xupl.
#
# Xupl is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Xupl is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Xupl.  If not, see <http://www.gnu.org/licenses/>.

.PHONY : clean test check run all enter finish leave dot push
M ?= bump

test: app lib
	@cd test; make

exe app: xupl up

lib: libxupl.a

xupl: libxupl.a src/test.o
	@date +v0.1+%y%j.%H%M | tee VERSION.txt
	@cc -o $@ $^ `xml2-config --libs`

up: src/xupl.l.o
	cc -ll -o $@ $^ `xml2-config --libs`

tup:
	@cd test; make testup EXE=up

libxupl.a: src/xupl.o src/main.o
	@ar rvs $@ $^

src/%.o:
	@cd src; make $^

check: test

clean:
	@cd src; make clean
	@cd test; make clean
	@rm -vfr *.a *.c *.dot *.png xupl up *.h *.l.c

bump:
	@make clean
	@make
	@git add .
	@git cma "$(M)"
	@git push
