#ifndef XUPL_HEADER_GUARD
#define XUPL_HEADER_GUARD

/*
Copyright 2013 Nice Robot Corporation

This file is part of Xupl.

Xupl is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Xupl is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Xupl.  If not, see <http://www.gnu.org/licenses/>.
*/

typedef struct xupl {
	struct xupl* (*parse)(struct xupl*);
	struct xupl* (*print)(struct xupl*);
	struct xupl* (*done)(struct xupl*);
	struct xupl* (*cleanup)(struct xupl*);
} xupl;
xupl *xupl_init(int argc, char *argv[]);

#define Xupl(x) { xupl*_=xupl_init(argc,argv);_->parse(_) ->x(_) ->done(_); }

#endif
