#ifndef XUPLA_HEADER_GUARD
#define XUPLA_HEADER_GUARD

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
