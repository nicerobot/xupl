
test: ../xupl
	@for test in $(wildcard *.xupl); do \
		printf '# %s\n\ninput:\n\n' $$test; cat $$test; \
		printf '\n\noutput:\n\n'; ../xupl $$test; done | tee README.md

../xupl:
	@cd ../make
