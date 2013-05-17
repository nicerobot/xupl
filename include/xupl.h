#ifndef XUPL_HEADER_GUARD
#define XUPL_HEADER_GUARD

typedef struct xupl {
	struct xupl* (*parse)(struct xupl*);
	struct xupl* (*print)(struct xupl*);
	struct xupl* (*done)(struct xupl*);
	struct xupl* (*cleanup)(struct xupl*);
} xupl;
xupl *xupl_init(int argc, char *argv[]);

#endif
