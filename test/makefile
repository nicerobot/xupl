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

XUPL=$(wildcard *.xupl)
XML=$(notdir $(wildcard expect/*.xml))

.PHONY: xmls test clean missing output

test: ../xupl README.md
	@cat README.md
	@head -1 README.md

../xupl:
	@cd ..; make $(notdir $@)

README.md: ../VERSION.txt
	@make --quiet output >README.md

output:
	@cat ../VERSION.txt
	@make --quiet xmls
	@printf '\n\n'
	@make --quiet missing

clean:
	@rm -vf *.xml README.md

xmls: ../xupl $(XML)

%.xml: %.xupl;
	@printf '\n# [%s](test/%s)\n\ninput:\n\n' $^ $^
	@cat $^ | sed 's/^/    /'
	@printf '\n\noutput:\n\n'
	@../xupl $^ | tee $(notdir $@) | sed 's/^/    /'
	@printf '\n\n'
	@diff $@ expect/$@ | sed 's/^/    /'

expect/%.xml: %.xupl; @echo missing $@

missing: $(addprefix expect/,$(XUPL:.xupl=.xml))
