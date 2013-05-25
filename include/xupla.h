#ifndef XUPLA_HEADER_GUARD
#define XUPLA_HEADER_GUARD

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

#include <stdarg.h>
#include "xupl.h"

xupl *xupl_init_with_file_pointer_and_buffer (FILE*,	off_t);
xupl *xupl_init_with_file_descriptor_and_buffer (int, off_t);
xupl *xupl_init_with_file_descriptor (int);
xupl *xupl_init_with_file_name (char*);
xupl *xupl_init_with_file_pointer (FILE*);

xupl *xupl_cleanup(xupl*);
xupl *xupl_nestv(xupl*, va_list);
xupl *xupl_nest(xupl*, ...);
xupl *xupl_chainv(xupl*, va_list);
xupl *xupl_chain(xupl*, ...);

#endif
