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

#include "xupl.h"

xupl *xupl_init_with_file_pointer_and_buffer (FILE* in,	off_t buffsize);
xupl *xupl_init_with_file_descriptor_and_buffer (int fd, off_t buffsize);
xupl *xupl_init_with_file_descriptor (int fd);
xupl *xupl_init_with_file_name (char* fn);
xupl *xupl_init_with_file_pointer (FILE* in);
xupl *xupl_parse (xupl *ctxupl);
xupl *xupl_print(xupl *ctxupl);

xupl *xupl_done (xupl *ctxupl);
xupl *xupl_cleanup(xupl *ctxupl);

#endif
